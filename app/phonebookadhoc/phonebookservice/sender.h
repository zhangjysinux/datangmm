#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDataStream>
#include "../../commondata/datatype/messagedata.h"

struct PacketData
{
    unsigned int size;
    unsigned int maxSize;
    unsigned int index;
    bool isStart;
    bool isEnd;
    QByteArray data;
};

inline QDataStream &operator<<(QDataStream &out,const PacketData &data)
{
    out << data.size << data.maxSize << data.index << data.isStart << data.isEnd << data.data;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, PacketData &data)
{
    in >> data.size >> data.maxSize >> data.index >> data.isStart >> data.isEnd >> data.data;
    return in;
}

Q_DECLARE_METATYPE(PacketData)

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = 0);
    qint64 sendMessage(const Message &message);
    void setSocket(QUdpSocket *socket, const QString &address, const int &port);

private:
    QHostAddress m_castHost;
    QUdpSocket *m_socket;
    int m_port;
};

#endif // SENDER_H
