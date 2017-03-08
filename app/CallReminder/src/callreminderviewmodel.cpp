#include "callreminderviewmodel.h"
#include <QDebug>
#include "miniphonecalladaptor.h"

CallReminderViewModel::CallReminderViewModel(QObject *parent) :
    QObject(parent),
    m_miniAdaptor(NULL)
{
    m_miniAdaptor = new MiniPhoneCallAdaptor(this);
    m_miniAdaptor->setIsMini(true);
}

CallReminderViewModel::~CallReminderViewModel()
{
    if (m_miniAdaptor)
        delete m_miniAdaptor;

    m_miniAdaptor->setIsMini(false);
}

QString CallReminderViewModel::getHeadImg()
{
    return m_headImg;
}

QString CallReminderViewModel::getCallInName()
{
    return m_callInName;
}

QString CallReminderViewModel::getTelphoneNumber()
{
    return m_telphoneNumber;
}

QString CallReminderViewModel::getCallHandleId()
{
    return m_callhandleId;
}

void CallReminderViewModel::setHeadImg(const QString &text)
{
    m_headImg = text;
    emit signalHeadImgChanged(m_headImg);
}

void CallReminderViewModel::setCallInName(const QString &text)
{
    m_callInName = text;
    emit signalCallInNameChanged(m_callInName);
}

void CallReminderViewModel::setTelphoneNumber(const QString &text)
{
    m_telphoneNumber = text;
    emit signalTelphoneNumberChanged(m_telphoneNumber);
}

void CallReminderViewModel::setCallHandleId(const QString &text)
{
    m_callhandleId = text;
    emit signalCallHandleIdChanged(m_callhandleId);
}

void CallReminderViewModel::setCallInfo(const QString &img, const QString &name, const QString &tel)
{
    setHeadImg(img);
    setCallInName(name);
    setTelphoneNumber(tel);
}

void CallReminderViewModel::callAccept()
{
    qDebug() << "callaccept";
    m_miniAdaptor->answer(m_callhandleId);
    m_miniAdaptor->showFullScreen();
    emit signalHideScreen();
}

void CallReminderViewModel::callResume()
{
    qDebug() << "callResume";
    m_miniAdaptor->hangup(m_callhandleId);
    emit signalHideScreen();
}

void CallReminderViewModel::callComing(int type , MiniDialData mini)
{
    if (type == HANDLER_ADD && mini.isIncoming)
    {
        qDebug() << "photoPath:" << mini.photoPath << mini.name << mini.dialNum;
        setCallInfo(mini.photoPath, mini.name, mini.dialNum);
        setCallHandleId(mini.handlerId);
        emit signalShowScreen();
    }
}

