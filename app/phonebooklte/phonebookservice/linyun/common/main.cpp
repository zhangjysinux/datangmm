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

//授权检查并更新函数
bool CheckAndUpdateAuth();

//主功能函数，各个模块实现不同
void FunctionFunc(const string &cap_key,const string &data_path,const string &test_data_path);

/*入口函数
    调用说明：
    账号信息读取模块，SYS初始化建议程序启动时执行，SYS反初始化建议在程序退出是执行
    这些模块不支持多线程调用，开发者在使用过程中应该注意避免并发
*/

int main(int argc,char *argv[])
{
    //获取AccountInfo单例
	AccountInfo *account_info = AccountInfo::GetInstance();

    //账号信息读取
	string account_info_file;
#ifdef _WIN32_WCE
	//wince 下取绝对路径，可根据实际情况修改。
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

    //SYS初始化
    HCI_ERR_CODE err_code = HCI_ERR_NONE;
    //配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
    string init_config = "";
    init_config += "appKey=" + account_info->app_key();              //灵云应用序号
    init_config += ",developerKey=" + account_info->developer_key(); //灵云开发者密钥
    init_config += ",cloudUrl=" + account_info->cloud_url();         //灵云云服务的接口地址
    init_config += ",authpath=" + account_info->auth_path();         //授权文件所在路径，保证可写
    init_config += ",logfilepath=" + account_info->logfile_path();   //日志的路径
    init_config += ",loglevel=5";								     //日志的等级
	init_config += ",logfilesize=512";								 //日志文件的大小
    //其他配置使用默认值，不再添加，如果想设置可以参考开发手册
	err_code = hci_init( init_config.c_str() );
    if( err_code != HCI_ERR_NONE )
    {
        printf( "hci_init return (%d:%s)\n", err_code, hci_get_error_info(err_code) );
        getchar();
        return -1;
    }
    printf( "hci_init success\n" );

    //检测授权，如果过期则更新授权
    if (!CheckAndUpdateAuth())
    {
        printf("CheckAndUpdateAuth failed\n");
        hci_release();
        getchar();
        return -1;
    }

    //设置当前用户,此处可以进行用户关联操作，即多个设备可以通过该接口建立请求数据映射
    //hci_set_current_userid("userid");

    //功能函数
    FunctionFunc(account_info->cap_key(),account_info->data_path(),account_info->test_data_path());

    //SYS反初始化
    err_code = hci_release();
    printf("hci_release\n");

    //退出
    printf("Press Enter to Exit");
    getchar();

    return 0;
}


bool CheckAndUpdateAuth()
{
    //获取过期时间
    int64 expire_time;
    int64 current_time = (int64)time( NULL );
    HCI_ERR_CODE err_code = hci_get_auth_expire_time( &expire_time );
    if( err_code == HCI_ERR_NONE )
    {
        //获取成功则判断是否过期
        if( expire_time > current_time )
        {
            //没有过期
            printf( "auth can use continue\n" );
            return true;
        }
    }

    //获取过期时间失败或已经过期
    //手动调用更新授权
	//HCI_ERR_CODE err_code;
    err_code = hci_check_auth();
    if( err_code == HCI_ERR_NONE )
    {
        //更新成功
        printf( "check auth success \n" );
        return true;
    }
    else
    {
        //更新失败
        printf( "check auth return (%d:%s)\n", err_code ,hci_get_error_info(err_code));
        return false;
    }
}