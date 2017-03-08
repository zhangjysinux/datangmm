#ifndef VIEWMODELADAPTOR_H
#define VIEWMODELADAPTOR_H

#include <QObject>

class ViewModel;

class ViewModelAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit ViewModelAdaptor(QObject *parent = 0);
    ~ViewModelAdaptor();

    //生成50个数据
    Q_INVOKABLE void startCreateData1();

    //生成500个数据
    Q_INVOKABLE void startCreateData2();

    //生成5000个数据
    Q_INVOKABLE void startCreateData3();

    //生成本地信息
    Q_INVOKABLE void startCreateLocalInfomation();

    //清空所有数据
    Q_INVOKABLE void clearAllData();

private:
    ViewModel *m_viewModel;

};

#endif // VIEWMODELADAPTOR_H
