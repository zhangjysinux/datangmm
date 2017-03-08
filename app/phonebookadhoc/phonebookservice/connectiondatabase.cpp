#include "connectiondatabase.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QUuid>
#include <QNetworkInterface>
#include <QTextCodec>
#include <QCoreApplication>
#include "chinesetopinyin.h"
#include "../../commondata/datatype/contacterdata.h"
#include "sqlfunction.h"
#include "../../commondata/datatype/commondata.h"
#include "contacterinterface.h"


extern QSqlDatabase db;

QTimer *ConnectionDatabase::m_timer = new QTimer();

ConnectionDatabase::ConnectionDatabase(QObject *parent) :
    QObject(parent)
{

}

QStringList ConnectionDatabase::getIp()
{
    QStringList addressList;
    foreach (QHostAddress address, QNetworkInterface::allAddresses())
        addressList.append(address.toString());
    return addressList;
}

bool ConnectionDatabase::closeDb()
{
    if(db.isOpen())
        db.close();

    if(db.isOpen())
        qDebug() << "db close success.........";
    else
        qDebug() << "db close fail.......";
}

bool ConnectionDatabase::openDb()
{
    if(!db.isOpen())
    {
        bool openFlag = db.open();
        qDebug() << "open db flag: " << openFlag;
    }
    else
    {
        qDebug() << "db is arealy open";
    }
}
QSqlDatabase& ConnectionDatabase::getGblDb()
{
    return db;
}

QString ConnectionDatabase::getPersonSurname(int index)
{
    QString info;
    info =  "赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨朱秦尤许"
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
    QList<int> indexs;
    indexs << 65 << 114 << 246 << 68 << 52 << 154 << 94 << 117 << 122 << 201
           << 229 << 95 << 204 << 88 << 15 << 241 << 18 << 118 << 8 << 5 << 84
           << 11 << 78 << 177 << 163 << 10 << 99 << 258 << 126 << 186 << 36
           << 107 << 232 << 202 << 19 << 17 << 72 << 34 << 13 << 49 << 203 << 80
           << 176 << 224 << 30 << 259 << 6 << 112 << 91 << 233 << 172 << 29
           << 188 << 169 << 171 << 42 << 23 << 43 << 116 << 33 << 230 << 61
           << 182 << 133 << 228 << 247 << 160 << 179 << 53 << 208 << 127 << 173
           << 219 << 151 << 244 << 198 << 44 << 185 << 111 << 216 << 3 << 194
           << 213 << 240 << 189 << 4 << 98 << 190 << 239 << 150 << 236 << 226
           << 97 << 149 << 7 << 102 << 73 << 22 << 183 << 110 << 120 << 232
           << 202 << 252 << 213 << 111 << 247 << 251 << 79 << 87 << 256 << 125
           << 41 << 69 << 95 << 121 << 254 << 197 << 158 << 160 << 90 << 38
           << 259 << 177 << 138 << 127 << 86 << 122 << 222 << 16 << 114
           << 169 << 97 << 67 << 183 << 210 << 30 << 13 << 74 << 249 << 133
           << 34 << 70 << 5 << 43 << 145 << 32 << 174 << 168 << 172 << 245
           << 10 << 91 << 235 << 167 << 62 << 187 << 7 << 80 << 59 << 14 << 240
           << 203 << 49 << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
           << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98
           << 129 << 26 << 253 << 257 << 53 << 140 << 83 << 176 << 3 << 204
           << 255 << 198 << 211 << 237 << 20 << 223 << 29 << 200  << 203 << 49
           << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
           << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98
           << 129 << 26 << 253 << 257 << 53 << 140 << 83 << 176 << 3 << 204
           << 255 << 198 << 211 << 237 << 20 << 223 << 29 << 200;
    return info.at(indexs[index]);
}

QString ConnectionDatabase::getPersonName(int index)
{
    QString name;
    QString info =
            "艾杰言柔尉蓉元雄进棉亚奇彦花滕龙牡蝶锐萧景萧"
            "建卿秋菲中贵兼迪书棋庆洪关卿喻欢金霏旭红同原"
            "庆惠加赋茹妍腾麟仙盈尚清兴江婕盈牡妍文权富赋"
            "苇仪蓝艳安驰继贵总在花落的叹息中觉醒总在晚霞"
            "的羞涩中离去季节蹁跶轻拂的风摇曳了一片心房又"
            "一季瑟秋再一次完美信手拈来一片落叶展现不一样"
            "的平凡回眸望去一片片纷飞的落叶弥却不禁觉察那"
            "悄然落下的一抹美徘徊阡陌的我凝视着铺满落叶的"
            "小径一片宁静弥漫心头轻轻飘飘的一阵阵香掠过鼻"
            "尖用手轻触心灵的跳动一丝心魂忍不住轻触顿时一"
            "丝丝美丽充溢心田我总是肆无忌惮地梦见空中飞舞"
            "的落叶我呼唤自己从梦中醒来勤勉以承白驹不返珍"
            "重以存每个人走过的这一路落叶放飞的是美丽而我"
            "们则需放飞有灵魂的梦想或许站在雪山之巅远望征"
            "途当月光披在大地上它悄然落下后那抹不平凡美丽";

    QList<int> firstIndexs;
    firstIndexs << 107 << 232 << 202 << 19 << 17 << 72 << 34 << 13 << 49 << 203 << 80
                << 176 << 224 << 30 << 259 << 6 << 112 << 91 << 233 << 172 << 29
                << 188 << 169 << 171 << 42 << 23 << 43 << 116 << 33 << 230 << 61
                << 182 << 133 << 228 << 247 << 160 << 179 << 53 << 208 << 127 << 173
                << 219 << 151 << 244 << 198 << 44 << 185 << 111 << 216 << 3 << 194
                << 213 << 240 << 189 << 4 << 98 << 190 << 239 << 150 << 236 << 226
                << 97 << 149 << 7 << 102 << 73 << 22 << 183 << 110 << 120 << 232
                << 202 << 252 << 213 << 111 << 247 << 251 << 79 << 87 << 256 << 125
                << 41 << 69 << 95 << 121 << 254 << 197 << 158 << 160 << 90 << 38
                << 259 << 177 << 138 << 127 << 86 << 122 << 222 << 16 << 114
                << 169 << 97 << 67 << 183 << 210 << 30 << 13 << 74 << 249 << 133
                << 34 << 70 << 5 << 43 << 145 << 32 << 174 << 168 << 172 << 245
                << 10 << 91 << 235 << 167 << 62 << 187 << 7 << 80 << 59 << 14 << 240
                << 203 << 49 << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
                << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98
                << 129 << 26 << 253 << 257 << 53 << 140 << 83 << 176 << 3 << 204
                << 255 << 198 << 211 << 237 << 20 << 223 << 29 << 200  << 203 << 49
                << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
                << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98
                << 129 << 26 << 253 << 257 << 53 << 140 << 83 << 176 << 3 << 204
                << 255 << 198 << 211 << 237 << 20 << 223 << 29 << 200
                << 229 << 95 << 204 << 88 << 15 << 241 << 18 << 118 << 8 << 5 << 84
                << 11 << 78 << 177 << 163 << 10 << 99 << 258 << 126 << 186 << 36
                << 107 << 232 << 202 << 19 << 17 << 72 << 34 << 13 << 49 << 203 << 80
                << 176 << 224 << 30 << 259 << 6 << 112 << 91 << 233 << 172 << 29
                << 188 << 169 << 171 << 42 << 23 << 43 << 116 << 33 << 230 << 61
                << 182 << 133 << 228 << 247 << 160 << 179 << 53 << 208 << 127 << 173
                << 219 << 151 << 244 << 198 << 44 << 185 << 111 << 216 << 3 << 194
                << 213 << 240 << 189 << 4 << 98 << 190 << 239 << 150 << 236 << 226
                << 97 << 149 << 7 << 102 << 73 << 22 << 183 << 110 << 120 << 232
                << 202 << 252 << 213 << 111 << 247 << 251 << 79 << 87 << 256 << 125
                << 11 << 78 << 177 << 163 << 10 << 99 << 258 << 126 << 186 << 36
                << 107 << 232 << 202 << 19 << 17 << 72 << 34 << 13 << 49 << 203 << 80
                << 176 << 224 << 30 << 259 << 6 << 112 << 91 << 233 << 172 << 29
                << 188 << 169 << 171 << 42 << 23 << 43 << 116 << 33 << 230 << 61
                << 182 << 133 << 228 << 247 << 160 << 179 << 53 << 208 << 127 << 173;
    QList<int> secondIndexs;
    secondIndexs << 41 << 69 << 95 << 121 << 254 << 197 << 158 << 160 << 90 << 38
                 << 259 << 177 << 138 << 127 << 86 << 122 << 222 << 16 << 114
                 << 169 << 97 << 67 << 183 << 210 << 30 << 13 << 74 << 249 << 133
                 << 34 << 70 << 5 << 43 << 145 << 32 << 174 << 168 << 172 << 245
                 << 10 << 91 << 235 << 167 << 62 << 187 << 7 << 80 << 59 << 14 << 240
                 << 203 << 49 << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
                 << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98
                 << 129 << 26 << 253 << 257 << 53 << 140 << 83 << 176 << 3 << 204
                 << 255 << 198 << 211 << 237 << 20 << 223 << 29 << 200  << 203 << 49
                 << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
                 << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98
                 << 129 << 26 << 253 << 257 << 53 << 140 << 83 << 176 << 3 << 204
                 << 255 << 198 << 211 << 237 << 20 << 223 << 29 << 200
                 << 182 << 133 << 228 << 247 << 160 << 179 << 53 << 208 << 127 << 173
                 << 219 << 151 << 244 << 198 << 44 << 185 << 111 << 216 << 3 << 194
                 << 213 << 240 << 189 << 4 << 98 << 190 << 239 << 150 << 236 << 226
                 << 97 << 149 << 7 << 102 << 73 << 22 << 183 << 110 << 120 << 232
                 << 202 << 252 << 213 << 111 << 247 << 251 << 79 << 87 << 256 << 125
                 << 169 << 97 << 67 << 183 << 210 << 30 << 13 << 74 << 249 << 133
                 << 34 << 70 << 5 << 43 << 145 << 32 << 174 << 168 << 172 << 245
                 << 10 << 91 << 235 << 167 << 62 << 187 << 7 << 80 << 59 << 14 << 240
                 << 203 << 49 << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
                 << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98
                 << 129 << 26 << 253 << 257 << 53 << 140 << 83 << 176 << 3 << 204
                 << 255 << 198 << 211 << 237 << 20 << 223 << 29 << 200  << 203 << 49
                 << 166 << 22 << 231 << 206 << 199 << 234 << 244 << 216
                 << 192 << 17 << 11 << 248 << 154 << 250 << 101 << 165 << 71 << 98;

    name.append(info[firstIndexs[index]]);
    name.append(info[secondIndexs[index]]);
    return name;
}

QString ConnectionDatabase::getDeviceNum()
{
    QString num = "abcdefghijklmnopqrstuvwxyz123456789";
    QString devicenum;
    for (int i = 0; i < 19; i++) {
        if ((i+1)%5 == 0) {
            devicenum.append("-");
        } else {
            devicenum.append(num.at(qrand()%35));
        }
    }
    return devicenum;

}

static QString local_id;
QString ConnectionDatabase::creatLocalPhoneInfo()
{
    QString localId;
    QSqlQuery query(ConnectionDatabase::getGblDb());
    query.prepare("INSERT INTO local (id, surname, name, title, network, photopath, leaders, snnumber, "
                  "bluetooth, namePinyin, type, selectedStatus, pocid) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    QStringList addressList = getIp();
    ContacterInterface *contacterInterface = ContacterInterface::getInstance();
    QStringList contacterIdList = contacterInterface->onGetContacters(3);
    for(int i = 0, iend = contacterIdList.count(); i < iend; i++)
    {
        QString id = contacterIdList[i];
        Contacter contacter = contacterInterface->getContacter(id);
        PhoneNumbers numbers = contacter.numbers;
        for(int j = 0, jend = numbers.count(); j < jend; j++)
        {
            PhoneNumber number = numbers[j];
            if(addressList.contains(number.number))
            {
                //find local
                localId = id;
                query.addBindValue(localId);
                query.addBindValue(contacter.surname);
                query.addBindValue(contacter.name);
                query.addBindValue(contacter.title);
                query.addBindValue(ContacterInterface::
                                   converNumbersToString(contacter.numbers));
                query.addBindValue("null");
                query.addBindValue("[]");
                query.addBindValue(contacter.snNumber);
                query.addBindValue(contacter.bluetooth);
                query.addBindValue(contacter.namePinyin);
                query.addBindValue(contacter.type);
                query.addBindValue(0);
                query.addBindValue(contacter.pocId);
                bool local_exec = query.exec();
                qDebug() << "local_exec:" << local_exec;
                break;
            }
        }
    }
    return localId;
}

bool ConnectionDatabase::creatContacterInfo(int num)
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    query.prepare("INSERT INTO contacter (id, surname, name, title, network, photopath,"
                  " leaders, snnumber, bluetooth, namePinyin, type, selectedStatus, pocid) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    //----------------------------id------------------------------------------//
    query.addBindValue(QString("%1").arg(num));
    //------------------------------------------------------------------------//

    //----------------------------surname-------------------------------------//
    QString surname = getPersonSurname(num);
    query.addBindValue(surname);
    //------------------------------------------------------------------------//

    //----------------------------name----------------------------------------//
    QString name = getPersonName(num);
    query.addBindValue(name);
    //------------------------------------------------------------------------//

    //----------------------------title---------------------------------------//
    QString title = "1";
    query.addBindValue(title);
    //------------------------------------------------------------------------//

    //----------------------------phone number--------------------------------//
    PhoneNumbers numbers;

//    //wifi init
//    PhoneNumber wifiNumber;
//    wifiNumber.netType = WIFI_NETWORK;
//    wifiNumber.number = "192.168.3." + QString::number(num);
//    wifiNumber.priority = 0;
//    wifiNumber.online = 1;
//    numbers.append(wifiNumber);

//    //lte init
//    PhoneNumber lteNumber;
//    lteNumber.netType = LTE_NETWORK;
//    lteNumber.number = "10.32.100." + QString::number(num);
//    lteNumber.priority = 1;
//    lteNumber.online = 1;
//    numbers.append(lteNumber);

//    //baotong init
//    PhoneNumber digitalNumber;
//    digitalNumber.netType = DIGITAL_WALKIE_TALKIE_NETWORK;
//    digitalNumber.number =  "19200844";
//    if(num < 10)
//        digitalNumber.number = digitalNumber.number + "00";
//    else if(num < 100)
//        digitalNumber.number = digitalNumber.number + "0";
//    digitalNumber.number = digitalNumber.number + QString::number(num);
//    digitalNumber.priority = 2;
//    digitalNumber.online = 1;
//    numbers.append(digitalNumber);

    //tiantong init
    PhoneNumber satelliteNumber;
    satelliteNumber.netType = SATELLITE_NETWORK;
    satelliteNumber.number = "18933456";
    if(num < 10)
        satelliteNumber.number = satelliteNumber.number + "00";
    else if(num < 100)
        satelliteNumber.number = satelliteNumber.number + "0";
    satelliteNumber.number = satelliteNumber.number + QString::number(num);
    satelliteNumber.priority = 3;
    satelliteNumber.online = 1;
    numbers.append(satelliteNumber);

    //adhoc init
    PhoneNumber adhocNumber;
    adhocNumber.netType = AD_HOC_NETWORK;
    adhocNumber.number = "192.168.67." + QString::number(num);
    adhocNumber.priority = 4;
    adhocNumber.online = 1;
    numbers.append(adhocNumber);

//    //battle init
//    PhoneNumber battleNumber;
//    battleNumber.netType = BATTLE_NETWORK;
//    battleNumber.number = "192.168.89." + QString::number(num);
//    battleNumber.priority = 5;
//    battleNumber.online = 1;
//    numbers.append(battleNumber);

    query.addBindValue(ContacterInterface::converNumbersToString(numbers));
    //------------------------------------------------------------------------//

    //--------------------------photopath-------------------------------------//
    query.addBindValue("null");
    //------------------------------------------------------------------------//

    //--------------------------leaders---------------------------------------//
    query.addBindValue("[]");
    //------------------------------------------------------------------------//

    //--------------------------snnumber--------------------------------------//
    QString snnumber = getDeviceNum();
    query.addBindValue(snnumber);
    //------------------------------------------------------------------------//

    //--------------------------bluetooth-------------------------------------//
    QString bluetooth = "C8:0F:10:6B:8B:";
    QString bluetoothoffset = QString::number(num, 16);
    bluetoothoffset.remove("0x");
    bluetooth = bluetooth  + bluetoothoffset;
    query.addBindValue(bluetooth);
    //------------------------------------------------------------------------//

    //------------------------pinyin------------------------------------------//
    query.addBindValue(ChineseToPinyin::getPinyinFirstLetter(surname+name) + " "
                       + ChineseToPinyin::getInstance()->getPinyin(surname+name) + " "
                       + surname+name + " "
                       + getTitleStringFromEnum(title.toInt()));
    //------------------------------------------------------------------------//

    //-----------------------contacter type-----------------------------------//
    query.addBindValue("1");
    //------------------------------------------------------------------------//

    //-----------------------selectstatus-------------------------------------//
    query.addBindValue("0");
    //------------------------------------------------------------------------//

    //-----------------------pocid--------------------------------------------//
    query.addBindValue("");
    //------------------------------------------------------------------------//
    query.exec();
    return true;
}

bool ConnectionDatabase::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "phonebookconnection");
    db.setDatabaseName("./contact.db");
    if (!db.open())
    {
        qDebug() << "Cannot open contact database" << db.lastError().text();
        return false;
    }

    db.exec("PRAGMA locking_mode = EXCLUSIVE");
    QSqlQuery query(db);
    SqlFunction::install(query);

    if(!db.tables().contains("contacter"))
    {
        query.exec("create table contacter (id varchar(50) primary key, "
                   "surname varchar(20), name varchar(20), title varchar(20), network varchar(200), "
                   "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
                   "bluetooth varchar(50), namePinyin varchar(40), type int, selectedStatus int, "
                   "pocid varchar(20))");
        //-------------------------contacter init---------------------------------//
        for(int i = 1; i<= 200; i++)
            creatContacterInfo(i);

        loadSettingContacterInfo();
        //-----------------------------------------------------------------------//
    }
    else
    {
        query.prepare("delete from contacter");
        query.exec();
        //-------------------------contacter init---------------------------------//
        for(int i = 1; i<= 200; i++)
            creatContacterInfo(i);

        loadSettingContacterInfo();
        //-----------------------------------------------------------------------//
    }

    if(!db.tables().contains("local"))
    {
        query.exec("create table local (id varchar(50) primary key, "
                   "surname varchar(20), name varchar(20), title varchar(20), network varchar(200), "
                   "photopath varchar(50), leaders varchar(50), snnumber varchar(50), "
                   "bluetooth varchar(50), namePinyin varchar(40), type int, selectedStatus int, "
                   "pocid varchar(20))");

        //-------------------------local init------------------------------------//
        creatLocalPhoneInfo();
        //-----------------------------------------------------------------------//
    }
    else
    {
        query.prepare("delete from local");
        query.exec();
        //-------------------------local init------------------------------------//
        creatLocalPhoneInfo();
        //-----------------------------------------------------------------------//
    }

    if(!db.tables().contains("groups"))
    {
        query.exec("create table groups (id varchar(20) primary key, "
                   "name varchar(20), iconpath varchar(50), creator varchar(20), members varchar(200), "
                   "leaders varchar(50), address varchar(50), port int, network varchar(20), "
                   "type varchar(20), parent varchar(20), org int)");

        //-------------------------group init------------------------------------//
        loadSettingGroupInfo();
        //createConversionGroup();
        //-----------------------------------------------------------------------//
    }
    else
    {
        query.prepare("delete from groups");
        query.exec();

        loadSettingGroupInfo();
        createConversionGroup();
    }

    if(!db.tables().contains("callhistory"))
    {
        query.exec("create table callhistory (id varchar(50) primary key, "
                   "dialTarget varchar(200), dialType varchar(20), dialTime int, dialNetworkType varchar(200), "
                   "dateStart varchar(50), dateEnd varchar(50),"
                   "contactId1 varchar(50), contactId2 varchar(50), contactId3 varchar(50),"
                   "contactId4 varchar(50), contactId5 varchar(50), contactId6 varchar(50),"
                   "number1 varchar(50), number2 varchar(50), number3 varchar(50),"
                   "number4 varchar(50), number5 varchar(50), number6 varchar(50))");
    }

    connect(m_timer, &QTimer::timeout, [](){
        ConnectionDatabase::onSignalTimeOut();
    });
    m_timer->start(5000);

    return true;
}

const QLatin1String ContacterID("ID");
const QLatin1String ContacterSurname("SURNAME");
const QLatin1String ContacterName("NAME");
const QLatin1String ContacterTitle("TITLE");
const QLatin1String ContacterWIFI("WIFI");
const QLatin1String ContacterLTE("LTE");
const QLatin1String ContacterBaoTong("BAOTONG");
const QLatin1String ContacterTianTong("TIANTONG");
const QLatin1String ContacterADHOC("ADHOC");
const QLatin1String ContacterBattle("BATTLE");
const QLatin1String ContacterBLUETOOTH("BLUETOOTH");
const QLatin1String ContacterLTEPocID("LTEPOCID");

bool ConnectionDatabase::loadSettingContacterInfo()
{
    QString resPath = "/data/apps/com.sinux.phonebookadhoc/res/contacter_phonebook.ini";

//    QDir currentPath = QDir::currentPath();
//    currentPath.cdUp();
    QString contacterIniPath = resPath;//.path() + "/contacter_phonebook.ini";
    qDebug() << "contacter ini path:" << contacterIniPath;
    QSettings settings(contacterIniPath, QSettings::IniFormat);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    settings.setIniCodec(codec);
    QStringList childGroups = settings.childGroups();
    for(int i = 0, iend = childGroups.count(); i < iend; i++)
    {
        settings.beginGroup(childGroups[i]);
        QSqlQuery query(ConnectionDatabase::getGblDb());
        query.prepare("INSERT INTO contacter (id, surname, name, title, network, photopath,"
                      " leaders, snnumber, bluetooth, namePinyin, type, selectedStatus, pocid) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

        //----------------------------id------------------------------------------//
        QString id = settings.value(ContacterID).toString();
        query.addBindValue(id);
        //------------------------------------------------------------------------//

        //----------------------------surname-------------------------------------//
        QString surname = settings.value(ContacterSurname).toString();
        query.addBindValue(surname);
        //------------------------------------------------------------------------//

        //----------------------------name----------------------------------------//

        QString name= settings.value(ContacterName).toString();
        query.addBindValue(name);
        //------------------------------------------------------------------------//

        //----------------------------title---------------------------------------//
        QString title = settings.value(ContacterTitle).toString();
        query.addBindValue(title);
        //------------------------------------------------------------------------//

        //----------------------------phone number--------------------------------//
        PhoneNumbers numbers;

        //wifi init
//        if(settings.contains(ContacterWIFI))
//        {
//            PhoneNumber wifiNumber;
//            wifiNumber.netType = WIFI_NETWORK;
//            wifiNumber.number = settings.value(ContacterWIFI).toString();
//            wifiNumber.priority = getContacterPriority(&settings, ContacterWIFI);
//            wifiNumber.online = 1;
//            numbers.append(wifiNumber);
//        }

//        if(settings.contains(ContacterLTE))
//        {
//            //lte init
//            PhoneNumber lteNumber;
//            lteNumber.netType = LTE_NETWORK;
//            lteNumber.number = settings.value(ContacterLTE).toString();
//            lteNumber.priority = getContacterPriority(&settings, ContacterLTE);
//            lteNumber.online = 1;
//            numbers.append(lteNumber);
//        }

//        if(settings.contains(ContacterBaoTong))
//        {
//            //baotong init
//            PhoneNumber digitalNumber;
//            digitalNumber.netType = DIGITAL_WALKIE_TALKIE_NETWORK;
//            digitalNumber.number =  settings.value(ContacterBaoTong).toString();
//            digitalNumber.priority = getContacterPriority(&settings, ContacterBaoTong);
//            digitalNumber.online = 1;
//            numbers.append(digitalNumber);
//        }

        if(settings.contains(ContacterTianTong))
        {
            //tiantong init
            PhoneNumber satelliteNumber;
            satelliteNumber.netType = SATELLITE_NETWORK;
            satelliteNumber.number = settings.value(ContacterTianTong).toString();
            satelliteNumber.priority = getContacterPriority(&settings, ContacterTianTong);
            satelliteNumber.online = 1;
            numbers.append(satelliteNumber);
        }

        if(settings.contains(ContacterADHOC))
        {
            //adhoc init
            PhoneNumber adhocNumber;
            adhocNumber.netType = AD_HOC_NETWORK;
            adhocNumber.number = settings.value(ContacterADHOC).toString();
            adhocNumber.priority = getContacterPriority(&settings, ContacterADHOC);
            adhocNumber.online = 1;
            numbers.append(adhocNumber);
        }

//        if(settings.contains(ContacterBattle))
//        {
//            //battle init
//            PhoneNumber battleNumber;
//            battleNumber.netType = BATTLE_NETWORK;
//            battleNumber.number = settings.value(ContacterBattle).toString();
//            battleNumber.priority = getContacterPriority(&settings, ContacterBattle);
//            battleNumber.online = 1;
//            numbers.append(battleNumber);
//        }
        query.addBindValue(ContacterInterface::converNumbersToString(numbers));
        //------------------------------------------------------------------------//

        //--------------------------photopath-------------------------------------//
        query.addBindValue("null");
        //------------------------------------------------------------------------//

        //--------------------------leaders---------------------------------------//
        query.addBindValue("[]");
        //------------------------------------------------------------------------//

        //--------------------------snnumber--------------------------------------//
        QString snnumber = getDeviceNum();
        query.addBindValue(snnumber);
        //------------------------------------------------------------------------//

        //--------------------------bluetooth-------------------------------------//
        QString bluetooth = settings.value(ContacterBLUETOOTH).toString();
        query.addBindValue(bluetooth);
        //------------------------------------------------------------------------//

        //------------------------pinyin------------------------------------------//
        query.addBindValue(ChineseToPinyin::getPinyinFirstLetter(surname+name) + " "
                           + ChineseToPinyin::getInstance()->getPinyin(surname+name) + " "
                           + surname+name + " "
                           + getTitleStringFromEnum(title.toInt()));
        //------------------------------------------------------------------------//

        //-----------------------contacter type-----------------------------------//
        query.addBindValue("1");
        //------------------------------------------------------------------------//

        //-----------------------selectstatus-------------------------------------//
        query.addBindValue("0");
        //------------------------------------------------------------------------//

        //-----------------------pocid--------------------------------------------//
        if(settings.contains(ContacterLTEPocID))
        {
            QString pocid = settings.value(ContacterLTEPocID).toString();
            query.addBindValue(pocid);
        }
        else
        {
            query.addBindValue("");
        }
        //------------------------------------------------------------------------//
        query.exec();
        settings.endGroup();
    }
    return true;
}

const QLatin1String ContacterPriority("PRIORITY");
int ConnectionDatabase::getContacterPriority(QSettings *setting, QString typeStr)
{
    QString priorityValue = setting->value(ContacterPriority).toString();
    QStringList prioritis = priorityValue.split(",");
    if(prioritis.contains(typeStr))
    {
        int index = prioritis.indexOf(typeStr);
        return index;
    }
    else
    {
        return 10;
    }
}

const QLatin1String GroupID("ID");
const QLatin1String GroupName("NAME");
const QLatin1String GroupCreator("CREATOR");
const QLatin1String GroupLeaders("LEADER");
const QLatin1String GroupMembers("MEMBER");
const QLatin1String GroupNetType("NETTYPE");
const QLatin1String GroupAddress("ADDRESS");
const QLatin1String GroupParent("PARENTGROUP");
const QLatin1String GroupPort("PORT");
const QLatin1String GroutOrgType("GROUPTYPE");

bool ConnectionDatabase::loadSettingGroupInfo()
{
//    QString appPath = QCoreApplication::applicationDirPath();
//    QDir resPath(appPath);
//    resPath.cdUp();
//    resPath.cd("res");
    QString resPath = "/data/apps/com.sinux.phonebookadhoc/res/group_phonebook.ini";

    QString groupIniPath = resPath;//.path() + "/group_phonebook.ini";
    qDebug() << "group ini path:" << groupIniPath;
    QSettings settings(groupIniPath, QSettings::IniFormat);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    settings.setIniCodec(codec);
    QStringList childGroups = settings.childGroups();
    for(int i = 0, iend = childGroups.count(); i < iend; i++)
    {
        settings.beginGroup(childGroups[i]);
        QSqlQuery query(ConnectionDatabase::getGblDb());        
        query.prepare("INSERT INTO groups (id, name, iconpath, creator, members, leaders,"
                      " address, port, network, type, parent, org) "
                      "VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

        //-------------------------------group id-----------------------------//
        QString id = settings.value(GroupID).toString();
        query.addBindValue(id);
        //--------------------------------------------------------------------//

        //-------------------------------group name---------------------------//
        QString name = settings.value(GroupName).toString();
        query.addBindValue(name);
        //--------------------------------------------------------------------//

        //-------------------------------group iconpath-----------------------//
        query.addBindValue("null");
        //--------------------------------------------------------------------//

        //-------------------------------group creator------------------------//
        QString creator = settings.value(GroupCreator).toString();
        query.addBindValue(creator);
        //--------------------------------------------------------------------//

        //-------------------------------group members------------------------//
        QString memberValue = settings.value(GroupMembers).toString();
        //QString members = "[" + memberValue + "]";
        memberValue.replace(QString("\/"), QString(","));
        query.addBindValue(memberValue);
        //--------------------------------------------------------------------//

        //-------------------------------group leaders------------------------//
        QString leaderValue = settings.value(GroupLeaders).toString();
        leaderValue.replace(QString("\/"), QString(","));
        query.addBindValue(leaderValue);
        //--------------------------------------------------------------------//

        //-------------------------------group address------------------------//
        QString address = settings.value(GroupAddress).toString();
        query.addBindValue(address);
        //--------------------------------------------------------------------//

        //-------------------------------group port---------------------------//
        QString port = settings.value(GroupPort).toString();
        query.addBindValue(port);
        //--------------------------------------------------------------------//

        //-------------------------------group network------------------------//
        QString net = settings.value(GroupNetType).toString();
        int nettype = WIFI_NETWORK;
        if(net == ContacterWIFI)
            nettype = WIFI_NETWORK;
        else if(net == ContacterLTE)
            nettype = LTE_NETWORK;
        else if(net == ContacterADHOC)
            nettype = AD_HOC_NETWORK;
        else if(net == ContacterBaoTong)
            nettype = DIGITAL_WALKIE_TALKIE_NETWORK;
        else if(net == ContacterTianTong)
            nettype = SATELLITE_NETWORK;
        else if(net == ContacterBattle)
            nettype = BATTLE_GROUP;
        query.addBindValue(nettype);
        //--------------------------------------------------------------------//

        //------------------------------group type----------------------------//
        query.addBindValue("subordinate");
        //--------------------------------------------------------------------//

        //------------------------------group parent--------------------------//
        QString parent = settings.value(GroupParent).toString();
        query.addBindValue(parent);
        //--------------------------------------------------------------------//

        //------------------------------group org type----------------------------//
        int type = settings.value(GroutOrgType).toInt();
        query.addBindValue(type);
        //--------------------------------------------------------------------//
        query.exec();
        settings.endGroup();
    }
    return true;
}

bool ConnectionDatabase::createConversionGroup()
{
//    QSqlQuery query(ConnectionDatabase::getGblDb());
//    query.prepare("INSERT INTO groups (id, name, iconpath, creator, members, leaders,"
//                  " address, port, network, type, parent, org) "
//                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");
//    query.addBindValue("200100000");
//    query.addBindValue("广播组");
//    query.addBindValue("null");
//    query.addBindValue("201");
//    query.addBindValue("[202]");
//    query.addBindValue("");
//    query.addBindValue("239.0.0.56");
//    query.addBindValue("10080");
//    query.addBindValue("ad_hoc_network");
//    query.addBindValue("conversion");
//    query.addBindValue("-1");
//    query.addBindValue("-1");
//    bool flag = query.exec();
//    qDebug() << "createConversionGroup flag: " << flag << query.lastError();

//    query.prepare("INSERT INTO groups (id, name, iconpath, creator, members, leaders,"
//                  " address, port, network, type, parent, org) "
//                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");
//    query.addBindValue("2002");
//    query.addBindValue("sinuxgroup2");
//    query.addBindValue("null");
//    query.addBindValue("201");
//    query.addBindValue("[202]");
//    query.addBindValue("");
//    query.addBindValue("239.0.0.43");
//    query.addBindValue("24901");
//    query.addBindValue("wifi_network");
//    query.addBindValue("conversion");
//    query.addBindValue("-1");
//    query.addBindValue("-1");
//    flag = query.exec();
    //    qDebug() << "createConversionGroup flag: " << flag << query.lastError();
}

void ConnectionDatabase::onSignalTimeOut()
{
    QSqlQuery query(ConnectionDatabase::getGblDb());
    if(query.exec("SELECT * FROM local"))
    {
        if(!query.next())
        {
            qDebug() << "get local info ......";
            creatLocalPhoneInfo();
        }
        else
        {
            qDebug() << "get over info ......";
            m_timer->stop();
        }
    }
}
