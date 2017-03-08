#include "ttssynth.h"
#include <QDebug>
#include <string.h>

TtsSynth::TtsSynth(QObject *parent) :
    QObject(parent)
{
}


//主功能函数该函数中除识别模块外其他模块不支持多线程调用，开发者在使用过程中应该注意避免并发
void TtsSynth::FunctionFunc(const std::string &cap_key, const std::string &data_path, const std::string &test_data_path)
{
    HCI_ERR_CODE err_code = HCI_ERR_NONE;

    //TTS 初始化
    string tts_init_config = "dataPath=";
    tts_init_config += data_path;
    tts_init_config += ",initCapkeys=";
    tts_init_config += cap_key;
    err_code = hci_tts_init(tts_init_config.c_str());
    if (err_code != HCI_ERR_NONE)
    {
        qDebug() << "hci_tts_init return" << err_code << hci_get_error_info(err_code);
        return;
    }

    //语音合成
    string txt_file = test_data_path + "/tts.txt";
    string out_pcm_file = test_data_path + "/synthResult.pcm";
    TTSSynth(cap_key, txt_file, out_pcm_file);

    //TTS反初始化
    hci_tts_release();
    qDebug() << "hci_tts_release";
    return;
}

void TtsSynth::FunctionPlayer(const std::string &cap_key, const std::string &data_path, const std::string &test_data_path)
{

}


//合成回调函数，通过此函数将合成结果保存到文件
bool TtsSynth::TtsSynthCallbackFunction(void *pvUserParam, TTS_SYNTH_RESULT *psTtsSynthResult, HCI_ERR_CODE hciErrCode)
{
    qDebug() << "TtsSynthCallbackFunction";
    if( hciErrCode != HCI_ERR_NONE )
    {
        return false;
    }

    qDebug() << "voice data size" << psTtsSynthResult->uiVoiceSize;
    // 将合成结果写入文件
    if (psTtsSynthResult->pvVoiceData != NULL)
    {
        FILE * fp = (FILE *)pvUserParam;
        fwrite(psTtsSynthResult->pvVoiceData, psTtsSynthResult->uiVoiceSize, 1, fp);
    }

    //mark 回调结果
    if (psTtsSynthResult->nMarkCount > 0)
    {
        for (int i=0; i<psTtsSynthResult->nMarkCount; ++i)
        {
            qDebug() << "MarkName:%s, with the time in audio:" << psTtsSynthResult->pMark[i].pszName << psTtsSynthResult->pMark[i].time;
        }

    }

    // 此回调函数返回false会中止合成，返回true表示继续合成
    return true;
}

void TtsSynth::TTSSynth(const std::string &cap_key, const std::string &txt_file, const std::string &out_pcm_file)
{
    qDebug() << "TTSSynth xxxx";
    // 合成文本读取
    HciExampleComon::FileReader txt_data;
    if( txt_data.Load(txt_file.c_str(),1) == false )
    {
        qDebug() << "Open input text file %s error!" << txt_file.c_str();
        return;
    }

    // 打开输出文件
    FILE * fp = fopen( out_pcm_file.c_str(), "wb" );
    if( fp == NULL )
    {
        qDebug() << "Create output pcm file %s error!" << out_pcm_file.c_str();
        return;
    }

    HCI_ERR_CODE err_code = HCI_ERR_NONE;
    // 启动 TTS Session
    string session_config = "capkey=";
    session_config += cap_key;
    int session_id = -1;

    qDebug() << "hci_tts_session_start config" << session_config.c_str();
    err_code = hci_tts_session_start( session_config.c_str(), &session_id );
    if( err_code != HCI_ERR_NONE )
    {
        qDebug() << "hci_tts_session_start return" << err_code << hci_get_error_info(err_code);
        fclose(fp);
        return;
    }
    qDebug() << "hci_tts_session_start success";

    string synth_config;
    if (cap_key.find("tts.cloud.synth") != string::npos)
    {
        //property 属于 私有云 云端能力 必填参数，具体请参考开发手册
        //none: 所有标记将会被视为文本读出，缺省值
        synth_config = "property=cn_xiaokun_common,tagmode=none";
    }

    //待合成文本需要以UTF-8进行编码并以'\0'结尾,文本文件为UTF-8格式，此处略过UTF-8 BOM
    char * synth_text = (char*)txt_data.buff_ + 3;
//    bool (HCIAPI * Callback_TtsSynth)(
//                _OPT_ _IN_ void * pvUserParam,
//                _MUST_ _IN_ TTS_SYNTH_RESULT * psTtsSynthResult,
//                _MUST_ _IN_ HCI_ERR_CODE  hciErrCode
//                );

    err_code = hci_tts_synth( session_id, synth_text, synth_config.c_str(), &TtsSynth::TtsSynthCallbackFunction, fp );
    fclose(fp);

    if( err_code != HCI_ERR_NONE )
    {
        qDebug() << "hci_tts_session_start return " << err_code << hci_get_error_info(err_code);
    }

    // 终止 TTS Session
    err_code = hci_tts_session_stop( session_id );
    if( err_code != HCI_ERR_NONE )
    {
        qDebug() << "hci_tts_session_stop return", err_code;
        return;
    }
    qDebug() << "hci_tts_session_stop success";

    return;
}
