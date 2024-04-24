#include "ToolbarView.h"
#include "Player.h"
#include <time.h>
#include <tchar.h>

const int wnd_height = 90;
const int timeedit_width = 65;
const int timeedit_height = 13;
const int slow_fast_btn_width = 17;
const int slow_fast_btn_height = 13;
const int video_slider_height = 13;
const int showhide_btn_width_height = 18;
const int big_btn_width_height = 48;
const int small_btn_width_height = 40;
const int voice_btn_width_height = 20;
const int voice_slider_width = 82;
const int voice_slider_height = 20;
const int media_btn_width = 24;
const int media_btn_height = 18;

extern Player g_AVPlayer;

ToolbarView::ToolbarView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
	InitUi();
	InitSignalSlots();
}

ToolbarView::~ToolbarView()
{
}

void ToolbarView::Init()
{
	ui.show_treeview_btn->setVisible(false);
	ui.pause_btn->setVisible(false);
	ui.mute_btn->setVisible(false);
	ui.normalscreen_btn->setVisible(false);
	m_pTimer = new QTimer;
	mMaxValue = 0;
	mSliderReleased = false;
	mTotalTime = 0;
	ui.video_slider->setMouseTracking(true);
	ui.video_slider->setTracking(false);
	ui.video_slider->installEventFilter(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	setFixedHeight(wnd_height);
	ui.curtime_edit->setFixedSize(timeedit_width, timeedit_height);
	ui.totaltime_edit->setFixedSize(timeedit_width, timeedit_height);
	ui.slow_btn->setFixedSize(slow_fast_btn_width, slow_fast_btn_height);
	ui.fast_btn->setFixedSize(slow_fast_btn_width, slow_fast_btn_height);
	ui.show_treeview_btn->setFixedSize(showhide_btn_width_height, showhide_btn_width_height);
	ui.hide_treeview_btn->setFixedSize(showhide_btn_width_height, showhide_btn_width_height);
	ui.close_btn->setFixedSize(small_btn_width_height, small_btn_width_height);
	ui.previous_video_btn->setFixedSize(small_btn_width_height, small_btn_width_height);
	ui.play_btn->setFixedSize(big_btn_width_height, big_btn_width_height);
	ui.pause_btn->setFixedSize(big_btn_width_height, big_btn_width_height);
	ui.next_video_btn->setFixedSize(small_btn_width_height, small_btn_width_height);
	ui.unmute_btn->setFixedSize(voice_btn_width_height, voice_btn_width_height);
	ui.mute_btn->setFixedSize(voice_btn_width_height, voice_btn_width_height);
	ui.voice_slider->setFixedSize(voice_slider_width, voice_slider_height);
	ui.video_slider->setFixedHeight(voice_slider_height);
	ui.openfile_btn->setFixedSize(media_btn_width, media_btn_height);
	ui.fullscreen_btn->setFixedSize(media_btn_width, media_btn_height);
	ui.normalscreen_btn->setFixedSize(media_btn_width, media_btn_height);

	ui.slow_btn->setToolTip(QStringLiteral("慢放"));
	ui.fast_btn->setToolTip(QStringLiteral("快放"));
	ui.show_treeview_btn->setToolTip(QStringLiteral("显示列表"));
	ui.hide_treeview_btn->setToolTip(QStringLiteral("隐藏列表"));
	ui.close_btn->setToolTip(QStringLiteral("停止播放"));
	ui.previous_video_btn->setToolTip(QStringLiteral("上一个"));
	ui.play_btn->setToolTip(QStringLiteral("开始播放"));
	ui.pause_btn->setToolTip(QStringLiteral("暂停播放"));
	ui.next_video_btn->setToolTip(QStringLiteral("下一个"));
	ui.unmute_btn->setToolTip(QStringLiteral("关闭声音"));
	ui.mute_btn->setToolTip(QStringLiteral("开启声音"));
	ui.voice_slider->setToolTip(QString::number(ui.voice_slider->value()));
	ui.video_slider->setToolTip(QString::number(ui.video_slider->value()));
	ui.openfile_btn->setToolTip(QStringLiteral("打开文件"));
	ui.fullscreen_btn->setToolTip(QStringLiteral("全屏"));
	ui.normalscreen_btn->setToolTip(QStringLiteral("退出全屏"));
}

void ToolbarView::InitUi()
{
	ui.curtime_edit->setStyleSheet("QTimeEdit{color: white; font: 75 12px; border: 0px solid rgb(41, 57, 85);border - radius: 3px;background:black;selection-background-color: black;}");
	ui.totaltime_edit->setStyleSheet("QTimeEdit{color: white; font: 75 12px; border: 0px solid rgb(41, 57, 85);border - radius: 3px;background:black;selection-background-color: black;}");
	ui.slow_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_slow_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_slow_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_slow_press.png);}"
		);
	ui.fast_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_fast_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_fast_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_fast_press.png);}"
		);
	ui.hide_treeview_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_playlist_hide_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_playlist_hide_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_playlist_hide_press.png);}"
		);
	ui.show_treeview_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_playlist_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_playlist_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_playlist_press.png);}"
		);
	ui.close_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_stop_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_stop_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_stop_press.png);}"
		);
	ui.previous_video_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_previous_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_previous_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_previous_press.png);}"
		);
	ui.play_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_play_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_play_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_play_press.png);}"
		);
	ui.pause_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_pause_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_pause_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_pause_press.png);}"
		);
	ui.next_video_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_next_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_next_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_next_press.png);}"
		);
	ui.mute_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_mute_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_mute_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_mute_press.png);}"
		);
	ui.unmute_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_unmute_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_unmute_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_unmute_press.png);}"
		);
	ui.voice_slider->setStyleSheet(
		//设置主体
		"QSlider{border-color: #f0f0f0;}"
		"QSlider::groove:horizontal{border: 1px solid #999999;height: 1px;margin: 0px 0;left: 5px; right: 5px;}"
		//设置中间的那个滑动的键
		"QSlider::handle:horizontal{border: 0px;border-image: url(:/images/slider/SliderPlayThumb_normal.png);width: 15px;margin: -7px -7px -7px -7px;}"
		//还没有滑上去的地方
		"QSlider::add-page:horizontal{background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #303134, stop:0.25 #303134, stop:0.5 #303134, stop:1 #303134);}"
		//已经划过的地方
		"QSlider::sub-page:horizontal{background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #005AAF, stop:0.25 #005AAF, stop:0.5 #005AAF, stop:1 #005AAF);}");
	ui.video_slider->setStyleSheet(
		//设置主体
		"QSlider{border-color: #f0f0f0;}"
		"QSlider::groove:horizontal{border: 1px solid #999999;height: 1px;margin: 0px 0;left: 5px; right: 5px;}"
		//设置中间的那个滑动的键
		"QSlider::handle:horizontal{border: 0px;border-image: url(:/images/slider/SliderPlayThumb_normal.png);width: 15px;margin: -7px -7px -7px -7px;}"
		//还没有滑上去的地方
		"QSlider::add-page:horizontal{background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #303134, stop:0.25 #303134, stop:0.5 #303134, stop:1 #303134);}"
		//已经划过的地方
		"QSlider::sub-page:horizontal{background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #005AAF, stop:0.25 #005AAF, stop:0.5 #005AAF, stop:1 #005AAF);}");
	
	ui.openfile_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_open_mini_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_open_mini_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_open_mini_press.png);}"
		);
	ui.fullscreen_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_screen_full_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_screen_full_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_screen_full_press.png);}"
		);
	ui.normalscreen_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_screen_normal_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_screen_normal_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_screen_normal_press.png);}"
		);

	SetPlayUi(false);
}

void ToolbarView::InitSignalSlots()
{
	connect(ui.slow_btn, SIGNAL(clicked()), this, SLOT(slot_click_slow_btn()));
	connect(ui.fast_btn, SIGNAL(clicked()), this, SLOT(slot_click_fast_btn()));
	connect(ui.show_treeview_btn, SIGNAL(clicked()), this, SLOT(slot_click_showtreeview_btn()));
	connect(ui.hide_treeview_btn, SIGNAL(clicked()), this, SLOT(slot_click_hidetreeview_btn()));
	connect(ui.close_btn, SIGNAL(clicked()), this, SLOT(slot_click_close_btn()));
	connect(ui.previous_video_btn, SIGNAL(clicked()), this, SLOT(slot_click_previous_video_btn()));
	connect(ui.play_btn, SIGNAL(clicked()), this, SLOT(slot_click_play_btn()));
	connect(ui.pause_btn, SIGNAL(clicked()), this, SLOT(slot_click_pause_btn()));
	connect(ui.next_video_btn, SIGNAL(clicked()), this, SLOT(slot_click_next_video_btn()));
	connect(ui.unmute_btn, SIGNAL(clicked()), this, SLOT(slot_click_unmute_btn()));
	connect(ui.mute_btn, SIGNAL(clicked()), this, SLOT(slot_click_mute_btn()));
	connect(ui.openfile_btn, SIGNAL(clicked()), this, SLOT(slot_click_openfile_btn()));
	connect(ui.fullscreen_btn, SIGNAL(clicked()), this, SLOT(slot_click_fullscreen_btn()));
	connect(ui.normalscreen_btn, SIGNAL(clicked()), this, SLOT(slot_click_normalscreen_btn()));
	connect(ui.voice_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_voice_value_changed(int)));
	//connect(ui.video_slider, SIGNAL(sliderPressed()), this, SLOT(slot_videoSliderPressed()));
	//connect(ui.video_slider, SIGNAL(sliderReleased()), this, SLOT(slot_videoSliderReleased()));
	//connect(ui.video_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_setVideo(int)));
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slot_timer()));
	m_pTimer->start(1000);
}

void ToolbarView::SetPlayUi(bool success, bool pause)
{
	if (success)
	{
		ui.play_btn->setVisible(false);
		ui.pause_btn->setVisible(true);
		ui.SlidWidget->setVisible(true);
		ui.TimeWidget->setVisible(true);
	}
	else
	{
		ui.play_btn->setVisible(true);
		ui.pause_btn->setVisible(false);
		if (!pause)
		{
			ui.SlidWidget->setVisible(false);
			ui.TimeWidget->setVisible(false);
		}
		else
		{
			ui.SlidWidget->setVisible(true);
			ui.TimeWidget->setVisible(true);
		}
	}
}

void ToolbarView::SetVideoRange(int maxValue)
{
	if (maxValue > 0)
	{
		ui.video_slider->setRange(0, maxValue);
		mMaxValue = maxValue;
	}
}

void ToolbarView::SetVolumeUi(bool mute)
{
	if (mute)
	{
		ui.mute_btn->setVisible(false);
		ui.unmute_btn->setVisible(true);
	}
	else
	{
		ui.mute_btn->setVisible(true);
		ui.unmute_btn->setVisible(false);
	}
}

void ToolbarView::SetFullScreenUi(bool full)
{
	/*if (full)
	{
		ui.fullscreen_btn->setVisible(false);
		ui.normalscreen_btn->setVisible(true);
	}
	else
	{
		ui.fullscreen_btn->setVisible(true);
		ui.normalscreen_btn->setVisible(false);
	}*/
}

void ToolbarView::slot_click_slow_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_SLOW);
}

void ToolbarView::slot_click_fast_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_FAST);
}

void ToolbarView::slot_click_showtreeview_btn()
{
	if (ui.show_treeview_btn->isVisible())
	{
		ui.hide_treeview_btn->setVisible(true);
		ui.show_treeview_btn->setVisible(false);
		emit signal_click_show_hide_treeview(true);
	}
}

void ToolbarView::slot_click_hidetreeview_btn()
{
	if (ui.hide_treeview_btn->isVisible())
	{
		ui.hide_treeview_btn->setVisible(false);
		ui.show_treeview_btn->setVisible(true);
		emit signal_click_show_hide_treeview(false);
	}
}

void ToolbarView::slot_click_close_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_STOP);
}

void ToolbarView::slot_click_previous_video_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_PREVIOUS);
}

void ToolbarView::slot_click_play_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_PLAY);
}

void ToolbarView::slot_click_pause_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_PAUSE);
}

void ToolbarView::slot_click_next_video_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_NEXT);
}

void ToolbarView::slot_click_unmute_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_AUDIO_CLOSE);
}

void ToolbarView::slot_click_mute_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_AUDIO_OPEN);
}

void ToolbarView::slot_click_openfile_btn()
{
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_OPEN);
}

void ToolbarView::slot_click_fullscreen_btn()
{
	ui.fullscreen_btn->setVisible(false);
	ui.normalscreen_btn->setVisible(true);
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_FULLSCREEN);
}

void ToolbarView::slot_click_normalscreen_btn()
{
	ui.fullscreen_btn->setVisible(true);
	ui.normalscreen_btn->setVisible(false);
	emit signal_toolbar_video_operation(TOOLBAR_VIDEO_NORMALSCREEN);
}

void ToolbarView::slot_voice_value_changed(int volume)
{
	emit signal_voice_value_changed(volume);
}

void ToolbarView::slot_video_value_changed(int volume)
{
	//emit signal_video_value_changed(volume);
}

void ToolbarView::slot_videoSliderPressed()
{
	emit signal_video_slider_behavior(0);
	m_pTimer->stop();
}

void ToolbarView::slot_videoSliderReleased()
{
	emit signal_video_slider_behavior(1);
	mSliderReleased = true;
}

void ToolbarView::slot_setVideo(int value)
{
	if (mSliderReleased)
	{
		float percent = value / (float)(mMaxValue);
		g_AVPlayer.SeekTo(percent);
		mSliderReleased = false;
		m_pTimer->start();
	}
}

void ToolbarView::slot_timer()
{
	if (g_AVPlayer.IsPlaying())
	{
		struct tm   tmTotal, tmCurrent;
		time_t totaltime = g_AVPlayer.GetTotalTime()/1000;
		if (totaltime > mTotalTime)
		{
			mTotalTime = totaltime;
			SetVideoRange(totaltime);
		}

		time_t timeCurrent = g_AVPlayer.GetTime() / 1000 + 1;
		char szTotal[MAX_PATH], szCurrent[MAX_PATH];
		gmtime_s(&tmTotal, &totaltime);
		gmtime_s(&tmCurrent, &timeCurrent);
		_tcsftime(szTotal, MAX_PATH, _T("%X"), &tmTotal);
		_tcsftime(szCurrent, MAX_PATH, _T("%X"), &tmCurrent);
		ui.video_slider->setValue(timeCurrent);
		ui.curtime_edit->setTime(QTime::fromString(szCurrent, "hh:mm:ss"));
		ui.totaltime_edit->setTime(QTime::fromString(szTotal, "hh:mm:ss"));
	}
}

void ToolbarView::showEvent(QShowEvent *event)
{
	//ui.video_slider->setFixedWidth(ui.SlidWidget->width() - 14 - 2 * slow_fast_btn_width);
	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(7, 7, 7)));
	setAutoFillBackground(true);
	setPalette(pal);
}

void ToolbarView::resizeEvent(QResizeEvent *e)
{
	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(7, 7, 7)));
	setAutoFillBackground(true);
	setPalette(pal);
}

bool ToolbarView::eventFilter(QObject *obj, QEvent *event)
{
	int nEventType = event->type();
	if (obj == ui.video_slider)
	{
		switch (nEventType)
		{
		case QEvent::MouseButtonRelease:
		{
			QMouseEvent *miceEvent = static_cast<QMouseEvent*>(event);
			if (miceEvent && miceEvent->button() == Qt::LeftButton)
			{
				m_pTimer->stop();
				QPoint rect = miceEvent->pos();
				QPoint rect1 = ui.video_slider->pos();
				int delta = abs(rect.x() - rect1.x());
				int sliderwidth = ui.video_slider->width();
				float percent = (float)delta / sliderwidth;
				g_AVPlayer.SeekTo(percent);
				g_AVPlayer.Play();
				m_pTimer->start();
			}
			break;
		}
		case QEvent::MouseButtonPress:
		{
			g_AVPlayer.Pause();
			m_pTimer->stop();
			break;
		}
		default:
			break;
		}
	}
	return false;
}