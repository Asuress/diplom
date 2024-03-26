#include "myglwidget.h"

#include <QtWidgets>
#include <QtOpenGL>
#include <QtMath>

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    r = 2;
    a = 3;

    viewSideLength = 10;

    wrenchPoints = getPoints(1);
    qDebug() << "point count:" << wrenchPoints.size();
}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setACoeff(int angle)
{
    a = angle;
    wrenchPoints=getPoints(1);
    updateGL();
}

void MyGLWidget::serRCoeff(int angle)
{
    r = angle;
    wrenchPoints=getPoints(1);
    updateGL();
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << event->angleDelta().y() / 120;
    viewSideLength += event->angleDelta().y() / 120;
    viewSideLength = viewSideLength < 0 ? 0 : viewSideLength;

    updateGL();
}

void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void MyGLWidget::paintGL()
{
    qDebug() << Q_FUNC_INFO << "zDist" << viewSideLength;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-viewSideLength / 2, viewSideLength / 2, -viewSideLength / 2, viewSideLength / 2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-viewSideLength / 2, viewSideLength / 2, -viewSideLength / 2, viewSideLength / 2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setACoeff(xRot + 8 * dy);
        serRCoeff(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setACoeff(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void MyGLWidget::draw()
{
    qglColor(Qt::red);
    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < wrenchPoints.size(); i++)
        {
            glVertex3f(wrenchPoints[i].x(), wrenchPoints[i].y(), 0);
        }
    glEnd();
/*
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);

    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
*/
}

QList<QPointF> MyGLWidget::getPoints(float dAngle)
{
    QList<QPointF> points;
    int count = 360 / dAngle;
    for (int i = 0; i < count; i++)
    {
        QPointF point;
        double dAngleRad = dAngle * M_PI / 180;
        double radAngle  = dAngleRad * i;
        double sqrt3     = qSqrt(3);

//        double length = qSin(radAngle);
//        point.setX(length * qCos(radAngle));
//        point.setY(length * qSin(radAngle));

        if (radAngle >= 0 && radAngle <= M_PI / 3)
        {
            double length = -a * qSin(radAngle + M_PI / 3) / sqrt3 +
                                 qSqrt(qPow((a + r), 2) -
                                       qPow(a * qCos(radAngle + M_PI / 3), 2) / 3);
            point.setX(length * qCos(radAngle));
            point.setY(length * qSin(radAngle));
        }
        else if (radAngle > M_PI / 3 && radAngle <= M_PI * 2 / 3)
        {
            double length = a * qSin(radAngle) / sqrt3 +
                                qSqrt(r * r -
                                      qPow(a * qCos(radAngle), 2) / 3);
            point.setX(length * qCos(radAngle));
            point.setY(length * qSin(radAngle));
        }
        else if (radAngle > M_PI * 2 / 3 && radAngle <= M_PI)
        {
            double length = -a * qSin(radAngle - M_PI / 3) / sqrt3 +
                                 qSqrt(qPow((a + r), 2) -
                                       qPow(a * qCos(radAngle - M_PI / 3), 2) / 3);
            point.setX(length * qCos(radAngle));
            point.setY(length * qSin(radAngle));
        }
        else if (radAngle > M_PI && radAngle <= M_PI * 4 / 3)
        {
            double length = -a * qSin(radAngle + M_PI / 3) +
                                 qSqrt(qPow(r, 2) -
                                       qPow(a * qCos(radAngle + M_PI / 3), 2) / 3);
            point.setX(length * qCos(radAngle));
            point.setY(length * qSin(radAngle));
        }
        else if (radAngle > M_PI * 4 / 3 && radAngle <= M_PI * 5 / 3)
        {
            double length = a * qSin(radAngle + M_PI / 3) / sqrt3 +
                                 qSqrt(qPow((a + r), 2) -
                                       qPow(a * qCos(radAngle), 2) / 3);
            point.setX(length * qCos(radAngle));
            point.setY(length * qSin(radAngle));
        }
        else if (radAngle > M_PI * 5 / 3 && radAngle <= M_PI * 2)
        {
            double length = -a * qSin(radAngle - M_PI / 3) / sqrt3 +
                                 qSqrt(qPow(r, 2) -
                                       qPow(qCos(radAngle - M_PI / 3), 2));
            point.setX(length * qCos(radAngle));
            point.setY(length * qSin(radAngle));
        }

        points.append(point);
    }
    return points;
}
