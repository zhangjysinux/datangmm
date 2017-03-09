#ifndef WAVTOMP3CONVERTER_H
#define WAVTOMP3CONVERTER_H

#include <string>
#include <QObject>
#include <QString>
#include <QMutex>

using namespace std;

// by michael3 zheng 2017.3.8 for no using

//class WavToMp3Converter
//{
//public:
//    WavToMp3Converter(const char *filename);
//    void convert();

//private:
//    static void *convert(void *arg);
//    string m_filename;
//};

// end by michael zheng

class WavToMp3RealTimeConverter : public QObject
{
    Q_OBJECT
public:
    static WavToMp3RealTimeConverter *getInstance();

    void startConvert(const QString& fileName);
    void stopConvert();

signals:
    void signalConvertFinished(const QString &fileName);

private:
    WavToMp3RealTimeConverter();
    static void *convert(void *arg);

private:
    static WavToMp3RealTimeConverter *instance;
    string m_fileName;
    bool m_quit;
    QMutex m_mutex;
};

#endif // WAVTOMP3CONVERTER_H
