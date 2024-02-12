#include "glwdget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include <QDebug>

GLWidget::GLWidget()
{
//    connect(this, &GLWidget::aboutToResize, this, &GLWidget::paintEvent);
    lightOn = false;
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(&painter, event);
    painter.end();
}

void GLWidget::paint(QPainter *painter, QPaintEvent *event)
{
//    qDebug() << Q_FUNC_INFO << lightOn;

    painter->fillRect(event->rect(), lightOn ? Qt::white : Qt::black);
//    painter->fillRect(event->rect(), Qt::white);
    painter->translate(100, 100);
    painter->save();

    painter->setBrush(Qt::blue);
    QPen pen(Qt::blue);
    pen.setWidth(1);
    painter->setPen(pen);

    int num = 360;
    QPointF* points = new QPointF[num];
    double radius = 100;
    for (int i = 0; i < num; ++i)
    {
        double angle = 2 * M_PI * i / num;
        points[i] = QPointF(radius * cos(angle), radius * sin(angle));
    }

    painter->drawPolyline(points, num);
}
