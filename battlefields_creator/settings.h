#ifndef SETTINGS_H
#define SETTINGS_H


#include <QDialog>
#include <QSettings>
#include <QLineEdit>


namespace Ui {
class Settings;
}


class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void save();
    void load();

    void configureGameFolder();
    void configurePackManagerFolder();
    void configureTmpFolder();
    void configureSaveFolder();

private:
    void configureFolder(QLineEdit* lineEdit);

    Ui::Settings *ui;

    QSettings settings;
};

#endif // SETTINGS_H
