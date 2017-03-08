#ifndef ROUNDIMAGE_H
#define ROUNDIMAGE_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>
#include <QPen>
#include <QColor>
#include <QLabel>
#include <QPolygon>
#include <QGraphicsProxyWidget>
#include <QPair>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include "QTime"
#include <QByteArray>
#include <QQuickItem>

using namespace std;
// HOLD THE NUMBER FOR imageList's max length
#define MAX_KEY_CACHE_LENGTH 20

typedef QPair<QString,QPixmap> imagecache;
typedef QList<imagecache> imageList;
typedef QHash<QString,imageList> roundImageCaches;

class ThreadGetImage;

/*!
  Image will be painted on this
 */
class BatLabel : public QLabel
{
    Q_OBJECT
public:
    explicit BatLabel(QWidget *parent = 0);
    /*!
      set round rectangle path
      \param radius round rect radius.
      */
    void setPath(int radius);
    /*!
      save QPixmap data.
      \param pixmap data.
      */
    void setPixmap(const QPixmap &pixmap);
    /*!
      set the original pixmap(for calculating width & height).
      */
    void setOriginalPixmap(const QPixmap &pixmap);
    ~BatLabel();
protected:
    void paintEvent(QPaintEvent *);
    QPainterPath m_path;
    QPixmap m_pixmap;
    QPixmap m_orignalPixmap;
    int m_radius;
    int m_borderWidth;
    Qt::SizeMode m_sizeMode;
public:
    int labelWidth;
    int labelHeight;
    QImage image;
    QSize isize;
    bool keepRatio;
};

class RoundImage : public QQuickItem
{
    Q_OBJECT
public:
    explicit RoundImage(QQuickItem *parent = 0);
    ~RoundImage();
    Q_PROPERTY(QString source READ imageSource WRITE setImageSource NOTIFY imageSourceChanged)
    Q_PROPERTY(int radius READ imageRadius WRITE setImageRadius NOTIFY imageRadiusChanged)
    Q_PROPERTY(int width READ imageWidth WRITE setImageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(int height READ imageHeight WRITE setImageHeight NOTIFY imageHeightChanged)
    Q_PROPERTY(QString cache READ cacheKey WRITE setCacheKey NOTIFY cacheKeyChanged)
    Q_PROPERTY(bool ratio READ imageRatio WRITE setRatio NOTIFY ratioChanged)
    Q_PROPERTY(bool asynchronous READ imageAsynchronous WRITE setAsynchronous)
    Q_PROPERTY(QImage img READ getImg WRITE setImg)
    Q_PROPERTY(QString failedName READ getImageFailedName)

    Q_CLASSINFO("DefaultProperty", "source")


signals:
    /*!
      signal for image src changed
      */
    void imageSourceChanged();
    /*!
      signal for image radius changed
      */
    void imageRadiusChanged();
    /*!
      signal for image width changed
      */
    void imageWidthChanged();
    /*!
      signal for image height changed
      */
    void imageHeightChanged();
    /*!
      signal for image cache key changed, cache key arms for cache images to improve performance.
      */
    void cacheKeyChanged();
    /*!
      signal for image ratio changed
      */
    void ratioChanged();
//    void getImageFailed(const QString filename);
public slots:
    /*!
      Get image source.
      */
    QString imageSource();
    /*!
      Get image uri.
      */
    void setImageSource(QString uri);
    /*!
      Get image radius.
      */
    int imageRadius();
    /*!
      set image radius.
      */
    void setImageRadius(int radius);

    /*!
      Get image width.
      */
    int imageWidth();
    /*!
      set image width.
      */
    void setImageWidth(int width);

    /*!
      Get image height
      */
    int imageHeight();
    /*!
      set image height.
      */
    void setImageHeight(int height);

    /*!
      Get image cache key
      */
    QString cacheKey();
    /*!
      Set image cache key
      */
    void setCacheKey(QString cacheKey);
    /*!
      Get image ratio.
      */
    bool imageRatio();
    /*!
      Set image ratio.
      */
    void setRatio(bool on);
    /*!
      asynchronous showing images, get async flag
      */
    bool imageAsynchronous();
    /*!
      asynchronous showing images, set async flag
      */
    void setAsynchronous(bool asynchronous);
    /*!
      Get QImage data.
      */
    QImage getImg();
    /*!
      Set QImage data.
      */
    void setImg(QImage img);
    /*!
      Get alternative image.
      */
    QString getImageFailedName();

    /*!
      asynchorous get image data finished.
      \param img image data
      \param size image size.
      */
    void doImageFinished(QImage img, QSize size);
    /*!
      Image loading failed, show a alternative image.
      \param imagename alternative image.
      */
    void doGetImageFailed(QString imagename);
protected:
    /*!
      set radius to show rounded corner.
      */
    void setRadiusMask();
    /*!
      resize image to a specific width/height
      \param width image width
      \param height image height.
      */
    void resizeImage(int width, int height);
//    void keepImageRatio(int *width, int *height);
private:
    QString m_imagesource;
    int m_imageRadius;
    int m_imageWidth;
    int m_imageHeight;
    QString m_cacheKey;
    bool m_asynchronous;
    QImage m_img;
    QString failedName;

    BatLabel *m_label;
    QGraphicsProxyWidget* m_proxy;
    QPixmap m_pixmap;

//    LoggerPtr m_logger;

    static roundImageCaches caches;

    ThreadGetImage *threadgetimage;

};


/*!
  A thread to get image asynchorously.
  */
class ThreadGetImage : public QThread
{
    Q_OBJECT
public:
    explicit ThreadGetImage(const QString filename = NULL, const int width = 0, const int height = 0);
    ~ThreadGetImage();

private:
        void run();
        QImage m_smallImg;
        QString m_imgName;
        int m_width;
        int m_height;
        //QTimer *m_timer;
        QSize m_size;
        QImage m_relImage;

public:
        /*!
          Set the image path
          \param filename image path
          */
        void setImgSource(const QString filename);
        /*!
          Set image width
          \param width image width
          */
        void setWidth(const int width);
        /*!
          set Image heigth.
          \param image height.
          */
        void setHeight(const int height);
        /*!
          set image data
          \param img image data.
          */
        void setImage(QImage img);

public slots:
        /*!
          emit signal getImgFinished to notify roundimage.
          */
        void EmitImageFinished();

signals:
        /*!
          signal for notice image loading finished.
          \param img image data
          \param size image size
          */
        void getImgFinished(QImage img, QSize size);
        /*!
          signal for notice image loading failed.
          \param filename image file path
          */
        void getImgFailed(const QString filename);

};

#endif // ROUNDIMAGE_H
