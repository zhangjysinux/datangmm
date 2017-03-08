#include <QSqlTableModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDBusMetaType>
#include <QDebug>
#include <QSqlQuery>
#include <QUuid>
#include "callhistoryinterface.h"
#include "../../commondata/bluetoothbackgroundtransferinterface.h"
#include "connectiondatabase.h"

QString CallHistoryInterface::transMessageDateToString(const QDateTime &date)
{
    QString value = date.toString("yyyyMMddhhmmss");
    return value;
}

QDateTime CallHistoryInterface::transStringToMessageDate(const QString &date)
{
    QDateTime value = QDateTime::fromString(date, "yyyyMMddhhmmss");
    return value;
}

CallHistoryInterface *CallHistoryInterface::m_instance = NULL;
CallHistoryInterface::CallHistoryInterface(QObject *parent) :
    QObject(parent), m_currentSyncCount(0),m_historyModel(0, ConnectionDatabase::getGblDb())
{
    qDBusRegisterMetaType<CallHistory>();
    qDBusRegisterMetaType<CallHistoryList>();

    //contacterInterface
    m_contacterInterface = ContacterInterface::getInstance();
    //init callHistory data
    m_historyModel.setTable("callhistory");
    m_historyModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_historyModel.select();

    CallHistory c;
    c.dialHistoryId = "365";

    qDebug() << "onSearchCallHistory:" << onSearchCallHistory("", 1);
}


CallHistoryInterface *CallHistoryInterface::getInstance()
{
    if(m_instance == NULL)
        m_instance = new CallHistoryInterface();

    return m_instance;
}

QStringList CallHistoryInterface::onGetCallHistoryList()
{
    qDebug()<<"onGetCallHistoryList in service";
    QStringList ids = onSearchCallHistory("");

//    ids = m_historyModel.getColumnDataWithSql("SELECT id FROM __tablename__ group by contactId1,contactId2,contactId3,contactId4,contactId5,contactId6,number1,number2,number3,number4,number5,number6 ORDER BY dateStart DESC");

//    bool ret;
//    QStringList mylist;
//    for(int i = 0, iend = m_historyModel.rowCount(); i < iend; i++)
//    {
//        ret = false;
//        QModelIndex index = m_historyModel.index(i, 1);
//        for (int j = 0, jend = mylist.count();j < jend; j++){
//            for (int m = 0;m < convertStringToTargets(m_historyModel.data(index).toString()).count();m++){
//                if (convertStringToTargets(m_historyModel.data(index).toString()).at(m) == mylist.at(j))
//                    ret = true;
//            }
//        }
//        if (!ret)
//        {
//            QModelIndex index1 = m_historyModel.index(i, 1);
//            mylist.append(convertStringToTargets(m_historyModel.data(index1).toString()));
//            QModelIndex index = m_historyModel.index(i, 0);
//            QString id = m_historyModel.data(index).toString();
//            ids.append(id);
//        }
//    }
    qDebug()<<"myids"<<ids;
    return ids;
}

QStringList CallHistoryInterface::onGetCallHistoryList(const QDateTime &startTime, const QDateTime &endTime)
{
    QString sql = QString("SELECT id FROM __tablename__ WHERE dateStart>=%1 and dateStart<=%2 "
                          "group by contactId1,contactId2,contactId3,contactId4,contactId5,contactId6, "
                          "number1,number2,number3,number4,number5,number6 "
                          "ORDER BY dateStart DESC"
                          )
            .arg(transMessageDateToString(startTime))
            .arg(transMessageDateToString(endTime));
    QStringList idList = m_historyModel.getColumnDataWithSql(sql);
    return idList;
}

QStringList CallHistoryInterface::onGetCallHistoryList(const int &startIndex, const int &endIndex)
{
    QString sql = QString("SELECT id,dateStart FROM __tablename__ "
                          "group by contactId1,contactId2,contactId3,contactId4,contactId5,contactId6, "
                          "number1,number2,number3,number4,number5,number6 "
                          "ORDER BY dateStart DESC "
                          "LIMIT %1 OFFSET %2"
                          )
            .arg(QString::number(endIndex - startIndex + 1))
            .arg(QString::number(startIndex));
    QStringList idList = m_historyModel.getColumnDataWithSql(sql);
    return idList;
}

QDBusVariant CallHistoryInterface::onGetCallHistory(const QString &historyId)
{
    if(historyId.isEmpty() || historyId == "")
        return QDBusVariant();

    QStringList rowData = m_historyModel.findDataInColumn("id", historyId);
    CallHistory history = getHistory(rowData);
    QDBusVariant dbusValue;
    QVariant historyVar = QVariant::fromValue(history);
    dbusValue.setVariant(historyVar);
    return dbusValue;


//    for(int i = 0, iend = m_historyModel.rowCount(); i < iend; i++)
//    {
//        QModelIndex index = m_historyModel.index(i, 0);
//        if(m_historyModel.data(index).toString() == historyId)
//        {
//            CallHistory history = getHistory(i);
//            QDBusVariant dbusValue;
//            QVariant historyVar = QVariant::fromValue(history);
//            dbusValue.setVariant(historyVar);
//            return dbusValue;
//        }
//    }
//    return QDBusVariant();

}

CallHistory CallHistoryInterface::addCallHistory(const CallHistory &callHistory)
{
    qDebug()<<"onAddCallHistory in service ";
//    if(callHistory.dialTarget.isEmpty())
//        return false;
    CallHistory changedCallHistory = callHistory;
    QString dialHistoryId = QUuid::createUuid().toString();
    changedCallHistory.dialHistoryId = dialHistoryId;

    int rowCount = m_historyModel.rowCount();
    bool insertFlag = m_historyModel.insertRow(rowCount);
    if(!insertFlag)
        return CallHistory();

    bool setFlag = setHistory(rowCount, changedCallHistory);
    if(!setFlag)
        return CallHistory();

    m_historyModel.submitAll();
    emitChangeSignal(HISTORY_ADD, changedCallHistory);

    return changedCallHistory;
}

CallHistory CallHistoryInterface::addRemoteCallHistory(const CallHistory &callHistory)
{
    qDebug()<<"onAddCallHistory in service ";
//    if(callHistory.dialTarget.isEmpty())
//        return false;
    CallHistory changedCallHistory = callHistory;

    int rowCount = m_historyModel.rowCount();
    bool insertFlag = m_historyModel.insertRow(rowCount);
    if(!insertFlag)
        return CallHistory();

    bool setFlag = setHistory(rowCount, changedCallHistory);
    if(!setFlag)
        return CallHistory();

    m_historyModel.submitAll();
    emitChangeSignal(HISTORY_ADD, changedCallHistory);

    return changedCallHistory;
}

bool CallHistoryInterface::removeCallHistory(const QString &historyId)
{
    qDebug()<<"onRemoveCallHistory in service";

    if(historyId.isEmpty())
        return false;

    for(int i = 0, iend = m_historyModel.rowCount(); i < iend; i++)
    {
        QModelIndex index = m_historyModel.index(i, 0);
        if(m_historyModel.data(index).toString() == historyId)
        {
            CallHistory changeHistory = getHistory(i);
            m_historyModel.removeRow(i);
            m_historyModel.submitAll();
            emitChangeSignal(HISTORY_REMOVE, changeHistory);
            return true;
        }
    }
    return false;
}

bool CallHistoryInterface::onAddCallHistory(const CallHistory &callHistory)
{
    CallHistory changedCallHistory = addCallHistory(callHistory);
    if(!changedCallHistory.dialHistoryId.isEmpty())
    {
        CallHistoryList callHistorys;
        callHistorys.append(changedCallHistory);
        emit signalCallHistoryChangedToChangeRemoteCallHistory(HISTORY_ADD, callHistorys);
        return true;
    }
    return false;
}

bool CallHistoryInterface::onRemoveCallHistory(const QString &historyId)
{
    bool flag = removeCallHistory(historyId);
    if(flag)
    {
        CallHistory callHistory;
        callHistory.dialHistoryId = historyId;
        CallHistoryList callHistorys;
        callHistorys.append(callHistory);
        emit signalCallHistoryChangedToChangeRemoteCallHistory(HISTORY_REMOVE, callHistorys);
    }
    return flag;
}

bool CallHistoryInterface::onClearCallHistory()
{
    qDebug()<<"onClearCallHistory ";
    CallHistoryList callHistorys;
    QStringList historyIdList = onGetCallHistoryList();

    foreach(const QString &id, historyIdList)
    {
        CallHistory callHistory;
        callHistory.dialHistoryId = id;
        callHistorys.append(callHistory);
    }
    emit signalCallHistoryChangedToChangeRemoteCallHistory(HISTORY_REMOVE, callHistorys);

    m_historyModel.clear();
    m_historyModel.submitAll();
    return true;
}

int CallHistoryInterface::onGetCallHistroyCount()
{
    return m_historyModel.rowCount();
}

QStringList CallHistoryInterface::onGetCallHistoryBytarget(const QString &targer)
{
    QStringList ids;

    QString sql = QString("SELECT id FROM __tablename__ WHERE contactId1='%1' ORDER BY dateStart DESC")
            .arg(targer);

    ids = m_historyModel.getColumnDataWithSql(sql);

//    for(int i = 0, iend = m_historyModel.rowCount(); i < iend; i++)
//    {
//        QModelIndex index = m_historyModel.index(i, 1);
//        QStringList list = convertStringToTargets(m_historyModel.data(index).toString());
//        for (int j = 0, jend = list.count(); j< jend; j++){
//            if (list.at(j) == targer){
//                QModelIndex indexId = m_historyModel.index(i, 0);
//                QString id = m_historyModel.data(indexId).toString();
//                ids.append(id);
//            }
//        }
//    }
    qDebug()<<"onGetCallHistoryBytarget in service "<<ids;
    return ids;
}

QStringList CallHistoryInterface::onGetCallHistoryByNumber(const QString &number)
{
    QStringList ids;

    QString sql = QString("SELECT id FROM __tablename__ WHERE number1='%1' ORDER BY dateStart DESC")
            .arg(number);

    ids = m_historyModel.getColumnDataWithSql(sql);
    qDebug()<<"onGetCallHistoryByNumber in service "<<ids;
    return ids;
}
#include <QSqlError>
#include "sqlfunction.h"
QStringList CallHistoryInterface::onSearchCallHistory(const QString &searchText)
{
//    return onSearchCallHistory(searchText, 0);
    QTime time;
    time.start();
    QStringList historyIdList;
    QString sql;

    if(!searchText.isEmpty())
    {
        sql = QString("SELECT historyid "
                      "FROM "

                      "(SELECT * FROM "

                      "( "
                      "SELECT callhistory.id AS historyid, "
                      "callhistory.dateStart AS historyDateStart, "
                      "PNL(contacter.namePinyin,'%1') AS idx, "
                      "INDEXOF(callhistory.number1,'%1') AS num1idx, "
                      "NotNoneStringOf(callhistory.contactId1, callhistory.number1) AS g1, "
                      "NotNoneStringOf(callhistory.contactId2, callhistory.number2) AS g2, "
                      "NotNoneStringOf(callhistory.contactId3, callhistory.number3) AS g3, "
                      "NotNoneStringOf(callhistory.contactId4, callhistory.number4) AS g4, "
                      "NotNoneStringOf(callhistory.contactId5, callhistory.number5) AS g5, "
                      "NotNoneStringOf(callhistory.contactId6, callhistory.number6) AS g6 "
                      "FROM callhistory,contacter "
                      "WHERE "
                      "(callhistory.contactId1=contacter.id OR "
                      "callhistory.contactId2=contacter.id OR "
                      "callhistory.contactId3=contacter.id OR "
                      "callhistory.contactId4=contacter.id OR "
                      "callhistory.contactId5=contacter.id OR "
                      "callhistory.contactId6=contacter.id) "

                      "UNION "

                      "SELECT callhistory.id AS historyid, "
                      "callhistory.dateStart AS historyDateStart, "
                      "INDEXOF(callhistory.number1,'%1') AS idx, "
                      "INDEXOF(callhistory.number1,'%1') AS num1idx, "
                      "NotNoneStringOf(callhistory.contactId1, callhistory.number1) AS g1, "
                      "NotNoneStringOf(callhistory.contactId2, callhistory.number2) AS g2, "
                      "NotNoneStringOf(callhistory.contactId3, callhistory.number3) AS g3, "
                      "NotNoneStringOf(callhistory.contactId4, callhistory.number4) AS g4, "
                      "NotNoneStringOf(callhistory.contactId5, callhistory.number5) AS g5, "
                      "NotNoneStringOf(callhistory.contactId6, callhistory.number6) AS g6 "
                      "FROM callhistory "
                      "WHERE "
                      "(callhistory.contactId1 is null AND "
                      "callhistory.contactId2 is null AND "
                      "callhistory.contactId3 is null AND "
                      "callhistory.contactId4 is null AND "
                      "callhistory.contactId5 is null AND "
                      "callhistory.contactId6 is null) "
                      ") "

                      "ORDER BY historyDateStart "
                      ") "

                      "WHERE "
                      "(idx >= 0 AND idx <10000 OR num1idx >=0 AND num1idx < 10000) "
                      "group by g1,g2,g3,g4,g5,g6 "
                      "ORDER BY historyDateStart DESC, num1idx ASC"
                      ).arg(searchText);
    }
    else
    {
        sql = QString("SELECT id FROM "
                      "( "
                      "SELECT id,dateStart,NotNoneStringOf(contactId1,number1) as g1, "
                      "NotNoneStringOf(contactId2,number2) as g2, "
                      "NotNoneStringOf(contactId3,number3) as g3, "
                      "NotNoneStringOf(contactId4,number4) as g4, "
                      "NotNoneStringOf(contactId5,number5) as g5, "
                      "NotNoneStringOf(contactId6,number6) as g6 "
                      "FROM callhistory "
                      ") "
                      "group by g1,g2,g3,g4,g5,g6 "
                      "ORDER BY dateStart DESC");
    }
    historyIdList = SqlFunction::getSqlExecResult(sql);
    qDebug() << "historyIdList" << historyIdList << time.elapsed();
    return historyIdList;
}

QStringList CallHistoryInterface::onSearchCallHistory(const QString &searchText, const int &type)
{
    QTime time;
    time.start();
    QStringList historyIdList;
    QString sql;

    QString dialType;
    if(type == 1)
        dialType = "IN_DIAL";
    else if(type == 2)
        dialType = "OUT_DIAL";
    else if(type == 3)
        dialType = "MISS_DIAL";
    else if(type == 4)
        dialType = "MEETING_DIAL";
    else
        dialType = "";

    QString findDialType;
    QString findDialTypeAll;
    if(!dialType.isEmpty())
    {
        findDialType = QString(" AND dialType='%1'" ).arg(dialType);
        findDialTypeAll = QString(" WHERE dialType='%1'" ).arg(dialType);
    }

    if(!searchText.isEmpty())
    {
        sql = QString("SELECT historyid "
                      "FROM "

                      "(SELECT * FROM "

                      "( "
                      "SELECT callhistory.id AS historyid, "
                      "callhistory.dateStart AS historyDateStart, "
                      "callhistory.dialType AS dialType, "
                      "PNL(contacter.namePinyin,'%1') AS idx, "
                      "INDEXOF(callhistory.number1,'%1') AS num1idx, "
                      "NotNoneStringOf(callhistory.contactId1, callhistory.number1) AS g1, "
                      "NotNoneStringOf(callhistory.contactId2, callhistory.number2) AS g2, "
                      "NotNoneStringOf(callhistory.contactId3, callhistory.number3) AS g3, "
                      "NotNoneStringOf(callhistory.contactId4, callhistory.number4) AS g4, "
                      "NotNoneStringOf(callhistory.contactId5, callhistory.number5) AS g5, "
                      "NotNoneStringOf(callhistory.contactId6, callhistory.number6) AS g6 "
                      "FROM callhistory,contacter "
                      "WHERE "
                      "(callhistory.contactId1=contacter.id OR "
                      "callhistory.contactId2=contacter.id OR "
                      "callhistory.contactId3=contacter.id OR "
                      "callhistory.contactId4=contacter.id OR "
                      "callhistory.contactId5=contacter.id OR "
                      "callhistory.contactId6=contacter.id) "

                      "UNION "

                      "SELECT callhistory.id AS historyid, "
                      "callhistory.dateStart AS historyDateStart, "
                      "callhistory.dialType AS dialType, "
                      "INDEXOF(callhistory.number1,'%1') AS idx, "
                      "INDEXOF(callhistory.number1,'%1') AS num1idx, "
                      "NotNoneStringOf(callhistory.contactId1, callhistory.number1) AS g1, "
                      "NotNoneStringOf(callhistory.contactId2, callhistory.number2) AS g2, "
                      "NotNoneStringOf(callhistory.contactId3, callhistory.number3) AS g3, "
                      "NotNoneStringOf(callhistory.contactId4, callhistory.number4) AS g4, "
                      "NotNoneStringOf(callhistory.contactId5, callhistory.number5) AS g5, "
                      "NotNoneStringOf(callhistory.contactId6, callhistory.number6) AS g6 "
                      "FROM callhistory "
                      "WHERE "
                      "(callhistory.contactId1 is null AND "
                      "callhistory.contactId2 is null AND "
                      "callhistory.contactId3 is null AND "
                      "callhistory.contactId4 is null AND "
                      "callhistory.contactId5 is null AND "
                      "callhistory.contactId6 is null) "
                      ") "

                      "ORDER BY historyDateStart "
                      ") "

                      "WHERE "
                      "(idx >= 0 AND idx <10000 OR num1idx >=0 AND num1idx < 10000) "
                      "%2"
                      "group by g1,g2,g3,g4,g5,g6 "
                      "ORDER BY historyDateStart DESC, num1idx ASC"
                      ).arg(searchText).arg(findDialType);
    }
    else
    {
        sql = QString("SELECT id FROM "
                      "( "
                      "SELECT id,dialType,dateStart,NotNoneStringOf(contactId1,number1) as g1, "
                      "NotNoneStringOf(contactId2,number2) as g2, "
                      "NotNoneStringOf(contactId3,number3) as g3, "
                      "NotNoneStringOf(contactId4,number4) as g4, "
                      "NotNoneStringOf(contactId5,number5) as g5, "
                      "NotNoneStringOf(contactId6,number6) as g6 "
                      "FROM callhistory "
                      ") "
                      "%1"
                      "group by g1,g2,g3,g4,g5,g6 "
                      "ORDER BY dateStart DESC").arg(findDialTypeAll);
    }
    historyIdList = SqlFunction::getSqlExecResult(sql);
    qDebug() << "historyIdList" << historyIdList << time.elapsed();
    return historyIdList;
}

void CallHistoryInterface::emitChangeSignal(int type, CallHistory callhistory)
{
    QDBusVariant dbusValue;
    QVariant dataVar = QVariant::fromValue(callhistory);
    dbusValue.setVariant(dataVar);
    emit signalCallHistoryUpdate(type, dbusValue);
}

bool CallHistoryInterface::setHistory(const int &row, const CallHistory &callhistory)
{
    bool ret;
    qDebug()<< "setHistory";
    //id, dialTarget, dialType, dialTime, dialNetworkType, date
    QModelIndex idIndex = m_historyModel.index(row, 0);
    m_historyModel.setData(idIndex, callhistory.dialHistoryId);

//    QModelIndex dialTargetIndex = m_historyModel.index(row, 1);
//    m_historyModel.setData(dialTargetIndex, convertTargetsToString(callhistory.dialTarget));

    QModelIndex dialTypeIndex = m_historyModel.index(row, 2);
    m_historyModel.setData(dialTypeIndex, converHistoryTypeToString((HistoryType)callhistory.dialType));

    QModelIndex dialTimeIndex = m_historyModel.index(row, 3);
    m_historyModel.setData(dialTimeIndex, callhistory.dialTime);

    QModelIndex dialNetworkTypeIndex = m_historyModel.index(row, 4);
    m_historyModel.setData(dialNetworkTypeIndex, converNetworkTypeToString((HistoryNetwork)callhistory.dialNetworkType));

    QModelIndex dateIndex = m_historyModel.index(row, 5);
    ret = m_historyModel.setData(dateIndex, transMessageDateToString(callhistory.dialDateStart));

    QModelIndex dateEndIndex = m_historyModel.index(row, 6);
    m_historyModel.setData(dateEndIndex, transMessageDateToString(callhistory.dialDateEnd));

    QModelIndex contactId1Index = m_historyModel.index(row, 7);
    m_historyModel.setData(contactId1Index, callhistory.dialTarget.value(0));

    QModelIndex contactId2Index = m_historyModel.index(row, 8);
    m_historyModel.setData(contactId2Index, callhistory.dialTarget.value(1));

    QModelIndex contactId3Index = m_historyModel.index(row, 9);
    m_historyModel.setData(contactId3Index, callhistory.dialTarget.value(2));

    QModelIndex contactId4Index = m_historyModel.index(row, 10);
    m_historyModel.setData(contactId4Index, callhistory.dialTarget.value(3));

    QModelIndex contactId5Index = m_historyModel.index(row, 11);
    m_historyModel.setData(contactId5Index, callhistory.dialTarget.value(4));

    QModelIndex contactId6Index = m_historyModel.index(row, 12);
    m_historyModel.setData(contactId6Index, callhistory.dialTarget.value(5));

    QModelIndex number1Index = m_historyModel.index(row, 13);
    m_historyModel.setData(number1Index, callhistory.dialTargetNumber.value(0));

    QModelIndex number2Index = m_historyModel.index(row, 14);
    m_historyModel.setData(number2Index, callhistory.dialTargetNumber.value(1));

    QModelIndex number3Index = m_historyModel.index(row, 15);
    m_historyModel.setData(number3Index, callhistory.dialTargetNumber.value(2));

    QModelIndex number4Index = m_historyModel.index(row, 16);
    m_historyModel.setData(number4Index, callhistory.dialTargetNumber.value(3));

    QModelIndex number5Index = m_historyModel.index(row, 17);
    m_historyModel.setData(number5Index, callhistory.dialTargetNumber.value(4));

    QModelIndex number6Index = m_historyModel.index(row, 18);
    m_historyModel.setData(number6Index, callhistory.dialTargetNumber.value(5));
    qDebug()<< "setHistory end";
    return true;
}

bool CallHistoryInterface::setHistory(const CallHistory &callhistory)
{
    int row = m_historyModel.findDataInColumn(0, callhistory.dialHistoryId);
    return setHistory(row, callhistory);
}

QString CallHistoryInterface::converNetworkTypeToString(const HistoryNetwork &type)
{
    return callhistoryNetworkTypeConvertToString(type);
}

HistoryNetwork CallHistoryInterface::converStringToNetworkType(const QString &value)
{
    return callhistoryStringConvertToNetworkType(value);
}

QString CallHistoryInterface::converHistoryTypeToString(const HistoryType &type)
{
    QString value;
    switch(type)
    {
    case IN_DIAL:
        value = "IN_DIAL";
        break;
    case OUT_DIAL:
        value = "OUT_DIAL";
        break;
    case MISS_DIAL:
        value = "MISS_DIAL";
        break;
    case MEETING_DIAL:
        value = "MEETING_DIAL";
        break;
    case UNDEFINE_DIAL:
        value = "UNDEFINE_DIAL";
        break;
    default:
        value = "UNDEFINE_DIAL";
    }

    return value;
}

HistoryType CallHistoryInterface::converStringToHistoryType(const QString &value)
{
    if(value == "IN_DIAL")
        return IN_DIAL;
    else if(value == "OUT_DIAL")
        return OUT_DIAL;
    else if(value == "MISS_DIAL")
        return MISS_DIAL;
    else if(value == "MEETING_DIAL")
        return MEETING_DIAL;
    else if(value == "UNDEFINE_DIAL")
        return UNDEFINE_DIAL;
    else
        return UNDEFINE_DIAL;

    return UNDEFINE_DIAL;
}

CallHistory CallHistoryInterface::getHistory(const int &row)
{
    //id, dialTarget, dialType, dialTime, dialNetworkType, date
    return getHistory(m_historyModel.rowData(row));
}

CallHistory CallHistoryInterface::getHistory(const QStringList &rowData)
{
    //id, dialTarget, dialType, dialTime, dialNetworkType, date
    if(rowData.isEmpty())
        return CallHistory();

    CallHistory callHistory;
    callHistory.dialHistoryId = rowData.value(0);

//    QModelIndex dialTargetIndex = m_historyModel.index(row, 1);
//    callHistory.dialTarget = convertStringToTargets(m_historyModel.data(dialTargetIndex).toString());

    callHistory.dialType = converStringToHistoryType(rowData.value(2));

    callHistory.dialTime = rowData.value(3);

    callHistory.dialNetworkType = converStringToNetworkType(rowData.value(4));

    callHistory.dialDateStart = transStringToMessageDate(rowData.value(5));

    callHistory.dialDateEnd = transStringToMessageDate(rowData.value(6));

    if(!rowData.value(7).isEmpty())
        callHistory.dialTarget.append(rowData.value(7));

    if(!rowData.value(8).isEmpty())
        callHistory.dialTarget.append(rowData.value(8));

    if(!rowData.value(9).isEmpty())
        callHistory.dialTarget.append(rowData.value(9));

    if(!rowData.value(10).isEmpty())
        callHistory.dialTarget.append(rowData.value(10));

    if(!rowData.value(11).isEmpty())
        callHistory.dialTarget.append(rowData.value(11));

    if(!rowData.value(12).isEmpty())
        callHistory.dialTarget.append(rowData.value(12));

    if(!rowData.value(13).isEmpty())
        callHistory.dialTargetNumber.append(rowData.value(13));

    if(!rowData.value(14).isEmpty())
        callHistory.dialTargetNumber.append(rowData.value(14));

    if(!rowData.value(15).isEmpty())
        callHistory.dialTargetNumber.append(rowData.value(15));

    if(!rowData.value(16).isEmpty())
        callHistory.dialTargetNumber.append(rowData.value(16));

    if(!rowData.value(17).isEmpty())
        callHistory.dialTargetNumber.append(rowData.value(17));

    if(!rowData.value(18).isEmpty())
        callHistory.dialTargetNumber.append(rowData.value(18));
    return callHistory;
}

QString CallHistoryInterface::convertTargetsToString(const QStringList &leaders)
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

//    QString val;

//    for(int i=0,count = leaders.count(); i<count; i++)
//    {
//        val.append(leaders.at(i));
//        if(i != count-1)
//            val.append(",");
//    }

//    return val;
}

QStringList CallHistoryInterface::convertStringToTargets(const QString &value)
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

    //qDebug()<<"convertStringToTargets "<<leaders<<array.count();
    return leaders;

//    QStringList leaders = value.split(",");
//    return leaders;
}

void CallHistoryInterface::onContacterEmitChangeSignal(int type, const Contacter &changeContacter)
{
    qDebug() << "onContacterEmitChangeSignal" << type;
    switch (type)
    {
    case CONTACTER_ADD_HANDLE:
    {
        //add
        foreach(const PhoneNumber &number, changeContacter.numbers)
        {
            QString sql = QString("SELECT id FROM __tablename__ WHERE "
                                  "number1='%1' OR number2='%1' OR number3='%1' OR "
                                  "number4='%1' OR number5='%1' OR number6='%1' "
                                  ).arg(number.number);
            QStringList idList = m_historyModel.getColumnDataWithSql(sql);
            foreach(const QString &id, idList)
            {
                QStringList rowData = m_historyModel.findDataInColumn("id", id);
                CallHistory history = getHistory(rowData);
                history.dialTarget.append(changeContacter.id);
                setHistory(history);
                m_historyModel.submitAll();

                emitChangeSignal(HISTORY_UPDATE, history);
            }
        }
        break;
    }
    case CONTACTER_DELETE_HANDLE:
    {
        //delete
        QString sql = QString("SELECT id FROM __tablename__ WHERE "
                              "contactId1='%1' OR contactId2='%1' OR contactId3='%1' OR "
                              "contactId4='%1' OR contactId5='%1' OR contactId6='%1' "
                              )
                .arg(changeContacter.id);
        QStringList idList = m_historyModel.getColumnDataWithSql(sql);
        foreach(const QString &id, idList)
        {
            QStringList rowData = m_historyModel.findDataInColumn("id", id);
            CallHistory history = getHistory(rowData);
            history.dialTarget.removeAll(changeContacter.id);
            setHistory(history);
            m_historyModel.submitAll();

            emitChangeSignal(HISTORY_UPDATE, history);
        }

        break;
    }
    case CONTACTER_UPDATE_HANDLE:
    {
        //delete
        QString sql = QString("SELECT id FROM __tablename__ WHERE "
                              "contactId1='%1' OR contactId2='%1' OR contactId3='%1' OR "
                              "contactId4='%1' OR contactId5='%1' OR contactId6='%1' "
                              )
                .arg(changeContacter.id);
        QStringList idList = m_historyModel.getColumnDataWithSql(sql);
        foreach(const QString &id, idList)
        {
            QStringList rowData = m_historyModel.findDataInColumn("id", id);
            CallHistory history = getHistory(rowData);
            int count = history.dialTargetNumber.count();
            QStringList deleteIdList;
            for(int i=0; i<count; i++)
            {
                QString dialOne = history.dialTarget.value(i);
                QString dialNumber = history.dialTargetNumber.value(i);
                if(!dialOne.isEmpty() && !dialNumber.isEmpty())
                {
                    if(!m_contacterInterface->isIdNumberAdapt(dialOne, dialNumber))
                    {
                        deleteIdList.append(history.dialTarget.value(i));
                    }
                }
            }
            foreach(const QString &id, deleteIdList)
            {
                history.dialTarget.removeAll(id);
            }
            setHistory(history);
            m_historyModel.submitAll();

            emitChangeSignal(HISTORY_UPDATE, history);
        }

        //add
        foreach(const PhoneNumber &number, changeContacter.numbers)
        {
            QString sql = QString("SELECT id FROM __tablename__ WHERE "
                                  "number1='%1' OR number2='%1' OR number3='%1' OR "
                                  "number4='%1' OR number5='%1' OR number6='%1' "
                                  ).arg(number.number);
            QStringList idList = m_historyModel.getColumnDataWithSql(sql);
            foreach(const QString &id, idList)
            {
                QStringList rowData = m_historyModel.findDataInColumn("id", id);
                CallHistory history = getHistory(rowData);
                history.dialTarget.append(changeContacter.id);
                setHistory(history);
                m_historyModel.submitAll();

                emitChangeSignal(HISTORY_UPDATE, history);
            }
        }

        break;
    }
    default:
        break;
    }
}

void CallHistoryInterface::onCallHistoryReceived(const int &handleType, const CallHistoryList &callHistorys)
{
    switch (handleType) {
    case HISTORY_ADD:
    {
        CallHistory history = callHistorys.value(0);
        addRemoteCallHistory(history);
        if(history.dialType == MISS_DIAL)
        {
            addSyncCount(1);
        }
        setSyncCountToAppIcon(getSyncCount());
        break;
    }
    case HISTORY_UPDATE:
//        on(callHistory);
        break;
    case HISTORY_REMOVE:
    {
        foreach(const CallHistory &history, callHistorys)
            removeCallHistory(history.dialHistoryId);
    }
        break;
    case HISTORY_INIT:
        break;
    default:
        break;
    }
}

//sync count from phone
void CallHistoryInterface::onClearSyncCount()
{
    clearSyncCount();
}

int CallHistoryInterface::onGetSyncCount()
{
    return getSyncCount();
}

//get the watch or phone
bool CallHistoryInterface::isWatch()
{
    return BluetoothBackgroundTransferInterface::getInstance()->onGetServiceType() == BLUETOOTHSERVICETYPE_CLIENT;
}

//sync count from phone
void CallHistoryInterface::addSyncCount(const int &count)
{
    m_currentSyncCount += count;
}

void CallHistoryInterface::clearSyncCount()
{
    m_currentSyncCount = 0;
}

int CallHistoryInterface::getSyncCount()
{
    return m_currentSyncCount;
}

void CallHistoryInterface::setSyncCountToAppIcon(const int &count)
{
    qDebug() << "CallHistoryInterface::setSyncCountToAppIcon" << count;
}

void CallHistoryInterface::onDbChanged()
{
    m_historyModel.select();
    emitChangeSignal(HISTORY_INIT, CallHistory());
}
