#ifndef REALTIMEVIDEOVIEWMODEL_H
#define REALTIMEVIDEOVIEWMODEL_H

#include <QObject>

class VoipServiceAdaptor;

class RealtimeVideoViewModel : public QObject
{
    Q_OBJECT
public:
    explicit RealtimeVideoViewModel(QObject *parent = 0);
    ~RealtimeVideoViewModel();

    Q_INVOKABLE void call(const QString &ip);
    Q_INVOKABLE void hangup();

private:
    VoipServiceAdaptor *m_voip;

    int m_callID;
};

#endif // REALTIMEVIDEOVIEWMODEL_H
