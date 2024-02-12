#ifndef GLWidget_H
#define GLWidget_H

#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget();
    void paintEvent(QPaintEvent *event) override;
    bool lightOn;
private:
    void paint(QPainter *, QPaintEvent *);
};

#endif // GLWidget_H
