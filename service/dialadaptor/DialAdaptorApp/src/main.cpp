#include "DialAdaptorApp_workspace.h"
#include "DialManager.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QT_DTT::DialManager::instance();
    return SYBEROS::SyberosGuiCache::qApplication(argc, argv, new DialAdaptorApp_Workspace());
}

