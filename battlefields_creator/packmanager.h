#ifndef PACKMANAGER_H
#define PACKMANAGER_H


#include <QObject>
#include <QThread>
#include <QImage>


class SceneData;


class PackManager : public QThread
{
    Q_OBJECT
public:
    explicit PackManager(SceneData* sceneData_, QImage image_, QObject *parent = nullptr);
    ~PackManager();

    void run();

signals:
    void resultReady(const QString &result);

private:
    bool loadPaths();
    bool fillTmpFolder();
    bool generate() const;
    bool pack() const;
    void installPack() const;
    bool patchXmls() const;

    bool patchHeightMapXmls() const;
    bool patchDeploymentAreasXmls() const;

    void copyPath(QString src, QString dst) const;

    QThread thread;

    QImage image;

    QString packManagerPath;
    QString gameFolderPath;
    QString tmpPath;
    QString terrainDataFolderPath;
    bool needInstall = true;
    bool needRemoveTmpFolder = true;

    SceneData* sceneData = nullptr;

    mutable QString errorString;
};

#endif // PACKMANAGER_H
