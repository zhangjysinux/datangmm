#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QAudioRecorder>
#include <QUrl>

class AudioRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY ( bool recording READ recording NOTIFY recordingChanged )
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    static AudioRecorder *getInstance();

    const bool recording() const;
    QString name() const;

    Q_INVOKABLE QStringList supportedAudioCodecs();
    Q_INVOKABLE QStringList supportedContainers();
    Q_INVOKABLE QUrl path() {
        return m_path;
    }

signals:
    void recordingChanged(bool);
    void nameChanged(QUrl);

public slots:
    void setName(QString name);
    void setRecording(bool recording );
    void record();
    void stop();

private:
    QString getFilePath(const QString filename) const;

private:
    explicit AudioRecorder(QObject *parent = 0);
    QAudioRecorder * m_audioRecorder;
    bool m_recording;
    QString m_name;
    QUrl m_path;
    static AudioRecorder *instance;
};

#endif // AUDIORECORDER_H
