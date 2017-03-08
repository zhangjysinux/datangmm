#include <SyberosServiceCache>
#include "MyService_workspace.h"
#include "adhocconnectadaptor.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    AdhocConnectAdaptor::getInstance();
    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new MyService_Workspace());
}

