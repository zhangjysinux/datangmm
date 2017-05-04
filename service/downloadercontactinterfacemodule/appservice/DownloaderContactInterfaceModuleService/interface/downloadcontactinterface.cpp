#include "downloadcontactinterface.h"

DownloadContactInterface* DownloadContactInterface::instance = NULL;

DownloadContactInterface* DownloadContactInterface::getInstance()
{
    if(instance == NULL)
        instance = new DownloadContactInterface();

    return instance;
}

DownloadContactInterface::DownloadContactInterface(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<ContacterServerList>("ContacterServerList");
    qRegisterMetaType<IdContacterNetworkServerMap>("IdContacterNetworkServerMap");
    qRegisterMetaType<IdContacterPhotoNameMap>("IdContacterPhotoNameMap");
    qRegisterMetaType<GroupOrganizationServerList>("GroupOrganizationServerList");
    qRegisterMetaType<GroupBattleServerList>("GroupBattleServerList");

    m_downloadContact = DownloadContact::getInstance();
    connect(m_downloadContact, SIGNAL(signalContactersDownload(ContacterServerList)),
            this, SLOT(onSignalContactersDownload(ContacterServerList)));
    connect(m_downloadContact, SIGNAL(signalContacterNetworksDownload(IdContacterNetworkServerMap)),
            this, SLOT(onSignalContacterNetworksDownload(IdContacterNetworkServerMap)));
    connect(m_downloadContact, SIGNAL(signalContacterPhotoDownload(IdContacterPhotoNameMap)),
            this, SLOT(onSignalContacterPhotoDownload(IdContacterPhotoNameMap)));
    connect(m_downloadContact, SIGNAL(signalGroupsDownload(GroupOrganizationServerList)),
            this, SLOT(onSignalGroupsDownload(GroupOrganizationServerList)));
    connect(m_downloadContact, SIGNAL(signalGroupBattlesDownload(GroupBattleServerList)),
            this, SLOT(onSignalGroupBattlesDownload(GroupBattleServerList)));

    connect(m_downloadContact, SIGNAL(signalPostFinished(int,int)),
            this, SLOT(onSignalPostFinished(int,int)));
}

void DownloadContactInterface::onStartGet()
{
    m_idContacterPhotoNameMap.clear();

    m_downloadContact->login(m_loginData);
    m_downloadContact->getContacts();
    m_downloadContact->getContacterNetworks();

    m_downloadContact->onStart();
}

void DownloadContactInterface::onPostContact(const Contacter &contacter)
{
    /**
     * @brief contacterServer
     * post must be divided to three parts
     */
    m_postContacter = contacter;

    ContacterServer contacterServer;
    contacterServer.id = contacter.id;
    contacterServer.surname = contacter.surname;
    contacterServer.name = contacter.name;
    contacterServer.title = QString::number(contacter.title);
    contacterServer.leaders = convertLeadersToString(contacter.leaders);
    contacterServer.snnumber = contacter.snNumber;
    contacterServer.photoid = contacter.photoPath;

    ContacterNetworkServer network;
    network.networkid = contacter.id;
    network.adhocaddress = getPhoneNumberType(contacter.numbers,
                                              AD_HOC_NETWORK).number;
    network.battleaddress =getPhoneNumberType(contacter.numbers,
                                              BATTLE_NETWORK).number;
    network.bluetooth = contacter.bluetooth;
    network.btaddress = getPhoneNumberType(contacter.numbers,
                                           DIGITAL_WALKIE_TALKIE_NETWORK).number;
    network.lteaddress = getPhoneNumberType(contacter.numbers,
                                            LTE_NETWORK).number;
    network.ttaddress = getPhoneNumberType(contacter.numbers,
                                           SATELLITE_NETWORK).number;
    network.wifiaddress = getPhoneNumberType(contacter.numbers,
                                             WIFI_NETWORK).number;

    ContacterPhotoServer photo;
    photo.phototid = contacter.id;
    photo.photodata = contacter.photoPath.toLocal8Bit();

    m_downloadContact->login(m_loginData);
    m_downloadContact->postContact(contacterServer);
    m_downloadContact->postContacterNetwork(network);
    m_downloadContact->postContacterPhoto(photo);
    m_downloadContact->onStart();
}

void DownloadContactInterface::onPostGroup(const Group &group)
{
    m_postGroup = group;

    GroupOrganizationServer groupServer;
    groupServer.groupid = group.id;
    groupServer.address = group.address;
    groupServer.creator = group.creator;
    groupServer.leaders = convertGroupLeadersToString(group.leaders);
    groupServer.members = convertMembersToString(group.members);
    groupServer.name = group.name;
    groupServer.network = group.networkType;
    groupServer.port = group.port;

    m_downloadContact->login(m_loginData);

    m_downloadContact->postGroup(groupServer);

    m_downloadContact->onStart();
}

void DownloadContactInterface::onPostGroupBattle(const Group &group)
{
    m_postGroupBattle = group;

    GroupBattleServer groupServer;
    groupServer.battleid = group.id;
    groupServer.address = group.address;
    groupServer.creator = group.creator;
    groupServer.leaders = convertGroupLeadersToString(group.leaders);
    groupServer.members = convertMembersToString(group.members);
    groupServer.name = group.name;
    groupServer.network = group.networkType;
    groupServer.port = group.port;

    m_downloadContact->login(m_loginData);

    m_downloadContact->postBattleGroup(groupServer);

    m_downloadContact->onStart();
}

void DownloadContactInterface::onSignalContactersDownload(const ContacterServerList &cList)
{
    m_contacterServerList = cList;
    foreach(const ContacterServer &c, cList)
    {
        m_downloadContact->getContacterPhoto(c.id);
    }
//    m_downloadContact->login(m_loginData);
    m_downloadContact->getGroups();
    m_downloadContact->getBattleGroup();

    m_downloadContact->onStart();
}

void DownloadContactInterface::onSignalContacterNetworksDownload(const IdContacterNetworkServerMap &idNetworkMap)
{
    m_idContacterNetworkServerMap = idNetworkMap;
}

void DownloadContactInterface::onSignalContacterPhotoDownload(const IdContacterPhotoNameMap &idPhotoMap)
{
    for(IdContacterPhotoNameMap::const_iterator it=idPhotoMap.begin(); it!=idPhotoMap.end(); it++)
    {
        m_idContacterPhotoNameMap.insert(it.key(), it.value());
    }
}

void DownloadContactInterface::onSignalGroupsDownload(const GroupOrganizationServerList &gList)
{
    m_groupOrganizationServerList = gList;

    GroupList localGroupList;
    for(GroupOrganizationServerList::const_iterator it=gList.begin(); it!=gList.end(); it++)
    {
        Group localGroup;
        localGroup.id = it->groupid;
        localGroup.address = it->address;
        localGroup.creator = it->creator;
        localGroup.leaders = convertStringToGroupLeaders(it->leaders);
        localGroup.members = convertStringToMembers(it->members);
        localGroup.name = it->name;
        localGroup.networkType = convertStringToNetworkType(it->network);
        localGroup.port = it->port.toUInt();
        localGroup.type = SUBORDINATE_GROUP;

        localGroupList.push_back(localGroup);
    }

    emit signalGroupsDownloadFinished(localGroupList);


    /**
      contacter download finished
      */
    ContacterList localContacterList;
    for(ContacterServerList::iterator it=m_contacterServerList.begin();
        it!=m_contacterServerList.end();
        it++)
    {
        const ContacterNetworkServer contacterNetworkServer = m_idContacterNetworkServerMap.value(it->id, ContacterNetworkServer());
        const QString photoName = m_idContacterPhotoNameMap.value(it->id, QString());

        Contacter contacter;
        contacter.id = it->id;
        contacter.leaders = convertStringToLeaders(it->leaders);
        contacter.name = it->name;
        contacter.surname = it->surname;
        contacter.snNumber = it->snnumber;
        contacter.photoPath = it->photoid;
        contacter.title = it->title.toInt();
        contacter.type = CONTACTERTYPE_ORGANIZATION;

        contacter.bluetooth = contacterNetworkServer.bluetooth;

        contacter.photoPath = photoName;

        PhoneNumbers phoneNumbers;
        PhoneNumber phoneNumber;
        phoneNumber.netType = AD_HOC_NETWORK;
        phoneNumber.number = contacterNetworkServer.adhocaddress;
        phoneNumbers.push_back(phoneNumber);

        phoneNumber.netType = LTE_NETWORK;
        phoneNumber.number = contacterNetworkServer.lteaddress;
        phoneNumbers.push_back(phoneNumber);

        phoneNumber.netType = WIFI_NETWORK;
        phoneNumber.number = contacterNetworkServer.wifiaddress;
        phoneNumbers.push_back(phoneNumber);

        phoneNumber.netType = BATTLE_NETWORK;
        phoneNumber.number = contacterNetworkServer.battleaddress;
        phoneNumbers.push_back(phoneNumber);

        phoneNumber.netType = DIGITAL_WALKIE_TALKIE_NETWORK;
        phoneNumber.number = contacterNetworkServer.btaddress;
        phoneNumbers.push_back(phoneNumber);

        phoneNumber.netType = SATELLITE_NETWORK;
        phoneNumber.number = contacterNetworkServer.ttaddress;
        phoneNumbers.push_back(phoneNumber);

        contacter.numbers = phoneNumbers;
        localContacterList.push_back(contacter);
    }
    emit signalContactersDownloadFinished(localContacterList);
}

void DownloadContactInterface::onSignalGroupBattlesDownload(const GroupBattleServerList &gList)
{
    m_groupBattleServerList = gList;

    GroupList localGroupList;
    for(GroupBattleServerList::const_iterator it=gList.begin(); it!=gList.end(); it++)
    {
        Group localGroup;
        localGroup.id = it->battleid;
        localGroup.address = it->address;
        localGroup.creator = it->creator;
        localGroup.leaders = convertStringToGroupLeaders(it->leaders);
        localGroup.members = convertStringToMembers(it->members);
        localGroup.name = it->name;
        localGroup.networkType = convertStringToNetworkType(it->network);
        localGroup.port = it->port.toUInt();
        localGroup.type = BATTLE_GROUP;

        localGroupList.push_back(localGroup);
    }

    emit signalGroupBattlesDownloadFinished(localGroupList);
    emit signalGetFinished(1);
}

void DownloadContactInterface::onSignalPostFinished(int type, int status)
{
    switch (type) {
    case RemoteContactType_PostContacterPhoto:
    {
        emit signalPostContactFinished(status, m_postContacter);
        break;
    }
    case RemoteContactType_PostGroups:
    {
        emit signalPostGroupFinished(status, m_postGroup);
        break;
    }
    case RemoteContactType_PostGroupsBattle:
    {
        emit signalPostGroupBattleFinished(status, m_postGroupBattle);
        break;
    }
    default:
        break;
    }
}
