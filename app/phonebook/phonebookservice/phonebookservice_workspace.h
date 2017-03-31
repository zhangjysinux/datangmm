#ifndef __PHONEBOOKSERVICE_WORKSPACE__
#define __PHONEBOOKSERVICE_WORKSPACE__

#include <SyberosServiceCache>
#include <cservice_application.h>
#include <cworkspace.h>
#include  "bodynetinterface.h"
#include "bodynetservice-interface.h"
using namespace SYBEROS;

class phonebookservice_Workspace : public CWorkspace
{
    Q_OBJECT

public:
    phonebookservice_Workspace();
    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);
public slots:
    void onSignalShortCutKey(int keyValue);
private:
    LocalBodyNetServiceInterface *m_bodyNetInterface;
//    BodyNetInterface *m_bodyNetInterface;

};


#endif //__PHONEBOOKSERVICE_WORKSPACE__

