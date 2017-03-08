#include <QCoreApplication>
#include "messagedata.h"
#include "sender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Sender sender("239.255.43.21", 45454);
    Message message1;
    message1.content = wrapTextMessage("hello world1");
    sender.sendMessage(message1);

    Message message2;
    message2.content = wrapTextMessage("hello world2");
    sender.sendMessage(message2);

    Message message3;
    message3.content = wrapTextMessage("hello world3");
    sender.sendMessage(message3);


    return a.exec();
}

