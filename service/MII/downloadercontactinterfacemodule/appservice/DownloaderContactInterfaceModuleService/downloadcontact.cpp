#include "downloadcontact.h"
#include <QDebug>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QFile>
#include <QTextCodec>
#include <QTimer>
//#include <../../commondata/datatype/commondata.h>
#include "./datatype/commondata.h"

DownloadContact* DownloadContact::instance = NULL;

DownloadContact* DownloadContact::getInstance()
{
    if(instance == NULL)
        instance = new DownloadContact();

    return instance;
}

DownloadContact::DownloadContact(QObject *parent) :
    QObject(parent),
    m_loginSuccess(false),
    m_requestFinished(true)
{
}

void DownloadContact::onStart()
{
    qDebug() << "onStartDownload";
//    LoginRequestData loginRequestData;
//    login(loginRequestData);

    m_requestFinished = false;
    if(m_queue.isEmpty())
    {
        signalDownloadPercent(1,1,0,"");
        m_requestFinished = true;
    }
    else
    {
        Request2 *request = m_queue.dequeue();
        request->startRequest();
    }
}

void DownloadContact::login(const LoginRequestData &loginRequestData)
{
    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName("login.html");
    request->setType(RemoteContactType_Login);
    request->setLoginData(loginRequestData);
    request->requestType = RequestType_Login;

    m_queue.enqueue(request);
}

void DownloadContact::onDownloadRequestFinished(const QString &content)
{
    Request2 *request = static_cast<Request2 *>(this->sender());
    switch (request->getType()) {
    case RemoteContactType_Login:
    {
        if(request->getHttpCode() == 200)
        {
            qDebug() << "login success--------------";
            m_loginSuccess = true;
//            switch (m_afterLoginToDo) {
//            case AfterLoginToDo_Download:
//            {
//                getGroups();
//                break;
//            }
//            case AfterLoginToDo_PostContacter:
//            {
//                postContact(wilPostContacterServer);
//                break;
//            }
//            case AfterLoginToDo_PostContacterNetwork:
//            {
//                postContacterNetwork(willPostContacterNetworkServer);
//                break;
//            }
//            case AfterLoginToDo_PostContacterPhoto:
//            {
//                postContacterPhoto(willPostContacterPhotoServer);
//                break;
//            }
//            case AfterLoginToDo_PostGroup:
//            {
//                postGroup(willPostGroupOrganizationServer);
//                break;
//            }
//            case AfterLoginToDo_PostGroupBattle:
//            {
//                postBattleGroup(willPostGroupBattleServer);
//                break;
//            }
//            default:
//                break;
//            }
//            getGroups();
//            getContacts();

//                ContacterServer contact;
//                contact.id = "123456";
//                contact.surname = "li";
//                contact.name = "orange";
//                contact.title = QString::number(1);
//                contact.leaders = "";
//                contact.snnumber = "1111";
//                contact.photoid = "111111.png";

//                onPostContact(contact);

//            ContacterNetworkServer network;
//            network.networkid = "123456";
//            network.adhocaddress = "192.168.1.100";
//            network.battleaddress = "192.168.1.100";
//            network.bluetooth = "22:22:22:22:22:22";
//            network.btaddress = "192.168.1.100";
//            network.lteaddress = "192.168.1.100";
//            network.ttaddress = "18190946318";
//            network.wifiaddress = "192.168.1.100";
//            onPostContacterNetwork(network);

//            ContacterPhotoServer photo;
//            photo.phototid = "123456";
//            photo.photodata = "/home/user/DCIM/Camera/myimage.jpg";
//            onPostContacterPhoto(photo);

//            getGroups();
//            getBattleGroup();
//            getContacterPassword("");

//            ContacterPasswordServer password;
//            password.passwordid = "123456";
//            password.password = "123456";
//            password.iris = "123456";
//            password.fingerprint = "123456";
//            postContacterPassword(password);

//            GroupOrganizationServer group;
//            group.groupid = "1111";
//            group.address = "224.0.1.5";
//            group.creator = "110";
//            group.leaders = "111";
//            group.members = "112";
//            group.name = "1002";
//            group.network = "1";
//            group.port = "9000";
//            postGroup(group);

//            getContacterPhoto("111");
        }
        else
        {
            qDebug() << "login error--------------" << content;
            ServerErrorInfo errorInfo = convertJsonToLoginError(content);
            qDebug() << "code: " << errorInfo.value("resultCode") << errorInfo.value("resultMsg");
        }
        break;
    }
    case RemoteContactType_Contacts:
    {
        if(!content.isEmpty())
        {
            qDebug() << "get contacts success------------";
            ContacterServerList cList = convertJsonToContacters(content);
//            m_contacterServerList = cList;
            emit signalContactersDownload(cList);
        }
        else
        {
            qDebug() << "get contacts fail------------";
        }
        break;
    }
    case RemoteContactType_ContacterNetwork:
    {
        if(request->getHttpCode() == 200)
        {
            qDebug() << "get network success------------" << content;
            IdContacterNetworkServerMap idNetworkMap = convertJsonToNetworks(content);
//            m_idContacterNetworkServerMap = idNetworkMap;

            for(IdContacterNetworkServerMap::iterator it=idNetworkMap.begin(); it!=idNetworkMap.end(); it++)
            {
                qDebug() << "idNetworkMap: " << it.key() << it.value().bluetooth;
            }
            emit signalContacterNetworksDownload(idNetworkMap);
        }
        else
        {
            qDebug() << "get network fail------------";
        }
        break;
    }
    case RemoteContactType_ContacterPhoto:
    {
        if(request->getHttpCode() == 200)
        {
            qDebug() << "get photo:" << request->downloadFileName;
            IdContacterPhotoNameMap idContacterPhotoNameMap;
            idContacterPhotoNameMap.insert(request->downloadContacterId,
                                             request->downloadFileName);

            emit signalContacterPhotoDownload(idContacterPhotoNameMap);
        }
        else
        {
            qDebug() << "get photo fail------------";
        }
        break;
    }

    case RemoteContactType_Groups:
    {
        if(request->getHttpCode() == 200)
        {
            qDebug() << "get groups success------------" << content;
            GroupOrganizationServerList groupOrganizationServerList = convertJsonToGroups(content);
            GroupList localGroupList;
            foreach(const GroupOrganizationServer& group, groupOrganizationServerList)
            {
                Group localGroup;
                localGroup.id = group.groupid;
                localGroup.name = group.name;
                localGroup.iconPath = "";
                localGroup.creator = group.creator;
                localGroup.members = convertStringToMembers(group.members);
                localGroup.leaders = convertStringToLeaders(group.leaders);
                localGroup.address = group.address;
                localGroup.port = group.port.toUInt();
                localGroup.networkType = convertStringToNetworkType(group.network);
                localGroup.type = SUBORDINATE_GROUP;
                localGroup.handle = GROUP_ADD_HANDLE;
                localGroupList.push_back(localGroup);
            }

            emit signalGroupsDownload(groupOrganizationServerList);
        }
        else
        {
            qDebug() << "get groups fail------------";
        }
        break;
    }
    case RemoteContactType_GroupsBattle:
    {
        if(request->getHttpCode() == 200)
        {
            qDebug() << "get group battle success------------";
            GroupBattleServer groupBattle = convertJsonToBattle(content);
            GroupBattleServerList battleGroupList;
            battleGroupList.push_back(groupBattle);
            qDebug() << "group battle: " << groupBattle.name << groupBattle.members << groupBattle.battleid;

            emit signalGroupBattlesDownload(battleGroupList);
        }
        else
        {
            qDebug() << "get group battle fail------------";
        }
        break;
    }
    //post
    case RemoteContactType_PostContacts:
    {
        int stauts;
        if(request->getHttpCode() == 200)
        {
            qDebug() << "post contacts success---";
            stauts = 1;
        }
        else
        {
            qDebug() << "post contacts fail------------";
            stauts = 0;
        }
        emit signalPostFinished(RemoteContactType_PostContacts, stauts);
        break;
    }
    case RemoteContactType_PostContacterNetwork:
    {
        int stauts;
        if(request->getHttpCode() == 200)
        {
            qDebug() << "post network success---";
            stauts = 1;
        }
        else
        {
            qDebug() << "post network fail------------";
            stauts = 0;
        }
        emit signalPostFinished(RemoteContactType_PostContacterNetwork, stauts);
        break;
    }
    case RemoteContactType_PostContacterPhoto:
    {
        int stauts;
        if(request->getHttpCode() == 200)
        {
            qDebug() << "post photo success---";
            stauts = 1;
        }
        else
        {
            qDebug() << "post photo fail------------";
            stauts = 0;
        }
        emit signalPostFinished(RemoteContactType_PostContacterPhoto, stauts);
        break;
    }
    case RemoteContactType_PostGroups:
    {
        int stauts;
        if(request->getHttpCode() == 200)
        {
            qDebug() << "post groups success---";
            stauts = 1;
        }
        else
        {
            qDebug() << "post groups fail------------";
            stauts = 0;
        }
        emit signalPostFinished(RemoteContactType_PostGroups, stauts);
        break;
    }
    case RemoteContactType_PostGroupsBattle:
    {
        int stauts;
        if(request->getHttpCode() == 200)
        {
            qDebug() << "post GroupsBattle success---";
            stauts = 1;
        }
        else
        {
            qDebug() << "post GroupsBattle fail------------";
            stauts = 0;
        }
        emit signalPostFinished(RemoteContactType_PostGroupsBattle, stauts);
        break;
    }
    case RemoteContactType_PostContacterPassword:
    {
        if(request->getHttpCode() == 200)
        {
            qDebug() << "post password success---";
        }
        else
        {
            qDebug() << "get password fail------------";
        }
        break;
    }
    default:
        break;
    }

    if(request)
        delete request;

    onStart();
}

//ContacterServerList DownloadContact::getContacterServer()
//{
//    return m_contacterServerList;
//}

//IdContacterNetworkServerMap DownloadContact::getContacterNetworkServer()
//{
//    return m_idContacterNetworkServerMap;
//}

//IdContacterPhotoNameMap DownloadContact::getContacterPhotoServer()
//{
//    return m_idContacterPhotoNameMap;
//}

//GroupOrganizationServerList DownloadContact::getGroupOrganizationServer()
//{
//    return m_groupOrganizationServerList;
//}

//GroupBattleServerList DownloadContact::getGroupBattleServer()
//{
//    return m_groupBattleServerList;
//}

void DownloadContact::getContacts()
{
    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName("contacts.json");
    request->setUrl("http://219.144.206.252:58000/contact/person/");
    request->setType(RemoteContactType_Contacts);
    request->requestType = RequestType_Normal;

    m_queue.enqueue(request);
}

void DownloadContact::postContact(const ContacterServer &contact)
{
    QString postDataFileName = "postcontacts.json";

    QString contactJson = convertContacterToJson(contact);
    qDebug() << "saveFile flag: " << saveFile(contactJson.toLocal8Bit(), postDataFileName);

    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(postDataFileName);
    request->setUrl("http://219.144.206.252:58000/contact/person/");
    request->setType(RemoteContactType_PostContacts);
//    request->postStart();
    request->requestType = RequestType_Post;

    m_queue.enqueue(request);
}

void DownloadContact::getContacterNetworks()
{
    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(QString("contactersaddresses.json"));
    request->setUrl(QString("http://219.144.206.252:58000/contact/address/"));
    request->setType(RemoteContactType_ContacterNetwork);
//    request->startDownload();
    request->requestType = RequestType_Normal;

    m_queue.enqueue(request);
}

void DownloadContact::postContacterNetwork(const ContacterNetworkServer &network)
{
    QString postDataFileName = "postcontactsnetwork.json";

    QString contactJson = convertNetworkToJson(network);
    qDebug() << "saveFile flag: " << saveFile(contactJson.toLocal8Bit(), postDataFileName);

    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(postDataFileName);
    request->setUrl("http://219.144.206.252:58000/contact/address/");
    request->setType(RemoteContactType_PostContacterNetwork);
//    request->postStart();
    request->requestType = RequestType_Post;

    m_queue.enqueue(request);
}

void DownloadContact::getContacterPhoto(const QString &id)
{
    ContacterServer contacterServer = findContacter(id);

    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));

    request->setFileName(QString("getphoto.jpg"));
    request->downloadContacterId = id;
    request->setType(RemoteContactType_ContacterPhoto);
//    request->start();
    request->requestType = RequestType_GetPhoto;

    m_queue.enqueue(request);
}

void DownloadContact::postContacterPhoto(const ContacterPhotoServer &photo)
{
    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(QString("postphoto.json"));
    request->setSendFileName(photo.photodata);
    request->setUrl(QString("http://219.144.206.252:58000/contact/image/?person_id=%1").arg(photo.phototid));
    request->setType(RemoteContactType_PostContacterPhoto);
//    request->postFileStart();
    request->requestType = RequestType_PostFile;

    m_queue.enqueue(request);
}

void DownloadContact::getGroups()
{
    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(QString("groups.json"));
    request->setUrl(QString("http://219.144.206.252:58000/contact/groups/"));
    request->setType(RemoteContactType_Groups);
//    request->startDownload();
    request->requestType = RequestType_Normal;

    m_queue.enqueue(request);
}

void DownloadContact::postGroup(const GroupOrganizationServer &group)
{
    QString postDataFileName = "postorganizationgroups.json";
    QString contactJson = convertGroupToJson(group);
    qDebug() << "saveFile flag: " << saveFile(contactJson.toLocal8Bit(), postDataFileName);

    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(postDataFileName);
    request->setUrl(QString("http://219.144.206.252:58000/contact/groups/"));
    request->setType(RemoteContactType_PostGroups);
//    request->postStart();
    request->requestType = RequestType_Post;

    m_queue.enqueue(request);
}

void DownloadContact::getBattleGroup()
{
    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(QString("groupsbattle.json"));
    request->setUrl(QString("http://219.144.206.252:58000/contact/battlegroup/"));
    request->setType(RemoteContactType_GroupsBattle);
//    request->startDownload();
    request->requestType = RequestType_Normal;

    m_queue.enqueue(request);
}

void DownloadContact::postBattleGroup(const GroupBattleServer &groupBattle)
{
    QString postDataFileName = "postbattlegroup.json";
    QString contactJson = convertBattleToJson(groupBattle);
    qDebug() << "saveFile flag: " << saveFile(contactJson.toLocal8Bit(), postDataFileName);

    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(postDataFileName);
    request->setUrl(QString("http://219.144.206.252:58000/contact/battlegroup/"));
    request->setType(RemoteContactType_PostGroupsBattle);
//    request->postStart();
    request->requestType = RequestType_Post;

    m_queue.enqueue(request);
}

bool DownloadContact::isRequestFinished()
{
    return m_requestFinished;
}

void DownloadContact::getContacterPassword(const QString &id)
{
    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(QString("getpassword.json"));
    request->setUrl(QString("http://219.144.206.252:58000/contact/password/"));
    request->setType(RemoteContactType_ContacterPassword);
//    request->startDownload();
    request->requestType = RequestType_Normal;

    m_queue.enqueue(request);
}

void DownloadContact::postContacterPassword(const ContacterPasswordServer &password)
{
    QString postDataFileName = "postpassword.json";
    QString contactJson = convertPasswordToJson(password);
    qDebug() << "saveFile flag: " << saveFile(contactJson.toLocal8Bit(), postDataFileName);

    Request2 *request = Request2::newInstance();
    connect(request, SIGNAL(signalDownloadOver(QString)),
            this, SLOT(onDownloadRequestFinished(QString)));
    request->setFileName(postDataFileName);
    request->setUrl(QString("http://219.144.206.252:58000/contact/password/"));
    request->setType(RemoteContactType_PostContacterPassword);
//    request->startDownload();
    request->requestType = RequestType_Normal;

    m_queue.enqueue(request);
}

ContacterServerList DownloadContact::convertJsonToContacters(const QString &content)
{
    qDebug() << "startParseContacters";
    ContacterServerList contacterList;
    QStringList strList = content.split("}", QString::SkipEmptyParts);
    foreach(QString str, strList)
    {
        contacterList.append(convertJsonToContacter(str.append("}")));
    }
    foreach(const ContacterServer &contacter, contacterList)
    {
        qDebug() << "contacter name :" << contacter.surname << contacter.name;
    }
    return contacterList;
}

GroupOrganizationServerList DownloadContact::convertJsonToGroups(const QString &content)
{
    qDebug() << "onStartParseGroups";
    QStringList strList = content.split("}", QString::SkipEmptyParts);
    GroupOrganizationServerList groupList;
    foreach(QString str, strList)
    {
        groupList.append(convertJsonToGroup(str.append("}")));
    }

    return groupList;
}

IdContacterNetworkServerMap DownloadContact::convertJsonToNetworks(const QString &content)
{
    qDebug() << "onStartParseGroups";
    QStringList strList = content.split("}", QString::SkipEmptyParts);
    IdContacterNetworkServerMap idNetworkMap;
    foreach(QString str, strList)
    {
        ContacterNetworkServer network = convertJsonToNetwork(str.append("}"));
        idNetworkMap.insert(network.networkid, network);
    }

    return idNetworkMap;
}

ContacterServer DownloadContact::convertJsonToContacter(const QString& content)
{
    ContacterServer contacter;

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(content.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {

        if(parse_doucment.isObject())
        {
            QString name, surname;
            QJsonObject obj = parse_doucment.object();

            if(obj.contains("person_id"))
            {
                QJsonValue name_value = obj.take("person_id");
                if(name_value.isString())
                {
                    contacter.id = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains person_id";
            }

            if(obj.contains("surname"))
            {
                QJsonValue name_value = obj.take("surname");
                if(name_value.isString())
                {
                    contacter.surname = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains surname";
            }

            if(obj.contains("name"))
            {
                QJsonValue name_value = obj.take("name");
                if(name_value.isString())
                {
                    contacter.name = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains name";
            }

            if(obj.contains("title"))
            {
                QJsonValue name_value = obj.take("title");
                if(name_value.isString())
                {
                    contacter.title = name_value.toInt();
                }
            }
            else
            {
                qDebug() << "error : not contains title";
            }

            if(obj.contains("photo"))
            {
                QJsonValue name_value = obj.take("photo");
                if(name_value.isString())
                {
                    contacter.photoid = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains photo";
            }

//            if(obj.contains("address"))
//            {
//                QJsonValue name_value = obj.take("address");
//                if(name_value.isString())
//                {
//                    contacter.networkid = name_value.toString();
//                }
//            }
//            else
//            {
//                qDebug() << "error : not contains address";
//            }

            if(obj.contains("snnumber"))
            {
                QJsonValue name_value = obj.take("snnumber");
                if(name_value.isString())
                {
                    contacter.snnumber = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains snnumber";
            }

//            if(obj.contains("password"))
//            {
//                QJsonValue name_value = obj.take("password");
//                if(name_value.isString())
//                {
//                    contacter.passwordid = name_value.toString();
//                }
//            }
//            else
//            {
//                qDebug() << "error : not contains password";
//            }

            if(obj.contains("leaders"))
            {
                QJsonValue name_value = obj.take("leaders");
                if(name_value.isString())
                {
                    contacter.leaders = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains leaders";
            }
        }
        else if(parse_doucment.isArray())
        {
            qDebug() << "parse_doucment is array";
        }
        else if(parse_doucment.isEmpty())
        {
            qDebug() << "parse_doucment is empty";
        }
    }
    else
    {
        json_error.offset;
        qDebug() << "json parse error: " << json_error.errorString() << content.mid(json_error.offset, 20);
    }

    return contacter;
}

QString DownloadContact::convertContacterToJson(const ContacterServer &contacter)
{
    QJsonObject object;

    object.insert("person_id", contacter.id);
    object.insert("surname", contacter.surname);
    object.insert("name", contacter.name);
    object.insert("title", contacter.title);
    object.insert("photo", contacter.photoid);
//    object.insert("address", contacter.networkid);
    object.insert("snnumber", contacter.snnumber);
//    object.insert("password", contacter.passwordid);
    object.insert("leaders", contacter.leaders);

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

GroupOrganizationServer DownloadContact::convertJsonToGroup(const QString& content)
{
    GroupOrganizationServer group;

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(content.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();

            if(obj.contains("groups_id"))
            {
                QJsonValue name_value = obj.take("groups_id");
                if(name_value.isString())
                {
                    group.groupid = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains groups_id";
            }

            if(obj.contains("name"))
            {
                QJsonValue name_value = obj.take("name");
                if(name_value.isString())
                {
                    group.name = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains id";
            }

            if(obj.contains("creater"))
            {
                QJsonValue name_value = obj.take("creater");
                if(name_value.isString())
                {
                    group.creator = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains creater";
            }

            if(obj.contains("leaders"))
            {
                QJsonValue name_value = obj.take("leaders");
                if(name_value.isString())
                {
                    group.leaders = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains leaders";
            }

            if(obj.contains("members"))
            {
                QJsonValue name_value = obj.take("members");
                if(name_value.isString())
                {
                    group.members = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains members";
            }

            if(obj.contains("address"))
            {
                QJsonValue name_value = obj.take("address");
                if(name_value.isString())
                {
                    group.address = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains address";
            }

            if(obj.contains("port"))
            {
                QJsonValue name_value = obj.take("port");
                if(name_value.isString())
                {
                    group.port = name_value.toInt();
                }
            }
            else
            {
                qDebug() << "error : not contains port";
            }

            if(obj.contains("network"))
            {
                QJsonValue name_value = obj.take("network");
                if(name_value.isString())
                {
                    group.network = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains network";
            }
        }
        else if(parse_doucment.isArray())
        {
            qDebug() << "parse_doucment is array";
        }
        else if(parse_doucment.isEmpty())
        {
            qDebug() << "parse_doucment is empty";
        }
    }
    else
    {
        json_error.offset;
        qDebug() << "json parse error: " << json_error.errorString() << content.mid(json_error.offset, 20);
    }

    return group;
}

QString DownloadContact::convertGroupToJson(const GroupOrganizationServer &group)
{
    QJsonObject object;

    object.insert("groups_id", group.groupid);
    object.insert("name", group.name);
    object.insert("creater", group.creator);
    object.insert("leaders", group.leaders);
    object.insert("members", group.members);
    object.insert("address", group.address);
    object.insert("port", group.port);
    object.insert("network", group.network);

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

ContacterPhotoServer DownloadContact::convertJsonToPhoto(const QString &content)
{
    return ContacterPhotoServer();
}

QString DownloadContact::convertPhotoToJson(const ContacterPhotoServer &photo)
{
    return QString();
}

ContacterNetworkServer DownloadContact::convertJsonToNetwork(const QString &content)
{
    ContacterNetworkServer network;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(content.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();

            if(obj.contains("network_id"))
            {
                QJsonValue name_value = obj.take("network_id");
                if(name_value.isString())
                {
                    network.networkid = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains network_id";
            }

            if(obj.contains("bluetooth"))
            {
                QJsonValue name_value = obj.take("bluetooth");
                if(name_value.isString())
                {
                    network.bluetooth = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains bluetooth";
            }

            if(obj.contains("lteaddress"))
            {
                QJsonValue name_value = obj.take("lteaddress");
                if(name_value.isString())
                {
                    network.lteaddress = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains lteaddress";
            }

            if(obj.contains("adhocaddress"))
            {
                QJsonValue name_value = obj.take("adhocaddress");
                if(name_value.isString())
                {
                    network.adhocaddress = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains adhocaddress";
            }

            if(obj.contains("battleaddress"))
            {
                QJsonValue name_value = obj.take("battleaddress");
                if(name_value.isString())
                {
                    network.battleaddress = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains battleaddress";
            }

            if(obj.contains("wifiaddress"))
            {
                QJsonValue name_value = obj.take("wifiaddress");
                if(name_value.isString())
                {
                    network.wifiaddress = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains wifiaddress";
            }

            if(obj.contains("btaddress"))
            {
                QJsonValue name_value = obj.take("btaddress");
                if(name_value.isString())
                {
                    network.btaddress = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains btaddress";
            }

            if(obj.contains("ttaddress"))
            {
                QJsonValue name_value = obj.take("ttaddress");
                if(name_value.isString())
                {
                    network.ttaddress = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains ttaddress";
            }

        }
        else if(parse_doucment.isArray())
        {
            qDebug() << "parse_doucment is array";
        }
        else if(parse_doucment.isEmpty())
        {
            qDebug() << "parse_doucment is empty";
        }
    }
    else
    {
        json_error.offset;
        qDebug() << "json parse error: " << json_error.errorString() << content.mid(json_error.offset, 20);
    }

    return network;
}

QString DownloadContact::convertNetworkToJson(const ContacterNetworkServer &contacterNetwork)
{
    QJsonObject object;

    object.insert("network_id", contacterNetwork.networkid);
    object.insert("bluetooth", contacterNetwork.bluetooth);
    object.insert("lteaddress", contacterNetwork.lteaddress);
    object.insert("adhocaddress", contacterNetwork.adhocaddress);
    object.insert("battleaddress", contacterNetwork.battleaddress);
    object.insert("wifiaddress", contacterNetwork.wifiaddress);
    object.insert("btaddress", contacterNetwork.btaddress);
    object.insert("ttaddress", contacterNetwork.ttaddress);

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

ContacterPasswordServer DownloadContact::convertJsonToPassword(const QString &content)
{
    ContacterPasswordServer passwordServer;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(content.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();

            if(obj.contains("password_id"))
            {
                QJsonValue name_value = obj.take("password_id");
                if(name_value.isString())
                {
                    passwordServer.passwordid = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains password_id";
            }

            if(obj.contains("iris"))
            {
                QJsonValue name_value = obj.take("iris");
                if(name_value.isString())
                {
                    passwordServer.iris = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains iris";
            }

            if(obj.contains("fingerprint"))
            {
                QJsonValue name_value = obj.take("fingerprint");
                if(name_value.isString())
                {
                    passwordServer.fingerprint = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains fingerprint";
            }

            if(obj.contains("password"))
            {
                QJsonValue name_value = obj.take("password");
                if(name_value.isString())
                {
                    passwordServer.password = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains password";
            }
        }
        else if(parse_doucment.isArray())
        {
            qDebug() << "parse_doucment is array";
        }
        else if(parse_doucment.isEmpty())
        {
            qDebug() << "parse_doucment is empty";
        }
    }
    else
    {
        json_error.offset;
        qDebug() << "json parse error: " << json_error.errorString() << content.mid(json_error.offset, 20);
    }

    return passwordServer;
}

QString DownloadContact::convertPasswordToJson(const ContacterPasswordServer &contacterPassword)
{
    QJsonObject object;

    object.insert("password_id", contacterPassword.passwordid);
    object.insert("iris", contacterPassword.iris);
    object.insert("fingerprint", contacterPassword.fingerprint);
    object.insert("password", contacterPassword.password);

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

GroupBattleServer DownloadContact::convertJsonToBattle(const QString &content)
{
    GroupBattleServer group;

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(content.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("battle_id"))
            {
                QJsonValue name_value = obj.take("battle_id");
                if(name_value.isString())
                {
                    group.battleid = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains battle_id";
            }

            if(obj.contains("name"))
            {
                QJsonValue name_value = obj.take("name");
                if(name_value.isString())
                {
                    group.name = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains id";
            }

            if(obj.contains("creater"))
            {
                QJsonValue name_value = obj.take("creater");
                if(name_value.isString())
                {
                    group.creator = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains creater";
            }

            if(obj.contains("leaders"))
            {
                QJsonValue name_value = obj.take("leaders");
                if(name_value.isString())
                {
                    group.leaders = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains leaders";
            }

            if(obj.contains("members"))
            {
                QJsonValue name_value = obj.take("members");
                if(name_value.isString())
                {
                    group.members = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains members";
            }

            if(obj.contains("address"))
            {
                QJsonValue name_value = obj.take("address");
                if(name_value.isString())
                {
                    group.address = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains address";
            }

            if(obj.contains("port"))
            {
                QJsonValue name_value = obj.take("port");
                if(name_value.isString())
                {
                    group.port = name_value.toInt();
                }
            }
            else
            {
                qDebug() << "error : not contains port";
            }

            if(obj.contains("network"))
            {
                QJsonValue name_value = obj.take("network");
                if(name_value.isString())
                {
                    group.network = name_value.toString();
                }
            }
            else
            {
                qDebug() << "error : not contains network";
            }


        }
        else if(parse_doucment.isArray())
        {
            qDebug() << "parse_doucment is array";
        }
        else if(parse_doucment.isEmpty())
        {
            qDebug() << "parse_doucment is empty";
        }
    }
    else
    {
        json_error.offset;
        qDebug() << "json parse error: " << json_error.errorString() << content.mid(json_error.offset, 20);
    }

    return group;
}

QString DownloadContact::convertBattleToJson(const GroupBattleServer &group)
{
    QJsonObject object;

    object.insert("battle_id", group.battleid);
    object.insert("name", group.name);
    object.insert("creater", group.creator);
    object.insert("leaders", group.leaders);
    object.insert("members", group.members);
    object.insert("address", group.address);
    object.insert("port", group.port);
    object.insert("network", group.network);

    QJsonDocument document(object);
    QByteArray array = document.toJson();
    QString value = QString::fromUtf8(array);

    return value;
}

ServerErrorInfo DownloadContact::convertJsonToLoginError(const QString &content)
{
    ServerErrorInfo keyValueMap;

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(content.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();

            if(obj.contains("resultCode"))
            {
                QJsonValue name_value = obj.take("resultCode");
                if(name_value.isString())
                {
                    keyValueMap.insert("resultCode", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains resultCode";
            }

            if(obj.contains("resultMsg"))
            {
                QJsonValue name_value = obj.take("resultMsg");
                if(name_value.isString())
                {
                    keyValueMap.insert("resultMsg", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains resultMsg";
            }

            if(obj.contains("transSeqNo"))
            {
                QJsonValue name_value = obj.take("transSeqNo");
                if(name_value.isString())
                {
                    keyValueMap.insert("transSeqNo", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains transSeqNo";
            }

            if(obj.contains("policeId"))
            {
                QJsonValue name_value = obj.take("policeId");
                if(name_value.isString())
                {
                    keyValueMap.insert("policeId", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains policeId";
            }

            if(obj.contains("verifyResult"))
            {
                QJsonValue name_value = obj.take("verifyResult");
                if(name_value.isString())
                {
                    keyValueMap.insert("verifyResult", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains verifyResult";
            }

            if(obj.contains("fingerFeature"))
            {
                QJsonValue name_value = obj.take("fingerFeature");
                if(name_value.isString())
                {
                    keyValueMap.insert("fingerFeature", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains fingerFeature";
            }

            if(obj.contains("irisFeature"))
            {
                QJsonValue name_value = obj.take("irisFeature");
                if(name_value.isString())
                {
                    keyValueMap.insert("irisFeature", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains irisFeature";
            }

            if(obj.contains("matchScore"))
            {
                QJsonValue name_value = obj.take("matchScore");
                if(name_value.isString())
                {
                    keyValueMap.insert("matchScore", name_value.toString());
                }
            }
            else
            {
                qDebug() << "error : not contains matchScore";
            }
        }
        else if(parse_doucment.isArray())
        {
            qDebug() << "parse_doucment is array";
        }
        else if(parse_doucment.isEmpty())
        {
            qDebug() << "parse_doucment is empty";
        }
    }
    else
    {
        json_error.offset;
        qDebug() << "json parse error: " << json_error.errorString() << content.mid(json_error.offset, 20);
    }


    return keyValueMap;
}

QString DownloadContact::removeSpecialWord(const QString &content)
{
    QString newContent = content;
    newContent.replace(QRegExp("u'"), "\"").replace(QRegExp("'"), "\"");
    return newContent;
}

ContacterServer DownloadContact::findContacter(const QString &id)
{
    ContacterServer contacter;

//    foreach(const ContacterServer &c, m_contacterServerList)
//    {
//        if(c.id == id)
//            contacter = c;
//    }
    return contacter;
}

void DownloadContact::combineServerDataToLocal()
{
//    m_contacterLocalList.clear();

//    foreach(const ContacterServer &contacterServer, m_contacterServerList)
//    {
//        const QString &networkId = contacterServer.id;
//        const ContacterNetworkServer &contacterNetworkServer = m_idContacterNetworkServerMap.value(networkId);


//        Contacter contacter;
//        contacter.id = contacterServer.id;
//        contacter.surname = contacterServer.surname;
//        contacter.name = contacterServer.name;
//        contacter.title = contacterServer.title.toInt();
//        contacter.leaders = convertStringToLeaders(contacterServer.leaders);
//        contacter.snNumber = contacterServer.snnumber;
//        contacter.bluetooth = contacterNetworkServer.bluetooth;

//        PhoneNumbers phoneNumbers;
//        PhoneNumber phoneNumber;
//        phoneNumber.netType = AD_HOC_NETWORK;
//        phoneNumber.number = contacterNetworkServer.adhocaddress;
//        phoneNumbers.push_back(phoneNumber);

//        phoneNumber.netType = LTE_NETWORK;
//        phoneNumber.number = contacterNetworkServer.lteaddress;
//        phoneNumbers.push_back(phoneNumber);

//        phoneNumber.netType = WIFI_NETWORK;
//        phoneNumber.number = contacterNetworkServer.wifiaddress;
//        phoneNumbers.push_back(phoneNumber);

//        phoneNumber.netType = BATTLE_NETWORK;
//        phoneNumber.number = contacterNetworkServer.battleaddress;
//        phoneNumbers.push_back(phoneNumber);

//        phoneNumber.netType = DIGITAL_WALKIE_TALKIE_NETWORK;
//        phoneNumber.number = contacterNetworkServer.btaddress;
//        phoneNumbers.push_back(phoneNumber);

//        phoneNumber.netType = SATELLITE_NETWORK;
//        phoneNumber.number = contacterNetworkServer.ttaddress;
//        phoneNumbers.push_back(phoneNumber);

//        contacter.numbers = phoneNumbers;

//        m_contacterLocalList.push_back(contacter);
//    }
}
