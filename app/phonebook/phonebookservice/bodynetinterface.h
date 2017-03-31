#ifndef BODYNETINTERFACE_H
#define BODYNETINTERFACE_H

#include <QObject>
#include <QDBusInterface>

class BodyNetInterface : public QObject
{
    Q_OBJECT
public:
    explicit BodyNetInterface(QObject *parent = 0);
    static BodyNetInterface *getInstance();

signals:
    void signalShortCutKey(int keyValue);
public slots:
    void onSignalShortCutKey(int keyValue);
private:
    QDBusInterface *m_interface;
    static BodyNetInterface *m_instance;
private slots:
    void init();
};

#endif // BODYNETINTERFACE_H
