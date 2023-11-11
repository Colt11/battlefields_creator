#include "rectpanelmanager.h"
#include <scenedata.h>


RectPanelManager::RectPanelManager(SceneData* data_,
                                   QSpinBox* spinBoxX_,
                                   QSpinBox* spinBoxY_,
                                   QSpinBox* spinBoxRotation_,
                                   QSpinBox* spinBoxWidth_,
                                   QSpinBox* spinBoxHeight_,
                                   QSpinBox* spinBoxOffset_,
                                   QObject* parent)
    : QObject(parent)
    , data{data_}
    , spinBoxX{spinBoxX_}
    , spinBoxY{spinBoxY_}
    , spinBoxRotation{spinBoxRotation_}
    , spinBoxWidth{spinBoxWidth_}
    , spinBoxHeight{spinBoxHeight_}
    , spinBoxOffset{spinBoxOffset_}
{
    connect(data, &SceneData::rectSelected, this, &RectPanelManager::rectSelected);
    connect(data, &SceneData::offsetChanged, this, qOverload<RectId, int>(&RectPanelManager::reload));
    connect(data, &SceneData::posChanged, this, qOverload<RectId, const QPointF&>(&RectPanelManager::reload));
    connect(data, &SceneData::rotationChanged, this, qOverload<RectId, float>(&RectPanelManager::reload));
    connect(data, &SceneData::widthChanged, this, qOverload<RectId, float>(&RectPanelManager::reload));
    connect(data, &SceneData::heightChanged, this, qOverload<RectId, float>(&RectPanelManager::reload));

    connect(this, &RectPanelManager::setEnable, spinBoxX, &QSpinBox::setEnabled);
    connect(this, &RectPanelManager::setEnable, spinBoxY, &QSpinBox::setEnabled);
    connect(this, &RectPanelManager::setEnable, spinBoxRotation, &QSpinBox::setEnabled);
    connect(this, &RectPanelManager::setEnable, spinBoxWidth, &QSpinBox::setEnabled);
    connect(this, &RectPanelManager::setEnable, spinBoxHeight, &QSpinBox::setEnabled);
    connect(this, &RectPanelManager::setEnable, spinBoxOffset, &QSpinBox::setEnabled);

    connect(spinBoxX, qOverload<int>(&QSpinBox::valueChanged), this, &RectPanelManager::xValueChanged);
    connect(spinBoxY, qOverload<int>(&QSpinBox::valueChanged), this, &RectPanelManager::yValueChanged);
    connect(spinBoxRotation, qOverload<int>(&QSpinBox::valueChanged), this, &RectPanelManager::rotationValueChanged);
    connect(spinBoxWidth, qOverload<int>(&QSpinBox::valueChanged), this, &RectPanelManager::widthValueChanged);
    connect(spinBoxHeight, qOverload<int>(&QSpinBox::valueChanged), this, &RectPanelManager::heightValueChanged);
    connect(spinBoxOffset, qOverload<int>(&QSpinBox::valueChanged), this, &RectPanelManager::offsetValueChanged);

    disable();
}


void RectPanelManager::enable()
{
    isEnable = true;
    emit setEnable(isEnable);
}


void RectPanelManager::disable()
{
    isEnable = false;
    emit setEnable(isEnable);
}


void RectPanelManager::rectSelected(RectId id)
{
    currentId = id;

    if(id == INVALID_RECT_ID) {
        disable();
        return;
    }

    reload();
    enable();
}


void RectPanelManager::reload(RectId, int)
{
    reload();
}


void RectPanelManager::reload(RectId, const QPointF&)
{
    reload();
}


void RectPanelManager::reload(RectId, float)
{
    reload();
}


void RectPanelManager::reload()
{
    QPointF pos = data->getPos(currentId);
    spinBoxX->setValue(pos.x());
    spinBoxY->setValue(pos.y());

    spinBoxRotation->setValue(data->getRotation(currentId));
    spinBoxOffset->setValue(data->getOffset(currentId));

    spinBoxWidth->setValue(data->getWidth(currentId));
    spinBoxHeight->setValue(data->getHeight(currentId));
}


void RectPanelManager::xValueChanged(int value)
{
    if(currentId == INVALID_RECT_ID) {
        return;
    }

    QPointF pos = data->getPos(currentId);
    data->setPos(currentId, QPointF{static_cast<qreal>(value), pos.y()});
}


void RectPanelManager::yValueChanged(int value)
{
    if(currentId == INVALID_RECT_ID) {
        return;
    }

    QPointF pos = data->getPos(currentId);
    data->setPos(currentId, QPointF{pos.x(), static_cast<qreal>(value)});
}


void RectPanelManager::rotationValueChanged(int value)
{
    if(currentId == INVALID_RECT_ID) {
        return;
    }

    data->setRotation(currentId, value);
}


void RectPanelManager::offsetValueChanged(int value)
{
    if(currentId == INVALID_RECT_ID) {
        return;
    }

    data->setOffset(currentId, value);
}


void RectPanelManager::widthValueChanged(int value)
{
    if(currentId == INVALID_RECT_ID) {
        return;
    }

    data->setWidth(currentId, value);
}


void RectPanelManager::heightValueChanged(int value)
{
    if(currentId == INVALID_RECT_ID) {
        return;
    }

    data->setHeight(currentId, value);
}

