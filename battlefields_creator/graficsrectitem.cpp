#include "graficsrectitem.h"
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QVector2D>
#include <QPainter>
#include <scenedata.h>


GraficsRectItem::GraficsRectItem(RectId id_, SceneData* data_, const RectParam &rectParam, QObject* parent)
    : id(id_)
    , data(data_)
    , QObject(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);

    setRect(rectParam.width / -2.0f, rectParam.height / -2.0f, rectParam.width, rectParam.height);

    setPos(rectParam.pos);
    data->setPos(id, rectParam.pos);
    setRotation(rectParam.rotation);
    updateColor(rectParam.offset);
    setZValue(rectParam.renderOrder < 0 ? id : rectParam.renderOrder);

    connect(data, &SceneData::offsetChanged, this, &GraficsRectItem::offsetChanged);
    connect(data, &SceneData::posChanged, this, &GraficsRectItem::posChanged);
    connect(data, &SceneData::rotationChanged, this, &GraficsRectItem::rotationChanged);
    connect(data, &SceneData::widthChanged, this, &GraficsRectItem::widthChanged);
    connect(data, &SceneData::heightChanged, this, &GraficsRectItem::heightChanged);
    connect(data, &SceneData::rectSelected, this, &GraficsRectItem::rectSelected);
    connect(data, &SceneData::renderOrderChanged, this, &GraficsRectItem::renderOrderChanged);

    update();
}


void GraficsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(currentState == State::OFFSET_CHANGING) {
        QPointF currentPos = event->scenePos();

        int currentOffset = data->getOffset(id);
        currentOffset += lastPos.y() - currentPos.y();
        data->setOffset(id, currentOffset);

        lastPos = currentPos;
    }

    if(currentState == State::POS_CHANGING) {
        QPointF scenePos = event->scenePos();
        QPointF newPos = data->getPos(id) + (scenePos - lastPos);
        data->setPos(id, newPos);
        lastPos = scenePos;
    }

    if(currentState == State::ROTATION_CHANGING) {
        QVector2D direction = QVector2D{event->scenePos() - mapToScene(rect().center())};
        direction.normalize();

        QVector2D lastDirection = QVector2D{lastPos - mapToScene(rect().center())};
        lastDirection.normalize();

        float dot = QVector2D::dotProduct(lastDirection, direction);
        float det = lastDirection.x() * direction.y() - lastDirection.y() * direction.x();
        float angleInRadians = atan2(det, dot);
        float angleInDegrees = angleInRadians * 57.2958f;

        float newRotation = data->getRotation(id) + angleInDegrees;
        data->setRotation(id, newRotation);
        lastPos = event->scenePos();
    }

    if(currentState == State::BORDER_CHANGING) {
        changeBorder(event->pos());
    }

    QGraphicsItem::mouseMoveEvent(event);
}


void GraficsRectItem::changeBorder(const QPointF& eventPos)
{
    QTransform rotationTransform = QTransform().rotate(data->getRotation(id) * 1);
    QTransform backRotationTransform = QTransform().rotate(data->getRotation(id) * -1);

    int direction = (changingBorder == Border::LEFT || changingBorder == Border::TOP) ? -1 : 1;
    QPointF oldCentrInScene = mapToScene(rect().center());
    QPointF centrToPos = direction * (mapToScene(eventPos) - oldCentrInScene);
    centrToPos = backRotationTransform.map(centrToPos);

    QPointF delta{0, 0};

    if(changingBorder == Border::LEFT || changingBorder == Border::RIGHT) {
        float halfOldfWidth = data->getWidth(id) / 2;
        float newWidth = qRound((centrToPos.x() + halfOldfWidth));

        if(MIN_WIDTH > newWidth) {
            return;
        }
        data->setWidth(id, newWidth); // centr will chanded

        delta = QPointF {newWidth / 2.0f - halfOldfWidth, 0.0f} *  direction;
    } else if (changingBorder == Border::TOP || changingBorder == Border::BOTTOM){
        float halfOldfHeight = data->getHeight(id) / 2;
        float newHeight = qRound((centrToPos.y() + halfOldfHeight));

        if(MIN_HEIGHT > newHeight) {
            return;
        }
        data->setHeight(id, newHeight); // centr will chanded

        delta = QPointF {0.0f, newHeight / 2.0f - halfOldfHeight} *  direction;
    }

    delta = rotationTransform.map(delta);
    data->setPos(id, oldCentrInScene + delta);
}


void GraficsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::MiddleButton) {
        currentState = State::OFFSET_CHANGING;
        lastPos = event->scenePos();
    } else if(event->button() & Qt::RightButton) {
        currentState = State::POS_CHANGING;
        lastPos = event->scenePos();
    } else if((event->button() & Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier)) {
        currentState = State::ROTATION_CHANGING;
        QVector2D direction = QVector2D(getPosInRect(event->pos()));
        direction.normalize();
        lastPos = event->scenePos();
    } else if((event->button() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier)) {
        data->moveToTop(id);
    }else if((event->button() & Qt::LeftButton) && (event->modifiers() & Qt::AltModifier)) {
        data->moveToBottom(id);
    } else if((event->button() & Qt::LeftButton) && (detectedBorder != Border::NON)) {
        currentState = State::BORDER_CHANGING;
        changingBorder = detectedBorder;
    }

    if(!isSelected) {
        data->setSelected(id);
    }

    QGraphicsItem::mousePressEvent(event);
    event->accept();
}


void GraficsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::MiddleButton && currentState == State::OFFSET_CHANGING) {
        currentState = State::NON;
    }
    if (event->button() & Qt::RightButton && currentState == State::POS_CHANGING) {
        currentState = State::NON;
    }
    if (event->button() & Qt::LeftButton && currentState == State::ROTATION_CHANGING) {
        currentState = State::NON;
    }
    if (event->button() & Qt::LeftButton && currentState == State::BORDER_CHANGING) {
        currentState = State::NON;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}


void GraficsRectItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int currentOffset = data->getOffset(id);
    data->setOffset(id, currentOffset + static_cast<float>(event->delta()) / 10.0f);

    QGraphicsItem::wheelEvent(event);
}


QPointF GraficsRectItem::getPosInRect(const QPointF& eventPos) const
{
    QPointF posInRect = eventPos - rect().center();
    posInRect = posInRect + QPointF{data->getWidth(id), data->getHeight(id)} / 2;
    return posInRect;
}


void GraficsRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    QPointF posInRect = getPosInRect(event->pos());

    if(posInRect.x() < BORDER_DETECTED_SIZE) {
        detectedBorder = Border::LEFT;
        showBordersDetection();
    } else if(posInRect.y() < BORDER_DETECTED_SIZE) {
        detectedBorder = Border::TOP;
        showBordersDetection();
    } else if(posInRect.x() > data->getWidth(id) - BORDER_DETECTED_SIZE) {
        detectedBorder = Border::RIGHT;
        showBordersDetection();
    } else if(posInRect.y() > data->getHeight(id) - BORDER_DETECTED_SIZE){
        detectedBorder = Border::BOTTOM;
        showBordersDetection();
    } else {
        detectedBorder = Border::NON;
        hideBordersDetection();
    }

    QGraphicsRectItem::hoverMoveEvent(event);
}


void GraficsRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    detectedBorder = Border::NON;
    hideBordersDetection();

    QGraphicsRectItem::hoverLeaveEvent(event);
}


void GraficsRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsRectItem::hoverEnterEvent(event);
}


void GraficsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
    if(borderHided) {
        return;
    }

    QPen currentPen = painter->pen();
    QFontMetrics fm = painter->fontMetrics();

    QString text = QString::number(id);
    QPointF posOffset = QPointF{static_cast<float>(fm.width(text)) / 2.0f, 0.0f};

    painter->setPen(QPen(Qt::black, 1));
    painter->drawText(rect().center() - posOffset, text);

    painter->setPen(currentPen);
}


void GraficsRectItem::SetNeedUpdateColor(bool value)
{
    needUpdateColor = value;
}


void GraficsRectItem::updateColor(int offset)
{
    if(!needUpdateColor) {
        return;
    }

    QColor color{0, 0, 0};
    QPen currentPen = pen();
    if(offset > 0){
        currentPen.setColor(Qt::red);
        color.setRed(qRound((static_cast<float>(offset) / SCENE_MAX_OFFSET) * 255.f));
        color.setGreen(qRound((255.f - color.red()) / 2));
    } else {
        currentPen.setColor(Qt::blue);
        color.setBlue(qRound((static_cast<float>(offset) / SCENE_MIN_OFFSET) * 255.f));
        color.setGreen(qRound((255.f - color.blue()) / 2));
    }
    currentPen.setWidth(BORDER_SIZE);
    setPen(currentPen);
    setBrush(color);
}


void GraficsRectItem::offsetChanged(RectId id_, int offset)
{
    if(id_ == id) {
        updateColor(offset);
        update();
    }
}


void GraficsRectItem::posChanged(RectId id_, const QPointF &pos)
{
    if(id_ == id) {
        setPos(pos);
        update();
    }
}


void GraficsRectItem::rotationChanged(RectId id_, float rotation)
{
    if(id_ == id) {
        setTransformOriginPoint(rect().center());
        setRotation(rotation);
        update();
    }
}


void GraficsRectItem::widthChanged(RectId id_, float width)
{
    if(id_ == id) {
        QRectF currentRect = rect();
        currentRect.setWidth(width);
        currentRect.setX(width / -2.0f);
        setRect(currentRect);
        update();
    }
}


void GraficsRectItem::heightChanged(RectId id_, float height)
{
    if(id_ == id) {
        QRectF currentRect = rect();
        currentRect.setHeight(height);
        currentRect.setY(height / -2.0f);
        setRect(currentRect);
        update();
    }
}


void GraficsRectItem::renderOrderChanged(RectId id_, int renderOrder)
{
    if(id_ == id) {
        setZValue(renderOrder);
        update();
    }
}


void GraficsRectItem::rectSelected(RectId id_)
{
    if(id_ == id) {
        QPen currentPen = pen();
        currentPen.setStyle(Qt::DashLine);
        setPen(currentPen);
        isSelected = true;
    } else if(isSelected ){
        QPen currentPen = pen();
        currentPen.setStyle(Qt::SolidLine);
        setPen(currentPen);
        isSelected = false;
    }
}


void GraficsRectItem::hideBorder()
{
    savedPen = pen();
    setPen(Qt::NoPen);
    borderHided = true;
}


void GraficsRectItem::showBorder()
{
    setPen(savedPen);
    borderHided = false;
}


void GraficsRectItem::showBordersDetection()
{
    QPen currentPen = pen();
    currentPen.setWidth(BORDER_DETECTED_SIZE);
    setPen(currentPen);
}


void GraficsRectItem::hideBordersDetection()
{
    QPen currentPen = pen();
    currentPen.setWidth(BORDER_SIZE);
    setPen(currentPen);
}


GraficsTeamRectItem::GraficsTeamRectItem(RectId id_, SceneData* data_, QColor color_, const RectParam& rectParam, QObject* parent)
    : GraficsRectItem(id_, data_, rectParam, parent)
    , color{color_}
{
    SetNeedUpdateColor(false);
    setBrush(color);
}


void GraficsTeamRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
    QPen currentPen = painter->pen();
    QFontMetrics fm = painter->fontMetrics();

    QString text = QString::number(getId());
    QPointF posOffset = QPointF{static_cast<float>(fm.width(text.replace("00", "_"))) / 2.0f, 0.0f};

    painter->setPen(QPen(Qt::black, 1));
    painter->drawText(rect().center() - posOffset, text);

    painter->setPen(currentPen);

}

