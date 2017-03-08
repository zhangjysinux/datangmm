#include "widget.h"
#include <assert.h>
#include <QDebug>
#include <QMessageBox>
#include "ui_widget.h"
#include "src/viewmodel.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_viewModel(NULL)
{
    ui->setupUi(this);
    connect(ui->button50, SIGNAL(clicked()), this, SLOT(onButton50Clicked()));
    connect(ui->button500, SIGNAL(clicked()), this, SLOT(onButton500Clicked()));
    connect(ui->button5000, SIGNAL(clicked()), this, SLOT(onButton5000Clicked()));
    connect(ui->localButton, SIGNAL(clicked()), this, SLOT(onLocalButtonClicked()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(onClearButtonClicked()));

    m_viewModel = new ViewModel();
}

Widget::~Widget()
{
    delete ui;

    if (m_viewModel)
    {
        delete m_viewModel;
        m_viewModel = NULL;
    }
}

void Widget::onButton50Clicked()
{
    assert(m_viewModel);

    m_viewModel->startCreateData1();
    QMessageBox::information(this, "info", "finish!!");
}

void Widget::onButton500Clicked()
{
    assert(m_viewModel);

    m_viewModel->startCreateData2();
    QMessageBox::information(this, "info", "finish!!");
}

void Widget::onButton5000Clicked()
{
    assert(m_viewModel);

    m_viewModel->startCreateData3();
    QMessageBox::information(this, "info", "finish!!");
}

void Widget::onLocalButtonClicked()
{
    assert(m_viewModel);

    m_viewModel->startCreateLocalInfomation();
    QMessageBox::information(this, "info", "finish!!");
}

void Widget::onClearButtonClicked()
{
    assert(m_viewModel);

    m_viewModel->clearAllData();
    QMessageBox::information(this, "info", "finish!!");
}


