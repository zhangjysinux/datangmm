#include "miniphonecalladaptor.h"
#include <QDebug>
#include <QDBusMetaType>
#include "callreminderviewmodel.h"

MiniPhoneCallAdaptor *MiniPhoneCallAdaptor::m_instance = NULL;
MiniPhoneCallAdaptor *MiniPhoneCallAdaptor::getInstance()
{
    if(m_instance == NULL)
        m_instance = new MiniPhoneCallAdaptor();

    return m_instance;
}

MiniPhoneCallAdaptor::MiniPhoneCallAdaptor(QObject *parent) :
    QObject(parent),
  m_interface("com.sinux.dial", "/dial", "com.sinux.DBus.dial", QDBusConnection::sessionBus())
{
    qDBusRegisterMetaType<MiniDialData>();
    if(!m_interface.isValid())
    {
        qDebug() << "qdbus error:" << QDBusConnection::sessionBus().lastError().message();
    }
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.connect("com.sinux.dial", "/dial", "com.sinux.DBus.dial", "signalSendMiniData",
                       this,SLOT(onReceiveMiniData(int,MiniDialData)));

}

int MiniPhoneCallAdaptor::dial(int net, QString phoneNum)
{
    m_interface.call("onDial", net , phoneNum);
}

void MiniPhoneCallAdaptor::hangup(const QString &handId)
{
    m_interface.call("onHangup", handId);
}

void MiniPhoneCallAdaptor::answer(const QString &handId)
{
    m_interface.call("onAnswer", handId);
}

int MiniPhoneCallAdaptor::setIsMini(bool on)
{
    int flag;
    QDBusReply<int> addReply =  m_interface.call("onSetIsMini", on);
    if(addReply.isValid())
        flag = addReply.value();
    else{
        qDebug() << "onSetIsMini reply error:" << addReply.error();
        flag = 0;
    }
    return flag;
}

int MiniPhoneCallAdaptor::showFullScreen()
{
    m_interface.call("onShowFullScreen");
    return 1;
}

void MiniPhoneCallAdaptor::onReceiveMiniData(int type, MiniDialData mini)
{
    emit signalSendDataTomini(type, mini);
    qDebug() << "onReceiveMiniData" << type << mini.dialNum << mini.handlerId << mini.name << mini.photoPath;

    CallReminderViewModel *viewModel = dynamic_cast<CallReminderViewModel*>(this->parent());
    if (viewModel)
    {
        viewModel->callComing(type, mini);
    }
}
