#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>

//此类为借口类,负责给用户或界面调用
class ViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ViewModel(QObject *parent = 0);

    //生成50个数据
    void startCreateData1();

    //生成500个数据
    void startCreateData2();

    //生成5000个数据
    void startCreateData3();

    //生成本地信息
    void startCreateLocalInfomation();

    //清空所有数据
    void clearAllData();

};

#endif // VIEWMODEL_H
