#ifndef SCENEDATA_H
#define SCENEDATA_H


#include <QObject>
#include <QMap>
#include <optional>
#include <sharedsettings.h>


class SceneData : public QObject
{
    Q_OBJECT

public:
    explicit SceneData(QObject* parent = nullptr);

    void setOffset(RectId id, int offset);
    int getOffset(RectId id) const;

    void setPos(RectId id, const QPointF& pos);
    QPointF getPos(RectId id) const;

    void setRotation(RectId id, float rotation);
    float getRotation(RectId id) const;

    void setWidth(RectId id, float width);
    float getWidth(RectId id) const;

    void setHeight(RectId id, float height);
    float getHeight(RectId id) const;

    void setSelected(RectId);
    RectId getSelected() const;

    void setTerrainHeight(int);
    int getTerrainHeight() const;

    void setTerrainScale(int);
    int getTerrainScale() const;

    void setTerrainWaterHeight(float);
    float getTerrainWaterHeight() const;

    void setTeamsSize(TeamsSize);
    TeamsSize getTeamsSize() const;

    void moveToTop(RectId);
    void moveToBottom(RectId);

    const QMap<RectId, RectParam>& getRects() const;
    const QMap<RectId, RectParam>& getTeamsRects() const;

    static int GetTeamIndex(RectId);

signals:
    void rectAdded(RectId);
    void rectRemoved(RectId);

    void offsetChanged(RectId, int);
    void posChanged(RectId, const QPointF&);
    void rotationChanged(RectId, float);
    void widthChanged(RectId, float);
    void heightChanged(RectId, float);
    void renderOrderChanged(RectId, int);

    void rectSelected(RectId);

    void terrainHeightChanged(int);
    void terrainScaleChanged(int);
    void terrainWaterHeightChanged(float);

    void teamsSizeChanged(TeamsSize);

    void needReload();

public slots:
    void addRect();
    void removeRect();

    void save();
    void saveAs();
    void load();
    void clearAll();

    void terrainHeightValueChanged(int);
    void terrainScaleValueChanged(int);
    void terrainWaterHeightValueChanged(double);

    void teamsSizeIndexChanged(int);

private:
    RectParam* getParams(RectId id);
    const RectParam* getParams(RectId id) const;

    QString writeToJson() const;
    void readFromJson(QString);

    void saveToFile(const QString& saveFilePath) const;

    RectId nextIndex = 1;
    QMap<RectId, RectParam> rects;
    QMap<RectId, RectParam> teamsRects;
    RectId selected = INVALID_RECT_ID;
    int terrainHeight = 128;
    int terrainScale = 64;
    float terrainWaterHeight = 12;
    TeamsSize teamsSize = TeamsSize::TEAMS_SIZE_1x1;

    QString filePath{};
    const QString fileExtension = "bfd";
};

#endif // SCENEDATA_H
