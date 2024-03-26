#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->xRotSlider, &QSlider::sliderMoved, ui->openGLWidget, &MyGLWidget::setXRotation);
    connect(ui->yRotSlider, &QSlider::sliderMoved, ui->openGLWidget, &MyGLWidget::setYRotation);
    connect(ui->zRotSlider, &QSlider::sliderMoved, ui->openGLWidget, &MyGLWidget::setZRotation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

