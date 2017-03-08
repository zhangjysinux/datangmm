#ifndef __MYAPP_WORKSPACE__
#define __MYAPP_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>

#include <QCheckBox>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <pjsua.h>


class VidWin;

using namespace SYBEROS;

class MyApp_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QWidget *m_view;

public:
    bool initStack();
	void showError(const char *title, pj_status_t status);
	void showStatus(const char *msg);

	void on_reg_state(pjsua_acc_id acc_id);
	void on_call_state(pjsua_call_id call_id, pjsip_event *e);
	void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata);
	void on_call_media_state(pjsua_call_id call_id);
    void on_video_stream_data(int* acc_id);

signals:
	void signalNewCall(int, bool);
	void signalCallReleased();
	void signalInitVideoWindow();
	void signalShowStatus(const QString&);

	public slots:
	void preview();
	void call();
	void hangup();
	void quit();
	void onVidEnabledChanged(int state);

	void onNewCall(int cid, bool incoming);
	void onCallReleased();
	void initVideoWindow();
	void doShowStatus(const QString& msg);

public:
    MyApp_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

private:
	pjsua_acc_id accountId_;
	pjsua_call_id currentCall_;
	bool preview_on;

private:
	QPushButton *callButton_,
	*hangupButton_,
	*quitButton_,
	*previewButton_;
	QCheckBox   *vidEnabled_;
	QLineEdit *url_;
	VidWin *video_;
	VidWin *video_prev_;
	//QStatusBar *statusBar_;
	QLabel *statusBar_;
	QLabel *localUri_;

	QVBoxLayout *vbox_left;

	void initLayout();

};


#endif //__MYAPP_WORKSPACE__

