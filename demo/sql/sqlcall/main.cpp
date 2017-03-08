#include <QCoreApplication>
#include <QDebug>
#include "commondata.h"
#include "contacterdata.h"
#include "contacterinterfaceadaptor.h"
#include "groupinterfaceadaptor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ContacterInterfaceAdaptor *contacterAdaptor = ContacterInterfaceAdaptor::getInstance();
    GroupInterfaceAdaptor *groupAdaptor = GroupInterfaceAdaptor::getInstance();

    qDebug() << "------------------getContacters---------------------------";
    QList<quint32> contacters = contacterAdaptor->getContacters();
    for(int i = 0, iend = contacters.count(); i < iend; i++)
        qDebug() << "contacter id:" << contacters[i];
    qDebug() << "------------------end---------------------------";

    qDebug() << "------------------getContacter----------------------------";
    for(int i = 0, iend = contacters.count(); i < iend; i++)
    {
        Contacter contacter = contacterAdaptor->getContacter(contacters[i]);
        if(contacter.id == 0)
        {
            qDebug() << "unvalid userid:" << contacters[i];
            continue;
        }

        qDebug() << "contacter id:" << contacter.id;
        qDebug() << "contacter name:" << contacter.name;
        qDebug() << "contacter title:" << contacter.title;
        qDebug() << "contacter photopath:" << contacter.photoPath;
        qDebug() << "contacter snnumber:" << contacter.snNumber;
        qDebug() << "contacter bluetooth:" << contacter.bluetooth;
        for(int j = 0, jend = contacter.numbers.count(); j < jend; j++)
            qDebug() << "contacter number:" << contacter.numbers[j].first << "|" << contacter.numbers[j].second;

        for(int j = 0, jend = contacter.leaders.count(); j < jend; j++)
            qDebug() << "contacter leader:" << contacter.leaders[j];
    }
    qDebug() << "------------------end---------------------------";

    qDebug() << "------------------addContacter----------------------------";
    Contacter addContacter;
    addContacter.id = 109;
    addContacter.name = "hello world";
    addContacter.title = SOLIDER;
    PhoneNumbers numbers;
    numbers.append(PhoneNumber((int)AD_HOC_NETWORK, "192.168.1.12"));
    addContacter.numbers = numbers;
    addContacter.leaders.append(100);
    addContacter.leaders.append(101);
    bool addFlag = contacterAdaptor->addContacter(addContacter);
    qDebug() << "addContacter flag:" << addFlag;
    qDebug() << "------------------end---------------------------";

    qDebug() << "------------------updateContacter----------------------------";
    Contacter contacter = contacterAdaptor->getContacter(101);
    contacter.name = "Li Ming";
    bool updateFlag = contacterAdaptor->updateContacter(contacter);
    qDebug() << "updateContacter flag:" << updateFlag;
    qDebug() << "------------------end---------------------------";

    qDebug() << "------------------removeContacter-----------------------------";
    bool removeFlag = contacterAdaptor->removeContacter(1099);
    qDebug() << "removeContacter flag:" << removeFlag;
    qDebug() << "------------------end---------------------------";

    qDebug() << "------------------getLocalInfo----------------------------";
    Contacter localInfo = contacterAdaptor->getLocalInfo();
    if(localInfo.id == 0)
    {
        qDebug() << "unvalid localid";
    }

    qDebug() << "local id:" << localInfo.id;
    qDebug() << "local name:" << localInfo.name;
    qDebug() << "local title:" << localInfo.title;
    qDebug() << "local photopath:" << localInfo.photoPath;
    qDebug() << "local snnumber:" << localInfo.snNumber;
    qDebug() << "local bluetooth:" << localInfo.bluetooth;
    for(int j = 0, jend = localInfo.numbers.count(); j < jend; j++)
        qDebug() << "local number:" << localInfo.numbers[j].first << "|" << localInfo.numbers[j].second;

    for(int j = 0, jend = localInfo.leaders.count(); j < jend; j++)
        qDebug() << "local leader:" << localInfo.leaders[j];
    qDebug() << "------------------end-------------------------------------";

    qDebug() << "------------------setLocalInfo----------------------------";
    localInfo.name = "Sinux Test";
    bool updateLocalInfoFlag = contacterAdaptor->updateLocalInfo(localInfo);
    qDebug() << "set local flag:" << updateLocalInfoFlag;
    qDebug() << "------------------end-------------------------------------";


    qDebug() << "------------------getGroups---------------------------";
    QList<quint32> groups = groupAdaptor->getGroups(CONVERSION_GROUP);
    for(int i = 0, iend = groups.count(); i < iend; i++)
        qDebug() << "group id:" << groups[i];
    qDebug() << "------------------end---------------------------";

    qDebug() << "------------------getGroup----------------------------";
    for(int i = 0, iend = groups.count(); i < iend; i++)
    {
        Group value = groupAdaptor->getGroup(groups[i]);
        qDebug() << "group id:" << value.id;
        qDebug() << "group name:" << value.name;
        qDebug() << "group iconpath:" << value.iconPath;
        for(int j = 0, jend = value.members.count(); j < jend; j++)
            qDebug() << "group member:" << value.members[j];
        for(int j = 0, jend = value.leaders.count(); j < jend; j++)
            qDebug() << "group leader:" << value.leaders[j];
        qDebug() << "group address:" << value.address;
        qDebug() << "group network:" << value.networkType;
        qDebug() << "group type:" << value.type;
    }
    qDebug() << "------------------end---------------------------------";

    qDebug() << "------------------addGroup----------------------------";
    Group addGroup;
    addGroup.id = 1002;
    addGroup.name = "sinux test";
    addGroup.members.append(100);
    addGroup.members.append(101);
    addGroup.members.append(102);
    addGroup.members.append(103);
    addGroup.leaders.append(100);
    addGroup.networkType = AD_HOC_NETWORK;
    addGroup.address = "192.168.1.255";
    addGroup.type = CONVERSION_GROUP;
    addFlag = groupAdaptor->addGroup(addGroup);
    qDebug() << "add gruop " << addFlag;
    qDebug() << "------------------end---------------------------------";

    qDebug() << "------------------updateGroup----------------------------";
    Group updateGroup = groupAdaptor->getGroup(1002);
    updateGroup.name = "hello sinux";
    updateFlag = groupAdaptor->updateGroup(updateGroup);
    qDebug() << "update gruop " << updateFlag;
    qDebug() << "------------------end---------------------------------";

    qDebug() << "------------------removeGroup----------------------------";
    removeFlag = groupAdaptor->removeGroup(1002);
    qDebug() << "remove gruop " << removeFlag;
    qDebug() << "------------------end---------------------------------";

    return a.exec();
}
