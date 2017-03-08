#include "filesendmodel.h"
#include "multifileshareadaptor.h"
#include "recordfilemodel.h"
#include <QDebug>
FileSendModel::FileSendModel(QObject *parent):QObject(parent)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    m_multFileShare= MultiFileShareAdaptor::getInstance();
    MultiFileShareAdaptor *fileShare = MultiFileShareAdaptor::getInstance();
    bool fileShareReg = bus.registerObject("/multifileshare", fileShare, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals);
    if (!fileShareReg)
        qDebug() << "bus error:" << "register multifileshare false";
    init();
}
FileSendModel::~FileSendModel()
{

}

void FileSendModel::init()
{
    //m_multFileShare->onSetConnectType(0);
    m_multFileShare->onSetPort(10005);
    m_multFileShare->onInitServer();
    QString recPath;
    recPath.append("/home/user/sinux/com.sinux.MultiFunction/videos/");
    m_multFileShare->onSetReceivePath(recPath);
}

void FileSendModel::setRemoteIp(const QString &remoteIp)
{
    m_remIp = remoteIp;
}
bool FileSendModel::sendFile()
{
    QList<QString> listSendFile = m_recordFileModel->getSendFile();

    QStringList ipList;
    ipList.push_back(m_remIp);
    m_multFileShare->onSetAddresses(ipList);
    m_multFileShare->onConnect();
    if(listSendFile.empty())
        return false;
//    if(!m_multFileShare->onIsConnected())
//        return false;
    QString sendPath;
    sendPath.append("/home/user/sinux/com.sinux.MultiFunction/videos/");
    QList<QString>::iterator iter = listSendFile.begin();
    for(;iter!= listSendFile.end();++iter)
    {sendPath.append(*iter);
        m_multFileShare->onSendFile(sendPath);
    }
    return true;
}
void FileSendModel::setRecordFileModel(RecordFileModel* recordFileModel)
{
    m_recordFileModel = recordFileModel;
}
