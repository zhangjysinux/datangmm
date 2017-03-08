#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include <QObject>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QTimer>
#include <QMutex>
#include <QFile>
#include "../../commondata/bluetoothdata.h"


class BluetoothClient : public QObject
{
    Q_OBJECT

    public:
        explicit BluetoothClient(QObject *parent = 0);
        ~BluetoothClient();

        void startClient(const QBluetoothServiceInfo &remoteService);
        int startClient(const QBluetoothAddress & address,
                         const QBluetoothUuid & uuid);
        void stopClient();
        bool isConnected();
        bool isFinished();

    public slots:
        qint64 onSendMessage(const BluetoothTransData &message);
        qint64 onSendFile(const QString &fileName);
        bool onCancelTransmit();
    private slots:
        bool getSendFileDataNext();

    signals:
        void messageReceived(const QString &sender, const BluetoothTransData &message);
        void connectFinished(const BluetoothAddressData &address);
        void disconnected();
        void messageSended(const int &sendStatus);
        void messagePercent(const double &percent);
        void connectError(QBluetoothSocket::SocketError error);
        void cancelTransmit();

    private slots:
        void onReadSocket();
        void onConnected();
        void onSendPacketData();
        void onError(QBluetoothSocket::SocketError error);

    private:
        QBluetoothSocket *m_socket;
        PackageDataList m_sendDataList;
        QTimer m_sendTimer;
        QMutex m_sendMutex;
        QFile m_sendingFile;
        bool m_sendingOver;
        bool m_sendingStart;
        bool m_cancel;
};

#endif // BLUETOOTHCLIENT_H
