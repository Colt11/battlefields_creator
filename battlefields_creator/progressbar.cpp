#include "progressbar.h"
#include "ui_progressbar.h"


ProgressBar::ProgressBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowTitleHint);
}


ProgressBar::~ProgressBar()
{
    delete ui;
}
