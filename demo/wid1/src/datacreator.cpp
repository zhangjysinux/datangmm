#include "datacreator.h"
#include <QStringList>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <time.h>
#include "common.h"


DataCreator::DataCreator(QObject *parent) :
    QObject(parent)
{
    srand(time(0));
}

DataCreator::~DataCreator()
{
}

QString DataCreator::createSurname()
{
    QString surname =
            "赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨朱秦尤许"
            "何吕施张孔曹严华金魏陶姜戚谢邹喻柏水窦章"
            "云苏潘葛奚范彭郎鲁韦昌马苗凤化花方俞任袁"
            "柳酆鲍史唐费廉岑薛雷贺倪汤滕殷罗毕郝邬安"
            "常乐于时傅皮卞齐康伍余元卜顾孟平黄和穆萧"
            "尹姚邵湛汪祁毛禹狄米贝明臧计伏成戴谈宋茅"
            "庞熊纪舒屈项祝董梁杜阮蓝闵席季麻强贾路娄"
            "危江童颜郭梅盛林刁钟徐邱骆高夏蔡田樊胡凌"
            "霍虞万支柯昝管卢莫经房裘缪干解应宗丁宣贲"
            "邓郁单杭洪包诸左石崔吉钮龚程嵇邢滑裴陆荣"
            "翁荀羊於惠甄曲家封芮羿储靳汲邴糜松井段富"
            "牧隗山谷车侯宓蓬全郗班仰秋仲伊宫宁仇栾暴"
            "甘钭厉戎祖武符刘景詹束龙闻莘党翟谭贡劳逄";

    return surname.at(qrand() % 260);
}

QString DataCreator::createName()
{
    QStringList nameList;

    nameList << "卿相" << "啸天" << "不败" << "雷动" << "惊羽" << "尚志" << "谨"  << "风累"
             << "筱柔" << "彩蝶" << "水烟" << "诺"   << "绝"  << "龙儿" << "文昭" << "楠琨"
             << "见晓" << "远春" << "如意" << "圆圆" << "婉儿" << "东凌" << "解放" << "东风"
             << "大锤" << "琪琪" << "君怡" << "绯羽" << "絮沫" << "柒柒" << "琳璇" << "一户"
             << "喜助" << "重国" << "右健" << "云龙" << "子龙" << "云长" << "意德" << "玄德"
             << "奉先" << "卧龙" << "孟德" << "澤東" << "恩来" << "小平" << "少奇" << "波比"
             << "中正" << "中山" << "大照" << "布什" << "庆"   << "金莲" << "如花" << "星星"
             << "飞龙" << "继永" << "继承" << "大举" << "狗屎" << "太君" << "八路" << "春秋"
             << "春" << "夏" << "秋" << "冬" << "梅" << "兰" << "竹" << "菊" << "八戒"
             << "悟空" << "无能" << "风" << "雨" << "雷" << "电";

    return nameList.at(qrand() % nameList.length());
}

QString DataCreator::createDeviceNumber()
{
    const int DEVICE_NUMBER_LENGTH = 19;
    QString num = "abcdefghijklmnopqrstuvwxyz";
    QString deviceNum;

    for (int i = 0; i < DEVICE_NUMBER_LENGTH; i++)
    {
        if ((i + 1) % 5 == 0)
        {
            deviceNum.append("-");
        }
        else
        {
            deviceNum.append(num.at(qrand() % num.size()));
        }
    }

    return deviceNum;
}

QString DataCreator::getTitleStringFromTitleType(int titleType)
{
    switch (titleType)
    {
    case BATTLION:
        return "营长";
    case COMPANY:
        return "连长";
    case PLATOON:
        return "排长";
    case SQUAD:
        return "班长";
    case SOLIDER:
        return "战士";
    default:
        return "将军";
    }
}

QString DataCreator::createGroupName(int index)
{
    QStringList groupList;

    groupList << "武当派" << "崆峒派" << "峨嵋派" << "华山派" << "衡山派" << "少林派" << "明教"
              << "丐帮" << "逍遥派" << "昆仑派" << "青城派" << "星星帮" << "洪星帮"
              << "东兴帮" << "三合会" << "山口组" << "黑手党" << "流沙河"
              << "高老庄" << "花果山";

   if (index > groupList.size())
   {
       index = groupList.size();
   }

   return groupList.at(index);
}

bool DataCreator::openDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("contact.db");
    if (!m_db.open())
    {
        qDebug() << "Cannot open contact database";
        return false;
    }
    return true;
}

void DataCreator::closeDatabase()
{
    m_db.close();
}
