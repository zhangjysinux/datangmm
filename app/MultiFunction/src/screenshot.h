#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QImage>
#include <QPainter>
#include <QQuickPaintedItem>

class ScreenShot : public QQuickPaintedItem
{
    Q_OBJECT
public:
    ScreenShot();
    void paint(QPainter *painter);
    void setImage(const QImage &image);
private:
    QImage m_image;

};

#endif // SCREENSHOT_H
