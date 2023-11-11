#include "scenedata.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QStandardPaths>
#include <QMessageBox>


SceneData::SceneData(QObject* parent)
    : QObject(parent)
{
    RectParam teamRect = DEFAULT_RECT;

    teamRect.pos = QPoint{575, 325};
    teamsRects.insert(1001, teamRect);
    teamRect.pos = QPoint{330, 325};
    teamsRects.insert(1002, teamRect);
    teamRect.pos = QPoint{890, 325};
    teamsRects.insert(1003, teamRect);

    teamRect.pos = QPoint{575, 690};
    teamsRects.insert(2001, teamRect);
    teamRect.pos = QPoint{330, 690};
    teamsRects.insert(2002, teamRect);
    teamRect.pos = QPoint{890, 690};
    teamsRects.insert(2003, teamRect);
}


void SceneData::addRect()
{
    auto it = rects.insert(nextIndex, DEFAULT_RECT);
    it.value().renderOrder = it.key();
    nextIndex++;
    selected = it.key();
    emit rectAdded(selected);
    emit rectSelected(selected);
}


void SceneData::removeRect()
{
    RectId id = selected;
    rects.remove(id);

    if(rects.empty()) {
        selected = INVALID_RECT_ID;
    } else {
        RectId min = INVALID_RECT_ID;
        for(auto it = rects.begin(); it != rects.end(); ++it) {
            if(min > it.key()) {
                min = it.key();
            }
        }
        selected = min;
    }

    emit rectRemoved(id);
    emit rectSelected(selected);
}


void SceneData::setOffset(RectId id, int offset)
{
    RectParam* params = getParams(id);
    if(params) {
        if(offset <= SCENE_MAX_OFFSET && offset >= SCENE_MIN_OFFSET) {
            params->offset = offset;
            emit offsetChanged(id, offset);
        }
    }
}


int SceneData::getOffset(RectId id) const
{
    const RectParam* params = getParams(id);
    if(params) {
        return params->offset;
    }
    return 0;
}


void SceneData::setPos(RectId id, const QPointF& pos)
{
    RectParam* params = getParams(id);
    if(params) {
        params->pos = pos;
        emit posChanged(id, pos);
    }
}


QPointF SceneData::getPos(RectId id) const
{
    const RectParam* params = getParams(id);
    if(params) {
        return params->pos;
    }
    return {0.0f, 0.0f};
}


void SceneData::setRotation(RectId id, float rotation)
{
    RectParam* params = getParams(id);
    if(params) {
        params->rotation = rotation;
        emit rotationChanged(id, rotation);
    }
}


float SceneData::getRotation(RectId id) const
{
    const RectParam* params = getParams(id);
    if(params) {
        return params->rotation;
    }
    return 0;
}


void SceneData::setWidth(RectId id, float width)
{
    RectParam* params = getParams(id);
    if(params) {
        params->width = width;
        emit widthChanged(id, width);
    }
}


float SceneData::getWidth(RectId id) const
{
    const RectParam* params = getParams(id);
    if(params) {
        return params->width;
    }
    return 0;
}


void SceneData::setHeight(RectId id, float height)
{
    RectParam* params = getParams(id);
    if(params) {
        params->height = height;
        emit heightChanged(id, height);
    }
}


float SceneData::getHeight(RectId id) const
{
    const RectParam* params = getParams(id);
    if(params) {
        return params->height;
    }
    return 0;
}


void SceneData::setSelected(RectId id)
{
    selected = id;
    emit rectSelected(id);
}


RectId SceneData::getSelected() const
{
    return selected;
}


void SceneData::setTerrainHeight(int terrainHeight_)
{
    terrainHeight = terrainHeight_;
    emit terrainHeightChanged(terrainHeight);
}


int SceneData::getTerrainHeight() const
{
    return terrainHeight;
}


void SceneData::setTerrainScale(int value)
{
    terrainScale = value;
    emit terrainScaleValueChanged(terrainScale);
}


int SceneData::getTerrainScale() const
{
    return terrainScale;
}


void SceneData::setTerrainWaterHeight(float value)
{
    terrainWaterHeight = value;
    emit terrainWaterHeightValueChanged(terrainWaterHeight);
}


float SceneData::getTerrainWaterHeight() const
{
    return terrainWaterHeight;
}


void SceneData::setTeamsSize(TeamsSize value)
{
    teamsSize = value;
    emit teamsSizeChanged(teamsSize);

}


TeamsSize SceneData::getTeamsSize() const
{
    return teamsSize;
}


void SceneData::moveToTop(RectId id)
{
    int maxOrderValue = 0;
    for(auto it = rects.begin(); it != rects.end(); ++it) {
        if(it.key() != id) {
            if(maxOrderValue < it.value().renderOrder) {
                maxOrderValue = it.value().renderOrder;
            }
        }
    }

    auto it = rects.find(id);
    if(it != rects.end()) {
        it.value().renderOrder = maxOrderValue + 1;
        emit renderOrderChanged(id, it.value().renderOrder);
    }
}


void SceneData::moveToBottom(RectId id)
{
    for(auto it = rects.begin(); it != rects.end(); ++it) {
        if(it.key() != id) {
            it.value().renderOrder += 1;
            emit renderOrderChanged(it.key(), it.value().renderOrder);
        }
    }

    auto it = rects.find(id);
    if(it != rects.end()) {
        it.value().renderOrder = 1;
        emit renderOrderChanged(id, it.value().renderOrder);
    }
}


const QMap<RectId, RectParam>& SceneData::getRects() const
{
    return rects;
}


const QMap<RectId, RectParam>& SceneData::getTeamsRects() const
{
    return teamsRects;
}


void SceneData::terrainHeightValueChanged(int value)
{
    terrainHeight = value;
    emit terrainHeightChanged(terrainHeight);
}


void SceneData::terrainScaleValueChanged(int value)
{
    terrainScale = value;
    emit terrainScaleChanged(terrainScale);
}


void SceneData::terrainWaterHeightValueChanged(double value)
{
    terrainWaterHeight = static_cast<float>(value);
    emit terrainWaterHeightChanged(terrainWaterHeight);
}


void SceneData::teamsSizeIndexChanged(int index)
{
    teamsSize = static_cast<TeamsSize>(index);
    emit teamsSizeChanged(teamsSize);
}


RectParam* SceneData::getParams(RectId id)
{
    {
        auto it = rects.find(id);
        if(it != rects.end()) {
            return &it.value();
        }
    }

    auto it = teamsRects.find(id);
    if(it != teamsRects.end()) {
        return &it.value();
    }

    return nullptr;
}


const RectParam* SceneData::getParams(RectId id) const
{
    {
        auto it = rects.find(id);
        if(it != rects.end()) {
            return &it.value();
        }
    }

    auto it = teamsRects.find(id);
    if(it != teamsRects.end()) {
        return &it.value();
    }

    return nullptr;
}


int SceneData::GetTeamIndex(RectId id)
{
    return  id - (id / 10 * 10) - 1;
}


QString SceneData::writeToJson() const
{
    QJsonObject json;
    json["terrain_height"] = terrainHeight;
    json["terrain_water_height"] = terrainWaterHeight;
    json["terrain_scale"] = terrainScale;
    json["teams_size"] = static_cast<int>(teamsSize);

    QJsonArray rectsArray{};
    for(auto it = rects.begin(); it != rects.end(); ++it) {
        QJsonObject rectObj{};
        rectObj["id"] = static_cast<int>(it.key());
        rectObj["x"] = it->pos.x();
        rectObj["y"] = it->pos.y();
        rectObj["height"] = it->height;
        rectObj["width"] = it->width;
        rectObj["rotation"] = it->rotation;
        rectObj["offset"] = it->offset;
        rectObj["render_order"] = it->renderOrder;
        rectsArray.push_back(rectObj);
    }
    json["rects"] = rectsArray;


    QJsonArray teamsRectsArray{};
    for(auto it = teamsRects.begin(); it != teamsRects.end(); ++it) {
        QJsonObject rectObj{};
        rectObj["id"] = static_cast<int>(it.key());
        rectObj["x"] = it->pos.x();
        rectObj["y"] = it->pos.y();
        rectObj["height"] = it->height;
        rectObj["width"] = it->width;
        rectObj["rotation"] = it->rotation;
        rectObj["offset"] = it->offset;
        teamsRectsArray.push_back(rectObj);
    }
    json["teams_rects"] = teamsRectsArray;

    QJsonDocument doc{json};
    return doc.toJson(QJsonDocument::Indented);
}


void SceneData::save()
{
    if(filePath.isEmpty()) {
        QSettings settings{SETTINGS_FILE, QSettings::IniFormat};
        QString saveDir = settings.value(SETTINGS_KEY_SAVE_FOLDER, "").toString();
        if(saveDir.isEmpty()) {
            saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        }
        filePath = QFileDialog::getSaveFileName(nullptr, "", saveDir, QString{"(*."} + fileExtension + ")");
        if(filePath.isEmpty()) {
            return;
        }
    }

    saveToFile(filePath);
}


void SceneData::saveAs()
{
    QSettings settings{SETTINGS_FILE, QSettings::IniFormat};
    QString saveDir = settings.value(SETTINGS_KEY_SAVE_FOLDER, "").toString();
    if(saveDir.isEmpty()) {
        saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    QString saveFilePath = QFileDialog::getSaveFileName(nullptr, "", saveDir, QString{"(*."} + fileExtension + ")");
    if(saveFilePath.isEmpty()) {
        return;
    }

    saveToFile(saveFilePath);
}


void SceneData::saveToFile(const QString& saveFilePath) const
{
    QFile file(saveFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Can't save file " + saveFilePath, QMessageBox::Ok);
        return;
    }

    QString jsonString = writeToJson();
    QTextStream stream( &file );
    stream << jsonString;
    file.close();
}


void SceneData::readFromJson(QString jsonString)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject json = doc.object();

    terrainHeight = json["terrain_height"].toInt();
    terrainWaterHeight = static_cast<float>(json["terrain_water_height"].toDouble());
    terrainScale = json["terrain_scale"].toInt();
    teamsSize = static_cast<TeamsSize>(json["teams_size"].toInt());

    QJsonArray rectsArray = json["rects"].toArray();
    foreach (const QJsonValue& rectObj, rectsArray) {
        if (rectObj.isObject()) {
            RectId id = rectObj["id"].toInt();
            RectParam rect{};
            rect.pos.setX(rectObj["x"].toDouble());
            rect.pos.setY(rectObj["y"].toDouble());

            rect.height = rectObj["height"].toDouble();
            rect.width = rectObj["width"].toDouble();
            rect.rotation = rectObj["rotation"].toDouble();
            rect.offset = rectObj["offset"].toInt();
            rect.renderOrder = rectObj["render_order"].toInt();

            rects.insert(id, rect);
            if(nextIndex <= id) {
                nextIndex = id + 1;
            }
        }
    }

    QJsonArray teamsRectsArray = json["teams_rects"].toArray();
    foreach (const QJsonValue& rectObj, teamsRectsArray) {
        if (rectObj.isObject()) {
            RectId id = rectObj["id"].toInt();
            RectParam rect{};
            rect.pos.setX(rectObj["x"].toDouble());
            rect.pos.setY(rectObj["y"].toDouble());

            rect.height = rectObj["height"].toDouble();
            rect.width = rectObj["width"].toDouble();
            rect.rotation = rectObj["rotation"].toDouble();
            rect.offset = rectObj["offset"].toInt();

            teamsRects[id] = rect;
        }
    }
}


void SceneData::load()
{
    QSettings settings{SETTINGS_FILE, QSettings::IniFormat};
    QString saveDir = settings.value(SETTINGS_KEY_SAVE_FOLDER, "").toString();
    if(saveDir.isEmpty()) {
        saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    QString newFilePath = QFileDialog::getOpenFileName(nullptr, "", saveDir, QString{"(*."} + fileExtension + ")");
    if(newFilePath.isEmpty()) {
        return;
    }

    QFile file(newFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Can't save file " + newFilePath, QMessageBox::Ok);
        return;
    }

    QString jsonString = file.readAll();
    file.close();

    filePath = newFilePath;
    rects.clear();
    selected = INVALID_RECT_ID;
    nextIndex = 0;

    readFromJson(jsonString);

    emit needReload();
    emit rectSelected(INVALID_RECT_ID);
}


void SceneData::clearAll()
{
    filePath.clear();
    rects.clear();
    selected = INVALID_RECT_ID;
    nextIndex = 0;

    emit needReload();
    emit rectSelected(INVALID_RECT_ID);
}
