#ifndef GRAFICSRECTITEM_H
#define GRAFICSRECTITEM_H


#include <QObject>
#include <QGraphicsRectItem>
#include <sharedsettings.h>
#include <QPen>


class SceneData;


class GraficsRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

    enum class State
    {
        NON,
        POS_CHANGING,
        OFFSET_CHANGING,
        ROTATION_CHANGING,
        BORDER_CHANGING
    };

    enum class Border
    {
        NON,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    static constexpr int BORDER_DETECTED_SIZE = 10;
    static constexpr int BORDER_SIZE = 1;
    static constexpr float MIN_WIDTH = 10;
    static constexpr float MIN_HEIGHT = 10;

public:
    explicit GraficsRectItem(RectId id_, SceneData* data_, const RectParam& rectParam = DEFAULT_RECT, QObject* parent = nullptr);

    RectId getId() const {return id;}

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void SetNeedUpdateColor(bool);

public slots:
    void hideBorder();
    void showBorder();

private slots:
    void offsetChanged(RectId id_, int offset);
    void posChanged(RectId id_, const QPointF& pos);
    void rotationChanged(RectId id_, float rotation);
    void widthChanged(RectId id_, float width);
    void heightChanged(RectId id_, float height);
    void renderOrderChanged(RectId id_, int renderOrder);

    void rectSelected(RectId);

private:
    void updateColor(int offset);
    void showBordersDetection();
    void hideBordersDetection();
    void changeBorder(const QPointF& eventPos);

    QPointF getPosInRect(const QPointF& eventPos) const;

    const RectId id;
    SceneData* data;

    State currentState = State::NON;
    QPointF lastPos{};

    Border detectedBorder = Border::NON;
    Border changingBorder = Border::NON;

    bool isSelected = false;

    QPen savedPen;
    bool borderHided = false;

    bool needUpdateColor = true;
};


class GraficsTeamRectItem : public GraficsRectItem
{
public:
    explicit GraficsTeamRectItem(RectId id_, SceneData* data_, QColor color, const RectParam& rectParam = DEFAULT_RECT, QObject* parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor color;
};

#endif // GRAFICSRECTITEM_H
