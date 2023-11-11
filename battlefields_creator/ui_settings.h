/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *labelGameFolder;
    QLineEdit *lineEditGameFolder;
    QPushButton *pushButtonGameFolder;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelPackManagerFolder;
    QLineEdit *lineEditPackManagerFolder;
    QPushButton *pushButtonPackManagerFolder;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelSaveFolder;
    QLineEdit *lineEditSaveFolder;
    QPushButton *pushButtonSaveFolder;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelTmpFolder;
    QLineEdit *lineEditTmpFolder;
    QPushButton *pushButtonTmpFolder;
    QCheckBox *checkBoxInstallMod;
    QCheckBox *checkBoxRemoveTmpFolder;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QString::fromUtf8("Settings"));
        Settings->resize(843, 257);
        Settings->setModal(true);
        verticalLayout_2 = new QVBoxLayout(Settings);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(Settings);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelGameFolder = new QLabel(groupBox);
        labelGameFolder->setObjectName(QString::fromUtf8("labelGameFolder"));

        horizontalLayout->addWidget(labelGameFolder);

        lineEditGameFolder = new QLineEdit(groupBox);
        lineEditGameFolder->setObjectName(QString::fromUtf8("lineEditGameFolder"));

        horizontalLayout->addWidget(lineEditGameFolder);

        pushButtonGameFolder = new QPushButton(groupBox);
        pushButtonGameFolder->setObjectName(QString::fromUtf8("pushButtonGameFolder"));

        horizontalLayout->addWidget(pushButtonGameFolder);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelPackManagerFolder = new QLabel(groupBox);
        labelPackManagerFolder->setObjectName(QString::fromUtf8("labelPackManagerFolder"));

        horizontalLayout_2->addWidget(labelPackManagerFolder);

        lineEditPackManagerFolder = new QLineEdit(groupBox);
        lineEditPackManagerFolder->setObjectName(QString::fromUtf8("lineEditPackManagerFolder"));

        horizontalLayout_2->addWidget(lineEditPackManagerFolder);

        pushButtonPackManagerFolder = new QPushButton(groupBox);
        pushButtonPackManagerFolder->setObjectName(QString::fromUtf8("pushButtonPackManagerFolder"));

        horizontalLayout_2->addWidget(pushButtonPackManagerFolder);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        labelSaveFolder = new QLabel(groupBox);
        labelSaveFolder->setObjectName(QString::fromUtf8("labelSaveFolder"));

        horizontalLayout_4->addWidget(labelSaveFolder);

        lineEditSaveFolder = new QLineEdit(groupBox);
        lineEditSaveFolder->setObjectName(QString::fromUtf8("lineEditSaveFolder"));

        horizontalLayout_4->addWidget(lineEditSaveFolder);

        pushButtonSaveFolder = new QPushButton(groupBox);
        pushButtonSaveFolder->setObjectName(QString::fromUtf8("pushButtonSaveFolder"));

        horizontalLayout_4->addWidget(pushButtonSaveFolder);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        labelTmpFolder = new QLabel(groupBox);
        labelTmpFolder->setObjectName(QString::fromUtf8("labelTmpFolder"));

        horizontalLayout_3->addWidget(labelTmpFolder);

        lineEditTmpFolder = new QLineEdit(groupBox);
        lineEditTmpFolder->setObjectName(QString::fromUtf8("lineEditTmpFolder"));

        horizontalLayout_3->addWidget(lineEditTmpFolder);

        pushButtonTmpFolder = new QPushButton(groupBox);
        pushButtonTmpFolder->setObjectName(QString::fromUtf8("pushButtonTmpFolder"));

        horizontalLayout_3->addWidget(pushButtonTmpFolder);


        verticalLayout->addLayout(horizontalLayout_3);

        checkBoxInstallMod = new QCheckBox(groupBox);
        checkBoxInstallMod->setObjectName(QString::fromUtf8("checkBoxInstallMod"));
        checkBoxInstallMod->setChecked(true);

        verticalLayout->addWidget(checkBoxInstallMod);

        checkBoxRemoveTmpFolder = new QCheckBox(groupBox);
        checkBoxRemoveTmpFolder->setObjectName(QString::fromUtf8("checkBoxRemoveTmpFolder"));
        checkBoxRemoveTmpFolder->setChecked(true);

        verticalLayout->addWidget(checkBoxRemoveTmpFolder);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(Settings);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(Settings);
        QObject::connect(buttonBox, SIGNAL(accepted()), Settings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Settings, SLOT(reject()));

        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Dialog", nullptr));
        groupBox->setTitle(QApplication::translate("Settings", "Settings", nullptr));
        labelGameFolder->setText(QApplication::translate("Settings", "Game Folder", nullptr));
        pushButtonGameFolder->setText(QApplication::translate("Settings", "...", nullptr));
        labelPackManagerFolder->setText(QApplication::translate("Settings", "Pack Manager Folder", nullptr));
        pushButtonPackManagerFolder->setText(QApplication::translate("Settings", "...", nullptr));
        labelSaveFolder->setText(QApplication::translate("Settings", "Save Folder", nullptr));
        pushButtonSaveFolder->setText(QApplication::translate("Settings", "...", nullptr));
        labelTmpFolder->setText(QApplication::translate("Settings", "Tmp Folder", nullptr));
        pushButtonTmpFolder->setText(QApplication::translate("Settings", "...", nullptr));
        checkBoxInstallMod->setText(QApplication::translate("Settings", "Install Mod", nullptr));
        checkBoxRemoveTmpFolder->setText(QApplication::translate("Settings", "Remove Tmp Folder", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
