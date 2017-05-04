#ifndef BLUETOOTHCLIENT2_H
#define BLUETOOTHCLIENT2_H

#include <QObject>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QTimer>
#include <QMutex>
#include <QFile>
//#include "../../commondata/bluetoothdata.h"
#include "./datatype/bluetoothdata.h"


class BluetoothClient2 : public QObject
{
    Q_OBJECT

    public:
        explicit BluetoothClient2(QObject *parent = 0);
        ~BluetoothClient2();

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

    signals:
        void connectFinished(const BluetoothAddressData &address);
        void disconnected();
        void connectError(QBluetoothSocket::SocketError error);
        void cancelTransmit();

        //percent
        void fileSendPercent(const QString &fileName, const int &dataType, const qint64 &size, const qint64 &maxSize);

    private slots:
        void onReadSocket();
        void onConnected();
        void onSendPacketData();
        void onError(QBluetoothSocket::SocketError error);

    private:
        qint64 sendFile(const QString &fileName);

    private:
        QBluetoothSocket *m_socket;
        QTimer m_sendTimer;
        QMutex m_sendMutex;
        QFile m_sendingFile;
        bool m_sendingOver;
        bool m_sendingStart;
        bool m_cancel;
        PackageData m_currentPackageData;
        int m_sentDataType; // -1 file, 0 data
};

#endif // BLUETOOTHCLIENT2_H
