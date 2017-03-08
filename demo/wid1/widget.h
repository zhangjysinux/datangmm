#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class ViewModel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void onButton50Clicked();
    void onButton500Clicked();
    void onButton5000Clicked();
    void onLocalButtonClicked();
    void onClearButtonClicked();

private:
    Ui::Widget *ui;
    ViewModel *m_viewModel;
};

#endif // WIDGET_H
