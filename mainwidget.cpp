#include "mainwidget.h"

#include <QPainter>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(checkBox, &QCheckBox::stateChanged, this, &MainWidget::checkBoxStateChanged);
    this->layout()->addWidget(&m_openGLWidget);
}

void MainWidget::checkBoxStateChanged()
{
    m_openGLWidget.lightOn = !m_openGLWidget.lightOn;
    m_openGLWidget.repaint();
}
