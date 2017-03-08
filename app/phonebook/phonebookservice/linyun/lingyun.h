#ifndef LINGYUN_H
#define LINGYUN_H

#include <QObject>
#include "hci_sys.h"
#include "hci_tts.h"
#include "ttssynth.h"
#include <QAudioFormat>
#include <QAudioOutput>
#include <QSound>
#include <QFile>

class LingYun : public QObject
{
    Q_OBJECT
public:
    explicit LingYun(QObject *parent = 0);
    ~LingYun();
    static LingYun *getInstance();
    int init();
    void transToTts(QString msg);

signals:

public slots:
    void onStateChanged(QAudio::State state);
private:
    static LingYun *instance;

    bool m_enable;
    QStringList m_msgList;
    TtsSynth *m_ttsSynth;
    QAudioOutput *m_audioOutput;
    QIODevice *m_audioOutputIODevice;
    QDataStream m_localPlayFile;
    QFile m_sourceFile;

    int unInit();
    void play();
    void stopPlay();

    void playDatatoDevice();
    void initAudioOutput();
    void writeToFile(QString msg);
    bool CheckAndUpdateAuth();
};

#endif // LINGYUN_H
