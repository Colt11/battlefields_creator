#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <graphicsscene.h>
#include <scenedata.h>
#include <rectpanelmanager.h>
#include <packmanager.h>
#include <progressbar.h>
#include <settings.h>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void reload();

    void createImage();

    void handlePackManagerResults(const QString &);

    void openSettings();

private:
    Ui::MainWindow *ui;
    GraphicsScene* scene = nullptr;
    SceneData* data = nullptr;
    RectPanelManager* rectPanelManager = nullptr;

    ProgressBar* progressBar = nullptr;
    Settings* settings = nullptr;
};


#endif // MAINWINDOW_H
