#ifndef QUICKCARITEM_H
#define QUICKCARITEM_H

#include <QQuickPaintedItem>
#include <QScopedPointer>

#include "model3ds.h"

class CarAnimator;
class CarOrientation;

class QuickCarItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
    Q_PROPERTY(int highlight READ highlight WRITE setHighlight NOTIFY highlightChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit QuickCarItem(QQuickItem *parent = 0);

    virtual void paint(QPainter *painter);

    Q_INVOKABLE void setOrientation(qreal deltaI, qreal deltaJ, qreal deltaK, int steps);

    QString note() const;
    int highlight() const;
    QString source() const;

public slots:
    void setNote(const QString &note);
    void setHighlight(int number);
    void setSource(const QString &source);

signals:
    void noteChanged(const QString &note);
    void highlightChanged(int highlight);
    void sourceChanged(const QString &source);

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    QScopedPointer<Model3DS> m_model;
    CarAnimator *m_animator;
    CarOrientation *m_defaultOrientation;
    bool m_GLSetupDone;
    int m_animationTimerId;

    QString m_note;
    int m_highlight;
    QString m_source;
};

#endif // QUICKCARITEM_H
