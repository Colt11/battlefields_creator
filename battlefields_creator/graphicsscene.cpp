#include "graphicsscene.h"
#include "scenedata.h"
#include "graficsrectitem.h"
#include <QPainter>
#include <QBrush>



GraphicsScene::GraphicsScene(QWidget* parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::darkGreen);
}


void GraphicsScene::setData(SceneData* data_)
{
    data = data_;

    connect(data, &SceneData::rectAdded, this, &GraphicsScene::rectAdded);
    connect(data, &SceneData::rectRemoved, this, &GraphicsScene::rectRemoved);
    connect(data, &SceneData::needReload, this, &GraphicsScene::reload);
    connect(data, &SceneData::teamsSizeChanged, this, &GraphicsScene::teamsSizeChanged);

    reinitBorders();
    reinitTeamsRects();
}


void GraphicsScene::rectAdded(RectId id)
{
    GraficsRectItem *rect = new GraficsRectItem(id, data);
    connect(this, &GraphicsScene::hideBorders, rect, &GraficsRectItem::hideBorder);
    connect(this, &GraphicsScene::showBorders, rect, &GraficsRectItem::showBorder);
    rectsToItem.insert(id, rect);
    addItem(rect);
}


void GraphicsScene::rectRemoved(RectId id)
{
    auto it = rectsToItem.find(id);
    if(it == rectsToItem.end()) {
        return;
    }

    removeItem(it.value());
    rectsToItem.remove(id);
}


void GraphicsScene::reload()
{
    clear();
    rectsToItem.clear();

    reinitBorders();
    reinitTeamsRects();

    const QMap<RectId, RectParam>& rects = data->getRects();
    for(auto it = rects.begin(); it != rects.end(); ++it) {
        GraficsRectItem *rect = new GraficsRectItem(it.key(), data, it.value());
        connect(this, &GraphicsScene::hideBorders, rect, &GraficsRectItem::hideBorder);
        connect(this, &GraphicsScene::showBorders, rect, &GraficsRectItem::showBorder);
        rectsToItem.insert(it.key(), rect);
        addItem(rect);
    }
}


void GraphicsScene::teamsSizeChanged(TeamsSize teamsSize)
{
    mapBorders[activeTeamsSize]->hide();
    activeTeamsSize = teamsSize;
    mapBorders[activeTeamsSize]->show();

    for(auto it = teamsRectsToItem.begin(); it != teamsRectsToItem.end(); ++it) {
        int teamIndex = SceneData::GetTeamIndex(it.key());
        if(teamIndex <= activeTeamsSize) {
            if(!teamsRectsIsHided) {
                it.value()->show();
            }
        } else {
            it.value()->hide();
        }
    }
}


void GraphicsScene::reinitBorders()
{
    QPen currentPen{};
    currentPen.setStyle(Qt::DashDotDotLine);
    currentPen.setColor(Qt::black);
    currentPen.setWidth(10);

    mapBorders[TEAMS_SIZE_1x1] = new QGraphicsRectItem(QRect(225, 225, 775, 575));
    mapBorders[TEAMS_SIZE_2x2] = new QGraphicsRectItem(QRect(25, 230, 975, 735));
    mapBorders[TEAMS_SIZE_3x3] = new QGraphicsRectItem(QRect(25, 230, 975, 775));

    for(auto& mapBorder : mapBorders) {
        mapBorder->setPen(currentPen);
        addItem(mapBorder);
        mapBorder->hide();
    }
    activeTeamsSize = data->getTeamsSize();
    mapBorders[activeTeamsSize]->show();
}


void GraphicsScene::reinitTeamsRects()
{
    teamsRectsToItem.clear();

    const QMap<RectId, RectParam>& rects = data->getTeamsRects();
    for(auto it = rects.begin(); it != rects.end(); ++it) {
        QColor color = it.key() < 2000 ? Qt::magenta : Qt::yellow;
        GraficsRectItem *rect = new GraficsTeamRectItem(it.key(), data, color, it.value());
        teamsRectsToItem.insert(it.key(), rect);
        addItem(rect);
        int teamIndex = SceneData::GetTeamIndex(it.key());
        if(teamIndex > activeTeamsSize) {
            rect->hide();
        }
    }
}


void GraphicsScene::hideRectBorders()
{
    emit hideBorders();
    for(auto it = teamsRectsToItem.begin(); it != teamsRectsToItem.end(); ++it) {
        it.value()->hide();
    }
}


void GraphicsScene::showRectBorders()
{
    emit showBorders();
    if(teamsRectsIsHided) {
        return;
    }

    for(auto it = teamsRectsToItem.begin(); it != teamsRectsToItem.end(); ++it) {
        int teamIndex = SceneData::GetTeamIndex(it.key());
        if(teamIndex <= activeTeamsSize) {
            it.value()->show();
        }
    }
}


void GraphicsScene::hideTeamsRectsValueChenged(int state)
{
    teamsRectsIsHided = state == Qt::Checked;
    if(teamsRectsIsHided) {
        for(auto it = teamsRectsToItem.begin(); it != teamsRectsToItem.end(); ++it) {
            it.value()->hide();
        }
    } else {
        for(auto it = teamsRectsToItem.begin(); it != teamsRectsToItem.end(); ++it) {
            int teamIndex = SceneData::GetTeamIndex(it.key());
            if(teamIndex <= activeTeamsSize) {
                it.value()->show();
            }
        }
    }
}
