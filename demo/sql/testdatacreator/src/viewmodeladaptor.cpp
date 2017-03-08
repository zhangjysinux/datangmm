#include "viewmodeladaptor.h"
#include "viewmodel.h"

ViewModelAdaptor::ViewModelAdaptor(QObject *parent) :
    QObject(parent),
    m_viewModel(NULL)
{
    m_viewModel = new ViewModel(parent);
}

ViewModelAdaptor::~ViewModelAdaptor()
{
    if (m_viewModel)
    {
        delete m_viewModel;
        m_viewModel = NULL;
    }
}

//生成50个数据
void ViewModelAdaptor::startCreateData1()
{
    m_viewModel->startCreateData1();
}

//生成500个数据
void ViewModelAdaptor::startCreateData2()
{
    m_viewModel->startCreateData2();
}

//生成5000个数据
void ViewModelAdaptor::startCreateData3()
{
    m_viewModel->startCreateData3();
}

//生成本地信息
void ViewModelAdaptor::startCreateLocalInfomation()
{
    m_viewModel->startCreateLocalInfomation();
}

//清空所有数据
void ViewModelAdaptor::clearAllData()
{
    m_viewModel->clearAllData();
}
