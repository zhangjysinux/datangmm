#ifndef SQLFUNCTION_H
#define SQLFUNCTION_H
#include <stdio.h>
#include <string.h>
#include <QString>
#include <QMultiMap>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QTextCodec>
#include <QLibrary>
#include <QSqlQuery>
#include "sqlite3.h"

class SqlFunction
{
public:
    SqlFunction();
    static void install(QSqlQuery &query);
    static QStringList getSqlExecResult(const QString &sql);
    static bool doSql(const QString &sql);
private:
    static QLibrary *m_sqliteLib;
public:
    static QLibrary *getLib();
};
static QFunctionPointer getFunc(const QString &funcName);
static QString getNumberLetter(const int &number);
static QStringList getNumbersLetter(const QString &numbers);//many zuhe
static int chinese_cmp(void *NotUsed, int nKey1, const void *pKey1, int nKey2, const void *pKey2);
static void phoneNumberLetterCmp(sqlite3_context * ctx, int argc, sqlite3_value ** argv);
static void getStringsection(sqlite3_context * ctx, int argc, sqlite3_value ** argv);
static void getNetworkNumber(sqlite3_context * ctx, int argc, sqlite3_value ** argv);
static void getNetworkExistNumber(sqlite3_context * ctx, int argc, sqlite3_value ** argv);//get the network type and the number is online
static void getStringIndexOf(sqlite3_context * ctx, int argc, sqlite3_value ** argv);
static void getNotNoneStringIn(sqlite3_context * ctx, int argc, sqlite3_value ** argv);
#endif // SQLFUNCTION_H
