#include "testPttUi_Workspace.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    return SYBEROS::SyberosGuiCache::qApplication(argc, argv, new testPttUi_Workspace());
}

