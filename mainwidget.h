#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ui_mainwidget.h"
#include "glwdget.h"

#include <QOpenGLWidget>

class MainWidget : public QWidget, private Ui::MainWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    GLWidget m_openGLWidget;

private slots:
    void checkBoxStateChanged();

};

#endif // MAINWIDGET_H
