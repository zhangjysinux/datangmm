#include <QSqlTableModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDBusMetaType>
#include <QUuid>
#include "contacterinterface.h"
#include "chinesetopinyin.h"
#include <QTime>
#include <QSqlQuery>
#include "connectiondatabase.h"
#include "../../commondata/datatype/messagedata.h"
#include "cnotification.h"
#include "cnotificationmanager.h"
#include "groupinterface.h"

ContacterInterface *ContacterInterface::m_instance = NULL;
ContacterInterface::ContacterInterface(QObject *parent)
    : QObject(parent),
      m_contacterModel(0, ConnectionDatabase::getGblDb()),
      m_localModel(0, ConnectionDatabase::getGblDb())
{
    qDBusRegisterMetaType<Contacter>();
    qDBusRegisterMetaType<PhoneNumber>();
    qDBusRegisterMetaType<ContacterList>();

    //init contacter data
    m_contacterModel.setTable("contacter");
    m_contacterModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
//    m_contacterModel.setSort(9, Qt::AscendingOrder);
    m_contacterModel.select();

    //init local data
    m_localModel.setTable("local");
    m_localModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_localModel.select();

//    QStringList cList = onSearchContactors(3, "");
//    qDebug() << "onSearchContacter: " << cList.count() << cList;
//    qDebug() << "onGetContacters ADHOC_LTE_NETWORK" << onSearchContactorsExcept(3, ADHOC_LTE_NETWORK, "192", QStringList());
    m_contacterPushAdaptor = ContacterPushAdaptor::getInstance();
    connect(m_contacterPushAdaptor, SIGNAL(signalContacterReceived(Contacter)),
            this, SLOT(onContacterChangeToChangeLowLevelPeople(Contacter)));
    m_timer.setInterval(500);
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(contacterChangeToChangeLowLevelPeople()));

    m_downloadAdaptor = DownloadContactAdaptor::getInstance();
}

ContacterInterface *ContacterInterface::getInstance()
{
    if(m_instance == NULL)
        m_instance = new ContacterInterface();

    return m_instance;
}

bool ContacterInterface::isIdNumberAdapt(const QString &id, const QString &number)
{
    QString sql = QString("SELECT id FROM __tablename__ WHERE "
                          "id='%1' AND network LIKE '%%2%' "
                          ).arg(id).arg(number);

    QStringList result = m_contacterModel.getColumnDataWithSql(sql);
    if(!result.isEmpty())
        return true;
    else
        return false;
}

Contacter ContacterInterface::getContacterByPocId(const QString &pocid)
{
    Contacter contacter;

    int row = m_contacterModel.findDataInColumn(12, pocid);
    if(row != -1)
        contacter = getContacterFromRowData(m_contacterModel.rowData(row));

    return contacter;
}

int ContacterInterface::addContacter(const Contacter &contacter, Contacter &addedContacter)
{
    if(contacter.surname.isEmpty() && contacter.name.isEmpty())
        return setLastError(-1, "surname and name is empty, add contacter fail");

    addedContacter = contacter;
    if(addedContacter.id.isEmpty()){
        addedContacter.id = getUseableContacterId();
    }

    addedContacter.type = 2;

    int rowCount = m_contacterModel.rowCount();
    bool insertFlag = m_contacterModel.insertRow(rowCount);
    if(!insertFlag)
        return setLastError(-2, "contacter insert last row fail, add contacter fail");
    bool setFlag = setContacter(rowCount, addedContacter);
    if(!setFlag)
        return setLastError(-3, "contacter setContacter fail, add contacter fail");

    return setLastError(0, "add contacter success");
}

int ContacterInterface::addOrganizationContacter(const Contacter &contacter, Contacter &addedContacter)
{
    if(contacter.surname.isEmpty() && contacter.name.isEmpty())
        return setLastError(-1, "surname and name is empty, add contacter fail");

    addedContacter = contacter;

    addedContacter.type = 1;

    int rowCount = m_contacterModel.rowCount();
    bool insertFlag = m_contacterModel.insertRow(rowCount);
    if(!insertFlag)
        return setLastError(-2, "contacter insert last row fail, add contacter fail");
    bool setFlag = setContacter(rowCount, addedContacter);
    if(!setFlag)
        return setLastError(-3, "contacter setContacter fail, add contacter fail");

    return setLastError(0, "add contacter success");
}

int ContacterInterface::removeContacter(const QString &id, Contacter &removedContacter)
{
    if(id.isEmpty())
        return setLastError(-1, "id is empty, remove contacter fail");

//    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
//    {
//        QModelIndex index = m_contacterModel.index(i, 0);
//        if(m_contacterModel.data(index).toString() == id)
//        {
//            Contacter changeContacter = getContacter(i);
//            m_contacterModel.removeRow(i);
//            emitChangeSignal(CONTACTER_DELETE_HANDLE, changeContacter);
//            m_contacterModel.submitAll();
//            return true;
//        }
//    }
    int row = m_contacterModel.findDataInColumn(0, id);
    if(row != -1)
    {
        Contacter changeContacter = getContacter(row);

        if(m_contacterModel.removeRow(row))
        {
            removedContacter = changeContacter;
            return setLastError(0, "remove contacter success");
        }
        else
        {
            return setLastError(-2, "remove row fail, remove contacter fail");
        }
        //delete contacter then delete callhistory record

    }
    return setLastError(-3, "id not exists, remove contacter fail");
}

int ContacterInterface::updateContacter(const Contacter &contacter, Contacter &updatedContacter)
{
    if(contacter.id.isEmpty())
        return setLastError(-1, "id is empty, update contacter fail");
    if(!checkUserExist(contacter.id))
        return setLastError(-2, "id exists, update contacter fail");

    int row = m_contacterModel.findDataInColumn(0, contacter.id);

    if(row != -1)
    {
        setContacter(row, contacter);
        updatedContacter = contacter;

        return setLastError(0, "update contacter success");
    }

    return setLastError(-3, "can not find contacter id, update contacter fail");
}

bool ContacterInterface::onAddContacter(const Contacter &contacter)
{
    Contacter addedContacter;
    addContacter(contacter, addedContacter);

    if(lastErrorIsNoError())
    {
        m_contacterModel.submitAll();

        emitChangeSignal(CONTACTER_ADD_HANDLE, addedContacter);

        ContacterList contacters;
        contacters.append(addedContacter);
        emit signalContacterChangedToChangeRemoteContacter(CONTACTER_ADD_HANDLE, contacters);

        //post contacter to server
        Contacter sendContacter = addedContacter;
        sendContacter.handle = CONTACTER_ADD_HANDLE;
        m_downloadAdaptor->onPostContact(sendContacter);
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << getLastErrorString();
    }

    return lastErrorIsNoError();
}

bool ContacterInterface::onRemoveContacter(const QString &id)
{
    Contacter removedContacter;
    removeContacter(id, removedContacter);

    if(lastErrorIsNoError())
    {
        m_contacterModel.submitAll();
        emitChangeSignal(CONTACTER_DELETE_HANDLE, removedContacter);

        ContacterList contacters;
        contacters.append(removedContacter);
        emit signalContacterChangedToChangeRemoteContacter(CONTACTER_DELETE_HANDLE, contacters);

//        Contacter sendContacter = removedContacter;
//        sendContacter.handle = CONTACTER_DELETE_HANDLE;
//        IdIpMap m;
//        m.contacter = sendContacter;
//        m.sendToContacterList = onGetLowLevelContacters(sendContacter.id);
//        contacterChangeToChangeLowLevelPeopleAddQueue(m);
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << getLastErrorString();
    }
    return lastErrorIsNoError();
}

bool ContacterInterface::onUpdateContacter(const Contacter &contacter)
{
    Contacter updatedContacter;
    updateContacter(contacter, updatedContacter);
    if(lastErrorIsNoError())
    {
        m_contacterModel.submitAll();
        emitChangeSignal(CONTACTER_UPDATE_HANDLE, contacter);

        ContacterList contacters;
        contacters.append(contacter);
        emit signalContacterChangedToChangeRemoteContacter(CONTACTER_UPDATE_HANDLE, contacters);


        //post contacter to server
        Contacter sendContacter = updatedContacter;
        sendContacter.handle = CONTACTER_UPDATE_HANDLE;
        m_downloadAdaptor->onPostContact(sendContacter);
//        IdIpMap m;
//        m.contacter = sendContacter;
//        m.sendToContacterList = onGetLowLevelContacters(sendContacter.id);
//        contacterChangeToChangeLowLevelPeopleAddQueue(m);
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << getLastErrorString();
    }
    return lastErrorIsNoError();
}

QDBusVariant ContacterInterface::onGetContacter(const QString &id, const bool &containsPhotoData)
{
//    qDebug() << "onGetContacter id" << id;
    Contacter contacter;
    if(id.isEmpty())
    {
        QDBusVariant dbusValue;
        QVariant contacterVar = QVariant::fromValue(contacter);
        dbusValue.setVariant(contacterVar);
        return dbusValue;
    }

    contacter = getContacter(id, containsPhotoData);
    QDBusVariant dbusValue;
    QVariant contacterVar = QVariant::fromValue(contacter);
    dbusValue.setVariant(contacterVar);
    return dbusValue;
}

Contacter ContacterInterface::onGetContacter2(const QString &id, const bool &containsPhotoData)
{
//    qDebug() << "onGetContacter id" << id;
    Contacter contacter;
    if(id.isEmpty())
    {
        return contacter;
    }

    contacter = getLocalInfo();
    if(contacter.id != id)
    {
        contacter = getContacter(id, containsPhotoData);
    }

    return contacter;
}

QStringList ContacterInterface::onGetContacters(const int &contacterType)
{
//    QStringList ids;
//    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
//    {
//        QModelIndex cType = m_contacterModel.index(i, 10);
//        if(contacterType == CONTACTERTYPE_UNDEFINE)
//        {
//            QModelIndex idIndex = m_contacterModel.index(i, 0);
//            QString id = m_contacterModel.data(idIndex).toString();
//            ids.append(id);
//        }
//        else if(cType.data().toInt() == contacterType)
//        {
//            QModelIndex idIndex = m_contacterModel.index(i, 0);
//            QString id = m_contacterModel.data(idIndex).toString();
//            ids.append(id);
//        }
//    }
//    return ids;
    return onSearchContactors(contacterType, "");
}

QStringList ContacterInterface::onGetContactersExcept(const int &contacterType,
                                                      const int &netType,
                                                      const QStringList &exceptContacters)
{
    QString sql;
    if(contacterType == 3)
    {
        sql = QString(
                    "SELECT id,namePinyin,type FROM "
                    "(SELECT id,namePinyin,type,NETWORKEXISTANDONLINE(network,'%1') AS online FROM __tablename__) "
                    "WHERE online=0 "
                    "ORDER BY namePinyin ").arg(netType);
    }
    else
    {
        sql = QString(
                    "SELECT id,namePinyin,type FROM "
                    "(SELECT id,namePinyin,type,NETWORKEXISTANDONLINE(network,'%1') AS online FROM __tablename__) "
                    "WHERE type=%2 AND online=0 "
                    "ORDER BY namePinyin ").arg(netType).arg(contacterType);
    }

    QStringList result = m_contacterModel.getColumnDataWithSql(sql);
    foreach(const QString &contacter, exceptContacters)
    {
        result.removeOne(contacter);
    }
    return result;
}

QStringList ContacterInterface::onGetContactersExcept(const int &contacterType, const QStringList &exceptContacters)
{
    QStringList contacters = onGetContacters(contacterType);
    foreach(const QString &contacter, exceptContacters)
    {
        contacters.removeOne(contacter);
    }
    return contacters;
}

int ContacterInterface::onGetContactersCount(const int &contacterType)
{
//    if(contacterType != CONTACTERTYPE_UNDEFINE)
//    {
//        QString filter = QString("type='%1'").arg(QString::number(contacterType));
//        m_contacterModel.setFilter(filter);
//        int count = m_contacterModel.rowCount();
//        m_contacterModel.setFilter("");
//        return count;
//    }
    return m_contacterModel.rowCount();
}



bool ContacterInterface::onUpdateLocalInfo(const Contacter &contacter)
{
    if(contacter.id.isEmpty())
        return false;
    if(contacter.surname.isEmpty())
        return false;
    if(contacter.name.isEmpty())
        return false;
    int row = 0;
    QModelIndex idIndex = m_contacterModel.index(row, 0);
    m_contacterModel.setData(idIndex, contacter.id);

    QModelIndex surnameIndex = m_contacterModel.index(row, 1);
    m_contacterModel.setData(surnameIndex, contacter.surname);

    QModelIndex nameIndex = m_contacterModel.index(row, 2);
    m_contacterModel.setData(nameIndex, contacter.name);

    QModelIndex titleIndex = m_contacterModel.index(row, 3);
    m_contacterModel.setData(titleIndex, contacter.title);

    QModelIndex numbersIndex = m_contacterModel.index(row, 4);
    QString numbers = converNumbersToString(contacter.numbers);
    m_contacterModel.setData(numbersIndex, numbers);


    //cp the image to /home/user/PhonebookPortrait
    QString address = contacter.photoPath;
    address.remove("file://");
    QString newAddress = "/home/user/PhonebookPortrait/" + contacter.photoPath.section("/", -1);

    qDebug() << "copy portrait from to : " << address << newAddress << QFile::copy(address, newAddress);
    QModelIndex photoIndex = m_contacterModel.index(row, 5);
    m_contacterModel.setData(photoIndex, "file://"+newAddress);

    QModelIndex leadersIndex = m_contacterModel.index(row, 6);
    QString leaders = convertLeadersToString(contacter.leaders);
    m_contacterModel.setData(leadersIndex, leaders);

    QModelIndex snNumberIndex = m_contacterModel.index(row, 7);
    m_contacterModel.setData(snNumberIndex, contacter.snNumber);

    QModelIndex bluetoothIndex = m_contacterModel.index(row, 8);
    m_contacterModel.setData(bluetoothIndex, contacter.bluetooth);

    QString namePinyin = ChineseToPinyin::getPinyinFirstLetter(contacter.surname.trimmed()+contacter.name.trimmed())
            + " " + ChineseToPinyin::getInstance()->getPinyin(contacter.surname.trimmed()+contacter.name.trimmed())
            + " " + contacter.surname.trimmed()+contacter.name.trimmed()
            + " " + getTitleStringFromEnum(contacter.title);
    QModelIndex namePinyinIndex = m_contacterModel.index(row, 9);
    m_contacterModel.setData(namePinyinIndex, namePinyin);

    QModelIndex typeIndex = m_contacterModel.index(row, 10);
    m_contacterModel.setData(typeIndex, contacter.type);

    QModelIndex selectedStatusIndex = m_contacterModel.index(row, 11);
    m_contacterModel.setData(selectedStatusIndex, contacter.selectedStatus);

    QModelIndex pocidIndex = m_contacterModel.index(row, 12);
    m_contacterModel.setData(pocidIndex, contacter.pocId);

    return true;
}

QStringList ContacterInterface::onSearchContactors(const int &contacterType, const QString& searchText)
{
    QTime time;
    time.start();
    QStringList result;
    QString sql;
    if(contacterType == 3)
    {
        if(!searchText.isEmpty())
        {
            sql = QString("SELECT id FROM "
                          "(SELECT id,type,INDEXOF(namePinyin, '%1') AS idx,NETWORKNUMBER(network) AS nm  FROM __tablename__) "
                          "WHERE "
                          "(idx>=0 AND idx<10000) OR nm LIKE '%%1%' OR "
                          "(id LIKE '%%1%' AND type=1) "
                          "ORDER BY idx"
                          ).arg(searchText);
        }
        else
        {
            sql = QString("SELECT id FROM __tablename__ "
                          "ORDER BY namePinyin"
                          );
        }
    }
    else
    {
        if(!searchText.isEmpty())
        {
            sql = QString("SELECT id FROM "
                          "(SELECT id,type,INDEXOF(namePinyin, '%2') AS idx,NETWORKNUMBER(network) AS nm  FROM __tablename__) "
                          "WHERE "
                          "type=%1 AND "
                          "( "
                          "(idx>=0 AND idx<10000) OR nm LIKE '%%2%' "
                          ") "
                          "ORDER BY idx"
                          ).arg(QString::number(contacterType)).arg(searchText);
        }
        else
        {
            sql = QString("SELECT id FROM __tablename__ WHERE "
                          "type=%1 "
                          "ORDER BY namePinyin"
                          ).arg(QString::number(contacterType));
        }
    }
    result = m_contacterModel.getColumnDataWithSql(sql);
//    qDebug() << "ContacterInterface::onSearchContactors(const int &contacterType, const QString& searchText)";
//    qDebug() << "result: " << result;
    return result;
}

QStringList ContacterInterface::onSearchContactors(const int &contacterType,
                                                   const int &netType,
                                                   const QString& searchText)
{
    QTime time;
    time.start();
    QStringList result;
    QString sql;
    if(contacterType == 3)
    {
        if(!searchText.isEmpty())
        {
            sql = QString("SELECT id FROM "
                          "(SELECT id,type,INDEXOF(namePinyin, '%1') AS idx,NETWORKNUMBER(network) AS nm,NETWORKEXISTANDONLINE(network,'%2') AS online  FROM __tablename__) "
                          "WHERE "
                          "online=0 AND "
                          "((idx>=0 AND idx<10000) OR nm LIKE '%%1%' OR "
                          "(id LIKE '%%1%' AND type=1)) "
                          "ORDER BY idx"
                          ).arg(searchText).arg(netType);
        }
        else
        {
            if(netType == -1)
            {
                sql = QString("SELECT id FROM __tablename__ "
                              "ORDER BY namePinyin"
                              );
            }
            else
            {
                sql = QString(
                            "SELECT id,namePinyin,type FROM "
                            "(SELECT id,namePinyin,type,NETWORKEXISTANDONLINE(network,'%1') AS online FROM __tablename__) "
                            "WHERE online=0 "
                            "ORDER BY namePinyin ").arg(netType);
            }
        }
    }
    else
    {
        if(!searchText.isEmpty())
        {
            //"(id LIKE '%%2%' AND type=1)) "
            sql = QString("SELECT id FROM "
                          "(SELECT id,type,INDEXOF(namePinyin, '%2') AS idx,NETWORKNUMBER(network) AS nm,NETWORKEXISTANDONLINE(network,'%3') AS online  FROM __tablename__) "
                          "WHERE "
                          "online=0 AND "
                          "(type=%1 AND "
                          "( "
                          "(idx>=0 AND idx<10000) OR nm LIKE '%%2%' "
                          ") "
                          ") "
                          "ORDER BY idx"
                          ).arg(QString::number(contacterType)).arg(searchText).arg(netType);
        }
        else
        {

            if(netType == -1)
            {
                sql = QString("SELECT id FROM __tablename__ WHERE "
                              "type=%1 "
                              "ORDER BY namePinyin"
                              ).arg(QString::number(contacterType));
            }
            else
            {
                sql = QString(
                            "SELECT id,namePinyin,type FROM "
                            "(SELECT id,namePinyin,type,NETWORKEXISTANDONLINE(network,'%2') AS online FROM __tablename__) "
                            "WHERE online=0 AND type=%1 "
                            "ORDER BY namePinyin ").arg(QString::number(contacterType)).arg(netType);
            }
        }
    }
    result = m_contacterModel.getColumnDataWithSql(sql);
    qDebug() << "ContacterInterface::onSearchContactors(const int &contacterType, const int &netType, const QString& searchText)";
    qDebug() << "result: " << result;
    return result;
}

ContacterList ContacterInterface::onSearchContactors2(const int &contacterType, const QString& searchText)
{
    QTime time;
    time.start();

    ContacterList contacterList;
    QStringList idList = onSearchContactors(contacterType, searchText);
    foreach(const QString& id, idList)
    {
        Contacter contacter = getContacter(id);
        contacterList.append(contacter);
    }

    qDebug() << "onSearchContactors2 time: " << time.elapsed();
    return contacterList;
}

QStringList ContacterInterface::onSearchContactorsExcept(const int &contacterType,
                                                         const QString& searchText,
                                                         const QStringList &exceptContacters)
{
    QStringList contacters = onSearchContactors(contacterType, searchText);
    foreach(const QString &contacter, exceptContacters)
    {
        contacters.removeOne(contacter);
    }
    return contacters;
}

QStringList ContacterInterface::onSearchContactorsExcept(const int &contacterType,
                                                         const int &netType,
                                                         const QString& searchText,
                                                         const QStringList &exceptContacters)
{
    QStringList contacters = onSearchContactors(contacterType, netType, searchText);
    foreach(const QString &contacter, exceptContacters)
    {
        contacters.removeOne(contacter);
    }
    return contacters;
}

QString ContacterInterface::onSearchContacterNumber(const int &contacterType, const QString& searchText)
{
    QString sql;
    if(contacterType == 3)
    {
        sql = QString("SELECT id FROM __tablename__ WHERE "
                              "network LIKE '%%1%'"
                              ).arg(searchText);
    }
    else
    {
        sql = QString("SELECT id FROM __tablename__ WHERE "
                              "network LIKE '%%1%' AND type=%2"
                              ).arg(searchText).arg(QString::number(contacterType));
    }

    QStringList result = m_contacterModel.getColumnDataWithSql(sql);
    if(result.count() > 1){
        qDebug() << "ContacterInterface error: query contacter by number more than one" << result;
        return result.value(0);
    }
    else if(!result.isEmpty())
        return result.value(0);
    else
        return QString();
}

QString ContacterInterface::onSearchContacterBluetooth(const QString& bluetoothAddress)
{
    QString sql = QString("SELECT id FROM __tablename__ WHERE "
                              "bluetooth='%1'"
                              ).arg(bluetoothAddress);

    QStringList result = m_contacterModel.getColumnDataWithSql(sql);
    if(result.count() > 1){
        qDebug() << "ContacterInterface bluetoothAddress error : query contacter by bluetoothAddress more than one" << result;
        return result.value(0);
    }
    else if(!result.isEmpty())
        return result.value(0);
    else
        return QString();
}

void ContacterInterface::onSetAllSelectedStatus(const int &status)
{
    SqlFunction::doSql(QString("UPDATE contacter SET selectedStatus='%1'").arg(status));
    m_contacterModel.select();
}

QDBusVariant ContacterInterface::onGetLocalInfo()
{
    Contacter local = getLocalInfo();
    QDBusVariant dbusValue;
    QVariant localVar = QVariant::fromValue(local);
    dbusValue.setVariant(localVar);
    return dbusValue;
}

bool ContacterInterface::checkUserExist(const QString &userId)
{
    int row = m_contacterModel.findDataInColumn(0, userId);
    return (row != -1);
//    for(int i = 0, iend = m_contacterModel.rowCount(); i < iend; i++)
//    {
//        QModelIndex index = m_contacterModel.index(i, 0);
//        if(m_contacterModel.data(index).toString() == userId)
//            return true;
//    }
//    return false;
}

Contacter ContacterInterface::getContacterFromRowData(const QStringList &rowData, const bool &containsPhotoData)
{
    if(rowData.isEmpty())
        return Contacter();

    Contacter contacter;
    contacter.id = rowData.value(0);

    contacter.surname = rowData.value(1);

    contacter.name = rowData.value(2);

    contacter.title = rowData.value(3).toInt();

    QString numbersValue = rowData.value(4);
    contacter.numbers = converNumbersToStruct(numbersValue);

    contacter.photoPath = rowData.value(5);
    if(containsPhotoData)
    {
        QString photoPath = contacter.photoPath;
        contacter.photoData = loadFile(photoPath.remove("file://"));
        qDebug() << "load photo name is: " << photoPath << contacter.photoData.isEmpty();
    }

    QString leadersValue = rowData.value(6);
    contacter.leaders = convertStringToLeaders(leadersValue);

    contacter.snNumber = rowData.value(7);

    contacter.bluetooth = rowData.value(8);

    contacter.namePinyin = rowData.value(9).left(1);//left(1) get the first letter like abc

    contacter.type = rowData.value(10).toInt(); //ContacterType

    contacter.selectedStatus = rowData.value(11).toInt();//ui selected status

    contacter.pocId = rowData.value(12);//poc id for lte

    return contacter;
}

Contacter ContacterInterface::getContacter(const int &row)
{
    return getContacterFromRowData(m_contacterModel.rowData(row));
}

Contacter ContacterInterface::getContacter(const QString& id, const bool &containsPhotoData)
{
    return getContacterFromRowData(m_contacterModel.findDataInColumn("id", id), containsPhotoData);
}

Contacter ContacterInterface::getLocalInfo()
{
    m_localModel.select();
    int rowCount = m_localModel.rowCount();
    if(rowCount < 0)
        return Contacter();

    QStringList rowData = m_localModel.rowData(0);

    Contacter contacter;
    contacter.id = rowData.value(0);

    contacter.surname = rowData.value(1);

    contacter.name = rowData.value(2);

    contacter.title = rowData.value(3).toInt();

    QString numbersValue = rowData.value(4);
    contacter.numbers = converNumbersToStruct(numbersValue);

    contacter.photoPath = rowData.value(5);
//    contacter.photoData = rowData.value(6);

    QString leadersValue = rowData.value(6);
    contacter.leaders = convertStringToLeaders(leadersValue);

    contacter.snNumber = rowData.value(7);

    contacter.bluetooth = rowData.value(8);

    contacter.namePinyin = rowData.value(9).left(1);//left(1) get the first letter like abc

    contacter.type = rowData.value(10).toInt(); //ContacterType

    contacter.selectedStatus = rowData.value(11).toInt();//ui selected status

    contacter.pocId = rowData.value(12);//poc id for lte

//    Contacter local;
//    QModelIndex idIndex = m_localModel.index(0, 0);
//    local.id = m_localModel.data(idIndex).toString();

//    QModelIndex surnameIndex = m_localModel.index(0, 1);
//    local.surname = m_localModel.data(surnameIndex).toString();

//    QModelIndex nameIndex = m_localModel.index(0, 2);
//    local.name = m_localModel.data(nameIndex).toString();

//    QModelIndex titleIndex = m_localModel.index(0, 3);
//    local.title = m_localModel.data(titleIndex).toInt();

//    QModelIndex numbersIndex = m_localModel.index(0, 4);
//    QString numbersValue = m_localModel.data(numbersIndex).toString();
//    local.numbers = converNumbersToStruct(numbersValue);

//    QModelIndex photoIndex = m_localModel.index(0, 5);
//    local.photoPath = m_localModel.data(photoIndex).toString();

//    QModelIndex leadersIndex = m_localModel.index(0, 6);
//    QString leadersValue = m_localModel.data(leadersIndex).toString();
//    local.leaders = convertStringToLeaders(leadersValue);

//    QModelIndex snNumberIndex = m_localModel.index(0, 7);
//    local.snNumber = m_localModel.data(snNumberIndex).toString();

//    QModelIndex bluetoothIndex = m_localModel.index(0, 8);
//    local.bluetooth = m_localModel.data(bluetoothIndex).toString();

//    QModelIndex typeIndex = m_localModel.index(0, 9);
//    local.type = m_localModel.data(typeIndex).toInt();
    return contacter;
}

void ContacterInterface::setLocalInfo(Contacter data)
{
    m_localModel.select();
    int rowCount = m_localModel.rowCount();
    if(rowCount <= 0)
        return;

    QModelIndex idIndex = m_localModel.index(0, 0);
    m_localModel.setData(idIndex, data.id);

    QModelIndex surnameIndex = m_localModel.index(0, 1);
    m_localModel.setData(surnameIndex, data.surname);

    QModelIndex nameIndex = m_localModel.index(0, 2);
    m_localModel.setData(nameIndex, data.name);

    QModelIndex titleIndex = m_localModel.index(0, 3);
    m_localModel.setData(titleIndex, data.title);

    QModelIndex numbersIndex = m_localModel.index(0, 4);
    QString numbers = converNumbersToString(data.numbers);
    m_localModel.setData(numbersIndex, numbers);

    //cp the image to /home/user/PhonebookPortrait
    QString address = data.photoPath;
    address.remove("file://");
    QString newAddress = "/home/user/PhonebookPortrait/" + data.photoPath.section("/", -1);

    qDebug() << "copy portrait from to : " << address << newAddress << QFile::copy(address, newAddress);
    QModelIndex photoIndex = m_localModel.index(0, 5);
    m_localModel.setData(photoIndex, "file://" + newAddress);
//    if(!contacter.photoPath.contains("home/user"))
//    {
//        QString address = contacter.photoPath;
//        QString newAddress = "/home/user/PhonebookPortrait/" + contacter.photoPath.section("/", -1);
//        qDebug() << "copy portrait: " << newAddress << QFile::copy(address.remove("file://"), newAddress);
//        QModelIndex photoIndex = m_contacterModel.index(row, 5);
//        m_contacterModel.setData(photoIndex, "file://"+newAddress);
//    }
//    else
//    {
//        QModelIndex photoIndex = m_contacterModel.index(row, 5);
//        m_contacterModel.setData(photoIndex, contacter.photoPath);
//    }

    QModelIndex leadersIndex = m_localModel.index(0, 6);
    QString leaders = convertLeadersToString(data.leaders);
    m_localModel.setData(leadersIndex, leaders);

    QModelIndex snNumberIndex = m_localModel.index(0, 7);
    m_localModel.setData(snNumberIndex, data.snNumber);

    QModelIndex bluetoothIndex = m_localModel.index(0, 8);
    m_localModel.setData(bluetoothIndex, data.bluetooth);

    QString namePinyin = ChineseToPinyin::getPinyinFirstLetter(data.surname.trimmed()+data.name.trimmed())
            + " " + ChineseToPinyin::getInstance()->getPinyin(data.surname.trimmed()+data.name.trimmed())
            + " " + data.surname.trimmed()+data.name.trimmed()
            + " " + getTitleStringFromEnum(data.title);
    QModelIndex namePinyinIndex = m_localModel.index(0, 9);
    m_localModel.setData(namePinyinIndex, namePinyin);

    QModelIndex typeIndex = m_localModel.index(0, 10);
    m_localModel.setData(typeIndex, data.type);

    QModelIndex selectedStatusIndex = m_localModel.index(0, 11);
    m_localModel.setData(selectedStatusIndex, data.selectedStatus);

    QModelIndex pocidIndex = m_localModel.index(0, 12);
    m_localModel.setData(pocidIndex, data.pocId);
    m_localModel.submitAll();
}

bool ContacterInterface::setContacter(const int &row, const Contacter &contacter)
{
    QModelIndex idIndex = m_contacterModel.index(row, 0);
    m_contacterModel.setData(idIndex, contacter.id);

    QModelIndex surnameIndex = m_contacterModel.index(row, 1);
    m_contacterModel.setData(surnameIndex, contacter.surname);

    QModelIndex nameIndex = m_contacterModel.index(row, 2);
    m_contacterModel.setData(nameIndex, contacter.name);

    QModelIndex titleIndex = m_contacterModel.index(row, 3);
    m_contacterModel.setData(titleIndex, contacter.title);

    QModelIndex numbersIndex = m_contacterModel.index(row, 4);
    QString numbers = converNumbersToString(contacter.numbers);
    m_contacterModel.setData(numbersIndex, numbers);


    //cp the image to /home/user/PhonebookPortrait
    QString address = contacter.photoPath;
    address.remove("file://");
    QString newAddress = "/home/user/PhonebookPortrait/" + contacter.photoPath.section("/", -1);

    qDebug() << "copy portrait from to : " << address << newAddress << QFile::copy(address, newAddress);
    QModelIndex photoIndex = m_contacterModel.index(row, 5);
    m_contacterModel.setData(photoIndex, "file://"+newAddress);
//    if(!contacter.photoPath.contains("home/user"))
//    {
//        QString address = contacter.photoPath;
//        QString newAddress = "/home/user/PhonebookPortrait/" + contacter.photoPath.section("/", -1);
//        qDebug() << "copy portrait: " << newAddress << QFile::copy(address.remove("file://"), newAddress);
//        QModelIndex photoIndex = m_contacterModel.index(row, 5);
//        m_contacterModel.setData(photoIndex, "file://"+newAddress);
//    }
//    else
//    {
//        QModelIndex photoIndex = m_contacterModel.index(row, 5);
//        m_contacterModel.setData(photoIndex, contacter.photoPath);
//    }

    QModelIndex leadersIndex = m_contacterModel.index(row, 6);
    QString leaders = convertLeadersToString(contacter.leaders);
    m_contacterModel.setData(leadersIndex, leaders);

    QModelIndex snNumberIndex = m_contacterModel.index(row, 7);
    m_contacterModel.setData(snNumberIndex, contacter.snNumber);

    QModelIndex bluetoothIndex = m_contacterModel.index(row, 8);
    m_contacterModel.setData(bluetoothIndex, contacter.bluetooth);

    QString namePinyin = ChineseToPinyin::getPinyinFirstLetter(contacter.surname.trimmed()+contacter.name.trimmed())
            + " " + ChineseToPinyin::getInstance()->getPinyin(contacter.surname.trimmed()+contacter.name.trimmed())
            + " " + contacter.surname.trimmed()+contacter.name.trimmed()
            + " " + getTitleStringFromEnum(contacter.title);
    QModelIndex namePinyinIndex = m_contacterModel.index(row, 9);
    m_contacterModel.setData(namePinyinIndex, namePinyin);

    QModelIndex typeIndex = m_contacterModel.index(row, 10);
    m_contacterModel.setData(typeIndex, contacter.type);

    QModelIndex selectedStatusIndex = m_contacterModel.index(row, 11);
    m_contacterModel.setData(selectedStatusIndex, contacter.selectedStatus);

    QModelIndex pocidIndex = m_contacterModel.index(row, 12);
    m_contacterModel.setData(pocidIndex, contacter.pocId);

    return true;
}

QString ContacterInterface::converNumbersToString(const QList<PhoneNumber> &numbers)
{
    return convertPhoneNumbersToString(numbers);
//    QJsonObject object;
//    for(int i = 0, iend = numbers.count(); i < iend; i++)
//    {
//        QString type = converNetworkTypeToString((NetworkType)numbers[i].netType);
//        QString address = numbers[i].number;
//        int online = numbers[i].online;
//        QJsonArray array;
//        array.append(QJsonValue(address));
//        array.append(QJsonValue(online));
//        object.insert(type, array);
//    }

//    QJsonDocument document(object);
//    QByteArray array = document.toJson();
//    QString value = QString::fromUtf8(array);

//    return value;
}

QList<PhoneNumber> ContacterInterface::converNumbersToStruct(const QString &value)
{
    return convertStringToPhoneNumbers(value);
//    QList<PhoneNumber> numbers;
//    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
//    QJsonObject object = document.object();
//    QJsonObject::iterator start = object.begin();
//    QJsonObject::iterator end = object.end();
//    for(; start != end; start++)
//    {
//        PhoneNumber number;
//        number.netType = converStringToNetworkType(start.key());

//        QJsonArray array = start.value().toArray();
//        if(array.size() >= 2)
//        {
//            number.number = array.at(0).toString();
//            number.online = array.at(1).toInt();
//        }

//        numbers.append(number);
//    }

//    return numbers;
}

QString ContacterInterface::converNetworkTypeToString(const NetworkType &type)
{
    return convertNetworkTypeToString(type);
//    QString value;
//    switch(type)
//    {
//    case AD_HOC_NETWORK:
//        value = "ad_hoc_network";
//        break;
//    case LTE_NETWORK:
//        value = "lte_network";
//        break;
//    case SATELLITE_NETWORK:
//        value = "satellite_network";
//        break;
//    case DIGITAL_WALKIE_TALKIE_NETWORK:
//        value = "digital_walkie_talkie_network";
//        break;
//    case BATTLE_NETWORK:
//        value = "battle_network";
//        break;
//    case ADHOC_LTE_NETWORK:
//        value = "adhoc_lte_network";
//        break;
//    case UNKNOW_NETWORK:
//        value = "unknow_network";
//        break;
//    default:
//        value = "unknow_network";
//    }

//    return value;
}

NetworkType ContacterInterface::converStringToNetworkType(const QString &value)
{
    return convertStringToNetworkType(value);
//    if(value == "ad_hoc_network")
//        return AD_HOC_NETWORK;
//    else if(value == "lte_network")
//        return LTE_NETWORK;
//    else if(value == "satellite_network")
//        return SATELLITE_NETWORK;
//    else if(value == "digital_walkie_talkie_network")
//        return DIGITAL_WALKIE_TALKIE_NETWORK;
//    else if(value == "battle_network")
//        return BATTLE_NETWORK;
//    else if(value == "adhoc_lte_network")
//        return ADHOC_LTE_NETWORK;
//    else if(value == "unknow_network")
//        return UNKNOW_NETWORK;
//    else
//        return UNKNOW_NETWORK;

//    return UNKNOW_NETWORK;
}

QString ContacterInterface::convertLeadersToString(const QStringList &leaders)
{
    QJsonArray leaderArray;
    for(int i = 0, iend = leaders.count(); i < iend; i++)
    {
        QJsonValue leaderValue(leaders[i]);
        leaderArray.append(leaderValue);
    }

    QJsonDocument document(leaderArray);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

QStringList ContacterInterface::convertStringToLeaders(const QString &value)
{
    QStringList leaders;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonArray array = document.array();
    for(int i = 0, iend = array.count(); i < iend; i++)
    {
        QJsonValue value = array.at(i);
        QString leaderId = value.toString();
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
    emit signalContacterChangedToChangeHistory(type, changeContacter);
}

bool ContacterInterface::removeOrganizationContacters()
{
    SqlFunction::doSql("DELETE FROM contacter WHERE type=1");
    m_contacterModel.select();

//    QStringList idList = m_contacterModel.getColumnDataWithSql("SELECT id FROM __tablename__ WHERE type=1");
//    foreach(const QString &id, idList)
//    {
//        int row = m_contacterModel.findDataInColumn(0, id);
//        bool flag = m_contacterModel.removeRow(row);
//        qDebug() << "remove id: " << id
//                 << "row: " << row
//                 << "remove flag: " << flag;
//    }
//    m_contacterModel.submitAll();
//    qDebug() << m_contacterModel.rowCount();
}


void ContacterInterface::onContacterReceived(const int &handleType, const ContacterList &contacters)
{
    switch (handleType) {
    case CONTACTER_ADD_HANDLE:
    {
        foreach(const Contacter contacter, contacters)
        {
            Contacter addedContacter;
            addContacter(contacter, addedContacter);

            if(lastErrorIsNoError())
            {
                m_contacterModel.submitAll();
                emitChangeSignal(CONTACTER_ADD_HANDLE, contacter);
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << getLastErrorString();
            }
        }
        break;
    }
    case CONTACTER_UPDATE_HANDLE:
    {
        foreach(const Contacter contacter, contacters)
        {
            Contacter updatedContacter;
            updateContacter(contacter, updatedContacter);
            if(lastErrorIsNoError())
            {
                m_contacterModel.submitAll();
                emitChangeSignal(CONTACTER_UPDATE_HANDLE, contacter);
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << getLastErrorString();
            }
        }

        break;
    }
    case CONTACTER_DELETE_HANDLE:
    {
        foreach(const Contacter contacter, contacters)
        {
            Contacter removedContacter;
            removeContacter(contacter.id, removedContacter);
            if(lastErrorIsNoError())
            {
                m_contacterModel.submitAll();
                emitChangeSignal(CONTACTER_DELETE_HANDLE, removedContacter);
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << getLastErrorString();
            }

        }

        break;
    }
    case CONTACTER_INIT_HANDLE:
    {
        m_contacterModel.clear();
        foreach(const Contacter contacter, contacters)
        {
            Contacter addedContacter;
            addContacter(contacter, addedContacter);
        }

        if(lastErrorIsNoError())
        {
            m_contacterModel.submitAll();
            emitChangeSignal(CONTACTER_INIT_HANDLE, Contacter());
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << getLastErrorString();
        }
        break;
    }
    default:
        break;
    }
}

void ContacterInterface::onContacterOneReceived(const int &handleType, const Contacter &ct)
{
    qDebug() << "ContacterInterface::onContacterOneReceived" << handleType << ct.photoPath << ct.photoData.isEmpty();
    Contacter contacter = ct;
    switch (handleType)
    {
    case CONTACTER_ADD_HANDLE:
    {
        Contacter addedContacter;
        QString savedPath = contacter.photoPath;

        savedPath = "/home/user/PhonebookPortrait/" + contacter.photoPath.section("/", -1);
        contacter.photoPath = "file://"+savedPath;

        qDebug() << "savedPath" << savedPath;
        qDebug() << "contacter.photoPath" << contacter.photoPath;
        qDebug() << "contacter.photoData.isEmpty: " << contacter.photoData.isEmpty();
        if(!contacter.photoData.isEmpty())
        {
            bool saveFlag = saveFile(contacter.photoData, savedPath);
            qDebug() << "contacter.photoData saveFile: " << saveFlag;
        }

        if(!checkUserExist(contacter.id))
        {
            addContacter(contacter, addedContacter);
            if(lastErrorIsNoError())
            {
                m_contacterModel.submitAll();
                emitChangeSignal(CONTACTER_ADD_HANDLE, addedContacter);
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << getLastErrorString();
            }
        }
        else
        {
            updateContacter(contacter, addedContacter);
            if(lastErrorIsNoError())
            {
                m_contacterModel.submitAll();
                emitChangeSignal(CONTACTER_UPDATE_HANDLE, contacter);
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << getLastErrorString();
            }
        }
        break;
    }
    case CONTACTER_UPDATE_HANDLE:
    {
        Contacter updatedContacter;
        updateContacter(contacter, updatedContacter);
        if(lastErrorIsNoError())
        {
            m_contacterModel.submitAll();
            emitChangeSignal(CONTACTER_UPDATE_HANDLE, contacter);
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << getLastErrorString();
        }

        break;
    }
    case CONTACTER_DELETE_HANDLE:
    {
        Contacter removedContacter;
        removeContacter(contacter.id, removedContacter);
        if(lastErrorIsNoError())
        {
            m_contacterModel.submitAll();
            emitChangeSignal(CONTACTER_DELETE_HANDLE, removedContacter);
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << getLastErrorString();
        }

        break;
    }
    case CONTACTER_INIT_HANDLE:
    {
        break;
    }
    default:
        break;
    }

    CNotificationManager notiMgr;
    CNotification genoti;
    genoti.setTitle("共享联系人");
    genoti.setSubtitle("收到1个共享联系人");
    genoti.setAttributes(CAbstractNotification::HideDisplayTime);
    genoti.setVibrationMode(CAbstractNotification::ForceVibrationMode);
    genoti.setMarqueeText("收到1个共享联系人");

    QString result;
    result = notiMgr.sendNotification(genoti);
    qDebug() <<"notiMgr.sendNotification -----------------" <<result;
}

void ContacterInterface::onDbChanged()
{
    m_contacterModel.select();
    m_localModel.select();
    emitChangeSignal(CONTACTER_INIT_HANDLE, Contacter());
}

void ContacterInterface::onContacterDownload(const ContacterList &contacters)
{
    qDebug() << "onContacterDownload";
    return;
    if(!contacters.isEmpty())
    {
        //remove all the organization contacts
        removeOrganizationContacters();
    }

    int index = 2000;
    foreach(Contacter contacter, contacters)
    {
        Contacter addedContacter;
        contacter.type = 1;
        if(contacter.id.isEmpty())
            contacter.id = QString::number(index++);
        qDebug() << "download contacter: "
                 << contacter.id
                 << contacter.surname+contacter.name
                 << contacter.title;
        if(checkUserExist(contacter.id))
        {
            Contacter updatedContacter;
            updateContacter(contacter, updatedContacter);
//            if(lastErrorIsNoError())
//            {
//                m_contacterModel.submitAll();
//                emitChangeSignal(CONTACTER_UPDATE_HANDLE, contacter);
//            }
//            else
//            {
//                qDebug() << __FILE__ << __LINE__ << getLastErrorString();
//            }
        }
        addOrganizationContacter(contacter, addedContacter);
    }
    m_contacterModel.submitAll();
    emitChangeSignal(CONTACTER_ADD_HANDLE, Contacter());
}

void ContacterInterface::onContacterPostFinished(int status, const Contacter &contacter)
{
    if(!status)
    {
        //add to others from org group directly
        IdIpMap m;
        m.contacter = contacter;
        m.sendToContacterList = onGetLowLevelContacters(contacter.id);
        contacterChangeToChangeLowLevelPeopleAddQueue(m);
    }
}

bool ContacterInterface::onCanEditContacter(const QString &id)
{
    Contacter localContact = getLocalInfo();
//    GroupInterface *groupInterface = GroupInterface::getInstance();
//    QStringList groupIdList = groupInterface->onGetGroups(SUBORDINATE_GROUP);
//    GroupList groupList;
//    foreach(const QString &groupId, groupIdList)
//    {
//        Group group = groupInterface->getGroup(groupId);
//        groupList.append(group);
//    }

//    QStringList alreadyHasLeaders;
//    alreadyHasLeaders.push_back(localContact.id);
//    QStringList members;
//    QStringList lowLevelMembers;
//    lowLevelMembers.push_back(localContact.id);

//    while(1)
//    {
//        QStringList lowLevelMembersTmp;
//        foreach(const QString &member, lowLevelMembers)
//        {
//            members = getLowLevel(member, alreadyHasLeaders, groupList);
//            alreadyHasLeaders.append(members);
//            lowLevelMembersTmp.append(members);
//        }
//        if(lowLevelMembersTmp.isEmpty())
//            break;
//        else
//            lowLevelMembers = lowLevelMembersTmp;
//    }
    QStringList alreadyHasLeaders = onGetLowLevelContacters(localContact.id);

    if(alreadyHasLeaders.contains(id))
        return true;

    return false;
}

QStringList ContacterInterface::onGetLowLevelContacters(const QString &id)
{
    GroupInterface *groupInterface = GroupInterface::getInstance();
    QStringList groupIdList = groupInterface->onGetGroups(SUBORDINATE_GROUP);
    GroupList groupList;
    foreach(const QString &groupId, groupIdList)
    {
        Group group = groupInterface->getGroup(groupId);
        groupList.append(group);
    }

    QStringList alreadyHasLeaders;
    alreadyHasLeaders.push_back(id);
    QStringList members;
    QStringList lowLevelMembers;
    lowLevelMembers.push_back(id);

    while(1)
    {
        QStringList lowLevelMembersTmp;
        foreach(const QString &member, lowLevelMembers)
        {
            members = getLowLevel(member, alreadyHasLeaders, groupList);
            alreadyHasLeaders.append(members);
            lowLevelMembersTmp.append(members);
        }
        if(lowLevelMembersTmp.isEmpty())
            break;
        else
            lowLevelMembers = lowLevelMembersTmp;
    }
    return alreadyHasLeaders;
}

QStringList ContacterInterface::getLowLevel(const QString &id, const QStringList &alreadyHasLeaders, const GroupList &groupList)
{
    QStringList idList;

    foreach(const Group &group, groupList)
    {
        if(group.leaders.contains(id))
        {
            QStringList allMembers = group.members;
            allMembers.append(group.creator);
            allMembers.append(group.leaders);

            foreach(const QString &member, allMembers)
            {
                if(member != id && !alreadyHasLeaders.contains(id))
                    idList.push_back(member);
            }
        }
    }

    return idList;
}

QString ContacterInterface::getUseableContacterId()
{
    QString sql = QString("SELECT id FROM __tablename__");
    QStringList result = m_contacterModel.getColumnDataWithSql(sql);
    QSet<QString> idSet = result.toSet();
    QString index = "";
    for(int i=10000; i<30000; i++)
    {
        if(!idSet.contains(QString::number(i)))
        {
            index = QString::number(i);
            break;
        }
    }
    return index;
}

void ContacterInterface::contacterChangeToChangeLowLevelPeopleAddQueue(const IdIpMap &idIpMap)
{
    m_sendContacterToGroupContacters.push_back(idIpMap);
    m_timer.stop();
    m_timer.start();
}

void ContacterInterface::contacterChangeToChangeLowLevelPeople()
{
    if(!m_sendContacterToGroupContacters.isEmpty())
    {
        IdIpMap idIpMap = m_sendContacterToGroupContacters.first();

        //connect
        QStringList ipList;
        qDebug() << idIpMap.sendToContacterList;
        foreach(const QString &id, idIpMap.sendToContacterList)
        {
            Contacter contacter = ContacterInterface::getInstance()->onGetContacter2(id);

            foreach(const PhoneNumber &number, contacter.numbers)
            {
                if(number.netType == WIFI_NETWORK)
                {
                    qDebug() << "groupChangeToChangeRemoteGroup number WIFI_NETWORK" << id << contacter.id  << number.number;
                    ipList.push_back(number.number);
                }
            }
        }
    //    m_sendGroup = group;
        m_contacterPushAdaptor->onSetAddresses(ipList);
        m_contacterPushAdaptor->onConnect();
    }
}

void ContacterInterface::onSignalContacterPushConnected(const int &status)
{
    qDebug() << "onSignalGroupShareConnected status" << status;
    if(!m_sendContacterToGroupContacters.isEmpty())
    {
        IdIpMap idIpMap = m_sendContacterToGroupContacters.first();
        m_sendContacterToGroupContacters.pop_front();

        if(status > 0)
        {
            m_contacterPushAdaptor->onSendContacter(idIpMap.contacter);
        }
        else
        {
            m_timer.start();
        }
    }
    else
    {
        qDebug() << "group message is empty: not send group";
    }
}

void ContacterInterface::onSignalSendPercent(const QString &fileName, const qint64 &size, const qint64 &maxSize)
{
    qDebug() << "onSignalSendPercent file name" << fileName << size << maxSize;
    if(size == maxSize)
    {
        qDebug() << "m_contacterPushAdaptor onDisconnect";
        m_timer.start();
    }
}

void ContacterInterface::onContacterChangeToChangeLowLevelPeople(const Contacter &contacter)
{
    switch (contacter.handle) {
    case CONTACTER_ADD_HANDLE:
    {
        Contacter addedContacter;
        addContacter(contacter, addedContacter);

        if(lastErrorIsNoError())
        {
            m_contacterModel.submitAll();
            emitChangeSignal(CONTACTER_ADD_HANDLE, contacter);
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << getLastErrorString();
        }
        break;
    }
    case CONTACTER_UPDATE_HANDLE:
    {
        Contacter updatedContacter;
        updateContacter(contacter, updatedContacter);
        if(lastErrorIsNoError())
        {
            m_contacterModel.submitAll();
            emitChangeSignal(CONTACTER_UPDATE_HANDLE, contacter);
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << getLastErrorString();
        }

        break;
    }
    case CONTACTER_DELETE_HANDLE:
    {
        Contacter removedContacter;
        removeContacter(contacter.id, removedContacter);
        if(lastErrorIsNoError())
        {
            m_contacterModel.submitAll();
            emitChangeSignal(CONTACTER_DELETE_HANDLE, removedContacter);
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << getLastErrorString();
        }

        break;
    }
    case CONTACTER_INIT_HANDLE:
    {
//        m_contacterModel.clear();
//        foreach(const Contacter contacter, contacters)
//        {
//            Contacter addedContacter;
//            addContacter(contacter, addedContacter);
//        }

//        if(lastErrorIsNoError())
//        {
//            m_contacterModel.submitAll();
//            emitChangeSignal(CONTACTER_INIT_HANDLE, Contacter());
//        }
//        else
//        {
//            qDebug() << __FILE__ << __LINE__ << getLastErrorString();
//        }
        break;
    }
    default:
        break;
    }
}



