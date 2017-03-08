#ifndef CHINESE_H
#define CHINESE_H

#include <QString>
#include <QMultiMap>

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
