#ifndef TREEWIDGET_H
#define TREEWIDGET_H


#include <QObject>
#include <QTreeWidget>
#include <QMap>
#include <sharedsettings.h>


class SceneData;


class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    TreeWidget(QWidget* parent = nullptr);

    void setData(SceneData* data_);


private slots:
    void rectAdded(RectId);
    void rectRemoved(RectId);
    void rectSelected(RectId);

    void itemSelectionChanged();
    void reload();

private:
    SceneData* data = nullptr;

    QTreeWidgetItem* terrainRootItem = nullptr;

    QMap<RectId, QTreeWidgetItem*> idToItems{};
    QMap<QTreeWidgetItem*, RectId> itemsToId{};

    bool selectedGuard = false;
};

#endif // TREEWIDGET_H
