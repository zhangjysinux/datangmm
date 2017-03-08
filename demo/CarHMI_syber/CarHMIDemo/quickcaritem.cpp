#include <QQuickWindow>
#include <QTimerEvent>
#include <QPainter>
#include <QtQml>
#include <QUrl>

#include "model3ds.h"
#include "caranimator.h"
#include "carorientation.h"
#include "quickcaritem.h"

QuickCarItem::QuickCarItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_animator(0)
    , m_GLSetupDone(false)
    , m_animationTimerId(-1)
    , m_highlight(-1)
{
    setRenderTarget(QQuickPaintedItem::InvertedYFramebufferObject);

    m_animator = new CarAnimator(this);
    m_animator->setAnimationSteps(100);

    m_defaultOrientation = new CarOrientation(this);
    m_defaultOrientation->setOrientation(300,360,315);
    m_defaultOrientation->setPosition(0.0, -0.2, -2.2);

    CarOrientation o;
    o.setOrientation(*m_defaultOrientation);
    o.mOrientation[0] -= 30;
    o.mOrientation[2] += 90;
    m_animator->setOrientation(o);
    m_animator->setOrientationTo(*m_defaultOrientation);

    m_animationTimerId = startTimer(20);
}

void QuickCarItem::paint(QPainter *painter)
{
    if (!m_model && !m_source.isEmpty()) {
        m_model.reset(Model3DS::create(m_source));
        if (m_model)
            m_model->setAspectRatio(window()->width(), window()->height());
    }

    if (!m_model)
        return;

    if (!m_GLSetupDone) {
        glViewport(0, 0, window()->width(), window()->height());
        m_model->generateShaders();
        m_GLSetupDone = true;
    }

    painter->beginNativePainting();
    m_model->setTransformation(m_animator->position(), m_animator->orientation());
    m_model->Draw();
    painter->endNativePainting();
}

void QuickCarItem::setOrientation(qreal deltaI, qreal deltaJ, qreal deltaK, int steps)
{
    CarOrientation o;
    o.setOrientation(*m_defaultOrientation);
    o.mOrientation[0] += deltaI;
    o.mOrientation[1] += deltaJ;
    o.mOrientation[2] += deltaK;
    m_animator->setOrientationTo(o);
    m_animator->setAnimationSteps(steps);
}

QString QuickCarItem::note() const
{
    return m_note;
}

int QuickCarItem::highlight() const
{
    return m_highlight;
}

QString QuickCarItem::source() const
{
    return m_source;
}

void QuickCarItem::setNote(const QString &note)
{
    if (note != m_note) {
        m_note = note;
        emit noteChanged(note);
    }
}

void QuickCarItem::setHighlight(int number)
{
    if (m_highlight == number)
        return;

    if (m_model) {
        m_model->SetHighlightMesh(number);
    }

    m_highlight = number;
    emit highlightChanged(number);
}

void QuickCarItem::setSource(const QString &source)
{
    QString adjusted = qmlContext(this)->resolvedUrl(source).toLocalFile();

    if (m_source == adjusted)
        return;

    m_model.reset();
    m_source = adjusted;
    m_GLSetupDone = false;
    emit sourceChanged(adjusted);
}

void QuickCarItem::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_animationTimerId) {
        if (m_model) {
            bool do_update = false;

            do_update |= m_animator->updateAnimation();
            do_update |= m_model->animate();

            if (do_update) {
                update();
            }
        }
    }
}
