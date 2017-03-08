#include <SyberosServiceCache>
#include "CallmanagerService_workspace.h"
#include "callmanager.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    CallManager::instance();
    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new CallmanagerService_Workspace());
}

