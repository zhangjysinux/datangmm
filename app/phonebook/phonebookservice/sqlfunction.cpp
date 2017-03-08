#include "sqlfunction.h"
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>
#include "contacterinterface.h"
#include "connectiondatabase.h"

typedef SQLITE_API int SQLITE_STDCALL (*func_sqlite3_value_type)(sqlite3_value*);
typedef SQLITE_API const unsigned char *SQLITE_STDCALL (*func_sqlite3_value_text)(sqlite3_value*);
typedef SQLITE_API void SQLITE_STDCALL (*func_sqlite3_result_text)(sqlite3_context*, const char*, int, void(*)(void*));
typedef SQLITE_API void SQLITE_STDCALL (*func_sqlite3_result_int)(sqlite3_context*, int);
typedef int (*func_sqlite3_create_function)(
  sqlite3 *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);

static func_sqlite3_value_type f_sqlite3_value_type = (func_sqlite3_value_type)SqlFunction::getLib()->resolve("sqlite3_value_type");
static func_sqlite3_value_text f_sqlite3_value_text = (func_sqlite3_value_text)SqlFunction::getLib()->resolve("sqlite3_value_text");
static func_sqlite3_result_int f_sqlite3_result_int = (func_sqlite3_result_int)SqlFunction::getLib()->resolve("sqlite3_result_int");
static func_sqlite3_result_text f_sqlite3_result_text = (func_sqlite3_result_text)SqlFunction::getLib()->resolve("sqlite3_result_text");
static func_sqlite3_create_function f_sqlite3_create_function = (func_sqlite3_create_function)SqlFunction::getLib()->resolve("sqlite3_create_function");

QLibrary *SqlFunction::m_sqliteLib = NULL;

QLibrary *SqlFunction::getLib()
{
    if(m_sqliteLib == NULL)
    {
        m_sqliteLib = new QLibrary("libsqlite3.so.0");
        m_sqliteLib->load();
        if (!m_sqliteLib->isLoaded())
        {
            qDebug() << "load libhello.so failed!";
        }
    }
    return m_sqliteLib;
}

QFunctionPointer getFunc(const QString &funcName)
{
    QLibrary *lib = SqlFunction::getLib();
    QFunctionPointer func = lib->resolve(funcName.toStdString().data());
    return func;
}

int chinese_cmp(void *NotUsed, int nKey1, const void *pKey1, int nKey2, const void *pKey2)
{
    QString str1 = (char*)pKey1;
    QString str2 = (char*)pKey2;

    return 0;
}

void phoneNumberLetterCmp(sqlite3_context * ctx, int argc, sqlite3_value ** argv)
{
//    func_sqlite3_value_type f_sqlite3_value_type = (func_sqlite3_value_type)SqlFunction::getLib()->resolve("sqlite3_value_type");
//    func_sqlite3_value_text f_sqlite3_value_text = (func_sqlite3_value_text)SqlFunction::getLib()->resolve("sqlite3_value_text");
//    func_sqlite3_result_int f_sqlite3_result_int = (func_sqlite3_result_int)SqlFunction::getLib()->resolve("sqlite3_result_int");
    if ( argc != 2 )
        return;
//    if( f_sqlite3_value_type( argv[ 0 ] ) != SQLITE_TEXT)
//        return;
//    if( f_sqlite3_value_type( argv[ 1 ] ) != SQLITE_TEXT)
//        return;
    int index = -1;
    QString name = (const char * )f_sqlite3_value_text( argv[ 0 ] );
    if(name.isEmpty())
    {
        f_sqlite3_result_int(ctx, index);
        return;
    }
    QString numbers = (const char * )f_sqlite3_value_text( argv[ 1 ] );
    numbers.remove("0");
    numbers.remove("1");
    numbers.remove("#");
    numbers.remove("*");
    QStringList zuheList = getNumbersLetter(numbers);
//    qDebug() << "zuheList" << zuheList;
    foreach(const QString &zuhe, zuheList)
    {
        index = name.indexOf(zuhe,0,Qt::CaseInsensitive);
        if(index >= 0)
        {
            break;
        }
    }
    if(index == -1)
    {
        index = 10000;
    }
    f_sqlite3_result_int(ctx, index);
}

void getStringsection(sqlite3_context * ctx, int argc, sqlite3_value ** argv)
{
//    func_sqlite3_value_type f_sqlite3_value_type = (func_sqlite3_value_type)SqlFunction::getLib()->resolve("sqlite3_value_type");
//    func_sqlite3_value_text f_sqlite3_value_text = (func_sqlite3_value_text)SqlFunction::getLib()->resolve("sqlite3_value_text");
//    func_sqlite3_result_text f_sqlite3_result_text = (func_sqlite3_result_text)SqlFunction::getLib()->resolve("sqlite3_result_text");
    if ( argc != 4 )
        return;
    QString arg1 = (const char * )f_sqlite3_value_text( argv[ 0 ] );
    QString arg2 = (const char * )f_sqlite3_value_text( argv[ 1 ] );
    QString arg3 = (const char * )f_sqlite3_value_text( argv[ 2 ] );
    QString arg4 = (const char * )f_sqlite3_value_text( argv[ 3 ] );
    QString result = arg1.section(arg2, arg3.toUInt(), arg4.toUInt());
    f_sqlite3_result_text(ctx, result.toStdString().data(), result.count(), SQLITE_TRANSIENT);
}

void getNetworkNumber(sqlite3_context * ctx, int argc, sqlite3_value ** argv)
{
//    func_sqlite3_value_type f_sqlite3_value_type = (func_sqlite3_value_type)SqlFunction::getLib()->resolve("sqlite3_value_type");
//    func_sqlite3_value_text f_sqlite3_value_text = (func_sqlite3_value_text)SqlFunction::getLib()->resolve("sqlite3_value_text");
//    func_sqlite3_result_text f_sqlite3_result_text = (func_sqlite3_result_text)SqlFunction::getLib()->resolve("sqlite3_result_text");
    if ( argc != 1 )
        return;
    QString numbersString = (const char * )f_sqlite3_value_text( argv[ 0 ] );
//    result.remove("ad_hoc_network");
//    result.remove("lte_network");
//    result.remove("satellite_network");
//    result.remove("digital_walkie_talkie_network");
//    result.remove("battle_network");
//    result.remove("adhoc_lte_network");
//    result.remove("unknow_network");

    PhoneNumbers numbers = ContacterInterface::converNumbersToStruct(numbersString);
    QString result;
    foreach(const PhoneNumber &phoneNumber, numbers)
        result.append(phoneNumber.number).append(",");

    std::string retString = result.toStdString();
    int size = retString.size();

    f_sqlite3_result_text(ctx, retString.data(), size, SQLITE_TRANSIENT);
}

static void getNetworkExistNumber(sqlite3_context * ctx, int argc, sqlite3_value ** argv)
{
    if ( argc != 2 )
        return;
    QString numbersString = (const char * )f_sqlite3_value_text( argv[ 0 ] );
    QString netTypeString = (const char * )f_sqlite3_value_text( argv[ 1 ] );
    int netType = netTypeString.toInt();
    int retInt = -1;//network is exist and online(just for some lte and adhoc)
    PhoneNumbers numbers = ContacterInterface::converNumbersToStruct(numbersString);
    if(netType == -1)
    {
        retInt = 0;
    }
    else if(netType == AD_HOC_NETWORK ||
            netType == LTE_NETWORK)
    {
        foreach(const PhoneNumber &phoneNumber, numbers)
        {
            if(phoneNumber.netType == netType && phoneNumber.online == 1)
            {
                retInt = 0;
                break;
            }
        }
    }
    else if(netType == ADHOC_LTE_NETWORK)//contains one of them
    {
        foreach(const PhoneNumber &phoneNumber, numbers)
        {
            if((phoneNumber.netType == AD_HOC_NETWORK || phoneNumber.netType == LTE_NETWORK) &&
                    phoneNumber.online == 1)
            {
                retInt = 0;
                break;
            }
        }
    }
    else
    {
        foreach(const PhoneNumber &phoneNumber, numbers)
        {
            if(phoneNumber.netType == netType)
            {
                retInt = 0;
                break;
            }
        }
    }


    f_sqlite3_result_int(ctx, retInt);
}

static void getStringIndexOf(sqlite3_context * ctx, int argc, sqlite3_value ** argv)
{
//    func_sqlite3_value_type f_sqlite3_value_type = (func_sqlite3_value_type)SqlFunction::getLib()->resolve("sqlite3_value_type");
//    func_sqlite3_value_text f_sqlite3_value_text = (func_sqlite3_value_text)SqlFunction::getLib()->resolve("sqlite3_value_text");
//    func_sqlite3_result_int f_sqlite3_result_int = (func_sqlite3_result_int)SqlFunction::getLib()->resolve("sqlite3_result_int");
    if ( argc != 2 )
        return;
    QString str = (const char * )f_sqlite3_value_text( argv[ 0 ] );
    QString otherString = (const char * )f_sqlite3_value_text( argv[ 1 ] );
    if(!str.trimmed().isEmpty() && !otherString.trimmed().isEmpty())
    {
        int index = str.indexOf(otherString,0,Qt::CaseInsensitive);
        if(index == -1)
            index = 10000;
        f_sqlite3_result_int(ctx, index);
    }
    else
    {
        f_sqlite3_result_int(ctx, 10000);
    }
}

void getNotNoneStringIn(sqlite3_context * ctx, int argc, sqlite3_value ** argv)
{
    if ( argc != 2 )
        return;
    QString str = (const char * )f_sqlite3_value_text( argv[ 0 ] );
    QString otherString = (const char * )f_sqlite3_value_text( argv[ 1 ] );
    if(!str.isEmpty())
        f_sqlite3_result_text(ctx, str.toStdString().data(), str.count(), SQLITE_TRANSIENT);
    else
        f_sqlite3_result_text(ctx, otherString.toStdString().data(), otherString.count(), SQLITE_TRANSIENT);
}

SqlFunction::SqlFunction()
{
}

void SqlFunction::install(QSqlQuery &query)
{
//    func_sqlite3_create_function f_sqlite3_create_function = (func_sqlite3_create_function)getLib()->resolve("sqlite3_create_function");
    QVariant v = query.driver()->handle();
    if (v.isValid() && qstrcmp(v.typeName(), "sqlite3*")==0) {
        // v.data() returns a pointer to the handle
        sqlite3 *handle = *static_cast<sqlite3 **>(v.data());
        if (handle != 0) { // check that it is not NULL
            f_sqlite3_create_function( handle, "PNL", 2, SQLITE_ANY, 0, phoneNumberLetterCmp, 0, 0 );
            f_sqlite3_create_function( handle, "SECTION", 4, SQLITE_ANY, 0, getStringsection, 0, 0 );
            f_sqlite3_create_function( handle, "NETWORKNUMBER", 1, SQLITE_ANY, 0, getNetworkNumber, 0, 0 );
            f_sqlite3_create_function( handle, "NETWORKEXISTANDONLINE", 2, SQLITE_ANY, 0, getNetworkExistNumber, 0, 0 );
            f_sqlite3_create_function( handle, "INDEXOF", 2, SQLITE_ANY, 0, getStringIndexOf, 0, 0 );
            f_sqlite3_create_function( handle, "NotNoneStringOf", 2, SQLITE_ANY, 0, getNotNoneStringIn, 0, 0);
        }
    }
}

QStringList SqlFunction::getSqlExecResult(const QString &sql)
{
    QStringList result;
    QSqlQuery query(ConnectionDatabase::getGblDb());
    if(query.exec(sql))
    {
        while(query.next()){
            result.append(query.value(0).toString());
        }
    }
    if(query.lastError().type() != QSqlError::NoError)
    {
        qDebug() << "query : " << query.lastError().text() << sql << ConnectionDatabase::getGblDb().tables();
    }

    return result;
}

bool SqlFunction::doSql(const QString &sql)
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    if(query.exec(sql))
    {
        qDebug() << "do sql success...";
        return true;
    }
    else
    {
        qDebug() << "do sql error : " << sql;
        return false;
    }
}

QString getNumberLetter(const int &number)
{
    QString charactors;
    switch (number) {
    case 2:
        charactors = "abc";
        break;
    case 3:
        charactors = "def";
        break;
    case 4:
        charactors = "ghi";
        break;
    case 5:
        charactors = "jkl";
        break;
    case 6:
        charactors = "mno";
        break;
    case 7:
        charactors = "pqrs";
        break;
    case 8:
        charactors = "tuv";
        break;
    case 9:
        charactors = "wxyz";
        break;
    default:
        break;
    }
    return charactors;
}

QStringList getNumbersLetter(const QString &numbers)
{
    QString num1Letter = getNumberLetter(numbers.mid(0,1).toInt());
    QString num2Letter = getNumberLetter(numbers.mid(1,1).toInt());
    QString num3Letter = getNumberLetter(numbers.mid(2,1).toInt());
//    qDebug() << "num1Letter" << num1Letter;
//    qDebug() << "num2Letter" << num2Letter;
//    qDebug() << "num3Letter" << num3Letter;
    QStringList zuheList;
    for(int i=0; i<num1Letter.count(); i++)
    {
        QString let1 = num1Letter.mid(i, 1);
        if(!num2Letter.isEmpty())
        {
            for(int j=0; j<num2Letter.count(); j++)
            {
                QString let2 = num2Letter.mid(j, 1);
                if(!num3Letter.isEmpty())
                {
                    for(int k=0; k<num3Letter.count(); k++)
                    {
                        QString let3 = num3Letter.mid(k, 1);
                        zuheList.append(let1+let2+let3);
                    }
                }
                else
                {
                    zuheList.append(let1+let2);
                }

            }
        }
        else
        {
            zuheList.append(let1);
        }
    }
    return zuheList;
}
