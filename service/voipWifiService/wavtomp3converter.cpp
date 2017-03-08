#include "wavtomp3converter.h"
#include <lame/lame.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <QDebug>

#define PCMBUFSIZE	2048
#define MP3BUFSIZE	9760

WavToMp3Converter::WavToMp3Converter(const char *filename) :
    m_filename(filename)
{
}

void WavToMp3Converter::convert()
{
    pthread_attr_t attr;
    pthread_t thread;
    int ret;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&thread, NULL, WavToMp3Converter::convert, this);
    pthread_attr_destroy(&attr);

    if (ret != 0)
        delete this;
}

void *WavToMp3Converter::convert(void *arg)
{
    WavToMp3Converter *converter;
    struct stat buf;
    int ret;
    lame_global_flags *gfp;
    short pcmbuf[PCMBUFSIZE];
    unsigned char mp3buf[MP3BUFSIZE];
    int nsamples;
    int nbytes;
    FILE *fpcm;
    FILE *fmp3;

    converter = static_cast<WavToMp3Converter *>(arg);

    string wavFile = converter->m_filename + ".wav";
    string mp3File = converter->m_filename + ".mp3";

    ret = stat(wavFile.c_str(), &buf);
    if (ret == 0 && buf.st_size > 44)
    {
        gfp = lame_init();

        lame_set_num_channels(gfp, 1);
        lame_set_in_samplerate(gfp, 16000);
        lame_set_brate(gfp, 32);
        lame_set_mode(gfp, MONO);
        lame_set_quality(gfp, 2);

        lame_init_params(gfp);

        fpcm = fopen(wavFile.c_str(), "rb");
        fmp3 = fopen(mp3File.c_str(), "wb");

        fseek(fpcm, 44, SEEK_SET);

        do
        {
            nsamples = fread(pcmbuf, sizeof(short), PCMBUFSIZE, fpcm);

            if (nsamples > 0)
            {
                nbytes = lame_encode_buffer(gfp, pcmbuf, NULL, nsamples, mp3buf, MP3BUFSIZE);
            }
            else if (nsamples == 0)
            {
                nbytes = lame_encode_flush(gfp, mp3buf, MP3BUFSIZE);
            }

            fwrite(mp3buf, 1, nbytes, fmp3);
        } while (nsamples > 0);

        lame_close(gfp);

        fclose(fmp3);
        fclose(fpcm);

        unlink(wavFile.c_str());
    }

    delete converter;

    return (void *)0;
}

WavToMp3RealTimeConverter *WavToMp3RealTimeConverter::instance = NULL;

WavToMp3RealTimeConverter *WavToMp3RealTimeConverter::getInstance()
{
    if(instance == NULL)
        instance = new WavToMp3RealTimeConverter();

    return instance;
}

WavToMp3RealTimeConverter::WavToMp3RealTimeConverter()
{

}

void WavToMp3RealTimeConverter::startConvert(const QString& fileName)
{
    QMutexLocker locker(&m_mutex);

    qDebug() << "WavToMp3RealTimeConverter startConvert";

    pthread_attr_t attr;
    pthread_t thread;
    int ret;

    m_quit = false;
    QString fileName2 = fileName;
    m_fileName = fileName2.remove(".wav").toStdString();
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&thread, NULL, WavToMp3RealTimeConverter::convert, this);
    pthread_attr_destroy(&attr);

    if (ret != 0)
        qDebug() << "WavToMp3RealTimeConverter pthread_create error";
}

void WavToMp3RealTimeConverter::stopConvert()
{
    qDebug() << "WavToMp3RealTimeConverter stopConvert";
    m_quit = true;
}

void *WavToMp3RealTimeConverter::convert(void *arg)
{
    WavToMp3RealTimeConverter *converter = static_cast<WavToMp3RealTimeConverter *>(arg);
    QMutexLocker locker(&converter->m_mutex);

    struct stat buf;
    int ret;
    lame_global_flags *gfp;
    short pcmbuf[PCMBUFSIZE];
    unsigned char mp3buf[MP3BUFSIZE];
    int nsamples;
    int nbytes;
    FILE *fpcm;
    FILE *fmp3;

    string wavFile = converter->m_fileName + ".wav";
    string mp3File = converter->m_fileName + ".mp3";

    while(1)
    {
        ret = stat(wavFile.c_str(), &buf);

        if (ret == 0 && buf.st_size > 44)
        {
            gfp = lame_init();

            lame_set_num_channels(gfp, 1);
            lame_set_in_samplerate(gfp, 8000);
            lame_set_brate(gfp, 32);
            lame_set_mode(gfp, MONO);
            lame_set_quality(gfp, 2);

            lame_init_params(gfp);

            fpcm = fopen(wavFile.c_str(), "rb");
            fmp3 = fopen(mp3File.c_str(), "wb");
            qDebug() << "fopen: " << fpcm << fmp3;

            fseek(fpcm, 44, SEEK_SET);

            while(1)
            {
                nsamples = fread(pcmbuf, sizeof(short), PCMBUFSIZE, fpcm);

                if (nsamples > 0)
                {
                    nbytes = lame_encode_buffer(gfp, pcmbuf, NULL, nsamples, mp3buf, MP3BUFSIZE);
                    fwrite(mp3buf, 1, nbytes, fmp3);
                }
                else/* if (nsamples == 0)*/
                {
                    usleep(10000);
                }

                if(converter->m_quit && feof(fpcm))
                    break;
            }
            //flush it to the file
            nbytes = lame_encode_flush(gfp, mp3buf, MP3BUFSIZE);

            lame_close(gfp);

            fclose(fmp3);
            fclose(fpcm);

            unlink(wavFile.c_str());

            emit converter->signalConvertFinished(mp3File.data());

            break;
        }
        else
        {
            qDebug() << "stat ret: " << ret << "buf size: " << buf.st_size;
            usleep(200000);

            if(converter->m_quit)
                break;
        }
    }

    return (void *)0;
}

