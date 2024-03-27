#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->aSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            ui->openGLWidget, &MyGLWidget::setACoeff);
    connect(ui->rSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            ui->openGLWidget, &MyGLWidget::setRCoeff);

    connect(ui->aSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aCoeffChanged);
    connect(ui->rSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::rCoeffChanged);

    ui->aSpinBox->setValue(ui->openGLWidget->getACoeff());
    ui->rSpinBox->setValue(ui->openGLWidget->getRCoeff());

    ui->horizontalLayout->setAlignment(Qt::AlignmentFlag::AlignRight);
    ui->horizontalLayout_2->setAlignment(Qt::AlignmentFlag::AlignRight);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aCoeffChanged(double value)
{
    ui->aSliderLabel->setText(QString("a: %1").arg(QString::number(value)));
}

void MainWindow::rCoeffChanged(double value)
{
    ui->rSliderLabel->setText(QString("r: %1").arg(QString::number(value)));
}
