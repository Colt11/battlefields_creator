#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H


#include <QDialog>


namespace Ui {
class ProgressBar;
}


class ProgressBar : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressBar(QWidget *parent = nullptr);
    ~ProgressBar();

private:
    Ui::ProgressBar *ui;
};

#endif // PROGRESSBAR_H
