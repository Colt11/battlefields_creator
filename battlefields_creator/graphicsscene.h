#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QWidget>
#include <QGraphicsScene>
#include <sharedsettings.h>
#include <QMap>


class SceneData;
class GraficsRectItem;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(QWidget* parent = nullptr);

    void setData(SceneData* data_);

    void hideRectBorders();
    void showRectBorders();

public slots:
    void hideTeamsRectsValueChenged(int);

private slots:
    void rectAdded(RectId);
    void rectRemoved(RectId);
    void teamsSizeChanged(TeamsSize);
    void reload();

signals:
    void hideBorders();
    void showBorders();

private:
    void reinitBorders();
    void reinitTeamsRects();

    SceneData* data = nullptr;

    QMap<RectId, GraficsRectItem*> rectsToItem;
    QMap<RectId, GraficsRectItem*> teamsRectsToItem;

    QGraphicsRectItem* mapBorders[TEAMS_SIZE_COUNT];
    TeamsSize activeTeamsSize = TEAMS_SIZE_1x1;

    bool teamsRectsIsHided = false;
};

#endif // GRAPHICSSCENE_H
