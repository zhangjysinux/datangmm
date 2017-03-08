#ifndef TTSSYNTH_H
#define TTSSYNTH_H

#include <QObject>
#include "hci_tts.h"
#include "common/FileReader.h"
#include "common/CommonTool.h"
#include <stdio.h>
using std::string;

class TtsSynth : public QObject
{
    Q_OBJECT
public:
    explicit TtsSynth(QObject *parent = 0);
    //合成回调函数，通过此函数将合成结果保存到文件
    static bool HCIAPI TtsSynthCallbackFunction(_OPT_ _IN_ void * pvUserParam,
                                         _MUST_ _IN_ TTS_SYNTH_RESULT * psTtsSynthResult,
                                         _MUST_ _IN_ HCI_ERR_CODE  hciErrCode);
    //合成函数
    void TTSSynth(const string &cap_key, const string &txt_file, const string &out_pcm_file );

    void FunctionFunc(const string &cap_key,const string &data_path,const string &test_data_path);

    void FunctionPlayer(const string &cap_key,const string &data_path,const string &test_data_path);

signals:

public slots:


};

#endif // TTSSYNTH_H
