#include "tablemodel.h"
#include <QDebug>
#include <QQmlPropertyMap>

ContacterModel::ContacterModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_contacterAdaptor = ContacterInterfaceAdaptor::getInstance();
    contacters = new QMap<QString, Contacter>();
}

QHash<int, QByteArray> ContacterModel::roleNames() const
{
    return m_roleNames;
}

QVariant ContacterModel::getContacterValue(int index, QString key)
{
    Contacter contacter;
    QHash<QString, QVariant> ContacterModelHash;
    contacter = m_contacterAdaptor->getContacter(m_idList.at(index));
    ContacterModelHash.insert("id",contacter.id);
    ContacterModelHash.insert("personName",contacter.snNumber+ " " + contacter.name);
    ContacterModelHash.insert("position",getTitleStringFromEnum(contacter.title));
    ContacterModelHash.insert("size",contacter.namePinyin);
    ContacterModelHash.insert("contacterType",QString("%1").arg(contacter.type));
    if (key == "") {
        return ContacterModelHash;
    } else {
        return ContacterModelHash.value(key);
    }
}

void ContacterModel::removeContacter(QString info, int rows)
{
    int temp = 0;
    bool bIsFind = false;
    for (int i = 0; i < rows; i++) {
        if (m_idList.at(i) == info) {
            bIsFind = true;
            temp = i;
            break;
        }
    }
    if (bIsFind) {
        beginRemoveRows(QModelIndex(),temp,temp);
        m_idList.removeAt(temp);
        endRemoveRows();
    }
}

QVariant ContacterModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if(role < Qt::UserRole)
    {
        qDebug() << "ContacterModel::data(const QModelIndex &index, int role) const";
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QString rowId = m_idList.at(index.row());
        Contacter contacter;
        if(!contacters->contains(rowId))
        {
            contacter = m_contacterAdaptor->getContacter(rowId);
            contacters->insert(rowId, contacter);
        }
        contacter = contacters->value(rowId);
        if (columnIdx == 0) {
            value = QVariant::fromValue(contacter.surname+ " " + contacter.name);
        } else if(columnIdx == 1) {
            QString title;
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
            value = QVariant::fromValue(title);
        } else if(columnIdx == 2) {
            if (contacter.namePinyin >= "1" && contacter.namePinyin <= "9") {
                value = QVariant::fromValue(QVariant("#"));
            } else {
                value = QVariant::fromValue(contacter.namePinyin);
            }
        } else if(columnIdx == 3) {
            value = QVariant::fromValue(contacter.id);
        } else if(columnIdx == 4) {
            value = QVariant::fromValue(QString("%1").arg(contacter.type));
        }
    }
    return value;
}

void ContacterModel::setModelList(const int &contacterType, const QString& searchText)
{
    beginResetModel();
    m_idList = m_contacterAdaptor->searchContactors(contacterType, searchText);
    endResetModel();
}

int ContacterModel::rowCount(const QModelIndex & parent) const
{
    return m_idList.count();
}

int ContacterModel::count()
{
    return m_idList.count();
}

int ContacterModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ContacterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void ContacterModel::setRoles(const QStringList &names)
{
    qDebug()<<names;
    for(int idx=0; idx<names.count(); idx++)
    {
        m_roleNames[Qt::UserRole + idx + 1] = names[idx].toLocal8Bit();
    }
    qDebug()<<"set end;";
}

/*
CallHistoryModel::CallHistoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_contacterAdaptor = ContacterInterfaceAdaptor::getInstance();
    m_historyAdaptor = CallHistoryInterfaceAdaptor::getInstance();
}

QVariant CallHistoryModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if(role < Qt::UserRole)
    {
        qDebug() << "CallHistoryModel::data(const QModelIndex &index, int role) const";
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QString rowId = m_idList.at(index.row());
        QString name,titleName,networkInfo,time,titleId;
        Contacter contacter;
        CallHistory callHistory = m_historyAdaptor->getCallHistory(rowId);
        if (callHistory.dialTarget.count() != 0) {
            contacter = m_contacterAdaptor->getContacter(callHistory.dialTarget[0]);
            titleId = contacter.id;
            name = contacter.surname + " " + contacter.name;
            switch (contacter.title) {
            case BATTLION:
                titleName = "营长";
                break;
            case COMPANY:
                titleName = "连长";
                break;
            case PLATOON:
                titleName = "排长";
                break;
            case SQUAD:
                titleName = "班长";
                break;
            case SOLIDER:
                titleName = "战士";
                break;
            default:
                titleName = "";
                break;
            }
        } else {
            name = "";
            titleName = "";
            titleId = "null";
        }
        switch (callHistory.dialNetworkType) {
        case PS_NETWORK_VOICE_DIAL:
        case PS_NETWORK_VIDEO_DIAL:
        case PS_NETWORK_VOICE_MUL_DIAL:
            networkInfo = "LTE";
            break;
        case CS_NETWORK_VOICE_DIAL:
        case CS_NETWORK_VOICE_MUL_DIAL:
            networkInfo = "天通网";
            break;
        default:
            break;
        }
        QDateTime nowtime = QDateTime::currentDateTime();
        int timediff = callHistory.dialDateStart.daysTo(nowtime);
        if (timediff == 0) {
            time = callHistory.dialDateStart.toString("hh:mm:ss");
        } else if ((timediff <= 7) && (timediff >= 1)) {
            time = callHistory.dialDateStart.toString("dddd hh:mm");
        } else {
            time = callHistory.dialDateStart.toString("yyyy年M月d日 hh:mm");
        }
        if (columnIdx == 0) {
            value = QVariant::fromValue(callHistory.dialHistoryId);
        } else if (columnIdx == 1) {
            value = QVariant::fromValue(titleId);
        } else if (columnIdx == 2) {
            value = QVariant::fromValue(callHistory.dialType);
        } else if (columnIdx == 3) {
            value = QVariant::fromValue(name);
        } else if (columnIdx == 4) {
            value = QVariant::fromValue(titleName);
        } else if (columnIdx == 5) {
            value = QVariant::fromValue(callHistory.dialTargetNumber.at(0));
        } else if (columnIdx == 6) {
            value = QVariant::fromValue(time);
        } else if (columnIdx == 7) {
            value = QVariant::fromValue(networkInfo);
        }
    }
    return value;
}

void CallHistoryModel::setModelList(int type,const QString& searchText)
{
    beginResetModel();
    m_idList = m_historyAdaptor->searchCallHistory(searchText,type);
    endResetModel();
}



int CallHistoryModel::rowCount(const QModelIndex & parent) const
{
    return m_idList.count();
}

int CallHistoryModel::count()
{
    return m_idList.count();
}

int CallHistoryModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CallHistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void CallHistoryModel::setRoles(const QStringList &names)
{
    m_roleNames.clear();
    for(int idx=0; idx<names.count(); idx++)
    {
        m_roleNames[Qt::UserRole + idx + 1] = names[idx].toLocal8Bit();
    }
    qDebug() << "m_roleNames" << m_roleNames;
}

QHash<int, QByteArray> CallHistoryModel::roleNames() const
{
    return m_roleNames;
}


QVariant CallHistoryModel::getCallHistoryValue(int index, QString key)
{
    Contacter contacter;
    QHash<QString, QVariant> callHistoryModelHash;

    QString time,networkInfo;
    CallHistory callHistory = m_historyAdaptor->getCallHistory(m_idList.at(index));
    qDebug()<<"get:id"<<callHistory.dialHistoryId;
    callHistoryModelHash.insert("id",callHistory.dialHistoryId);
    callHistoryModelHash.insert("iconValue",callHistory.dialType);
    if (callHistory.dialTarget.count() != 0) {
        contacter = m_contacterAdaptor->getContacter(callHistory.dialTarget[0]);
        callHistoryModelHash.insert("serialnum",contacter.id);
        callHistoryModelHash.insert("theName",contacter.surname + " " + contacter.name);
        callHistoryModelHash.insert("info",getTitleStringFromEnum(contacter.title));
    } else {
        callHistoryModelHash.insert("serialnum","null");
        callHistoryModelHash.insert("theName","");
        callHistoryModelHash.insert("info","");
    }
    switch (callHistory.dialNetworkType) {
    case PS_NETWORK_VOICE_DIAL:
    case PS_NETWORK_VIDEO_DIAL:
    case PS_NETWORK_VOICE_MUL_DIAL:
        networkInfo = "LTE";
        break;
    case CS_NETWORK_VOICE_DIAL:
    case CS_NETWORK_VOICE_MUL_DIAL:
        networkInfo = "天通网";
        break;
    default:
        break;
    }
    QDateTime nowtime = QDateTime::currentDateTime();
    int timediff = callHistory.dialDateStart.daysTo(nowtime);
    if (timediff == 0) {
        time = callHistory.dialDateStart.toString("hh:mm:ss");
    } else if ((timediff <= 7) && (timediff >= 1)) {
        time = callHistory.dialDateStart.toString("dddd hh:mm");
    } else {
        time = callHistory.dialDateStart.toString("yyyy年M月d日 hh:mm");
    }
    callHistoryModelHash.insert("num",callHistory.dialTargetNumber.at(0));
    callHistoryModelHash.insert("timeinfo",time);
    callHistoryModelHash.insert("netinfo",networkInfo);
    if (key == "") {
        return callHistoryModelHash;
    } else {
        return callHistoryModelHash.value(key);
    }
}

void CallHistoryModel::removeCallHistory(QString info, int rows)
{
    int temp = 0;
    bool bIsFind = false;
    for (int i = 0; i < rows; i++) {
        if (m_idList.at(i) == info) {
            bIsFind = true;
            temp = i;
            break;
        }
    }
    if (bIsFind) {
        beginRemoveRows(QModelIndex(),temp,temp);
        m_idList.removeAt(temp);
        endRemoveRows();
    }
}
*/
