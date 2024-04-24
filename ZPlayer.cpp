#include "ZPlayer.h"
#include <QDesktopWidget>
#include <QDesktopServices>

ZPlayer* g_pMainWnd = NULL;
bool		g_DbIsOnline = false;

const int const_mainwnd_width = 1224;
const int const_mainwnd_height = 760;

ZPlayer::ZPlayer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	g_pMainWnd = this;
	mRuntimePath = QApplication::applicationDirPath();

	QDesktopWidget desktop;
	mAvailableWidth = desktop.availableGeometry().width();
	mAvailabelHeight = desktop.availableGeometry().height();

	ui.PlayerWidget->installEventFilter(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setFixedSize(const_mainwnd_width, const_mainwnd_height);

	//设置颜色
	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(43, 43, 43)));
	setAutoFillBackground(true);
	setPalette(pal);

	//标题栏的信号与槽对应关系
	connect(ui.TitleWidget, SIGNAL(signal_show_minimum()), this, SLOT(OnMinimumWnd()));
	connect(ui.TitleWidget, SIGNAL(signal_show_maximised()), this, SLOT(OnMaximumWnd()));
	connect(ui.TitleWidget, SIGNAL(signal_show_normal()), this, SLOT(OnNormalWnd()));
	connect(ui.TitleWidget, SIGNAL(signal_close_wnd()), this, SLOT(OnCloseWnd()));

	connect(ui.PlayerWidget, SIGNAL(signal_showhide_treeview(bool)), this, SLOT(OnShowHideTreeview(bool)));
	connect(ui.PlayerWidget, SIGNAL(signal_switchOpenUrl()), this, SLOT(OnSwitchOpenUrl()));
	connect(ui.PlayerWidget, SIGNAL(signal_playermenu(int)), this, SLOT(OnPlayerMenu(int)));
	connect(ui.PlayerWidget, SIGNAL(signal_playvideo(int,int)), this, SLOT(OnPlayvideo(int,int)));
	connect(ui.PlayerWidget, SIGNAL(signal_playaudiodev(std::string)), this, SLOT(OnPlayAudioDev(std::string)));
	connect(ui.PlayerWidget, SIGNAL(signal_playaudiomodule(int)), this, SLOT(OnPlayAudioModule(int)));
	connect(ui.PlayerWidget, SIGNAL(signal_playspeed(float)), this, SLOT(OnPlaySpeed(float)));

	connect(ui.TreeWidget, SIGNAL(signal_playvideo()), this, SLOT(OnDbClickVideoFile()));
}

ZPlayer::~ZPlayer()
{
}

void ZPlayer::ExitApplication()
{
	close();
}

void ZPlayer::OnMinimumWnd()
{
	showMinimized();
}

void ZPlayer::OnMaximumWnd()
{
	showMaximized();
}

void ZPlayer::OnNormalWnd()
{
	showNormal();
}

void ZPlayer::OnCloseWnd()
{
	ExitApplication();
}


void ZPlayer::OnPlayerMenu(int type)
{
	if (type == PLAYER_MENU_OPEN_FILE)
	{
		ui.PlayerWidget->OpenVideo();
	}
	else if (type == PLAYER_MENU_OPEN_MORE_FILE)
	{
		ui.PlayerWidget->OpenMoreVideo();
	}
	else if (type == PLAYER_MENU_OPEN_FULLSCREEN)
	{
		ui.PlayerWidget->SetFullScreen(true);
	}
	else if (type == PLAYER_MENU_OPEN_EXITFULLSCREEN)
	{
		ui.PlayerWidget->SetFullScreen(false);
	}
	else if (type == PLAYER_MENU_OPEN_FRONT)
	{
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		this->show();
	}
	else if (type == PLAYER_MENU_OPEN_NEVERFRONT)
	{
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
		this->show();
	}
	else if (type == PLAYER_MENU_OPEN_PLAYSTART)
	{
		ui.PlayerWidget->ToolOperation(TOOLBAR_VIDEO_PLAY);
	}
	else if (type == PLAYER_MENU_OPEN_PLAYSTOP)
	{
		ui.PlayerWidget->ToolOperation(TOOLBAR_VIDEO_STOP);
	}
	else if (type == PLAYER_MENU_OPEN_SNAP)
	{
		ui.PlayerWidget->Snap();
	}
	else if (type == PLAYER_MENU_OPEN_ADD_SUBTITLE)
	{
		ui.PlayerWidget->addSubTitle();
	}
	else if (type == PLAYER_MENU_OPEN_URL)
	{
		ui.PlayerWidget->OpenUrl();
	}
}


void ZPlayer::OnPlayvideo(int type, int index)
{
	if (type == PLAYER_VIDEO_SCALE)
	{
		ui.PlayerWidget->setScale(index);
	}
	else if (type == PLAYER_VIDEO_RADIO)
	{
		ui.PlayerWidget->setRadio(index);
	}
	else if (type == PLAYER_VIDEO_CROP)
	{
		ui.PlayerWidget->setCrop(index);
	}
}

void ZPlayer::OnPlayAudioDev(std::string dev)
{
	ui.PlayerWidget->setAudioDev(dev);
}

void ZPlayer::OnPlayAudioModule(int index)
{
	ui.PlayerWidget->setAudioModule(index);
}

void ZPlayer::OnPlaySpeed(float speed)
{
	ui.PlayerWidget->setPlaySpeed(speed);
}

void ZPlayer::OnShowHideTreeview(bool show)
{
	ui.TreeWidget->setVisible(show);
}

void ZPlayer::OnDbClickVideoFile()
{
	ui.PlayerWidget->PlayVideoFile(ui.TreeWidget->GetSelTreeItem());
}

void ZPlayer::mousePressEvent(QMouseEvent *event)
{
	if (!event)	return;
	if (event->button() == Qt::LeftButton)
	{
		mIsPressed = true;
		mWindowPos = this->pos();
		mMousePos = event->globalPos();
		mPosDelta = mMousePos - mWindowPos;
	}
}

void ZPlayer::mouseMoveEvent(QMouseEvent *event)
{
	if (mIsPressed)
		this->move(event->globalPos() - mPosDelta);
}

void ZPlayer::mouseReleaseEvent(QMouseEvent *event)
{
	mIsPressed = false;
}

void ZPlayer::resizeEvent(QResizeEvent *event)
{
	showEvent(NULL);
}

bool ZPlayer::eventFilter(QObject *pObject, QEvent *pEvent)
{
	int nEventType = pEvent->type();
	if (pObject == ui.PlayerWidget)
	{
		switch (nEventType)
		{
		case QEvent::MouseButtonPress:
		{
			QMouseEvent *miceEvent = static_cast<QMouseEvent*>(pEvent);
			if (miceEvent && ui.PlayerWidget && miceEvent->button() == Qt::RightButton)
			{
				ui.PlayerWidget->ShowRightMenu(miceEvent->globalPos());
				return true;
			}
			break;
		}
		default:
			break;
		}
	}
	return false;
}