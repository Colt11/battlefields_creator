#include "packmanager.h"
#include <QProcess>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include <QDomDocument>
#include <QtMath>
#include <sharedsettings.h>
#include <scenedata.h>


PackManager::PackManager(SceneData *sceneData_, QImage image_, QObject *parent)
    : QThread{parent}
    , image{image_}
    , sceneData{sceneData_}
{

}


PackManager::~PackManager()
{
    if(tmpPath.isEmpty()) {
        return;
    }

    if(needRemoveTmpFolder) {
        QDir{tmpPath}.removeRecursively();
    }
}


void PackManager::run()
{
    if(!loadPaths()) {
        emit resultReady(errorString);
        return;
    }

    if(!fillTmpFolder()) {
        emit resultReady(errorString);
        return;
    }

    if(!generate()) {
        emit resultReady(errorString);
        return;
    }

    if(!patchXmls()) {
        emit resultReady(errorString);
        return;
    }

    if(!pack()) {
        emit resultReady(errorString);
        return;
    }

    if(needInstall) {
        installPack();
    }

    emit resultReady(errorString);
}


bool PackManager::patchHeightMapXmls() const
{
    int terrainScele = sceneData->getTerrainScale();
    float terrainWaterHeight = sceneData->getTerrainWaterHeight() * -1;

    for(int i = 0; i < 4; ++i) {
        QString fileName = QString{"height_map_"} + QString::number(i) + "_settings";
        QString heigthMapXmlPath = QDir(terrainDataFolderPath).filePath(fileName + ".xml");
        QDomDocument doc(fileName);
        QFile file(heigthMapXmlPath);
        if (!file.open(QIODevice::ReadOnly)) {
            errorString = "Can't open file " + heigthMapXmlPath + ".";
            return false;
        }
        if (!doc.setContent(&file)) {
            errorString = "Can't parse the content of file " + heigthMapXmlPath + ".";
            return false;
        }
        file.close();

        QDomNodeList heightFieldSettings = doc.elementsByTagName("HEIGHTFIELD_SETTINGS");
        if (heightFieldSettings.size() < 1) {
            errorString = "Can't find HEIGHTFIELD_SETTINGS tag in file " + heigthMapXmlPath + ".";
            return false;
        }

        QDomElement root = heightFieldSettings.at(0).toElement();
        root.setAttribute("scale", QString::number(terrainScele));
        root.setAttribute("bias", QString::number(terrainWaterHeight, 'f', 4));

        if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
            errorString = "Can't write file " + heigthMapXmlPath + ".";
            return false;
        }
        QByteArray xml = doc.toByteArray();
        file.write(xml);
        file.close();
    }

    return true;
}


bool PackManager::patchDeploymentAreasXmls() const
{
    QString fileName = "deployment_areas";
    QString deploymentAreasXmlPath = QDir(terrainDataFolderPath).filePath(fileName + ".xml");
    QDomDocument doc(fileName);
    QFile file(deploymentAreasXmlPath);

    if (!file.open(QIODevice::WriteOnly)) {
        errorString = "Can't write file " + deploymentAreasXmlPath + ".";
        return false;
    }

    const QMap<RectId, RectParam>& rects = sceneData->getTeamsRects();

    QDomElement hashTable = doc.createElement("BATTLE_DEPLOYMENT_AREA_HASH_TABLE");
    doc.appendChild(hashTable);
    QDomElement deploymantAreas = doc.createElement("BATTLE_DEPLOYMENT_AREAS");
    hashTable.appendChild(deploymantAreas);

    for(int a = 0; a < 2; ++a) {
        QDomElement alliance = doc.createElement("ALLIANCE");
        deploymantAreas.appendChild(alliance);
        alliance.setAttribute("id", QString::number(a));

        for(int i = 0; i < 3; ++i) {
            RectId id = (a + 1) * 1000 + (i + 1);
            auto it = rects.find(id);
            if(it == rects.end()) {
                continue;
            }
            QDomElement deploymantArea = doc.createElement("deployment_area");
            alliance.appendChild(deploymantArea);
            deploymantArea.setAttribute("id", QString::number(i));

            QDomElement centre = doc.createElement("centre");
            deploymantArea.appendChild(centre);
            centre.setAttribute("x", QString::number(it->pos.x() * 2 - 1025));
            centre.setAttribute("y", QString::number((it->pos.y() * 2 - 1025) * -1));

            QDomElement width = doc.createElement("width");
            deploymantArea.appendChild(width);
            width.setAttribute("metres", QString::number(it->width * 2));

            QDomElement height = doc.createElement("height");
            deploymantArea.appendChild(height);
            height.setAttribute("metres", QString::number(it->height * 2));

            QDomElement orientation = doc.createElement("orientation");
            deploymantArea.appendChild(orientation);
            float radians = qDegreesToRadians(static_cast<float>((static_cast<int>(it->rotation) + 180) % 359));
            orientation.setAttribute("radians", QString::number(radians));
        }
    }

    QByteArray xml = doc.toByteArray();
    file.write(xml);
    file.close();

    return true;
}


bool PackManager::patchXmls() const
{
    return patchHeightMapXmls() && patchDeploymentAreasXmls();
}


void PackManager::installPack() const
{
    QString scriptFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    scriptFilePath = QDir(scriptFilePath).filePath("..\\The Creative Assembly\\Empire\\scripts\\user.empire_script.txt");

    QFile file(scriptFilePath);
    bool isNewFile = !file.exists();

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        errorString = "Can't open file " + scriptFilePath + " for install mod.";
        return;
    }

    QString installString = QString {"mod "} + PACK_FILE_NAME + ";";

    QTextStream stream(&file);
    QString allPacks = stream.readAll();
    if(allPacks.indexOf(installString) == -1) {
        if(!allPacks.isEmpty()) {
            installString = QString{"\n"} + installString;
        } else if (isNewFile) {
            stream.setGenerateByteOrderMark(true);
            stream.setCodec("UTF-8");
        }
        stream<<installString;
    }
    file.close();
}


bool PackManager::generate() const
{
    QProcess process{};

    QString imagePath = "\"" + QDir(tmpPath).filePath(IMAGE_FILE_NAME) + "\"";

    QString cmd = QString{"battlefield_map_generator\\battlefield_map_generator.exe "}
                  + imagePath
                  + " \"" + terrainDataFolderPath + "\""
                  + " " + QString::number(sceneData->getTeamsSize() + 1)
                  + " " + QString::number(sceneData->getTerrainWaterHeight())
                  + " " + QString::number(sceneData->getTerrainScale());

    process.start(cmd);
    process.waitForFinished();
    if(process.exitCode() != 0) {
        errorString = process.readAllStandardOutput();
        return false;
    }

    return true;
}


bool PackManager::pack() const
{
    int buttleDbIndex = sceneData->getTeamsSize() + 1;

    QString packFilePath = QDir(gameFolderPath).filePath(PACK_FILE_NAME);
    QFile::remove(packFilePath);
    packFilePath = "\"" + packFilePath + "\"";

    QStringList cmds{};

    cmds.push_back(packManagerPath
            + " -g empire pack create --pack-path "
            + packFilePath);


    cmds.push_back(packManagerPath
            + " -g empire pack add -F "
            + QDir(tmpPath).filePath("battleterrain\\presets\\Custom")
            + ";\"battleterrain\\presets\\Custom\""
            +  " --pack-path "
            + packFilePath
            );

    cmds.push_back(packManagerPath
            + " -g empire pack add -f "
            + QDir(tmpPath).filePath("notes.rpfm_reserved")
            +  " --pack-path "
            + packFilePath
            );

    cmds.push_back(packManagerPath
            + " -g empire pack add -f "
            + QDir(tmpPath).filePath("db\\battles_tables\\battles_" + QString::number(buttleDbIndex))
            + ";\"db\\battles_tables\\battles\""
            +  " --pack-path "
            + packFilePath
            );

    QProcess process{};
    for(auto& cmd : cmds) {
        process.start(cmd);
        process.waitForFinished();

        if(process.exitCode() != 0) {
            errorString = process.readAllStandardOutput();
            return false;
        }
    }

    return true;
}


bool PackManager::fillTmpFolder()
{
    QDir().mkdir(tmpPath);
    QString imagePath = QDir(tmpPath).filePath(IMAGE_FILE_NAME);
    if(!image.save(imagePath)) {
        errorString = "Can't save image " + imagePath;
        return false;
    }

    copyPath("data\\battlefield_default_data", tmpPath);

    terrainDataFolderPath = QDir(tmpPath).filePath("battleterrain\\presets\\Custom");

    return true;
}


bool PackManager::loadPaths()
{
    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);

    needInstall = settings.value(SETTINGS_KEY_INSTALL_MOD, true).toBool();
    needRemoveTmpFolder = settings.value(SETTINGS_KEY_REMOVE_TMP_FOLDER, true).toBool();

    gameFolderPath = settings.value(SETTINGS_KEY_GAME_FOLDER, "").toString();
    if(gameFolderPath.isEmpty()) {
        errorString = "Need to specify the Game Folder";
        return false;
    }
    gameFolderPath = QDir(gameFolderPath).filePath("data");

    packManagerPath = settings.value(SETTINGS_KEY_PACK_MANAGER_FOLDER, "").toString();
    if(packManagerPath.isEmpty()) {
        errorString = "Need to specify the Pack Manager Folder, you can download pack manager by <a href='https://github.com/Frodo45127/rpfm'>reference</a>";
        return false;
    }

    packManagerPath = QDir(packManagerPath).filePath("rpfm_cli.exe");
    QFileInfo checkFile(packManagerPath);
    if (!checkFile.exists()) {
        errorString = "File \"rpfm_cli.exe\" don't found in Pack Manager Folder";
        return false;
    }
    packManagerPath = "\"" + packManagerPath + "\"";

    tmpPath = settings.value(SETTINGS_KEY_TMP_FOLDER, "").toString();
    if(tmpPath.isEmpty()) {
        tmpPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    tmpPath = QDir(tmpPath).filePath(TMP_FOLDER_NAME);

    return true;
}


void PackManager::copyPath(QString src, QString dst) const
{
    QDir dir(src);
    if (!dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src + QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}
