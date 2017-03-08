#include "datacreator.h"
#include <QStringList>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "common.h"


DataCreator::DataCreator(QObject *parent) :
    QObject(parent)
{
    openDatabase();
}

DataCreator::~DataCreator()
{
    closeDatabase();
}

QString DataCreator::createSurname()
{
    QStringList surnameList;

    surnameList << "欧阳" << "太史" << "端木" << "上官" << "司马" << "东方" << "独孤"
                << "南宫" << "万俟" << "闻人" << "夏侯" << "诸葛" << "尉迟" << "赫连"
                << "澹台" << "皇甫" << "宗政" << "濮阳" << "公冶" << "太叔" << "申屠"
                << "公孙" << "慕容" << "仲孙" << "钟离" << "长孙" << "宇文" << "司徒"
                << "鲜于" << "司空" << "闾丘" << "子车" << "亓官" << "司寇" << "巫马"
                << "公西" << "颛孙" << "壤驷" << "公良" << "漆雕" << "乐正" << "宰父"
                << "谷梁" << "拓跋" << "夹谷" << "轩辕" << "令狐" << "段干" << "百里"
                << "呼延" << "东郭" << "南门" << "羊舌" << "微生" << "公户" << "公玉"
                << "公仪" << "梁丘" << "公仲" << "公上" << "公门" << "公山" << "公坚"
                << "左丘" << "公伯" << "西门" << "公祖" << "第五" << "公乘" << "贯丘"
                << "公皙" << "南荣" << "东里" << "东宫" << "仲长" << "子书" << "子桑"
                << "即墨" << "达奚" << "褚师" << "吴铭";

    return surnameList.at(qrand() % surnameList.length());
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
        return "";
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
