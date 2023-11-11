/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "graphicsview.h"
#include "treewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    GraphicsView *graphicsView;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelTerrainHeight;
    QSpinBox *spinBoxTerrainHeight;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *labelTeamsSize;
    QComboBox *comboBoxTeamsSize;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBoxHideTeamsRects;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelTerrainScale;
    QSpinBox *spinBoxTerrainScale;
    QSpacerItem *horizontalSpacer_3;
    QLabel *labelWaterHeight;
    QDoubleSpinBox *doubleSpinBoxWaterHeight;
    QPushButton *createButton;
    TreeWidget *treeWidget;
    QLabel *label;
    QPushButton *pushButtonClear;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelX;
    QSpinBox *spinBoxX;
    QLabel *labelY;
    QSpinBox *spinBoxY;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelWidth;
    QSpinBox *spinBoxWidth;
    QLabel *labelHeight;
    QSpinBox *spinBoxHeight;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelRotation;
    QSpinBox *spinBoxRotation;
    QLabel *labelOffset;
    QSpinBox *spinBoxOffset;
    QHBoxLayout *horizontalLayout;
    QPushButton *addButton;
    QPushButton *removeButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        graphicsView = new GraphicsView(splitter);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(10);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        splitter->addWidget(graphicsView);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelTerrainHeight = new QLabel(verticalLayoutWidget);
        labelTerrainHeight->setObjectName(QString::fromUtf8("labelTerrainHeight"));

        horizontalLayout_2->addWidget(labelTerrainHeight);

        spinBoxTerrainHeight = new QSpinBox(verticalLayoutWidget);
        spinBoxTerrainHeight->setObjectName(QString::fromUtf8("spinBoxTerrainHeight"));
        spinBoxTerrainHeight->setMinimum(1);
        spinBoxTerrainHeight->setMaximum(254);
        spinBoxTerrainHeight->setValue(128);

        horizontalLayout_2->addWidget(spinBoxTerrainHeight);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        labelTeamsSize = new QLabel(verticalLayoutWidget);
        labelTeamsSize->setObjectName(QString::fromUtf8("labelTeamsSize"));

        horizontalLayout_7->addWidget(labelTeamsSize);

        comboBoxTeamsSize = new QComboBox(verticalLayoutWidget);
        comboBoxTeamsSize->addItem(QString());
        comboBoxTeamsSize->addItem(QString());
        comboBoxTeamsSize->addItem(QString());
        comboBoxTeamsSize->setObjectName(QString::fromUtf8("comboBoxTeamsSize"));

        horizontalLayout_7->addWidget(comboBoxTeamsSize);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        checkBoxHideTeamsRects = new QCheckBox(verticalLayoutWidget);
        checkBoxHideTeamsRects->setObjectName(QString::fromUtf8("checkBoxHideTeamsRects"));

        horizontalLayout_7->addWidget(checkBoxHideTeamsRects);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        labelTerrainScale = new QLabel(verticalLayoutWidget);
        labelTerrainScale->setObjectName(QString::fromUtf8("labelTerrainScale"));

        horizontalLayout_6->addWidget(labelTerrainScale);

        spinBoxTerrainScale = new QSpinBox(verticalLayoutWidget);
        spinBoxTerrainScale->setObjectName(QString::fromUtf8("spinBoxTerrainScale"));
        spinBoxTerrainScale->setMinimum(1);
        spinBoxTerrainScale->setMaximum(255);
        spinBoxTerrainScale->setValue(64);

        horizontalLayout_6->addWidget(spinBoxTerrainScale);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        labelWaterHeight = new QLabel(verticalLayoutWidget);
        labelWaterHeight->setObjectName(QString::fromUtf8("labelWaterHeight"));

        horizontalLayout_6->addWidget(labelWaterHeight);

        doubleSpinBoxWaterHeight = new QDoubleSpinBox(verticalLayoutWidget);
        doubleSpinBoxWaterHeight->setObjectName(QString::fromUtf8("doubleSpinBoxWaterHeight"));
        doubleSpinBoxWaterHeight->setMinimum(-4.000000000000000);
        doubleSpinBoxWaterHeight->setMaximum(255.000000000000000);
        doubleSpinBoxWaterHeight->setSingleStep(1.000000000000000);
        doubleSpinBoxWaterHeight->setValue(12.000000000000000);

        horizontalLayout_6->addWidget(doubleSpinBoxWaterHeight);


        verticalLayout->addLayout(horizontalLayout_6);

        createButton = new QPushButton(verticalLayoutWidget);
        createButton->setObjectName(QString::fromUtf8("createButton"));

        verticalLayout->addWidget(createButton);

        treeWidget = new TreeWidget(verticalLayoutWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->header()->setVisible(false);

        verticalLayout->addWidget(treeWidget);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        pushButtonClear = new QPushButton(verticalLayoutWidget);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));

        verticalLayout->addWidget(pushButtonClear);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        labelX = new QLabel(verticalLayoutWidget);
        labelX->setObjectName(QString::fromUtf8("labelX"));

        horizontalLayout_3->addWidget(labelX);

        spinBoxX = new QSpinBox(verticalLayoutWidget);
        spinBoxX->setObjectName(QString::fromUtf8("spinBoxX"));
        spinBoxX->setMinimum(-1025);
        spinBoxX->setMaximum(2050);

        horizontalLayout_3->addWidget(spinBoxX);

        labelY = new QLabel(verticalLayoutWidget);
        labelY->setObjectName(QString::fromUtf8("labelY"));

        horizontalLayout_3->addWidget(labelY);

        spinBoxY = new QSpinBox(verticalLayoutWidget);
        spinBoxY->setObjectName(QString::fromUtf8("spinBoxY"));
        spinBoxY->setMinimum(-1025);
        spinBoxY->setMaximum(2050);

        horizontalLayout_3->addWidget(spinBoxY);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        labelWidth = new QLabel(verticalLayoutWidget);
        labelWidth->setObjectName(QString::fromUtf8("labelWidth"));

        horizontalLayout_5->addWidget(labelWidth);

        spinBoxWidth = new QSpinBox(verticalLayoutWidget);
        spinBoxWidth->setObjectName(QString::fromUtf8("spinBoxWidth"));
        spinBoxWidth->setMinimum(10);
        spinBoxWidth->setMaximum(1025);

        horizontalLayout_5->addWidget(spinBoxWidth);

        labelHeight = new QLabel(verticalLayoutWidget);
        labelHeight->setObjectName(QString::fromUtf8("labelHeight"));

        horizontalLayout_5->addWidget(labelHeight);

        spinBoxHeight = new QSpinBox(verticalLayoutWidget);
        spinBoxHeight->setObjectName(QString::fromUtf8("spinBoxHeight"));
        spinBoxHeight->setMinimum(10);
        spinBoxHeight->setMaximum(1025);

        horizontalLayout_5->addWidget(spinBoxHeight);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        labelRotation = new QLabel(verticalLayoutWidget);
        labelRotation->setObjectName(QString::fromUtf8("labelRotation"));

        horizontalLayout_4->addWidget(labelRotation);

        spinBoxRotation = new QSpinBox(verticalLayoutWidget);
        spinBoxRotation->setObjectName(QString::fromUtf8("spinBoxRotation"));
        spinBoxRotation->setMinimum(-359);
        spinBoxRotation->setMaximum(359);

        horizontalLayout_4->addWidget(spinBoxRotation);

        labelOffset = new QLabel(verticalLayoutWidget);
        labelOffset->setObjectName(QString::fromUtf8("labelOffset"));

        horizontalLayout_4->addWidget(labelOffset);

        spinBoxOffset = new QSpinBox(verticalLayoutWidget);
        spinBoxOffset->setObjectName(QString::fromUtf8("spinBoxOffset"));
        spinBoxOffset->setMinimum(-255);
        spinBoxOffset->setMaximum(255);

        horizontalLayout_4->addWidget(spinBoxOffset);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout->addLayout(verticalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addButton = new QPushButton(verticalLayoutWidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        horizontalLayout->addWidget(addButton);

        removeButton = new QPushButton(verticalLayoutWidget);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));

        horizontalLayout->addWidget(removeButton);


        verticalLayout->addLayout(horizontalLayout);

        splitter->addWidget(verticalLayoutWidget);

        gridLayout->addWidget(splitter, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 21));
        menuBar->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        comboBoxTeamsSize->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        labelTerrainHeight->setText(QApplication::translate("MainWindow", "Terrain relative height", nullptr));
        labelTeamsSize->setText(QApplication::translate("MainWindow", "Teams size", nullptr));
        comboBoxTeamsSize->setItemText(0, QApplication::translate("MainWindow", "1x1", nullptr));
        comboBoxTeamsSize->setItemText(1, QApplication::translate("MainWindow", "2x2", nullptr));
        comboBoxTeamsSize->setItemText(2, QApplication::translate("MainWindow", "3x3", nullptr));

        checkBoxHideTeamsRects->setText(QApplication::translate("MainWindow", "Hide teams rects", nullptr));
        labelTerrainScale->setText(QApplication::translate("MainWindow", "Terrain scale", nullptr));
        labelWaterHeight->setText(QApplication::translate("MainWindow", "Water height", nullptr));
        createButton->setText(QApplication::translate("MainWindow", "Create Pak", nullptr));
        label->setText(QApplication::translate("MainWindow", "RB - move\n"
"LB - change size\n"
"LB + Ctrl - rotate\n"
"LB + Shift - to top\n"
"LB + Alt - to bottom\n"
"MB - change height\n"
"Wheel - change height", nullptr));
        pushButtonClear->setText(QApplication::translate("MainWindow", "\320\241lear", nullptr));
        labelX->setText(QApplication::translate("MainWindow", "X:", nullptr));
        labelY->setText(QApplication::translate("MainWindow", "Y:", nullptr));
        labelWidth->setText(QApplication::translate("MainWindow", "Width:", nullptr));
        labelHeight->setText(QApplication::translate("MainWindow", "Height:", nullptr));
        labelRotation->setText(QApplication::translate("MainWindow", "Rotation:", nullptr));
        labelOffset->setText(QApplication::translate("MainWindow", "Terrain offset:", nullptr));
        addButton->setText(QApplication::translate("MainWindow", "Add", nullptr));
        removeButton->setText(QApplication::translate("MainWindow", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
