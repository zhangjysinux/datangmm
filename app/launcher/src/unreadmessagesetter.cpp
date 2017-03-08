#include "unreadmessagesetter.h"
#include "mainviewmodel.h"

UnreadMessageSetter::UnreadMessageSetter(QObject *parent) :
    QObject(parent)
{
}

bool UnreadMessageSetter::setUnreadGroupMessageCount(int count)
{
    MainViewModel *viewModel = dynamic_cast<MainViewModel*>(this->parent()->parent());
    if (!viewModel)
        return false;

    viewModel->setUnreadGroupMessage(count);
    return true;
}

bool UnreadMessageSetter::setUnreadMessageCount(int count)
{
    MainViewModel *viewModel = dynamic_cast<MainViewModel*>(this->parent()->parent());
    if (!viewModel)
        return false;

    viewModel->setUnreadMessage(count);
    return true;
}
