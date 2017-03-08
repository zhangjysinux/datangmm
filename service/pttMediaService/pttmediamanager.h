#ifndef PTTMEDIAMANAGER_H
#define PTTMEDIAMANAGER_H

#include <pjlib.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>
#include <string>
#include <list>
#include "wavtomp3converter.h"

using std::string;
using std::list;

class PttMediaStream : public QObject
{
    Q_OBJECT
public:
    enum MediaDirection
    {
        MediaDirNone,
        MediaDirRecving,
        MediaDirSending
    };

    PttMediaStream(const char *groupAddr, unsigned short port);
    ~PttMediaStream();

    MediaDirection getMediaDirection() const;
    bool setMediaDirection(MediaDirection dir);

private slots:
    void onConvertFinished(const QString &fileName);

signals:
    void signalConvertFinished(unsigned groupId, const QString &fileName);

private:
    bool init();
    void shutdown();

    pj_status_t createTransport();
    pj_status_t createStream();
    void generateFilename();

    string m_groupAddr;
    pj_uint16_t m_port;

    MediaDirection m_dir;
    pjmedia_transport *m_transport;
    pj_pool_t *m_pool;
    pjmedia_stream *m_stream;
    unsigned m_streamSlot;
    unsigned m_recSlot;
    string m_filename;
    pjmedia_snd_port *m_snd_port;
    pjmedia_snd_port *m_snd_port_for_record;
    pjmedia_port *m_stream_port;
    pjmedia_port *m_rec_file_port;

    WavToMp3RealTimeConverter *m_converter;
};

class PttMediaManager : public QObject
{
    Q_OBJECT
public:
    static PttMediaManager &instance();

    PttMediaManager();
    ~PttMediaManager();

    PttMediaStream *createStream(const char *groupAddr, unsigned short port);
    bool destoryStream(PttMediaStream *stream);
    bool hasStream(PttMediaStream *stream);

signals:
    void signalConvertFinished(unsigned groupId, const QString &fileName);

private:
    bool init();
    void shutdown();

    static PttMediaManager *m_instance;

    const unsigned m_clockRate;
    const string m_recordDir;

    pj_caching_pool m_cp;
    pjmedia_endpt *m_medEndpt;
    pj_pool_t *m_pool;
    pjmedia_conf *m_conf;
    const pjmedia_codec_info *m_codecInfo;

    list<PttMediaStream *> m_streams;

    friend class PttMediaStream;
};

#endif // PTTMEDIAMANAGER_H
