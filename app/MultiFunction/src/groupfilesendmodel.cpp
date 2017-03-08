#include "groupfilesendmodel.h"
#include "../../commondata/datatype/groupdata.h"
#include "recordfilemodel.h"
GroupFileSendModel::GroupFileSendModel(QObject *parent):QObject(parent)
{
    //m_pGroupInterfaceAdaptor = new GroupInterfaceAdaptor();
    m_groupAdaptor = GroupInterfaceAdaptor::getInstance();
    m_adaptor = ContacterInterfaceAdaptor::getInstance();
    m_pMutiCastAdaptor = MutiCastAdaptor::getInstance();
    m_groupId = -1;
    connect(m_groupAdaptor,SIGNAL(signalGroupChanged(int,Group)),this,SLOT(onGroupChanged(int,Group)));
}
GroupFileSendModel::~GroupFileSendModel()
{

}
void GroupFileSendModel::GetOtherMember(QString groupId)
{
    m_remoteAddress.clear();
    Contacter local;
    local = m_adaptor->getLocalInfo();
    m_localId = local.id;

    m_groupInfo = m_groupAdaptor->getGroup(groupId);
    Group value;
    Contacter contacter;
    QString titleName;
    QString name;

    value = m_groupAdaptor->getGroup(groupId);
    /*bIsLeader: creater:1 leader:2 member:3*/
    if (m_localId != value.creator) {
        contacter = m_adaptor->getContacter(value.creator);


        //m_remoteAddress.push_back();
        // m_remoteAddress.append(contacter.numbers);
        //name = contacter.surname + " " + contacter.name;
        //titleName = getTitleStringFromEnum(contacter.title);
        //emit signalSendMemberInfoToGroup(contacter.id, "1",name, titleName, QString("%1").arg(contacter.type));
    }
    qDebug()<<"getGroup leaders"<<value.leaders;
    for (int i = 0; i < value.leaders.count(); i++) {
        if (m_localId == value.leaders[i]) {
            continue;
        } else {
            contacter = m_adaptor->getContacter(value.leaders[i]);
            if(!contacter.numbers.empty())
            {
                QList<PhoneNumber>::iterator itr = contacter.numbers.begin();
                for(;itr!=contacter.numbers.end();++itr)
                {
                    m_remoteAddress.push_back(itr->number);
                }
            }
        }

        //name = contacter.surname + " " + contacter.name;
        //titleName = getTitleStringFromEnum(contacter.title);
        //emit signalSendMemberInfoToGroup(contacter.id, "2",name, titleName, QString("%1").arg(contacter.type));
    }
    qDebug()<<"getGroup members"<<value.members;
    for (int i = 0; i < value.members.count(); i++) {
        if (m_localId == value.members[i]) {
            continue;
        } else {
            contacter = m_adaptor->getContacter(value.members[i]);
            if(!contacter.numbers.empty())
            {
                QList<PhoneNumber>::iterator itr = contacter.numbers.begin();
                for(;itr!=contacter.numbers.end();++itr)
                {
                    m_remoteAddress.push_back(itr->number);
                }
            }
        }
        //name = contacter.surname + " " + contacter.name;
        //titleName = getTitleStringFromEnum(contacter.title);
        //emit signalSendMemberInfoToGroup(contacter.id, "3",name, titleName, QString("%1").arg(contacter.type));
    }
}
void GroupFileSendModel::SetGroupId(QString groupId)
{
    GetOtherMember(groupId);
    m_groupId = m_pMutiCastAdaptor->GetGroupId(groupId);
}

void GroupFileSendModel::getGroups(bool bIsDelete)
{

    qDebug() << "------------------getGroups---------------------------";
    QStringList groupList;
    if (bIsDelete)
        groupList = m_groupAdaptor->getSelfCreatedGroups(CONVERSION_GROUP);
    else
        groupList = m_groupAdaptor->getGroups(CONVERSION_GROUP);
    qDebug() <<"bIsDelete:"<<bIsDelete<<groupList;
    for(int i = 0, iend = groupList.count(); i < iend; i++) {
        Group group = m_groupAdaptor->getGroup(groupList[i]);
        if (group.id == "") {
            continue;
        }
        qDebug() << "group id:" << group.id;
        qDebug() << "group name:" << group.name;
        qDebug() << "group members:" << group.members.count();
        qDebug() << "group leaders:" << group.leaders.count();
        QString num = QString("%1").arg(group.members.count() + group.leaders.count() + 1);
        QString networkType;
        switch (group.networkType) {
        case AD_HOC_NETWORK:
            networkType = "自组网";
            break;
        case LTE_NETWORK:
            networkType = "LTE";
            break;
        case DIGITAL_WALKIE_TALKIE_NETWORK:
            networkType = "模块数字对讲";
            break;
        case BATTLE_NETWORK:
            networkType = "战互网";
            break;
        case SATELLITE_NETWORK:
            networkType = "天通网";
            break;
        case ADHOC_LTE_NETWORK:
            networkType = "默认";
            break;
        default:
            networkType = "";
            break;
        }
        emit signalSendGroupInfoToGroup(group.id, group.name, networkType, num);
    }
    /*
        QStringList contacters = m_adaptor->getContacters(type);
        for(int i = 0, iend = contacters.count(); i < iend; i++)
        {
            Contacter contacter = m_adaptor->getContacter(contacters[i]);
            if(contacter.id == "")
            {
                continue;
            }
            QVariant name = contacter.surname + " " + contacter.name;
            QVariant title;
            switch (contacter.title) {
            case BATTLION:
                title = "营长";
                break;
            case COMPANY:
                title = "连长";
                break;
            case PLATOON:
                title = "排长";
                break;
            case SQUAD:
                title = "班长";
                break;
            case SOLIDER:
                title = "战士";
                break;
            default:
                title = "";
                break;
            }
            QVariant size = contacter.namePinyin;
            QVariant userid = contacter.id;
            QVariant contactertype = QString("%1").arg(contacter.type);
            QMetaObject::invokeMethod(m_qmlRootObject, "insertMainList",Q_ARG(QVariant,name),Q_ARG(QVariant,title),Q_ARG(QVariant,size)
                                      ,Q_ARG(QVariant,userid), Q_ARG(QVariant,contactertype));
        }
*/
}
void GroupFileSendModel::onGroupChanged(int type, Group group)
{
    QString id = QString("%1").arg(group.id);
    QString num = QString("%1").arg(group.members.count() + group.leaders.count() + 1);
    QString networkType;
    switch (group.networkType) {
    case AD_HOC_NETWORK:
        networkType = "自组网";
        break;
    case LTE_NETWORK:
        networkType = "LTE";
        break;
    case SATELLITE_NETWORK:
        networkType = "天通网";
        break;
    case DIGITAL_WALKIE_TALKIE_NETWORK:
        networkType = "模块数字对讲";
        break;
    case BATTLE_NETWORK:
        networkType = "战互网";
        break;
    case ADHOC_LTE_NETWORK:
        networkType = "默认";
        break;
    case WIFI_NETWORK:
        networkType = "WIFI";
        break;
    default:
        networkType = "";
        break;
    }
    emit signalGroupChanged(type,group.id,group.name,networkType,num);
}

void GroupFileSendModel::SendFile()
{
    QList<QString> listSendFile = m_recordFileModel->getSendFile();
    if(m_groupId == -1)
        return ;
    QList<QString>::iterator iter = listSendFile.begin();
    for(;iter!= listSendFile.end();++iter)
    {
        QString sendPath = QString("/home/user/sinux/com.sinux.MultiFunction/videos/");
        sendPath=sendPath+(*iter);
        m_pMutiCastAdaptor->sendData(m_groupId,(*iter),sendPath);
    }
}

void GroupFileSendModel::setRecordFileModel(RecordFileModel* recordFileModel)
{
    m_recordFileModel = recordFileModel;
}
