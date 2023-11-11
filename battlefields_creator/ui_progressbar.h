/********************************************************************************
** Form generated from reading UI file 'progressbar.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSBAR_H
#define UI_PROGRESSBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProgressBar
{
public:
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;

    void setupUi(QDialog *ProgressBar)
    {
        if (ProgressBar->objectName().isEmpty())
            ProgressBar->setObjectName(QString::fromUtf8("ProgressBar"));
        ProgressBar->resize(851, 28);
        ProgressBar->setModal(true);
        verticalLayout = new QVBoxLayout(ProgressBar);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 0, 6, 0);
        progressBar = new QProgressBar(ProgressBar);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximum(0);
        progressBar->setValue(-1);
        progressBar->setTextVisible(false);

        verticalLayout->addWidget(progressBar);


        retranslateUi(ProgressBar);

        QMetaObject::connectSlotsByName(ProgressBar);
    } // setupUi

    void retranslateUi(QDialog *ProgressBar)
    {
        ProgressBar->setWindowTitle(QApplication::translate("ProgressBar", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProgressBar: public Ui_ProgressBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSBAR_H
