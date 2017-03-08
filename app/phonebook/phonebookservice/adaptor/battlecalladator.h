#ifndef BATTLECALLADATOR_H
#define BATTLECALLADATOR_H

#include <QObject>
#include <QDBusInterface>
#include "../../commondata/datatype/commondata.h"

class BattleCallAdator : public QObject
{
    Q_OBJECT
public:
    static BattleCallAdator *getInstance();

    int onCallBattle(const QString &addr);
    int onHangupBattle(const QString &addr);
    int onSetBattlePttState(const QString &addr, int pttState);
    int onGetBattlePttState(const QString &addr);
    int onSetPort(int port);

private:
    explicit BattleCallAdator(QObject *parent = 0);
    static BattleCallAdator *instance;
    QDBusInterface m_interface;

signals:
    void signalBattleCallStateChanged(int network, const QString &ip, int state);

private slots:
    void onSignalBattleCallStateChanged(const QString &ip, int state);

};

#endif // BATTLECALLADATOR_H
