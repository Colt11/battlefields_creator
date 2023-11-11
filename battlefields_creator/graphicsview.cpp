#include "graphicsview.h"
#include <scenedata.h>
#include <graficsrectitem.h>
#include <QEvent>


void GraphicsView::enterEvent(QEvent* e)
{
    QGraphicsView::enterEvent(e);
    viewport()->setCursor(Qt::CrossCursor);

    this->installEventFilter(this);
}


void GraphicsView::mouseReleaseEvent(QMouseEvent* e)
{
    QGraphicsView::mouseReleaseEvent(e);
    viewport()->setCursor(Qt::CrossCursor);
}


void GraphicsView::mouseMoveEvent(QMouseEvent* e)
{
    QGraphicsView::mouseMoveEvent(e);
}


bool GraphicsView::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}
