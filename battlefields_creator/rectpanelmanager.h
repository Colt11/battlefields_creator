#ifndef RECTPANELMANAGER_H
#define RECTPANELMANAGER_H


#include <QObject>
#include <QSpinBox>
#include <sharedsettings.h>


class SceneData;


class RectPanelManager : public QObject
{
    Q_OBJECT

public:
    RectPanelManager(SceneData* data_,
                     QSpinBox* spinBoxX,
                     QSpinBox* spinBoxY,
                     QSpinBox* spinBoxRotation,
                     QSpinBox* spinBoxWidth,
                     QSpinBox* spinBoxHeight,
                     QSpinBox* spinBoxOffset,
                     QObject* parent = nullptr);

private slots:
    void rectSelected(RectId);

    void xValueChanged(int value);
    void yValueChanged(int value);
    void rotationValueChanged(int value);
    void offsetValueChanged(int value);
    void widthValueChanged(int value);
    void heightValueChanged(int value);

    void reload(RectId, int);
    void reload(RectId, const QPointF&);
    void reload(RectId, float);

signals:
    void setEnable(bool);

private:
    void enable();
    void disable();
    void reload();

    SceneData* data = nullptr;
    QSpinBox* spinBoxX = nullptr;
    QSpinBox* spinBoxY = nullptr;
    QSpinBox* spinBoxRotation = nullptr;
    QSpinBox* spinBoxWidth = nullptr;
    QSpinBox* spinBoxHeight = nullptr;
    QSpinBox* spinBoxOffset = nullptr;

    bool isEnable = false;

    RectId currentId = INVALID_RECT_ID;
};

#endif // RECTPANELMANAGER_H
