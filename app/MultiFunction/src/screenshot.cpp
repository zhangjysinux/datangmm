#include "screenshot.h"
#include <QDebug>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
extern pthread_mutex_t m_lock;
extern pthread_cond_t cond ;\

ScreenShot::ScreenShot()
{

}

void ScreenShot::paint(QPainter *painter)
{
    painter->drawImage(QRect(0, 0, this->width(), this->height()), m_image);
}

void ScreenShot::setImage(const QImage &image)
{
    m_image = image;
    this->update();
}


