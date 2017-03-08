#include <QCoreApplication>
#include "receiver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Receiver receiver("239.255.43.21", 45454);
    return a.exec();
}

