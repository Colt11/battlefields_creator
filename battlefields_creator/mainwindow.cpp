#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QProcess>
#include <QFile>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new GraphicsScene(this))
    , data(new SceneData(this))
    , progressBar(new ProgressBar(this))
    , settings(new Settings(this))
{
    ui->setupUi(this);

    rectPanelManager = new RectPanelManager(data, ui->spinBoxX, ui->spinBoxY, ui->spinBoxRotation, ui->spinBoxWidth, ui->spinBoxHeight, ui->spinBoxOffset, this);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMaximumSize(1025 + 20,1025);
    ui->graphicsView->setSceneRect(0,0,1025,1025);
    scene->setData(data);
    ui->treeWidget->setData(data);

    QAction* saveAction = new QAction("Save", this);
    QAction* saveAsAction = new QAction("Save As...", this);
    QAction* loadAction = new QAction("Load", this);
    QAction* settingsAction = new QAction("Settings", this);

    QMenu* file = menuBar()->addMenu("File");
    file->addAction(saveAction);
    file->addAction(saveAsAction);
    file->addAction(loadAction);

    QMenu* settings = menuBar()->addMenu("Settings");
    settings->addAction(settingsAction);

    setWindowTitle("Battlefields Creator");

    connect(ui->addButton, &QPushButton::clicked, data, &SceneData::addRect);
    connect(ui->removeButton, &QPushButton::clicked, data, &SceneData::removeRect);
    connect(ui->spinBoxTerrainHeight, qOverload<int>(&QSpinBox::valueChanged), data, &SceneData::terrainHeightValueChanged);
    connect(ui->comboBoxTeamsSize, qOverload<int>(&QComboBox::currentIndexChanged), data, &SceneData::teamsSizeIndexChanged);
    connect(ui->doubleSpinBoxWaterHeight, qOverload<double>(&QDoubleSpinBox::valueChanged), data, &SceneData::terrainWaterHeightValueChanged);
    connect(ui->spinBoxTerrainScale, qOverload<int>(&QSpinBox::valueChanged), data, &SceneData::terrainScaleValueChanged);
    connect(saveAction, &QAction::triggered, data, &SceneData::save);
    connect(saveAsAction, &QAction::triggered, data, &SceneData::saveAs);
    connect(loadAction, &QAction::triggered, data, &SceneData::load);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::openSettings);
    connect(ui->pushButtonClear, &QPushButton::clicked, data, &SceneData::clearAll);
    connect(data, &SceneData::needReload, this, &MainWindow::reload);
    connect(ui->createButton, &QPushButton::clicked, this, &MainWindow::createImage);
    connect(ui->checkBoxHideTeamsRects, &QCheckBox::stateChanged, scene, &GraphicsScene::hideTeamsRectsValueChenged);

    data->addRect();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::reload()
{
    ui->spinBoxTerrainHeight->setValue(data->getTerrainHeight());
    ui->doubleSpinBoxWaterHeight->setValue(data->getTerrainWaterHeight());
    ui->spinBoxTerrainScale->setValue(data->getTerrainScale());
    ui->comboBoxTeamsSize->setCurrentIndex(static_cast<TeamsSize>(data->getTeamsSize()));
}


void MainWindow::createImage()
{
    QImage image(1025,1025, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    scene->hideRectBorders();
    QPainter painter(&image);
    scene->render(&painter, image.rect(), ui->graphicsView->sceneRect());
    scene->showRectBorders();

    int defaultHeight = ui->spinBoxTerrainHeight->value();

    for (int x = 0; x < image.width(); ++x) {
        for (int y = 0; y < image.height(); ++y) {
            QColor color = image.pixelColor(x, y);
            QColor newColor{defaultHeight, defaultHeight, defaultHeight};
            if(color.red() > 0) {
                int newValue = defaultHeight + qRound((static_cast<float>(color.red()) / 255.f) * (255.f - defaultHeight));
                newColor = QColor{newValue, newValue, newValue};
            } else if(color.blue() > 0) {
                int newValue = defaultHeight - qRound((static_cast<float>(color.blue()) / 255.f) * defaultHeight);
                newColor = QColor{newValue, newValue, newValue};
            }
            image.setPixelColor(x, y, newColor);
        }
    }

    PackManager* packManager = new PackManager(data, image, this);
    connect(packManager, &PackManager::resultReady, this, &MainWindow::handlePackManagerResults);
    connect(packManager, &PackManager::finished, packManager, &QObject::deleteLater);
    packManager->start();

    progressBar->show();
}


void MainWindow::handlePackManagerResults(const QString& errorString)
{
    progressBar->hide();
    if(!errorString.isEmpty()) {
        QMessageBox::warning(this, "Can't pack mod.", errorString, QMessageBox::Ok);
    }
}


void MainWindow::openSettings()
{
    settings->show();
}
