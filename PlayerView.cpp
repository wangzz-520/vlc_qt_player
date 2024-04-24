#include "PlayerView.h"
#include "Player.h"
#include "ZPlayer.h"
#include "WNetDialog.h"
#include <QDesktopWidget>

const int const_mainwnd_height = 40;
const int const_menubtn_width = 39;
const int const_menubtn_height = 26;
const int const_modebtn_width = 81;
const int const_modebtn_height = 30;
const int const_operabtn_width = 20;
const int const_operabtn_height = 20;
const int toolbar_height = 90;

Player	g_AVPlayer;
extern ZPlayer* g_pMainWnd;
extern QString gCurrentTreeItemPath;

BOOL CALLBACK EnumerateVLC(HWND hWndvlc, LPARAM lParam)
{
	TCHAR szWndTitle[1024];
	int nLen = GetWindowText(hWndvlc, szWndTitle, 1024);
	if (0 != nLen)
	{
		//禁用鼠标消息
		EnableWindow(hWndvlc, FALSE);
	}
	return TRUE;
}


PlayerView::PlayerView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QDesktopWidget desktop;
	mAvailableWidth = desktop.width();
	mAvailabelHeight = desktop.height();
	mMediaFile.empty();

	g_AVPlayer.SetHWND((void*)ui.ActivePage->winId());

	m_pVideoTimer = new QTimer(this);
	m_pVideoTimer->setInterval(1000);

	m_pEnumTimer = new QTimer(this);
	m_pEnumTimer->setInterval(1000);

	m_pEnumTimer->start();
	ui.ActivePage->installEventFilter(this);

	connect(ui.ToolbarWidget, SIGNAL(signal_click_show_hide_treeview(bool)), this, SLOT(slot_click_show_hide_treeview(bool)));
	connect(ui.ToolbarWidget, SIGNAL(signal_toolbar_video_operation(int)), this, SLOT(slot_toolbar_operation(int)));
	connect(ui.ToolbarWidget, SIGNAL(signal_voice_value_changed(int)), this, SLOT(slot_volume_changed(int)));
	connect(ui.ToolbarWidget, SIGNAL(signal_video_value_changed(int)), this, SLOT(slot_video_changed(int)));
	connect(ui.ToolbarWidget, SIGNAL(signal_video_slider_behavior(int)), this, SLOT(slot_video_slider_behavior(int)));
	connect(m_pVideoTimer, SIGNAL(timeout()), this, SLOT(slot_video_timer()));
	connect(m_pEnumTimer, SIGNAL(timeout()), this, SLOT(slot_enum_timer()));
	connect(ui.ActivePage, SIGNAL(signal_openvideobtn_clicked()), this, SLOT(slot_openvideobtn_clicked()));
	connect(ui.ActivePage, SIGNAL(signal_openmenubtn_clicked(int)), this, SLOT(slot_openmenubtn_clicked(int)));
}

PlayerView::~PlayerView()
{
}

void PlayerView::showEvent(QShowEvent *event)
{
	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(7, 7, 7)));
	setAutoFillBackground(true);
	setPalette(pal);
}

bool PlayerView::PlayStart(std::string mediaFile, int nWidth, int nHeight)
{
	if (mediaFile.empty())
		return false;

	bool bRet = false;
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.Stop();
		mVideoStatus = ENUM_PLAYSTATUS_STOP;
	}
	if (g_AVPlayer.Play(mediaFile))
	{
		g_AVPlayer.Volume(25);
		bRet = true;
		mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
		ui.ActivePage->SetActiveMode(true);
		ui.ToolbarWidget->SetPlayUi(true);
		mMediaFile = mediaFile;
		m_pVideoTimer->start();
	}
	else
	{
		ui.ActivePage->SetActiveMode(false);
	}

	return bRet;
}

bool PlayerView::PlayVideoFile(VIDEO_TREE_ITEM videoItem)
{
	QString fileName = QString::fromLocal8Bit(videoItem.videoFilePath.c_str());

	// 验证地址是网络地址，还是本地地址
	bool bURL = false;
	std::vector<std::string> vctURL;

	vctURL.push_back("http");
	vctURL.push_back("https");
	vctURL.push_back("ftp");
	vctURL.push_back("rstp");
	vctURL.push_back("rtmp");

	for (unsigned i = 0; i < vctURL.size(); i++)
	{
		// 实际使用请判断大小写
		if (!fileName.toStdString().compare(0, vctURL[i].size(), vctURL[i]))
		{
			bURL = true;
			break;
		}
	}
	if(!bURL)
		fileName.replace("/", "\\");
	QByteArray cdata = fileName.toUtf8();
	std::string mediafile(cdata);
	return PlayStart(mediafile, ui.ActivePage->width(), ui.ActivePage->height());
}

void PlayerView::DockToolbar(bool IsDock)
{
	if (IsDock)
	{
		ui.ToolbarWidget->setParent(NULL);
		ui.ToolbarWidget->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
		ui.ToolbarWidget->setGeometry(0, mAvailabelHeight - toolbar_height, mAvailableWidth, toolbar_height);
		ui.ToolbarWidget->show();
	}
	else
	{
		ui.verticalLayout_2->addWidget(ui.ToolbarWidget);
		ui.ToolbarWidget->setParent(this);
		//ui.ToolbarWidget->showNormal();
		ui.ToolbarWidget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
		ui.ToolbarWidget->setGeometry(mToolbarOldSize);
		ui.ToolbarWidget->show();
	}
}

void PlayerView::OpenVideo()
{
	QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("打开"), "", QStringLiteral("媒体文件（所有文件）") + ";*.*");
	if (filePath.isEmpty() || !g_pMainWnd)
	{
		return;
	}

	vector<VIDEO_TREE_ITEM> vecVideoList;
	g_pMainWnd->GetTreeViewInst()->GetVecVideoList(vecVideoList);
	int videoIdStart = vecVideoList.size();
	VIDEO_TREE_ITEM videoInfo;
	QFileInfo fileInfo(filePath);
	QString fileName = fileInfo.fileName();
	QString videoIdQ = "video_" + QString::number(videoIdStart);
	QString videoSet = "videoset_0";
	videoInfo.videoId = std::string(videoIdQ.toLocal8Bit().data());
	videoInfo.videoName = std::string(fileName.toLocal8Bit().data());
	videoInfo.videoFilePath = std::string(filePath.toLocal8Bit().data());
	videoInfo.videoPlayTimeStamp = 0;
	videoInfo.setId = std::string(videoSet.toLocal8Bit().data());

	g_pMainWnd->GetTreeViewInst()->AddVideoItem(videoInfo);
	g_pMainWnd->GetTreeViewInst()->PushVideoItem(videoInfo);

	PlayVideoFile(videoInfo);
}


void PlayerView::OpenUrl()
{
	WNetDialog w;
	if (w.exec() == QDialog::Accepted)
	{
		vector<VIDEO_TREE_ITEM> vecVideoList;
		g_pMainWnd->GetTreeViewInst()->GetVecVideoList(vecVideoList);
		int videoIdStart = vecVideoList.size();
		VIDEO_TREE_ITEM videoInfo;

		QString fileName = w.getNetPath();
		QString videoIdQ = "video_" + QString::number(videoIdStart);
		QString videoSet = "videoset_0";
		videoInfo.videoId = std::string(videoIdQ.toLocal8Bit().data());
		videoInfo.videoName = std::string(fileName.toLocal8Bit().data());
		videoInfo.videoFilePath = std::string(fileName.toLocal8Bit().data());
		videoInfo.videoPlayTimeStamp = 0;
		videoInfo.setId = std::string(videoSet.toLocal8Bit().data());

		g_pMainWnd->GetTreeViewInst()->AddVideoItem(videoInfo);
		g_pMainWnd->GetTreeViewInst()->PushVideoItem(videoInfo);

		PlayVideoFile(videoInfo);
	}
}

void PlayerView::OpenMoreVideo()
{
	QStringList filePaths = QFileDialog::getOpenFileNames(this, QStringLiteral("打开"), "", QStringLiteral("媒体文件（所有文件）") + ";*.*");
	if (filePaths.isEmpty() || !g_pMainWnd)
	{
		return;
	}

	for (int i = 0; i < filePaths.size(); i++)
	{
		QString filePath = filePaths.at(i);
		vector<VIDEO_TREE_ITEM> vecVideoList;
		g_pMainWnd->GetTreeViewInst()->GetVecVideoList(vecVideoList);
		int videoIdStart = vecVideoList.size();
		VIDEO_TREE_ITEM videoInfo;
		QFileInfo fileInfo(filePath);
		QString fileName = fileInfo.fileName();
		QString videoIdQ = "video_" + QString::number(videoIdStart);
		QString videoSet = "videoset_0";
		videoInfo.videoId = std::string(videoIdQ.toLocal8Bit().data());
		videoInfo.videoName = std::string(fileName.toLocal8Bit().data());
		videoInfo.videoFilePath = std::string(filePath.toLocal8Bit().data());
		videoInfo.videoPlayTimeStamp = 0;
		videoInfo.setId = std::string(videoSet.toLocal8Bit().data());

		g_pMainWnd->GetTreeViewInst()->AddVideoItem(videoInfo);
		g_pMainWnd->GetTreeViewInst()->PushVideoItem(videoInfo);

		if (i == filePaths.size() - 1)
		{
			PlayVideoFile(videoInfo);
		}
	}
}

void PlayerView::SetFullScreen(bool fullscreen)
{
	DockToolbar(fullscreen);
	m_enterFullscreen = fullscreen;
	ui.ToolbarWidget->SetFullScreenUi(fullscreen);

	if (fullscreen)
	{
		mToolbarOldSize = ui.ToolbarWidget->geometry();
		mActivePageOldSize = ui.ActivePage->geometry();
		ui.ActivePage->setParent(NULL);
		g_pMainWnd->hide();
		ui.ActivePage->showFullScreen();
		ui.ToolbarWidget->hide();
	}
	else
	{
		ui.ActivePage->setParent(ui.stackedWidget);
		ui.ActivePage->setGeometry(mActivePageOldSize);
		g_pMainWnd->show();
		slot_click_show_hide_treeview(false);
		slot_click_show_hide_treeview(true);
		ui.ToolbarWidget->SetHideTreeBtn(false);
		ui.ToolbarWidget->show();
	}
}

void PlayerView::Snap()
{
	if (g_AVPlayer.IsPlaying())
	{
		// 获取当前时间
		QDateTime dTime = QDateTime::currentDateTime();
		// 格式化
		QString fullPath = "picture_" + dTime.toString("yyMMddhhmmss.jpg");
		// 保存文件全路径
		QString strDir;
		QString appPath = QApplication::applicationDirPath();
		appPath.append("/picture/");
		appPath.append(fullPath);
		if (0 != g_AVPlayer.SnapShoot(fullPath.toLocal8Bit().data()))
		{
			/*MyTipsBox::showMessage(QStringLiteral("截图"),
				QStringLiteral("截图失败，请重试"),
				this, QStringLiteral("提示"));*/
		}
	}
	else
	{
		/*MyTipsBox::showMessage(QStringLiteral("截图"),
			QStringLiteral("请先开启视频再进行截图"),
			this, QStringLiteral("提示"));*/
	}
}

void PlayerView::ShowRightMenu(QPoint point)
{
	QAction *pActionOpenFile = new QAction(QStringLiteral("打开文件"), this);
	QAction *pActionOpenMoreFile = new QAction(QStringLiteral("打开多个文件"), this);
	QAction *pActionOpenUrl = new QAction(QStringLiteral("打开URL"), this);
	QAction *pActionFullscreen = new QAction(QStringLiteral("全屏"), this);
	QAction *pActionExitFullscreen = new QAction(QStringLiteral("退出全屏"), this);
	QAction *pActionSubTitle = new QAction(QStringLiteral("添加字幕文件"), this);
	QAction *pActionFront = new QAction(QStringLiteral("总是最前"), this);
	QAction *pActionNeverFront = new QAction(QStringLiteral("取消最前"), this);	

	QMenu *pMenu = new QMenu(this);
	QMenu *pMenu2 = new QMenu(QStringLiteral("打开"), this);
	QMenu *pMenu3= new QMenu(QStringLiteral("视频"), this);
	QMenu *pMenu4 = new QMenu(QStringLiteral("缩放"), this);	//缩放
	QMenu *pMenu5 = new QMenu(QStringLiteral("宽高比"), this);	//宽高比
	QMenu *pMenu6 = new QMenu(QStringLiteral("裁剪"), this);	//裁剪
	QMenu *pMenu7 = new QMenu(QStringLiteral("音频"), this);
	QMenu *pMenu8 = new QMenu(QStringLiteral("音频设备"), this);
	QMenu *pMenu9 = new QMenu(QStringLiteral("立体声模式"), this);
	QMenu *pMenu10 = new QMenu(QStringLiteral("速度"), this);
	QAction *pActionCapture = new QAction(QStringLiteral("截图"), this);

	pMenu->addMenu(pMenu2);
	pMenu2->addAction(pActionOpenFile);
	pMenu2->addAction(pActionOpenMoreFile);
	pMenu2->addAction(pActionOpenUrl);
	connect(pActionOpenFile, SIGNAL(triggered()), this, SLOT(slot_playermenu_openfile()));
	connect(pActionOpenMoreFile, SIGNAL(triggered()), this, SLOT(slot_playermenu_openmorefile()));
	connect(pActionOpenUrl, SIGNAL(triggered()), this, SLOT(slot_playermenu_openurl()));

	pMenu->addMenu(pMenu3);
	pMenu3->addMenu(pMenu4);
	QAction *m_scale1_4 = pMenu4->addAction(QStringLiteral("1:4 四分之一"));
	QAction *m_scale1_2 = pMenu4->addAction(QStringLiteral("1:2 二分之一"));
	QAction *m_scale1_1 = pMenu4->addAction(QStringLiteral("1:1 原始"));
	QAction *m_scale2_1 = pMenu4->addAction(QStringLiteral("2:1 双倍"));
	m_scale1_4->setData(PLAYER_SCALE_1_4);
	m_scale1_2->setData(PLAYER_SCALE_1_2);
	m_scale1_1->setData(PLAYER_SCALE_1_1);
	m_scale2_1->setData(PLAYER_SCALE_2_1);

	pMenu3->addMenu(pMenu5);
	QAction *m_default = pMenu5->addAction(QStringLiteral("默认"));
	QAction *m_16_9 = pMenu5->addAction(QStringLiteral("16:9"));
	QAction *m_4_3 = pMenu5->addAction(QStringLiteral("4:3"));
	QAction *m_1_1 = pMenu5->addAction(QStringLiteral("1:1"));
	QAction *m_16_10 = pMenu5->addAction(QStringLiteral("16:10"));
	QAction *m_2_21_1 = pMenu5->addAction(QStringLiteral("2.21:1"));
	QAction *m_2_35_1 = pMenu5->addAction(QStringLiteral("2.35:1"));
	QAction *m_2_38_1 = pMenu5->addAction(QStringLiteral("2.38:1"));
	QAction *m_5_4  = pMenu5->addAction(QStringLiteral("5:4"));
	m_default->setData(PLAYER_RADIO_DEFULT);
	m_16_9->setData(PLAYER_RADIO_16_9);
	m_4_3->setData(PLAYER_RADIO_4_3);
	m_1_1->setData(PLAYER_RADIO_1_1);
	m_16_10->setData(PLAYER_RADIO_16_10);
	m_2_21_1->setData(PLAYER_RADIO_2_21_1);
	m_2_35_1->setData(PLAYER_RADIO_2_35_1);
	m_2_38_1->setData(PLAYER_RADIO_2_38_1);
	m_5_4->setData(PLAYER_RADIO_5_4);

	pMenu3->addMenu(pMenu6);
	QAction *m__cropDefault = pMenu6->addAction(QStringLiteral("默认"));
	QAction *m__crop16_10 = pMenu6->addAction("16:10");
	QAction *m__crop16_9 = pMenu6->addAction("16:9");
	QAction *m__crop4_3 = pMenu6->addAction("4:3");
	QAction *m__crop1_85_1 = pMenu6->addAction("1.85:1");
	QAction *m__crop2_21_1 = pMenu6->addAction("2.21:1");
	QAction *m__crop2_35_1 = pMenu6->addAction("2.35:1");
	QAction *m__crop2_39_1 = pMenu6->addAction("2.39:1");
	QAction *m__crop5_3 = pMenu6->addAction("5:3");
	QAction *m__crop5_4 = pMenu6->addAction("5:4");
	QAction *m__crop1_1 = pMenu6->addAction("1:1");

	m__cropDefault->setData(PLAYER_CROP_DEFULT);
	m__crop16_10->setData(PLAYER_CROP_16_10);
	m__crop16_9->setData(PLAYER_CROP_16_9);
	m__crop4_3->setData(PLAYER_CROP_4_3);
	m__crop1_85_1->setData(PLAYER_CROP_1_85_1);
	m__crop2_21_1->setData(PLAYER_CROP_2_21_1);
	m__crop2_35_1->setData(PLAYER_CROP_2_35_1);
	m__crop2_39_1->setData(PLAYER_CROP_2_39_1);
	m__crop5_3->setData(PLAYER_CROP_5_3);
	m__crop5_4->setData(PLAYER_CROP_5_4);
	m__crop1_1->setData(PLAYER_CROP_1_1);

	pMenu3->addAction(pActionCapture);

	QList<QAction *> lstAction;
	pMenu->addMenu(pMenu7);
	pMenu7->addMenu(pMenu8);
	for (int i = 0; i < g_lstAudioDeviceDes.size(); i++)
	{
		QAction *action = pMenu8->addAction(QString::fromStdString(g_lstAudioDeviceDes.at(i)));
		action->setData(QString::fromStdString(g_lstAudioDeviceName.at(i)));
		connect(action, SIGNAL(triggered()), this, SLOT(slot_playmenu_audiodev()));
		lstAction.append(action);
	}

	pMenu7->addMenu(pMenu9);
	QAction *m_actionMono = pMenu9->addAction(QStringLiteral("单声道"));
	QAction *m_actionStereo = pMenu9->addAction(QStringLiteral("立体声"));
	QAction *m_actionLeft = pMenu9->addAction(QStringLiteral("左"));
	QAction *m_actionRight = pMenu9->addAction(QStringLiteral("右"));
	QAction *m_actionRevStereo = pMenu9->addAction(QStringLiteral("反转立体声"));

	m_actionMono->setData((int)AudioChannel_Dolbys);
	m_actionStereo->setData((int)AudioChannel_Stereo);
	m_actionLeft->setData((int)AudioChannel_Left);
	m_actionRight->setData((int)AudioChannel_Right);
	m_actionRevStereo->setData((int)AudioChannel_RStereo);

	pMenu->addMenu(pMenu10);
	QStringList lstRate;
	QList<float> m_lstRate;
	lstRate<<"2.0x" << "1.5x" << "1.25x" << "1.0x" << "0.75x" << "0.5x";
	m_lstRate << 2.0 << 1.5 << 1.25 << 1.0 << 0.75 << 0.5;
	for (int i = 0; i < lstRate.size(); i++)
	{
		QAction *action = pMenu10->addAction(lstRate.at(i));
		action->setData(m_lstRate.at(i));
		connect(action, SIGNAL(triggered()), this, SLOT(slot_playmenu_speed()));
		lstAction.append(action);
	}

	pMenu->addAction(pActionSubTitle);
	if (m_enterFullscreen)
		pMenu->addAction(pActionExitFullscreen);
	else
		pMenu->addAction(pActionFullscreen);

	pMenu->addAction(pActionFront);
	pMenu->addAction(pActionNeverFront);

	connect(pActionSubTitle, SIGNAL(triggered()), this, SLOT(slot_playermenu_addsubtitle()));
	connect(pActionFullscreen, SIGNAL(triggered()), this, SLOT(slot_playermenu_fullscreen()));
	connect(pActionExitFullscreen, SIGNAL(triggered()), this, SLOT(slot_playermenu_exitfullscreen()));
	connect(pActionFront, SIGNAL(triggered()), this, SLOT(slot_playermenu_front()));
	connect(pActionNeverFront, SIGNAL(triggered()), this, SLOT(slot_playermenu_neverfront()));
	connect(pActionCapture, SIGNAL(triggered()), this, SLOT(slot_playermenu_snap()));

	connect(m_scale1_4, SIGNAL(triggered()), this, SLOT(slot_playmenu_scale()));
	connect(m_scale1_2, SIGNAL(triggered()), this, SLOT(slot_playmenu_scale()));
	connect(m_scale1_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_scale()));
	connect(m_scale2_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_scale()));

	connect(m_default, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_16_9, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_4_3, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_1_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_16_10, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_2_21_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_2_35_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_2_38_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));
	connect(m_5_4, SIGNAL(triggered()), this, SLOT(slot_playmenu_radio()));

	connect(m__cropDefault, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop16_10, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop16_9, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop4_3, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop1_85_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop2_21_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop2_35_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop2_39_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop5_3, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop5_4, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));
	connect(m__crop1_1, SIGNAL(triggered()), this, SLOT(slot_playmenu_crop()));

	connect(m_actionMono, SIGNAL(triggered()), this, SLOT(slot_playmenu_audiomodule()));
	connect(m_actionStereo, SIGNAL(triggered()), this, SLOT(slot_playmenu_audiomodule()));
	connect(m_actionLeft, SIGNAL(triggered()), this, SLOT(slot_playmenu_audiomodule()));
	connect(m_actionRight, SIGNAL(triggered()), this, SLOT(slot_playmenu_audiomodule()));
	connect(m_actionRevStereo, SIGNAL(triggered()), this, SLOT(slot_playmenu_audiomodule()));
	lstAction.append(m_actionMono);
	lstAction.append(m_actionStereo);
	lstAction.append(m_actionLeft);
	lstAction.append(m_actionRight);
	lstAction.append(m_actionRevStereo);

	pMenu->exec(point);

	for (int i = 0; i < lstAction.size(); i++)
	{
		delete lstAction.at(i);
	}
	delete m_scale1_4;
	delete m_scale1_2;
	delete m_scale1_1;
	delete m_scale2_1;
	delete m_default;
	delete m_16_9;
	delete m_4_3;
	delete m_1_1;
	delete m_16_10;
	delete m_2_21_1;
	delete m_2_35_1;
	delete m_2_38_1;
	delete m_5_4;
	delete m__cropDefault;
	delete m__crop16_10;
	delete m__crop16_9;
	delete m__crop4_3;
	delete m__crop1_85_1;
	delete m__crop2_21_1;
	delete m__crop2_35_1;
	delete m__crop2_39_1;
	delete m__crop5_3;
	delete m__crop5_4;
	delete m__crop1_1;
	delete pActionOpenFile;
	delete pActionOpenUrl;
	delete pActionOpenMoreFile;
	delete pActionFullscreen;
	delete pActionFront;
	delete pActionNeverFront;
	delete pActionCapture;
	delete pMenu2;
	delete pMenu;
	delete pMenu3;
	delete pMenu4;
	delete pMenu5;
	delete pMenu6;
	delete pMenu7;
	delete pMenu8;
	delete pMenu9;
}

void PlayerView::ToolOperation(int type)
{
	int curIdx;
	VIDEO_TREE_ITEM treeItem;
	if (type == TOOLBAR_VIDEO_AUDIO_CLOSE)
	{
		if (g_AVPlayer.IsPlaying())
		{
			mVideoVolume = g_AVPlayer.GetVolume();
			g_AVPlayer.Volume(0);
			ui.ToolbarWidget->SetVolumeUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_AUDIO_OPEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.Volume(mVideoVolume);
			ui.ToolbarWidget->SetVolumeUi(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_FAST)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekForward();
		}
	}
	else if (type == TOOLBAR_VIDEO_FULLSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_NORMALSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_STOP)
	{
		if (g_AVPlayer.IsPlaying() || mVideoStatus != ENUM_PLAYSTATUS_STOP)
		{
			g_AVPlayer.Stop();
			mVideoStatus = ENUM_PLAYSTATUS_STOP;
			ui.ToolbarWidget->SetPlayUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_PLAY)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				g_AVPlayer.Play();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ActivePage->SetActiveMode(true);
				ui.ToolbarWidget->SetPlayUi(true);
			}
		}
		else
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_PAUSE)
			{
				g_AVPlayer.Resume();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ToolbarWidget->SetPlayUi(true);
			}
			else if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				if (!gCurrentTreeItemPath.isEmpty())
				{
					QString fileName = gCurrentTreeItemPath;
					fileName.replace("/", "\\");
					QByteArray cdata = fileName.toUtf8();
					std::string mediafile(cdata);
					mMediaFile = mediafile;
				}
				PlayStart(mMediaFile, ui.ActivePage->width(), ui.ActivePage->height());
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_PAUSE)
	{
		if (g_AVPlayer.IsPlaying() && mVideoStatus != ENUM_PLAYSTATUS_PAUSE)
		{
			g_AVPlayer.Pause();
			mVideoStatus = ENUM_PLAYSTATUS_PAUSE;
			ui.ToolbarWidget->SetPlayUi(false, true);
		}
	}
	else if (type == TOOLBAR_VIDEO_SLOW)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekBackward();
		}
	}
	else if (type == TOOLBAR_VIDEO_PREVIOUS)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetPreviousItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx - 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_NEXT)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetNextItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx + 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_OPEN)
	{
		OpenVideo();
	}
}


void PlayerView::setScale(int index)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.setScale((PLAYER_VIDEO_SCALE_TYPE)index);
	}
}


void PlayerView::setRadio(int index)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.setRadio((PLAYER_VIDEO_RADIO_TYPE)index);
	}
}


void PlayerView::setCrop(int index)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.setCrop((PLAYER_VIDEO_CROP_TYPE)index);
	}
}


void PlayerView::setAudioDev(std::string dev)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.setAudioDev(dev);
	}
}


void PlayerView::setAudioModule(int index)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.setAudioModule(index);
	}
}


void PlayerView::setPlaySpeed(float speed)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.setPlaySpeed(speed);
	}
}


void PlayerView::addSubTitle()
{
	if (g_AVPlayer.IsPlaying())
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
			"./",
			tr("videos (*.srt *.ssa *.ass *.txt)"));

		if (fileName.isEmpty())
			return;

		const QString nativePath = QDir::toNativeSeparators(fileName);

		g_AVPlayer.addSubTitle(nativePath.toUtf8().constData());
	}
}

void PlayerView::slot_click_show_hide_treeview(bool showOrhide)
{
	emit signal_showhide_treeview(showOrhide);
}

void PlayerView::slot_toolbar_video_operation(int type)
{
	int curIdx;
	VIDEO_TREE_ITEM treeItem;
	if (type == TOOLBAR_VIDEO_AUDIO_CLOSE)
	{
		if (g_AVPlayer.IsPlaying())
		{
			mVideoVolume = g_AVPlayer.GetVolume();
			g_AVPlayer.Volume(0);
			ui.ToolbarWidget->SetVolumeUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_AUDIO_OPEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.Volume(mVideoVolume);
			ui.ToolbarWidget->SetVolumeUi(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_FAST)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekForward();
		}
	}
	else if (type == TOOLBAR_VIDEO_FULLSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_NORMALSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_STOP)
	{
		if (g_AVPlayer.IsPlaying() || mVideoStatus != ENUM_PLAYSTATUS_STOP)
		{
			g_AVPlayer.Stop();
			mVideoStatus = ENUM_PLAYSTATUS_STOP;
			ui.ToolbarWidget->SetPlayUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_PLAY)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				g_AVPlayer.Play();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ActivePage->SetActiveMode(true);
				ui.ToolbarWidget->SetPlayUi(true);
			}
		}
		else
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_PAUSE)
			{
				g_AVPlayer.Resume();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ToolbarWidget->SetPlayUi(true);
			}
			else if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				if (!mMediaFile.empty())
				{
					PlayStart(mMediaFile, ui.ActivePage->width(), ui.ActivePage->height());
				}
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_PAUSE)
	{
		if (g_AVPlayer.IsPlaying() && mVideoStatus != ENUM_PLAYSTATUS_PAUSE)
		{
			g_AVPlayer.Pause();
			mVideoStatus = ENUM_PLAYSTATUS_PAUSE;
			ui.ToolbarWidget->SetPlayUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_SLOW)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekBackward();
		}
	}
	else if (type == TOOLBAR_VIDEO_PREVIOUS)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetPreviousItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx - 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_NEXT)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetNextItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx + 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_OPEN)
	{
		OpenVideo();
		/*int curItemIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
		g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curItemIdx);*/
	}
}

void PlayerView::slot_toolbar_operation(int type)
{
	ToolOperation(type);
}

void PlayerView::slot_volume_changed(int volume)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.Volume(volume);
	}
}

void PlayerView::slot_video_changed(int volume)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.SeekTo((float)volume / (ui.ToolbarWidget->GetVideoRange()));
	}
}

void PlayerView::slot_video_slider_behavior(int behave)
{
	if (behave == 0)
	{
		g_AVPlayer.Pause();
		m_startDragValue = ui.ToolbarWidget->GetVideoSliderValue();
		m_isVideoSliderPause = true;
	}
	else if (behave == 1)
	{
		int value = ui.ToolbarWidget->GetVideoSliderValue();
		m_stopDragValue = value;
		if (m_stopDragValue == m_startDragValue)
		{
			g_AVPlayer.Play();
			m_startDragValue = -1;
			m_stopDragValue = -1;
			return;
		}

		float percent = value / (float)(ui.ToolbarWidget->GetVideoRange());
		g_AVPlayer.SeekTo(percent);
		m_isVideoSliderPause = false;
		g_AVPlayer.Play();
		m_startDragValue = -1;
		m_stopDragValue = -1;
	}
}

void PlayerView::slot_video_timer()
{
	if (!g_AVPlayer.IsPlaying())
	{
		ui.ActivePage->SetActiveMode(false);
		m_pVideoTimer->stop();
	}
}

void PlayerView::slot_enum_timer()
{
	if (g_AVPlayer.IsPlaying())
		EnumChildWindows((HWND)ui.ActivePage->winId(), EnumerateVLC, NULL);
}

void PlayerView::slot_openvideobtn_clicked()
{
	OpenVideo();
}


void PlayerView::slot_openmenubtn_clicked(int type)
{
	if(type == PLAYACTIVE_OPEN_DIR)
		OpenVideo();
	else if (type == PLAYACTIVE_OPEN_URL)
	{
		WNetDialog w;
		if (w.exec() == QDialog::Accepted)
		{
			vector<VIDEO_TREE_ITEM> vecVideoList;
			g_pMainWnd->GetTreeViewInst()->GetVecVideoList(vecVideoList);
			int videoIdStart = vecVideoList.size();
			VIDEO_TREE_ITEM videoInfo;

			QString fileName = w.getNetPath();
			QString videoIdQ = "video_" + QString::number(videoIdStart);
			QString videoSet = "videoset_0";
			videoInfo.videoId = std::string(videoIdQ.toLocal8Bit().data());
			videoInfo.videoName = std::string(fileName.toLocal8Bit().data());
			videoInfo.videoFilePath = std::string(fileName.toLocal8Bit().data());
			videoInfo.videoPlayTimeStamp = 0;
			videoInfo.setId = std::string(videoSet.toLocal8Bit().data());

			g_pMainWnd->GetTreeViewInst()->AddVideoItem(videoInfo);
			g_pMainWnd->GetTreeViewInst()->PushVideoItem(videoInfo);

			PlayVideoFile(videoInfo);
		}
	}
}

void PlayerView::slot_playermenu_openfile()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_FILE);
}


void PlayerView::slot_playermenu_openmorefile()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_MORE_FILE);
}


void PlayerView::slot_playmenu_scale()
{
	QAction *action = (QAction*)sender();
	emit signal_playvideo(PLAYER_VIDEO_SCALE, action->data().toInt());
}


void PlayerView::slot_playmenu_radio()
{
	QAction *action = (QAction*)sender();
	emit signal_playvideo(PLAYER_VIDEO_RADIO, action->data().toInt());
}


void PlayerView::slot_playmenu_crop()
{
	QAction *action = (QAction*)sender();
	emit signal_playvideo(PLAYER_VIDEO_CROP, action->data().toInt());
}


void PlayerView::slot_playmenu_audiodev()
{
	QAction *action = (QAction*)sender();
	emit signal_playaudiodev(action->data().toString().toStdString());
}


void PlayerView::slot_playmenu_audiomodule()
{
	QAction *action = (QAction*)sender();
	emit signal_playaudiomodule(action->data().toInt());
}


void PlayerView::slot_playmenu_speed()
{
	QAction *action = (QAction*)sender();
	emit signal_playspeed(action->data().toFloat());
}

void PlayerView::slot_playermenu_openurl()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_URL);
}


void PlayerView::slot_playermenu_addsubtitle()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_ADD_SUBTITLE);
}

void PlayerView::slot_playermenu_fullscreen()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_FULLSCREEN);
}

void PlayerView::slot_playermenu_exitfullscreen()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_EXITFULLSCREEN);
}

void PlayerView::slot_playermenu_front()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_FRONT);
}

void PlayerView::slot_playermenu_neverfront()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_NEVERFRONT);
}

void PlayerView::slot_playermenu_snap()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_SNAP);
}

void PlayerView::resizeEvent(QResizeEvent *event)
{
	ui.ActivePage->setFixedSize(ui.stackedWidget->width(), ui.stackedWidget->height());
}

bool PlayerView::eventFilter(QObject *pObject, QEvent *pEvent)
{
	int nEventType = pEvent->type();
	if (pObject == ui.ActivePage)
	{
		switch (nEventType)
		{
		case QEvent::KeyPress:
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(pEvent);
			if (keyEvent->key() == Qt::Key_Escape)
			{
				if (g_AVPlayer.IsPlaying() && m_enterFullscreen)
				{
					SetFullScreen(false);
				}
			}
			break;
		}
		case QEvent::MouseButtonDblClick:
		{
			if (g_AVPlayer.IsPlaying())
			{
				if (m_enterFullscreen)
				{
					SetFullScreen(false);
				}
				else
				{
					SetFullScreen(true);
				}
			}
			break;
		}
		case QEvent::MouseButtonPress:
		{
			QString name = pObject->objectName();
			QMouseEvent *miceEvent = static_cast<QMouseEvent*>(pEvent);
			if (miceEvent && miceEvent->button() == Qt::RightButton)
			{
				ShowRightMenu(miceEvent->globalPos());
				return true;
			}
			break;
		}
		default:
			break;
		}
	}
	return FALSE;
}