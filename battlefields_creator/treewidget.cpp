#include "treewidget.h"
#include <scenedata.h>


TreeWidget::TreeWidget(QWidget* parent)
    : QTreeWidget(parent)
{
    terrainRootItem = new QTreeWidgetItem();
    terrainRootItem->setText(0, "Terrain elements");
    addTopLevelItem(terrainRootItem);
}


void TreeWidget::setData(SceneData* data_)
{
    data = data_;

    connect(data, &SceneData::rectAdded, this, &TreeWidget::rectAdded);
    connect(data, &SceneData::rectRemoved, this, &TreeWidget::rectRemoved);
    connect(data, &SceneData::rectSelected, this, &TreeWidget::rectSelected);
    connect(data, &SceneData::needReload, this, &TreeWidget::reload);
    connect(this, &QTreeWidget::itemSelectionChanged, this, &TreeWidget::itemSelectionChanged);
}


void TreeWidget::reload()
{
    selectedGuard = true;
    for(auto it = idToItems.begin(); it != idToItems.end(); ++it) {
        int index = terrainRootItem->indexOfChild(it.value());
        it.value()->setSelected(false);
        delete terrainRootItem->takeChild(index);
    }
    idToItems.clear();
    itemsToId.clear();

    QList<RectId> idList = data->getRects().keys();
    for(auto& id : idList) {
        rectAdded(id);
    }
    selectedGuard = false;
}


void TreeWidget::rectAdded(RectId id)
{
    QTreeWidgetItem* rectItem = new QTreeWidgetItem(terrainRootItem);
    rectItem->setText(0, QString::number(id));
    addTopLevelItem(rectItem);
    idToItems.insert(id, rectItem);
    itemsToId.insert(rectItem, id);
}


void TreeWidget::rectRemoved(RectId id)
{
    auto it = idToItems.find(id);
    if(it == idToItems.end()) {
        return;
    }

    int index = terrainRootItem->indexOfChild(it.value());
    delete terrainRootItem->takeChild(index);

    idToItems.remove(id);
    itemsToId.remove(it.value());
}


void TreeWidget::rectSelected(RectId id)
{
    auto it = idToItems.find(id);
    if(it == idToItems.end()) {
        return;
    }
    selectedGuard = true;
    QList<QTreeWidgetItem*> items = selectedItems();
    for(auto& item : items) {
        if(item != it.value()) {
            item->setSelected(false);
        }
    }

    it.value()->setSelected(true);

    selectedGuard = false;
}


void TreeWidget::itemSelectionChanged()
{
    if(selectedGuard) {
        return;
    }

    QList<QTreeWidgetItem*> items = selectedItems();
    if(items.empty()) {
        data->setSelected(INVALID_RECT_ID);
    }

    QTreeWidgetItem* selectedItem =  items.front();
    auto it = itemsToId.find(selectedItem);
    if(it != itemsToId.end()) {
        data->setSelected(it.value());
    }
}
