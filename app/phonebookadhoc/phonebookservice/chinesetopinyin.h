#ifndef CHINESE_H
#define CHINESE_H
#include <stdio.h>
#include <string.h>
#include <QString>
#include <QMultiMap>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QTextCodec>
using namespace std;

class ChineseToPinyin
{
public:
    static ChineseToPinyin *getInstance();
    ~ChineseToPinyin();
private:
    ChineseToPinyin();
private:
    static ChineseToPinyin *instance;
    QMultiMap<QString,QString> m_pinyin_map;
public:
   static QString getPinyinFirstLetter(const QString& str);
   QString getPinyin(const QString& str);
};

#endif // CHINESE_H
