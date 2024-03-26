#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = nullptr);

protected:
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void draw();
    QList<QPointF> getPoints(float dAngle);

    int xRot;
    int yRot;
    int zRot;

    float viewSideLength;

    QPoint lastPos;
    QList<QPointF> wrenchPoints;

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // MYGLWIDGET_H
