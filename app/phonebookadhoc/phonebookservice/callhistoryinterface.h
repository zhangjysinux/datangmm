#ifndef CALLHISTORYINTERFACE_H
#define CALLHISTORYINTERFACE_H

#include <QObject>
#include <QSqlTableModel>
#include "../../commondata/datatype/callhistorydata.h"
#include "contacterinterface.h"
#include "../../commondata/datatype/contacterdata.h"
#include "sqltablemodelwithcache.h"

class CallHistoryInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.sinux.DBus.callhistory")
public:
    explicit CallHistoryInterface(QObject *parent = 0);
    static CallHistoryInterface *getInstance();

signals:
    void signalCallHistoryUpdate(int type, QDBusVariant callHistory);
    void signalCallHistoryChangedToChangeRemoteCallHistory(int type, const CallHistoryList &callHistorys);

public slots:
    QStringList onGetCallHistoryList();
    QDBusVariant onGetCallHistory(const QString &historyId);
    bool onAddCallHistory(const CallHistory &callHistory);
    bool onRemoveCallHistory(const QString &historyId);
    bool onClearCallHistory();
    int onGetCallHistroyCount();
    QStringList onGetCallHistoryBytarget(const QString &targer);
    QStringList onGetCallHistoryByNumber(const QString &number);
    QStringList onSearchCallHistory(const QString &searchText);
    QStringList onSearchCallHistory(const QString &searchText, const int &type);
    QStringList onGetCallHistoryList(const QDateTime &startTime, const QDateTime &endTime);
    QStringList onGetCallHistoryList(const int &startIndex, const int &endIndex);

    //respond to contacter
    void onContacterEmitChangeSignal(int type, const Contacter &changeContacter);

    //for share
    void onCallHistoryReceived(const int &handleType, const CallHistoryList &callHistorys);
    void onDbChanged();

    //sync count from phone
    void onClearSyncCount();
    int onGetSyncCount();

private:
    CallHistory addCallHistory(const CallHistory &callHistory);
    CallHistory addRemoteCallHistory(const CallHistory &callHistory);
    bool removeCallHistory(const QString &historyId);

private:
    SqlTableModelWithCache m_historyModel;
    static CallHistoryInterface *m_instance;
    ContacterInterface *m_contacterInterface;
    int m_currentSyncCount;

private:

    void emitChangeSignal(int type, CallHistory callhistory);
    CallHistory getHistory(const int &row);
    CallHistory getHistory(const QStringList &rowData);
    bool setHistory(const int &row, const CallHistory &callhistory);
    bool setHistory(const CallHistory &callhistory);
    QString transMessageDateToString(const QDateTime &date);
    QDateTime transStringToMessageDate(const QString &date);
    QString converNetworkTypeToString(const HistoryNetwork &type);
    HistoryNetwork converStringToNetworkType(const QString &value);
    QString converHistoryTypeToString(const HistoryType &type);
    HistoryType converStringToHistoryType(const QString &value);
    QString convertTargetsToString(const QStringList &leaders);
    QStringList convertStringToTargets(const QString &value);

    //get the watch or phone
    bool isWatch();

    //sync count from phone
    void addSyncCount(const int &count);
    void clearSyncCount();
    int getSyncCount();
    void setSyncCountToAppIcon(const int &count);
};

#endif // CALLHISTORYINTERFACE_H
