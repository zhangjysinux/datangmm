#include "roundimage.h"
#include <stdlib.h>

roundImageCaches RoundImage::caches = roundImageCaches();

RoundImage::RoundImage(QQuickItem *parent) :
    QQuickItem(parent),m_cacheKey(""), m_asynchronous(false)
{
    m_label = new BatLabel;

    m_proxy = new QGraphicsProxyWidget(this);
    m_proxy->setWidget(m_label);

    this->threadgetimage = new ThreadGetImage(NULL, 0, 0);
    QObject::connect(this->threadgetimage, SIGNAL(getImgFinished(QImage,QSize)), this, SLOT(doImageFinished(QImage,QSize)),Qt::QueuedConnection);
    QObject::connect(this->threadgetimage, SIGNAL(getImgFailed(QString)), this, SLOT(doGetImageFailed(QString)));
}

RoundImage::~RoundImage()
{
    QObject::disconnect(this->threadgetimage,SIGNAL(getImgFinished(QImage,QSize)),this,SLOT(doImageFinished(QImage,QSize)));
    delete m_label;
    m_label = NULL;

    if (threadgetimage) {
        threadgetimage->terminate();
        threadgetimage->wait();
        delete threadgetimage;
    }
}


void RoundImage::setRadiusMask()
{
    m_label->setPath(m_imageRadius);
    m_label->repaint();
}

QString RoundImage::imageSource()
{
    return m_imagesource;
}

bool RoundImage::imageAsynchronous()
{
    return m_asynchronous;
}

void RoundImage::setAsynchronous(bool asynchronous)
{
    m_asynchronous = asynchronous;
    return;
}

QImage RoundImage::getImg()
{
    return m_img;
}

void RoundImage::setImg(QImage img)
{
    m_img = img;
}

QString RoundImage::getImageFailedName()
{
    return failedName;
}

void RoundImage::resizeImage(int width, int height)
{
    //qDebug()<<"w/h changed."<<width<<","<<height;
    if(width>0 && height > 0)
    {
        m_label->resize(width,height);
    }

    this->setRadiusMask();
}

void RoundImage::doImageFinished(QImage img, QSize size)
{
    if(m_label != NULL)
    {
        m_label->isize = size;
        m_label->setOriginalPixmap(QPixmap::fromImage(img));
        QPixmap pixmap = QPixmap::fromImage(img);
        //        m_pixmap = pixmap;
        if(pixmap.isNull() == false)
        {
            m_label->setPixmap(pixmap);
            resizeImage(this->imageWidth(),this->imageHeight());
        }
    }
}

void RoundImage::doGetImageFailed(QString imagename)
{
    failedName = imagename;
    return;
}

void RoundImage::setImageSource(QString uri)
{
    //    qDebug()<<"set source to "<<uri;
    m_imagesource = uri;
    QPixmap pixmap;

    //    bool foundCache = false;
    //    if(m_cacheKey.length() > 0)
    //    {
    //        if(caches.contains(m_cacheKey))
    //        {
    //            imageList list = caches[m_cacheKey];
    //            int len = list.count();
    //            for(int i=0;i<len;i++)
    //            {
    //                imagecache c = list[i];
    //                if(c.first == uri)
    //                {
    //                    pixmap = c.second;
    //                    foundCache = true;
    //                    qDebug()<<"using cached images====================================== w:"<<pixmap.width();
    //                    break;
    //                }
    //            }
    //        }
    //    }

    int width = this->imageWidth(),height = this->imageHeight();

    //    if(width > 0)
    //    {
    //        if(height == 0){
    //            height = imageSize.height()/imageSize.width()*width;
    //        }
    //    }

    //    if(height > 0 )
    //    {
    //        if(width == 0)
    //        {
    //            width = imageSize.width()/imageSize.height()*height;
    //        }
    //    }
    if(m_asynchronous)
    {
        //        this->threadgetimage->setImage(getImg());
//        if(this->threadgetimage->isRunning())
//        {
//            this->threadgetimage->terminate();
//            this->threadgetimage->wait();
//            qDebug() << "thread get image terminate";
//        }
        this->threadgetimage->setImgSource(uri);
        this->threadgetimage->setWidth(width);
        this->threadgetimage->setHeight(height);
        this->threadgetimage->start();
    }
    else
    {
        //        if(!foundCache)
        //        {

        QImage image(uri);
        if(!image.isNull())
        {
            QSize imageSize = image.size();
            m_label->isize = imageSize;
            m_label->setOriginalPixmap(QPixmap::fromImage(image));

            qDebug() << "imagesize : qlabel" << imageSize.width() << imageSize.height();

            if(width > 0)
            {
                if(height == 0){
                    height = imageSize.height()/imageSize.width()*width;
                }
            }

            if(height > 0 )
            {
                if(width == 0)
                {
                    width = imageSize.width()/imageSize.height()*height;
                }
            }

            if( QString::compare(m_cacheKey,"background") == 0)
            {
                // special for background in settings.
                image = image.scaledToWidth(253,Qt::FastTransformation);
            }
            else
            {
                image = image.scaledToWidth(width,Qt::SmoothTransformation);
            }

            image = image.scaledToWidth(width,Qt::SmoothTransformation);
            pixmap = QPixmap::fromImage(image);
            //                m_pixmap = pixmap;

            //                if(m_cacheKey.length() > 0)
            //                {
            //                    if(caches.contains(m_cacheKey))
            //                    {
            //                        imageList l = caches[m_cacheKey];
            //                        imagecache c = imagecache(uri,pixmap);
            //                        l.append(c);
            //                        int len = l.count();
            //                        if(len > MAX_KEY_CACHE_LENGTH)
            //                        {
            //                            l.removeAt(0);
            //                        }
            //                        caches[m_cacheKey] = l;
            //                    }
            //                    else
            //                    {
            //                        imageList l = imageList();
            //                        imagecache c = imagecache(uri,pixmap);
            //                        l.append(c);
            //                        caches.insert(m_cacheKey,l);
            //                    }
            //                }
        }
        //        }

        if(pixmap.isNull() == false)
        {
            m_label->setPixmap(pixmap);
            resizeImage(width,height);
        }
    }



}

bool RoundImage::imageRatio()
{
    return m_label->keepRatio;
}

void RoundImage::setRatio(bool on)
{
    m_label->keepRatio = on;
}

QString RoundImage::cacheKey()
{
    return m_cacheKey;
}

void RoundImage::setCacheKey(QString cacheKey)
{
    qDebug()<<"set cacheKey to "<<cacheKey;
    m_cacheKey = cacheKey;
    if(m_cacheKey.length() > 0 && m_pixmap.isNull() == false && m_imagesource.length()>0)
    {
        if(caches.contains(m_cacheKey))
        {
            imageList l = caches[m_cacheKey];
            imagecache c = imagecache(m_imagesource,m_pixmap);
            l.append(c);
            int len = l.count();
            if(len > MAX_KEY_CACHE_LENGTH)
            {
                l.removeAt(0);
            }
            caches[m_cacheKey] = l;
            qDebug()<<"-------------------------------------------current cached: "<<len;
        }
        else
        {
            imageList l = imageList();
            imagecache c = imagecache(m_imagesource,m_pixmap);
            l.append(c);
            caches.insert(m_cacheKey,l);
        }
    }
}

int RoundImage::imageRadius()
{
    return m_imageRadius;
}

void RoundImage::setImageRadius(int radius)
{
    m_imageRadius = radius;
    resizeImage(m_imageWidth,m_imageHeight);
}

int RoundImage::imageWidth()
{
    return m_imageWidth;
}

void RoundImage::setImageWidth(int width)
{
    m_imageWidth = width;
    resizeImage(m_imageWidth,m_imageHeight);
}

int RoundImage::imageHeight()
{
    return m_imageWidth;
}

void RoundImage::setImageHeight(int height)
{
    m_imageHeight = height;
    resizeImage(m_imageWidth,m_imageHeight);
}

//void RoundImage::keepImageRatio(int *width, int *height)
//{
//    int w = *width;
//    int h = *height;
//    if(w)
//}

BatLabel::BatLabel(QWidget *parent)
    :QLabel(parent), keepRatio(true)
{
    m_borderWidth = 1;
    m_sizeMode = Qt::RelativeSize;

    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //    setScaledContents(false);
    setAttribute(Qt::WA_TranslucentBackground);
    //setStyleSheet("border: 2px");
}

BatLabel::~BatLabel()
{

}

void BatLabel::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}

void BatLabel::setOriginalPixmap(const QPixmap &pixmap)
{
    m_orignalPixmap = pixmap;
}

void BatLabel::paintEvent(QPaintEvent *)
{
    if(!m_pixmap.isNull())
    {
        QPainter p(this);
        p.setBrush(Qt::SolidPattern);
        p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
        float w = this->width(),h = this->height(),x = 0,y = 0;
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);

        if(keepRatio)
        {
            //            qDebug()<<"ratio is "<<this->width()/m_pixmap.width()-this->height()/m_pixmap.height();
            float rto = 100*((float)(this->width())/(float)(m_pixmap.width())-(float)(this->height())/(float)(m_pixmap.height()));
            if(abs(rto) > 1)
            {
                p.setBrush(QBrush(QColor(0,0,0)));
                p.drawRoundedRect(this->rect(),m_radius,m_radius,m_sizeMode);

                w = (float)(width());
                h = (float)(height());

                float ratio2 =(float)isize.width() / (float)isize.height();
                float ratio1 = w/h;

                x = 0;
                y = 0;

                if(ratio1 < ratio2)
                {
                    h = w/ratio2;
                    y = (height() - h)/2;
                    m_pixmap = m_orignalPixmap.scaledToWidth(this->width());
                }
                else
                {
                    w = h*ratio2;
                    x = (width() - w)/2;
                    m_pixmap = m_orignalPixmap.scaledToHeight(this->height());
                }
                p.drawPixmap(x,y,w,h,m_pixmap);
            }
            else
            {
                QRectF rect = QRectF(x,y,qreal(w),qreal(h));
                path.addRoundedRect(rect,qreal(m_radius),qreal(m_radius),m_sizeMode);
                m_pixmap = m_orignalPixmap.scaledToWidth(this->width());
                QBrush pb(m_pixmap);
                //qDebug()<<"pixmap width:"<<m_pixmap.width()<<", height:"<<m_pixmap.height()<<", w:"<<this->width()<<",h:"<<this->height();
                p.fillPath(path,pb);
            }
        }
        else
        {

            QRectF rect = QRectF(x,y,qreal(w),qreal(h));
            path.addRoundedRect(rect,qreal(m_radius),qreal(m_radius),m_sizeMode);
            m_pixmap = m_orignalPixmap.scaledToWidth(this->width());
            QBrush pb(m_pixmap);
            //qDebug()<<"pixmap width:"<<m_pixmap.width()<<", height:"<<m_pixmap.height()<<", w:"<<this->width()<<",h:"<<this->height();
            p.fillPath(path,pb);
        }
    }
}

void BatLabel::setPath(int radius)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF rect = QRectF(0,0,qreal(width()),qreal(height()));
    path.addRoundedRect(rect,qreal(radius),qreal(radius),m_sizeMode);

    m_path = path;
    m_radius = radius;
    QPolygon polygon = path.toFillPolygon().toPolygon();
    QRegion region(polygon);
    this->setMask(region);
}

ThreadGetImage::ThreadGetImage(const QString filename, const int width, const int height)
{
    m_imgName = filename;
    m_width = width;
    m_height = height;
    //    this->m_timer = new QTimer(this);
    //    this->m_timer->setInterval(300);
    //    this->m_timer->setSingleShot(true);
    //    QObject::connect(this->m_timer, SIGNAL(timeout()), this, SLOT(EmitImageFinished()));
}

ThreadGetImage::~ThreadGetImage()
{
    //    delete this->m_timer;
}

void ThreadGetImage::run()
{
//    qDebug() << "GET IMAGE START IN THREAD";
    if(m_imgName != NULL)
    {
        QTime time;
        time.start();
        QImage bigImg;
        if(m_imgName.length() < 500)
        {
            bigImg = QImage(m_imgName);
        }
        else
        {
            QByteArray text_img = QByteArray::fromBase64(m_imgName.toAscii());
            bigImg.loadFromData(text_img);
        }
        if(bigImg.isNull())
        {
            m_smallImg.load("Resources/Images/errorimage.png");
            m_size = m_smallImg.size();
            emit getImgFailed(m_imgName);
        }
        else
        {
            emit getImgFailed(NULL);
            if(bigImg.width() > 253)
            {
                m_smallImg = bigImg.scaledToWidth(253,Qt::SmoothTransformation);
            }
            else
            {
                m_smallImg = bigImg;
            }
            m_size = bigImg.size();
        }

        this->EmitImageFinished();
        qDebug() << "GET IMAGE END:" << time.elapsed() << "ms";
    }
}

void ThreadGetImage::EmitImageFinished()
{
    emit getImgFinished(m_smallImg, m_size);
}

void ThreadGetImage::setImgSource(const QString filename)
{
    m_imgName = filename;
}

void ThreadGetImage::setWidth(const int width)
{
    m_width = width;
}

void ThreadGetImage::setHeight(const int height)
{
    m_height = height;
}

void ThreadGetImage::setImage(QImage img)
{
    m_relImage = img;
}

