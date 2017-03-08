#include <QSqlTableModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDBusMetaType>
#include "contacterinterface.h"

ContacterInterface::ContacterInterface(QObject *parent)
    : QObject(parent)
{
    qDBusRegisterMetaType<Contacter>();
    qDBusRegisterMetaType<PhoneNumber>();
    qDBusRegisterMetaType<ContacterList>();

    //init contacter data
    m_contacterModel.setTable("contacter");
    m_contacterModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_contacterModel.select();

    //init local data
    m_localModel.setTable("local");
    m_localModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_localModel.select();
}

bool ContacterInterface::onAddContacter(const Contacter &contacter)
{
    if(contacter.id == 0)
        return false;
    if(contacter.name.isEmpty())
        return false;
    if(checkUserExist(contacter.id))
        return false;

    int rowCount = m_contacterModel.rowCount();
    bool insertFlag = m_contacterModel.insertRow(rowCount);
    if(!insertFlag)
        return false;
    bool setFlag = setContacter(rowCount, contacter);
    if(!setFlag)
        return false;

    emitChangeSignal(CONTACTER_ADD_HANDLE, contacter);
    m_contacterModel.submitAll();
    return true;
}

bool ContacterInterface::onRemoveContacter(const quint32 &id)
{
    if(id == 0)
        return false;

    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_contacterModel.index(i, 0);
        if(m_contacterModel.data(index).toUInt() == id)
        {
            Contacter changeContacter = getContacter(i);
            m_contacterModel.removeRow(i);
            emitChangeSignal(CONTACTER_DELETE_HANDLE, changeContacter);
            m_contacterModel.submitAll();
            return true;
        }
    }
    return false;
}

QDBusVariant ContacterInterface::onGetContacter(const quint32 &id)
{
    if(id == 0)
        return QDBusVariant();

    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_contacterModel.index(i, 0);
        if(m_contacterModel.data(index).toUInt() == id)
        {
            Contacter contacter = getContacter(i);
            QDBusVariant dbusValue;
            QVariant contacterVar = QVariant::fromValue(contacter);
            dbusValue.setVariant(contacterVar);
            return dbusValue;
        }
    }
    return QDBusVariant();
}

QDBusVariant ContacterInterface::onGetContacters()
{
    QList<quint32> ids;
    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_contacterModel.index(i, 0);
        quint32 id = m_contacterModel.data(index).toUInt();
        ids.append(id);
    }

    QDBusVariant dbusValue;
    QVariant idsVar = QVariant::fromValue(ids);
    dbusValue.setVariant(idsVar);
    return dbusValue;
}

int ContacterInterface::onGetContactersCount()
{
    return m_contacterModel.rowCount();
}

bool ContacterInterface::onUpdateContacter(const Contacter &contacter)
{
    if(contacter.id == 0)
        return false;
    if(contacter.name.isEmpty())
        return false;
    if(!checkUserExist(contacter.id))
        return false;

    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_contacterModel.index(i, 0);
        if(m_contacterModel.data(index).toUInt() == contacter.id)
        {
            setContacter(i, contacter);
            Contacter contacter = getContacter(i);
            emitChangeSignal(CONTACTER_UPDATE_HANDLE, contacter);
            m_contacterModel.submitAll();
            return true;
        }
    }
    return false;
}

bool ContacterInterface::onUpdateLocalInfo(const Contacter &localInfo)
{
    QModelIndex idIndex = m_localModel.index(0, 0);
    m_localModel.setData(idIndex, localInfo.id);

    QModelIndex nameIndex = m_localModel.index(0, 1);
    m_localModel.setData(nameIndex, localInfo.name);

    QModelIndex titleIndex = m_localModel.index(0, 2);
    m_localModel.setData(titleIndex, localInfo.title);

    QModelIndex numbersIndex = m_localModel.index(0, 3);
    QString numbers = converNumbersToString(localInfo.numbers);
    m_localModel.setData(numbersIndex, numbers);

    QModelIndex photoIndex = m_localModel.index(0, 4);
    m_localModel.setData(photoIndex, localInfo.photoPath);

    QModelIndex leadersIndex = m_localModel.index(0, 5);
    QString leaders = convertLeadersToString(localInfo.leaders);
    m_localModel.setData(leadersIndex, leaders);

    QModelIndex snNumberIndex = m_localModel.index(0, 6);
    m_localModel.setData(snNumberIndex, localInfo.snNumber);

    QModelIndex bluetoothIndex = m_localModel.index(0, 7);
    m_localModel.setData(bluetoothIndex, localInfo.bluetooth);

    return true;
}

QDBusVariant ContacterInterface::onGetLocalInfo()
{
    Contacter local = getLocalInfo();
    QDBusVariant dbusValue;
    QVariant localVar = QVariant::fromValue(local);
    dbusValue.setVariant(localVar);
    return dbusValue;
}

bool ContacterInterface::checkUserExist(const quint32 userId)
{
    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_contacterModel.index(i, 0);
        if(m_contacterModel.data(index).toUInt() == userId)
            return true;
    }
    return false;
}

Contacter ContacterInterface::getContacter(const int &row)
{
    int rowCount = m_contacterModel.rowCount();
    if(rowCount < row || row < 0)
        return Contacter();

    Contacter contacter;
    QModelIndex idIndex = m_contacterModel.index(row, 0);
    contacter.id = m_contacterModel.data(idIndex).toUInt();

    QModelIndex nameIndex = m_contacterModel.index(row, 1);
    contacter.name = m_contacterModel.data(nameIndex).toString();

    QModelIndex titleIndex = m_contacterModel.index(row, 2);
    contacter.title = m_contacterModel.data(titleIndex).toInt();

    QModelIndex numbersIndex = m_contacterModel.index(row, 3);
    QString numbersValue = m_contacterModel.data(numbersIndex).toString();
    contacter.numbers = converNumbersToStruct(numbersValue);

    QModelIndex photoIndex = m_contacterModel.index(row, 4);
    contacter.photoPath = m_contacterModel.data(photoIndex).toString();

    QModelIndex leadersIndex = m_contacterModel.index(row, 5);
    QString leadersValue = m_contacterModel.data(leadersIndex).toString();
    contacter.leaders = convertStringToLeaders(leadersValue);

    QModelIndex snNumberIndex = m_contacterModel.index(row, 6);
    contacter.snNumber = m_contacterModel.data(snNumberIndex).toString();

    QModelIndex bluetoothIndex = m_contacterModel.index(row, 7);
    contacter.bluetooth = m_contacterModel.data(bluetoothIndex).toString();
    return contacter;
}

Contacter ContacterInterface::getLocalInfo()
{
    int rowCount = m_localModel.rowCount();
    if(rowCount < 0)
        return Contacter();

    Contacter local;
    QModelIndex idIndex = m_localModel.index(0, 0);
    local.id = m_localModel.data(idIndex).toUInt();

    QModelIndex nameIndex = m_localModel.index(0, 1);
    local.name = m_localModel.data(nameIndex).toString();

    QModelIndex titleIndex = m_localModel.index(0, 2);
    local.title = m_localModel.data(titleIndex).toInt();

    QModelIndex numbersIndex = m_localModel.index(0, 3);
    QString numbersValue = m_localModel.data(numbersIndex).toString();
    local.numbers = converNumbersToStruct(numbersValue);

    QModelIndex photoIndex = m_localModel.index(0, 4);
    local.photoPath = m_localModel.data(photoIndex).toString();

    QModelIndex leadersIndex = m_localModel.index(0, 5);
    QString leadersValue = m_localModel.data(leadersIndex).toString();
    local.leaders = convertStringToLeaders(leadersValue);

    QModelIndex snNumberIndex = m_localModel.index(0, 6);
    local.snNumber = m_localModel.data(snNumberIndex).toString();

    QModelIndex bluetoothIndex = m_localModel.index(0, 7);
    local.bluetooth = m_localModel.data(bluetoothIndex).toString();

    return local;
}

bool ContacterInterface::setContacter(const int &row, const Contacter &contacter)
{
    QModelIndex idIndex = m_contacterModel.index(row, 0);
    m_contacterModel.setData(idIndex, contacter.id);

    QModelIndex nameIndex = m_contacterModel.index(row, 1);
    m_contacterModel.setData(nameIndex, contacter.name);

    QModelIndex titleIndex = m_contacterModel.index(row, 2);
    m_contacterModel.setData(titleIndex, contacter.title);

    QModelIndex numbersIndex = m_contacterModel.index(row, 3);
    QString numbers = converNumbersToString(contacter.numbers);
    m_contacterModel.setData(numbersIndex, numbers);

    QModelIndex photoIndex = m_contacterModel.index(row, 4);
    m_contacterModel.setData(photoIndex, contacter.photoPath);

    QModelIndex leadersIndex = m_contacterModel.index(row, 5);
    QString leaders = convertLeadersToString(contacter.leaders);
    m_contacterModel.setData(leadersIndex, leaders);

    QModelIndex snNumberIndex = m_contacterModel.index(row, 6);
    m_contacterModel.setData(snNumberIndex, contacter.snNumber);

    QModelIndex bluetoothIndex = m_contacterModel.index(row, 7);
    m_contacterModel.setData(bluetoothIndex, contacter.bluetooth);

    return true;
}

QString ContacterInterface::converNumbersToString(const QList<PhoneNumber> &numbers)
{
    QJsonObject object;
    for(int i = 0, iend = numbers.count(); i < iend; i++)
    {
        QString type = converNetworkTypeToString((NetworkType)numbers[i].first);
        QString address = numbers[i].second;
        object.insert(type, address);
    }

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

QList<PhoneNumber> ContacterInterface::converNumbersToStruct(const QString &value)
{
    QList<PhoneNumber> numbers;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonObject object = document.object();
    QJsonObject::iterator start = object.begin();
    QJsonObject::iterator end = object.end();
    for(; start != end; start++)
    {
        PhoneNumber number;
        number.first = converStringToNetworkType(start.key());
        number.second = start.value().toString();

        numbers.append(number);
    }

    return numbers;
}

QString ContacterInterface::converNetworkTypeToString(const NetworkType &type)
{
    QString value;
    switch(type)
    {
    case AD_HOC_NETWORK:
        value = "ad_hoc_network";
        break;
    case LTE_NETWORK:
        value = "lte_network";
        break;
    case SATELLITE_NETWORK:
        value = "satellite_network";
        break;
    case DIGITAL_WALKITE_TALKIE_NETWORK:
        value = "digital_walkite_talkie_network";
        break;
    case BATTLE_NETWORK:
        value = "battle_network";
        break;
    case UNKNOW_NETWORK:
        value = "unknow_network";
        break;
    default:
        value = "unknow_network";
    }

    return value;
}

NetworkType ContacterInterface::converStringToNetworkType(const QString &value)
{
    if(value == "ad_hoc_network")
        return AD_HOC_NETWORK;
    else if(value == "lte_network")
        return LTE_NETWORK;
    else if(value == "satellite_network")
        return SATELLITE_NETWORK;
    else if(value == "digital_walkite_talkie_network")
        return DIGITAL_WALKITE_TALKIE_NETWORK;
    else if(value == "battle_network")
        return BATTLE_NETWORK;
    else if(value == "unknow_network")
        return UNKNOW_NETWORK;
    else
        return UNKNOW_NETWORK;

    return UNKNOW_NETWORK;
}

QString ContacterInterface::convertLeadersToString(const QList<quint32> &leaders)
{
    QJsonArray leaderArray;
    for(int i = 0, iend = leaders.count(); i < iend; i++)
    {
        QJsonValue leaderValue((qint64)leaders[i]);
        leaderArray.append(leaderValue);
    }

    QJsonDocument document(leaderArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

QList<quint32> ContacterInterface::convertStringToLeaders(const QString &value)
{
    QList<quint32> leaders;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        quint32 leaderId = value.toInt();
        leaders.append(leaderId);
    }

    return leaders;
}

void ContacterInterface::emitChangeSignal(int type, Contacter changeContacter)
{
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(changeContacter);
    dbusValue.setVariant(dataVar);
    emit signalContacterChanged(type, dbusValue);
}
