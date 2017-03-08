#include <SyberosServiceCache>
#include "DialAdaptorService_workspace.h"
#include "adaptor/DialHandler.h"
#include "adaptor/DialManager.h"


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    DialManager::instance();
    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new DialAdaptorService_Workspace());
}

