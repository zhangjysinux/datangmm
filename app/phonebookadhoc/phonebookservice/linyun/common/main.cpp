#include <stdio.h>
#include "AccountInfo.h"
#include "hci_sys.h"
#include <time.h>

#ifdef _WIN32_WCE
#include <windows.h>
#define main _tmain
time_t time( time_t *inTT ) {
	SYSTEMTIME sysTimeStruct;
	FILETIME fTime;
	ULARGE_INTEGER int64time;
	time_t locTT = 0;

	if ( inTT == NULL ) {
		inTT = &locTT;
	}

	GetSystemTime( &sysTimeStruct );
	if ( SystemTimeToFileTime( &sysTimeStruct, &fTime ) ) {
		memcpy( &int64time, &fTime, sizeof( FILETIME ) );
		/* Subtract the value for 1970-01-01 00:00 (UTC) */
		int64time.QuadPart -= 0x19db1ded53e8000;
		/* Convert to seconds. */
		int64time.QuadPart /= 10000000;
		*inTT = int64time.QuadPart;
	}

	return *inTT;
}
#endif

//��Ȩ��鲢���º���
bool CheckAndUpdateAuth();

//�����ܺ���������ģ��ʵ�ֲ�ͬ
void FunctionFunc(const string &cap_key,const string &data_path,const string &test_data_path);

/*��ں���
    ����˵����
    �˺���Ϣ��ȡģ�飬SYS��ʼ�������������ʱִ�У�SYS����ʼ�������ڳ����˳���ִ��
    ��Щģ�鲻֧�ֶ��̵߳��ã���������ʹ�ù�����Ӧ��ע����Ⲣ��
*/

int main(int argc,char *argv[])
{
    //��ȡAccountInfo����
	AccountInfo *account_info = AccountInfo::GetInstance();

    //�˺���Ϣ��ȡ
	string account_info_file;
#ifdef _WIN32_WCE
	//wince ��ȡ����·�����ɸ���ʵ������޸ġ�
	account_info_file = "/SDMMC/testdata/AccountInfo.txt";
#else
	account_info_file = "../../testdata/AccountInfo.txt";
#endif
    bool account_success = account_info->LoadFromFile(account_info_file);
    //bool account_success = account_info->LoadFromCode();
    if (!account_success)
    {
        printf( "AccountInfo read from %s failed\n", account_info_file.c_str());
        getchar();
        return -1;
    }
    printf( "AccountInfo Read success\n" );

    //SYS��ʼ��
    HCI_ERR_CODE err_code = HCI_ERR_NONE;
    //���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
    string init_config = "";
    init_config += "appKey=" + account_info->app_key();              //����Ӧ�����
    init_config += ",developerKey=" + account_info->developer_key(); //���ƿ�������Կ
    init_config += ",cloudUrl=" + account_info->cloud_url();         //�����Ʒ���Ľӿڵ�ַ
    init_config += ",authpath=" + account_info->auth_path();         //��Ȩ�ļ�����·������֤��д
    init_config += ",logfilepath=" + account_info->logfile_path();   //��־��·��
    init_config += ",loglevel=5";								     //��־�ĵȼ�
	init_config += ",logfilesize=512";								 //��־�ļ��Ĵ�С
    //��������ʹ��Ĭ��ֵ��������ӣ���������ÿ��Բο������ֲ�
	err_code = hci_init( init_config.c_str() );
    if( err_code != HCI_ERR_NONE )
    {
        printf( "hci_init return (%d:%s)\n", err_code, hci_get_error_info(err_code) );
        getchar();
        return -1;
    }
    printf( "hci_init success\n" );

    //�����Ȩ����������������Ȩ
    if (!CheckAndUpdateAuth())
    {
        printf("CheckAndUpdateAuth failed\n");
        hci_release();
        getchar();
        return -1;
    }

    //���õ�ǰ�û�,�˴����Խ����û�����������������豸����ͨ���ýӿڽ�����������ӳ��
    //hci_set_current_userid("userid");

    //���ܺ���
    FunctionFunc(account_info->cap_key(),account_info->data_path(),account_info->test_data_path());

    //SYS����ʼ��
    err_code = hci_release();
    printf("hci_release\n");

    //�˳�
    printf("Press Enter to Exit");
    getchar();

    return 0;
}


bool CheckAndUpdateAuth()
{
    //��ȡ����ʱ��
    int64 expire_time;
    int64 current_time = (int64)time( NULL );
    HCI_ERR_CODE err_code = hci_get_auth_expire_time( &expire_time );
    if( err_code == HCI_ERR_NONE )
    {
        //��ȡ�ɹ����ж��Ƿ����
        if( expire_time > current_time )
        {
            //û�й���
            printf( "auth can use continue\n" );
            return true;
        }
    }

    //��ȡ����ʱ��ʧ�ܻ��Ѿ�����
    //�ֶ����ø�����Ȩ
	//HCI_ERR_CODE err_code;
    err_code = hci_check_auth();
    if( err_code == HCI_ERR_NONE )
    {
        //���³ɹ�
        printf( "check auth success \n" );
        return true;
    }
    else
    {
        //����ʧ��
        printf( "check auth return (%d:%s)\n", err_code ,hci_get_error_info(err_code));
        return false;
    }
}