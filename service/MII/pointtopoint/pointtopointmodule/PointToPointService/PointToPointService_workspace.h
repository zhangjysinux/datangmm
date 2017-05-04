#ifndef __POINTTOPOINTSERVICE_WORKSPACE__
#define __POINTTOPOINTSERVICE_WORKSPACE__

#include <SyberosServiceCache>
#include <cservice_application.h>
#include <cworkspace.h>

using namespace SYBEROS;

class PointToPointService_Workspace : public CWorkspace
{
    Q_OBJECT

public:
    PointToPointService_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

};


#endif //__POINTTOPOINTSERVICE_WORKSPACE__

