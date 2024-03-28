#include "myglwidget.h"

#include <QtWidgets>
#include <QtOpenGL>
#include <QtMath>

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    r = 5;
    a = 15;
    dx = 1;

    viewSideLength = 2*qMax(r, a);

    wrenchPoints = getPointsAffine(dx);
    qDebug() << "points count:" << wrenchPoints.size();
}

double MyGLWidget::getACoeff()
{
    return a;
}

double MyGLWidget::getRCoeff()
{
    return r;
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

void MyGLWidget::setACoeff(double aCoeff)
{
    a = aCoeff;
    wrenchPoints=getPointsAffine(dx);
    update();
}

void MyGLWidget::setRCoeff(double radius)
{
    r = radius;
    wrenchPoints=getPointsAffine(dx);
    update();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << event->angleDelta().y() / 120;
    viewSideLength += event->angleDelta().y() / 120;
    viewSideLength = viewSideLength < 0 ? 0 : viewSideLength;

    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_1)
        update();
}

void MyGLWidget::initializeGL()
{
    glClearColor(255,255,255,255);

//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glShadeModel(GL_SMOOTH);
}

void MyGLWidget::paintGL()
{
    qDebug() << Q_FUNC_INFO << "viewSideLength:" << viewSideLength;
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

    qDebug() << "dx dy:" << dx << dy;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(dx, dy, 0);
    glMatrixMode(GL_MODELVIEW);

    lastPos = event->pos();
}



void MyGLWidget::draw()
{
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(255, 0, 0);

    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < wrenchPoints.size(); i++)
        {
            glVertex3f(wrenchPoints[i].x(), wrenchPoints[i].y(), 0);
        }
    glEnd();
}

QList<QPointF> MyGLWidget::getPointsAffine(float dAngle)
{
    QList<QPointF> points;
    int count              = 360 / dAngle;
    int usingFormulaNumber = 1;
    double dAngleRad       = dAngle * M_PI / 180;

    auto function1 = [](double a, double r, double radAngle){
        return -a * qSin(radAngle + M_PI / 3) / qSqrt(3) +
                    qSqrt(qPow((a + r), 2) -
                          qPow(a * qCos(radAngle + M_PI / 3), 2) / 3);
    };
    auto function2 = [](double a, double r, double radAngle){
        return a * qSin(radAngle) / qSqrt(3) +
                   qSqrt(r * r -
                         qPow(a * qCos(radAngle), 2) / 3);
    };
    auto function3 = [](double a, double r, double radAngle){
        return -a * qSin(radAngle - M_PI / 3) / qSqrt(3) +
                    qSqrt(qPow((a + r), 2) -
                          qPow(a * qCos(radAngle - M_PI / 3), 2) / 3);
    };
    auto function4 = [](double a, double r, double radAngle){
        return -a * qSin(radAngle + M_PI / 3) / qSqrt(3) +
                    qSqrt(qPow(r, 2) -
                          qPow(a * qCos(radAngle + M_PI / 3), 2) / 3);
    };
    auto function5 = [](double a, double r, double radAngle){
        return a * qSin(radAngle) / qSqrt(3) +
                   qSqrt(qPow((a + r), 2) -
                         qPow(a * qCos(radAngle), 2) / 3);
    };
    auto function6 = [](double a, double r, double radAngle){
        return -a * qSin(radAngle - M_PI / 3) / qSqrt(3) +
                    qSqrt(qPow(r, 2) -
                          qPow(a * qCos(radAngle - M_PI / 3), 2) / 3);
    };

    for (int i = 0; i < count; i++)
    {
        QPointF point;
        double currentValue;
        double nextValue;
        double radAngle = dAngleRad * i;

        switch(usingFormulaNumber)
        {
            case 1:
                currentValue = function1(a, r, radAngle);
                nextValue    = function2(a, r, radAngle);
                break;
            case 2:
                currentValue = function2(a, r, radAngle);
                nextValue    = function3(a, r, radAngle);
                break;
            case 3:
                currentValue = function3(a, r, radAngle);
                nextValue    = function4(a, r, radAngle);
                break;
            case 4:
                currentValue = function4(a, r, radAngle);
                nextValue    = function5(a, r, radAngle);
                break;
            case 5:
                currentValue = function5(a, r, radAngle);
                nextValue    = function6(a, r, radAngle);
                break;
            case 6:
                currentValue = function6(a, r, radAngle);
                nextValue    = function1(a, r, radAngle);
                break;
            default:
                currentValue = 0;
                nextValue    = 0;
                break;
        }

        if (qAbs(currentValue - nextValue) <= 0.001)
        {
            usingFormulaNumber = usingFormulaNumber == 6 ? 1 : usingFormulaNumber + 1;
            point.setX(currentValue * qCos(radAngle));
            point.setY(currentValue * qSin(radAngle));
        }
        else
        {
            point.setX(currentValue * qCos(radAngle));
            point.setY(currentValue * qSin(radAngle));
        }

        points.append(point);
    }
    return points;
}
