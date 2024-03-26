#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->aSlider, &QSlider::sliderMoved, ui->openGLWidget, &MyGLWidget::setACoeff);
    connect(ui->rSlider, &QSlider::sliderMoved, ui->openGLWidget, &MyGLWidget::serRCoeff);
    connect(ui->zRotSlider, &QSlider::sliderMoved, ui->openGLWidget, &MyGLWidget::setZRotation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

