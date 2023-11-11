#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H


#include <QWidget>
#include <QGraphicsView>


class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsView(QWidget* parent  = nullptr)
        : QGraphicsView(parent)
    {}

protected:
    void enterEvent(QEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // GRAPHICSVIEW_H
