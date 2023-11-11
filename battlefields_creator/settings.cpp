#include "settings.h"
#include "ui_settings.h"
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <sharedsettings.h>


Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    settings(SETTINGS_FILE, QSettings::IniFormat)
{
    ui->setupUi(this);
    load();

    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &Settings::load);
    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &Settings::save);
    connect(ui->pushButtonGameFolder, &QPushButton::clicked, this, &Settings::configureGameFolder);
    connect(ui->pushButtonPackManagerFolder, &QPushButton::clicked, this, &Settings::configurePackManagerFolder);
    connect(ui->pushButtonTmpFolder, &QPushButton::clicked, this, &Settings::configureTmpFolder);
    connect(ui->pushButtonSaveFolder, &QPushButton::clicked, this, &Settings::configureSaveFolder);
    setWindowTitle("Settings");
}


Settings::~Settings()
{
    delete ui;
}


void Settings::save()
{
    settings.setValue(SETTINGS_KEY_INSTALL_MOD, ui->checkBoxInstallMod->isChecked());
    settings.setValue(SETTINGS_KEY_REMOVE_TMP_FOLDER, ui->checkBoxRemoveTmpFolder->isChecked());
    settings.setValue(SETTINGS_KEY_GAME_FOLDER, ui->lineEditGameFolder->text());
    settings.setValue(SETTINGS_KEY_PACK_MANAGER_FOLDER, ui->lineEditPackManagerFolder->text());
    settings.setValue(SETTINGS_KEY_TMP_FOLDER, ui->lineEditTmpFolder->text());

    settings.sync();
}


void Settings::load()
{
    ui->checkBoxInstallMod->setChecked(settings.value(SETTINGS_KEY_INSTALL_MOD, true).toBool());
    ui->checkBoxRemoveTmpFolder->setChecked(settings.value(SETTINGS_KEY_REMOVE_TMP_FOLDER, true).toBool());
    ui->lineEditGameFolder->setText(settings.value(SETTINGS_KEY_GAME_FOLDER, "").toString());
    ui->lineEditPackManagerFolder->setText(settings.value(SETTINGS_KEY_PACK_MANAGER_FOLDER, "").toString());
    QString tmpDir = settings.value(SETTINGS_KEY_TMP_FOLDER, "").toString();
    if(tmpDir.isEmpty()) {
        tmpDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    ui->lineEditTmpFolder->setText(tmpDir);
    QString saveDir = settings.value(SETTINGS_KEY_SAVE_FOLDER, "").toString();
    if(saveDir.isEmpty()) {
        saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if(!QDir{saveDir}.exists()) {
            QDir().mkdir(saveDir);
        }
    }
    ui->lineEditSaveFolder->setText(saveDir);
}


void Settings::configureGameFolder()
{
    configureFolder(ui->lineEditGameFolder);
}


void Settings::configurePackManagerFolder()
{
    configureFolder(ui->lineEditPackManagerFolder);
}


void Settings::configureTmpFolder()
{
    configureFolder(ui->lineEditTmpFolder);
}


void Settings::configureSaveFolder()
{
    configureFolder(ui->lineEditSaveFolder);
}


void Settings::configureFolder(QLineEdit* lineEdit)
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose Directory", lineEdit->text(), QFileDialog::DontResolveSymlinks);
    if(dirPath.isEmpty()) {
        return;
    }
    lineEdit->setText(dirPath);
}
