#include "pttmediamanager.h"
#include "wavtomp3converter.h"
#include <QDebug>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

using namespace std;

PttMediaStream::PttMediaStream(const char *groupAddr, unsigned short port) :
    m_groupAddr(groupAddr), m_port(port),
    m_dir(MediaDirNone),
    m_transport(NULL),
    m_pool(NULL),
    m_stream(NULL),
    m_snd_port(NULL),
    m_snd_port_for_record(NULL),
    m_stream_port(NULL),
    m_rec_file_port(NULL),
    m_streamSlot(0),
    m_recSlot(0)
{
    m_converter = WavToMp3RealTimeConverter::getInstance();
    QObject::connect(m_converter, SIGNAL(signalConvertFinished(QString)),
                     this, SLOT(onConvertFinished(QString)));
    init();
}

PttMediaStream::~PttMediaStream()
{
    shutdown();
}

string PttMediaStream::getGroupAddr()
{
    return m_groupAddr;
}

unsigned short PttMediaStream::getGroupPort()
{
    return m_port;
}

PttMediaStream::MediaDirection PttMediaStream::getMediaDirection() const
{
    return m_dir;
}

bool PttMediaStream::setMediaDirection(PttMediaStream::MediaDirection dir)
{
//    pjmedia_conf *conf;
    pj_status_t status;

//    conf = PttMediaManager::instance()->m_conf;

    if (dir == m_dir)
        return false;

    if (m_snd_port)
    {
        pjmedia_snd_port_destroy(m_snd_port);
        m_snd_port = NULL;
    }
    if (m_snd_port_for_record)
    {
        pjmedia_snd_port_destroy(m_snd_port_for_record);
        m_snd_port_for_record = NULL;
    }

    if (dir == MediaDirRecving)
    {
        status = pjmedia_snd_port_create_player(m_pool, -1,
                    PJMEDIA_PIA_SRATE(&m_stream_port->info),
                    PJMEDIA_PIA_CCNT(&m_stream_port->info),
                    PJMEDIA_PIA_SPF(&m_stream_port->info),
                    PJMEDIA_PIA_BITS(&m_stream_port->info),
                    0, &m_snd_port);
        qDebug() << "pjmedia_snd_port_create_player status" << status;

        status = pjmedia_snd_port_connect(m_snd_port, m_stream_port);
        qDebug() << "pjmedia_snd_port_connect status" << status;

        if(m_rec_file_port != NULL)
        {
            status = pjmedia_port_destroy(m_rec_file_port);
            m_rec_file_port = NULL;
            m_converter->stopConvert();
        }
    }
    else if (dir == MediaDirSending)
    {
        status = pjmedia_snd_port_create_rec(m_pool, -1,
                    PJMEDIA_PIA_SRATE(&m_stream_port->info),
                    PJMEDIA_PIA_CCNT(&m_stream_port->info),
                    PJMEDIA_PIA_SPF(&m_stream_port->info),
                    PJMEDIA_PIA_BITS(&m_stream_port->info),
                    0, &m_snd_port);
        qDebug() << "pjmedia_snd_port_create_rec status" << status;

        status = pjmedia_snd_port_connect(m_snd_port, m_stream_port);
        qDebug() << "pjmedia_snd_port_connect status" << status;

        //save wav file
        status = pjmedia_snd_port_create_rec(m_pool, -1,
                    PJMEDIA_PIA_SRATE(&m_stream_port->info),
                    PJMEDIA_PIA_CCNT(&m_stream_port->info),
                    PJMEDIA_PIA_SPF(&m_stream_port->info),
                    PJMEDIA_PIA_BITS(&m_stream_port->info),
                    0, &m_snd_port_for_record);
        qDebug() << "pjmedia_snd_port_create_rec status" << status;
        generateFilename();
        status = pjmedia_wav_writer_port_create(m_pool, m_filename.data(),
                                    PJMEDIA_PIA_SRATE(&m_stream_port->info),
                                    PJMEDIA_PIA_CCNT(&m_stream_port->info),
                                    PJMEDIA_PIA_SPF(&m_stream_port->info),
                                    PJMEDIA_PIA_BITS(&m_stream_port->info),
                                0, 0, &m_rec_file_port);

        status = pjmedia_snd_port_connect(m_snd_port_for_record, m_rec_file_port);
        qDebug() << "pjmedia_snd_port_connect m_snd_port_for_record m_rec_file_port status: " << status;

        m_converter->startConvert(m_filename.data());
    }
    else
    {
        if(m_rec_file_port)
        {
            status = pjmedia_port_destroy(m_rec_file_port);
            m_rec_file_port = NULL;
        }
        if (m_snd_port)
        {
            status = pjmedia_snd_port_destroy(m_snd_port);
            m_snd_port = NULL;
        }
        if (m_snd_port_for_record)
        {
            pjmedia_snd_port_destroy(m_snd_port_for_record);
            m_snd_port_for_record = NULL;
        }
    }

    m_dir = dir;
    return true;
}

bool PttMediaStream::init()
{
    qDebug() << "PttMediaStream init";
    unsigned clockRate;
    pj_caching_pool *cp;
//    pjmedia_conf *conf;
    pj_status_t status;

    clockRate = PttMediaManager::instance()->m_clockRate;
    cp = &PttMediaManager::instance()->m_cp;
//    conf = PttMediaManager::instance()->m_conf;

    status = createTransport();
    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    m_pool = pj_pool_create(&cp->factory, "strm", 4000, 4000, NULL);

    status = createStream();
    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    status = pjmedia_stream_get_port(m_stream, &m_stream_port);
    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

//    pjmedia_conf_add_port(conf, m_pool, m_stream_port, NULL, &m_streamSlot);
//    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    pjmedia_stream_start(m_stream);

    generateFilename();

    qDebug() << "PttMediaStream init end";

    return true;
}

void PttMediaStream::shutdown()
{
//    pjmedia_conf *conf;

//    conf = PttMediaManager::instance()->m_conf;
    pj_status_t status;

    if (m_stream)
    {
//        status = pjmedia_conf_remove_port(conf, m_streamSlot);
//        qDebug() << "pjmedia_conf_remove_port(conf, m_streamSlot) status :" << status;

        status = pjmedia_stream_destroy(m_stream);
        qDebug() << "pjmedia_stream_destroy(m_stream) status :" << status;
        m_stream = NULL;
    }

    if (m_transport)
    {
        status = pjmedia_transport_close(m_transport);
        qDebug() << "pjmedia_transport_close(m_transport) status :" << status;
        m_transport = NULL;
    }

    if (m_snd_port)
    {
        status = pjmedia_snd_port_disconnect(m_snd_port);
        qDebug() << "pjmedia_snd_port_disconnect(m_snd_port) status :" << status;

        status = pjmedia_snd_port_destroy(m_snd_port);
        qDebug() << "pjmedia_snd_port_destroy(m_snd_port) status :" << status;
        m_snd_port = NULL;
    }

    if (m_snd_port_for_record)
    {
        status = pjmedia_snd_port_disconnect(m_snd_port_for_record);
        qDebug() << "pjmedia_snd_port_disconnect(m_snd_port) status :" << status;

        status = pjmedia_snd_port_destroy(m_snd_port_for_record);
        qDebug() << "pjmedia_snd_port_destroy(m_snd_port_for_record) status :" << status;
        m_snd_port_for_record = NULL;
    }

    if (m_stream_port)
    {
        status = pjmedia_port_destroy(m_stream_port);
        qDebug() << "pjmedia_port_destroy(m_stream_port) status :" << status;
        m_stream_port = NULL;
    }

    if (m_rec_file_port)
    {
        status = pjmedia_port_destroy(m_rec_file_port);
        qDebug() << "pjmedia_port_destroy(m_rec_file_port) status :" << status;
        m_rec_file_port = NULL;
    }

    if (m_pool)
    {
        pj_pool_release(m_pool);
        m_pool = NULL;
    }

    usleep(200000);

    qDebug() << "PttMediaStream::shutdown";
}

pj_status_t PttMediaStream::createTransport()
{
    pjmedia_endpt *medEndpt;
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *addr;
    pjmedia_sock_info si;
    pj_status_t status;
    const int af = pj_AF_INET();
    struct ip_mreq mreq;
    struct in_addr if_addr;
    u_char loop = 0;

    medEndpt = PttMediaManager::instance()->m_medEndpt;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
#ifdef voipAdHocService
    strcpy(ifr.ifr_name, "lmi45");
#else
    strcpy(ifr.ifr_name, "wlan0");
#endif
    status = ioctl(sockfd, SIOCGIFADDR, &ifr);
    close(sockfd);
    if (status != PJ_SUCCESS)
        goto on_error;

    addr = (struct sockaddr_in *)&ifr.ifr_addr;
    mreq.imr_multiaddr.s_addr = inet_addr(m_groupAddr.c_str());
    mreq.imr_interface.s_addr = addr->sin_addr.s_addr;
    if_addr.s_addr = addr->sin_addr.s_addr;

    pj_bzero(&si, sizeof(pjmedia_sock_info));
    si.rtp_sock = si.rtcp_sock = PJ_INVALID_SOCKET;

    status = pj_sock_socket(af, pj_SOCK_DGRAM(), 0, &si.rtp_sock);
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sockaddr_init(af, &si.rtp_addr_name, NULL, m_port);
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_bind(si.rtp_sock, &si.rtp_addr_name,
                          pj_sockaddr_get_len(&si.rtp_addr_name));
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_setsockopt(si.rtp_sock, pj_SOL_IP(), pj_IP_ADD_MEMBERSHIP(),
                                &mreq, sizeof(mreq));
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_setsockopt(si.rtp_sock, pj_SOL_IP(), pj_IP_MULTICAST_IF(),
                                &if_addr, sizeof(if_addr));
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_setsockopt(si.rtp_sock, pj_SOL_IP(), pj_IP_MULTICAST_LOOP(),
                                &loop, sizeof(loop));
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_socket(af, pj_SOCK_DGRAM(), 0, &si.rtcp_sock);
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sockaddr_init(af, &si.rtcp_addr_name, NULL, m_port + 1);
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_bind(si.rtcp_sock, &si.rtcp_addr_name,
                          pj_sockaddr_get_len(&si.rtcp_addr_name));
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_setsockopt(si.rtcp_sock, pj_SOL_IP(), pj_IP_ADD_MEMBERSHIP(),
                                &mreq, sizeof(mreq));
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_setsockopt(si.rtcp_sock, pj_SOL_IP(), pj_IP_MULTICAST_IF(),
                                &if_addr, sizeof(if_addr));
    if (status != PJ_SUCCESS)
        goto on_error;

    status = pj_sock_setsockopt(si.rtcp_sock, pj_SOL_IP(), pj_IP_MULTICAST_LOOP(),
                                &loop, sizeof(loop));
    if (status != PJ_SUCCESS)
        goto on_error;

    return pjmedia_transport_udp_attach(medEndpt, NULL, &si,
                                        PJMEDIA_UDP_NO_SRC_ADDR_CHECKING,
                                        &m_transport);

on_error:
    if (si.rtp_sock != PJ_INVALID_SOCKET)
        pj_sock_close(si.rtp_sock);
    if (si.rtcp_sock != PJ_INVALID_SOCKET)
        pj_sock_close(si.rtcp_sock);
    qDebug() << "on_error: " << status;
    return status;
}

pj_status_t PttMediaStream::createStream()
{
    pjmedia_endpt *medEndpt;
    const pjmedia_codec_info *codecInfo;
    pjmedia_stream_info info;
    pj_str_t addr;
    pj_sockaddr_in rem_addr;
    pj_status_t status;

    medEndpt = PttMediaManager::instance()->m_medEndpt;
    codecInfo = PttMediaManager::instance()->m_codecInfo;

    pj_bzero(&info, sizeof(info));

    info.type = PJMEDIA_TYPE_AUDIO;
    info.dir = PJMEDIA_DIR_ENCODING_DECODING;
    pj_memcpy(&info.fmt, codecInfo, sizeof(pjmedia_codec_info));
    info.tx_pt = codecInfo->pt;
    info.rx_pt = codecInfo->pt;
    info.ssrc = pj_rand();

    info.jb_init = -1;
    info.jb_min_pre = -1;
    info.jb_max_pre = -1;
    info.rtcp_sdes_bye_disabled = true;
    info.proto = PJMEDIA_TP_PROTO_RTP_SAVP;

    addr = pj_str((char *)m_groupAddr.c_str());
    pj_bzero(&rem_addr, sizeof(rem_addr));
    status = pj_sockaddr_in_init(&rem_addr, &addr, m_port);
    if (status != PJ_SUCCESS)
        return status;

    pj_memcpy(&info.rem_addr, &rem_addr, sizeof(pj_sockaddr_in));

    if (info.rem_addr.addr.sa_family == 0)
    {
        const pj_str_t addr = pj_str((char *)"127.0.0.1");
        pj_sockaddr_in_init(&info.rem_addr.ipv4, &addr, 0);
    }

    return pjmedia_stream_create(medEndpt, m_pool, &info, m_transport,
                                 NULL, &m_stream);
}

void PttMediaStream::generateFilename()
{
    struct timespec ts;
    struct tm *tp;
    char s[18];

    clock_gettime(CLOCK_REALTIME, &ts);
    tp = localtime(&ts.tv_sec);
    strftime(s, sizeof(s), "%Y%m%d_%H%M%S", tp);
    m_filename = PttMediaManager::instance()->m_recordDir + s;
    m_filename = m_filename + ".wav";
}

void PttMediaStream::onConvertFinished(const QString &fileName)
{
    unsigned groupId = reinterpret_cast<unsigned>(this);
    qDebug() << "onConvertFinished: " << groupId << fileName;
    emit  signalConvertFinished(groupId, fileName);
}

PttMediaManager *PttMediaManager::m_instance = NULL;

PttMediaManager *PttMediaManager::instance()
{
    if(m_instance == NULL)
        m_instance = 0;/*new PttMediaManager();*/

    return m_instance;
}

PttMediaManager::PttMediaManager() :
    m_clockRate(8000),
    m_recordDir("/home/user/ptt/"),
    m_medEndpt(NULL),
    m_pool(NULL),
    m_conf(NULL),
    m_codecInfo(NULL)
{
    mkdir(m_recordDir.c_str(), 0755);

    init();
}

PttMediaManager::~PttMediaManager()
{
    shutdown();
}

PttMediaStream *PttMediaManager::createStream(const char *groupAddr, unsigned short port)
{
    PttMediaStream *stream = new PttMediaStream(groupAddr, port);
    QObject::connect(stream, SIGNAL(signalConvertFinished(uint,QString)),
                     this, SIGNAL(signalConvertFinished(uint,QString)));
    m_streams.push_back(stream);

    return stream;
}

bool PttMediaManager::destoryStream(PttMediaStream *stream)
{
    bool result = false;

    for (list<PttMediaStream *>::iterator it = m_streams.begin();
         it != m_streams.end(); ++it)
    {
        if (*it == stream)
        {
            m_streams.erase(it);
            delete stream;
            result = true;
            break;
        }
    }

    return result;
}

bool PttMediaManager::hasStream(PttMediaStream *stream)
{
    bool result = false;

    for (list<PttMediaStream *>::iterator it = m_streams.begin();
         it != m_streams.end(); ++it)
    {
        if (*it == stream)
        {
            result = true;
            break;
        }
    }

    return result;
}

unsigned PttMediaManager::getGroupId(const QString &groupAddr, unsigned short port)
{
    unsigned groupId = 0;

    for (list<PttMediaStream *>::iterator it = m_streams.begin();
         it != m_streams.end(); ++it)
    {
        PttMediaStream *stream = *it;
        if (stream->getGroupAddr() == groupAddr.toStdString() && stream->getGroupPort() == port)
        {
            groupId = reinterpret_cast<unsigned>(stream);
            break;
        }
    }

    return groupId;
}

bool PttMediaManager::init()
{
    pj_status_t status;
    pjmedia_codec_mgr *codec_mgr;
    pj_str_t codec_id;
    unsigned count = 1;

    status = pj_init();
    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    pj_caching_pool_init(&m_cp, &pj_pool_factory_default_policy, 0);

    status = pjmedia_endpt_create(&m_cp.factory, NULL, 1, &m_medEndpt);
    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    m_pool = pj_pool_create(&m_cp.factory, "mgr", 4000, 4000, NULL);

    status = pjmedia_conf_create(m_pool, 21,
                                 m_clockRate, 1, (m_clockRate * 20 / 1000), 16,
                                 PJMEDIA_CONF_USE_LINEAR | PJMEDIA_CONF_NO_MIC | PJMEDIA_CONF_NO_DEVICE, &m_conf);

    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    pj_log_set_level(5);

    pjmedia_audio_codec_config codec_config;
    pjmedia_audio_codec_config_default(&codec_config);
    pjmedia_format media_format;
    pjmedia_format_init_audio(&media_format, PJMEDIA_FORMAT_ILBC, m_clockRate, 1, (5950 * 60 / 1000), 60000, 3000, 5950);
    codec_config.passthrough.setting.fmts = &media_format;

    qDebug() << "codec_config" << codec_config.speex.quality << codec_config.ilbc.mode
             << "clock_rate: " << codec_config.passthrough.setting.fmts->det.aud.clock_rate
             << "channel_count: " << codec_config.passthrough.setting.fmts->det.aud.channel_count
             << "frame_time_usec: " << codec_config.passthrough.setting.fmts->det.aud.frame_time_usec
             << "bits_per_sample: " << codec_config.passthrough.setting.fmts->det.aud.bits_per_sample
             << "avg_bps: " << codec_config.passthrough.setting.fmts->det.aud.avg_bps
             << "max_bps: " << codec_config.passthrough.setting.fmts->det.aud.max_bps;
    codec_config.speex.quality = 0;
    status = pjmedia_codec_register_audio_codecs(m_medEndpt, &codec_config);
    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    codec_mgr = pjmedia_endpt_get_codec_mgr(m_medEndpt);
    codec_id = pj_str((char *)"speex/8000");//iLBC
    status = pjmedia_codec_mgr_find_codecs_by_id(codec_mgr, &codec_id, &count,
                                                 &m_codecInfo, NULL);
    qDebug() << "m_codecInfo" << m_codecInfo->encoding_name.ptr << m_codecInfo->clock_rate;
    PJ_ASSERT_RETURN(status == PJ_SUCCESS, false);

    /*
    unsigned codec_count = -1;
    pjmedia_codec_info info[255];

    status = pjmedia_codec_mgr_enum_codecs(codec_mgr, &codec_count, info, NULL);

    qDebug() << "pjmedia_codec_mgr_enum_codecs" << status << codec_count;
    for(int i=0; i<codec_count; i++)
    {
        qDebug() << "pjmedia_codec_mgr_enum_codecs" << info[i].encoding_name.ptr << info[i].clock_rate;
    }
    */

    return true;
}

void PttMediaManager::shutdown()
{
    /*
    if (m_conf)
    {
        pjmedia_conf_destroy(m_conf);
        m_conf = NULL;
    }
    */

    if (m_pool)
    {
        pj_pool_release(m_pool);
        m_pool = NULL;
    }

    if (m_medEndpt)
    {
        pjmedia_endpt_destroy(m_medEndpt);
        m_medEndpt = NULL;
    }

    pj_caching_pool_destroy(&m_cp);

    pj_shutdown();
}
