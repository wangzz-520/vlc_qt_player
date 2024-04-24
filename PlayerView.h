#pragma once

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QFileDialog>
#include "ui_PlayerView.h"
#include "Global.h"

using namespace zz;

class PlayerView : public QWidget
{
	Q_OBJECT

public:
	PlayerView(QWidget *parent = Q_NULLPTR);
	~PlayerView();

private:
	Ui::PlayerView ui;

private:
	int mAvailableWidth = 0;
	int mAvailabelHeight = 0;
	QTimer	 *m_pVideoTimer = NULL;
	QTimer	*m_pEnumTimer = NULL;
	ENUM_PLAYSTATUS_TYPE mVideoStatus = ENUM_PLAYSTATUS_STOP;
	int mVideoVolume = 25;
	bool m_enterFullscreen = false;
	std::string mMediaFile;
	long mCurVideoSliderRange = 0;
	QRect	mToolbarOldSize;
	QRect	mActivePageOldSize;

	int m_startDragValue = 0;
	int m_stopDragValue = 0;
	bool m_isVideoSliderPause = false;

private:
	bool PlayStart(std::string mediaFile, int nWidth, int nHeight);
	void DockToolbar(bool);

public:
	//播放文件
	bool PlayVideoFile(VIDEO_TREE_ITEM videoItem);
	//打开文件
	void OpenVideo();
	//打开url
	void OpenUrl();
	//打开多个文件
	void OpenMoreVideo();
	//设置全屏
	void SetFullScreen(bool fullscreen);
	//截图
	void Snap();
	//show菜单
	void ShowRightMenu(QPoint point);
	//按钮操作
	void ToolOperation(int type);
	//设置缩放
	void setScale(int index);
	//设置宽高比
	void setRadio(int index);
	//设置裁剪
	void setCrop(int index);
	//设置音频设备
	void setAudioDev(std::string dev);
	//设置立体声模式
	void setAudioModule(int index);
	//设置播放速度
	void setPlaySpeed(float speed);
	//添加字幕文件
	void addSubTitle();

signals:
	void signal_showhide_treeview(bool show);
	void signal_playermenu(int type);
	void signal_playvideo(int type,int index);
	void signal_playaudiodev(std::string dev);
	void signal_playaudiomodule(int index);
	void signal_playspeed(float speed);

private slots:
	void slot_click_show_hide_treeview(bool showOrhide);
	void slot_video_timer();
	void slot_enum_timer();
	void slot_volume_changed(int);
	void slot_video_changed(int);
	void slot_openvideobtn_clicked();
	void slot_openmenubtn_clicked(int);
	void slot_toolbar_operation(int);
	void slot_video_slider_behavior(int behave);

	//打开文件
	void slot_playermenu_openfile();
	//打开多个文件
	void slot_playermenu_openmorefile();
	//缩放
	void slot_playmenu_scale();
	//宽高比
	void slot_playmenu_radio();
	//裁剪
	void slot_playmenu_crop();
	//音频设备
	void slot_playmenu_audiodev();
	//音频模式
	void slot_playmenu_audiomodule();
	//速度
	void slot_playmenu_speed();
	//打开URL
	void slot_playermenu_openurl();
	//添加字幕文件
	void slot_playermenu_addsubtitle();
	//全屏
	void slot_playermenu_fullscreen();
	//退出全屏
	void slot_playermenu_exitfullscreen();
	//总是最前
	void slot_playermenu_front();
	//取消最前
	void slot_playermenu_neverfront();
	//截图
	void slot_playermenu_snap();

public:
	void slot_toolbar_video_operation(int type);

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *pObject, QEvent *pEvent);
};
