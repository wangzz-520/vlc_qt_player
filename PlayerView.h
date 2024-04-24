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
	//�����ļ�
	bool PlayVideoFile(VIDEO_TREE_ITEM videoItem);
	//���ļ�
	void OpenVideo();
	//��url
	void OpenUrl();
	//�򿪶���ļ�
	void OpenMoreVideo();
	//����ȫ��
	void SetFullScreen(bool fullscreen);
	//��ͼ
	void Snap();
	//show�˵�
	void ShowRightMenu(QPoint point);
	//��ť����
	void ToolOperation(int type);
	//��������
	void setScale(int index);
	//���ÿ�߱�
	void setRadio(int index);
	//���òü�
	void setCrop(int index);
	//������Ƶ�豸
	void setAudioDev(std::string dev);
	//����������ģʽ
	void setAudioModule(int index);
	//���ò����ٶ�
	void setPlaySpeed(float speed);
	//�����Ļ�ļ�
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

	//���ļ�
	void slot_playermenu_openfile();
	//�򿪶���ļ�
	void slot_playermenu_openmorefile();
	//����
	void slot_playmenu_scale();
	//��߱�
	void slot_playmenu_radio();
	//�ü�
	void slot_playmenu_crop();
	//��Ƶ�豸
	void slot_playmenu_audiodev();
	//��Ƶģʽ
	void slot_playmenu_audiomodule();
	//�ٶ�
	void slot_playmenu_speed();
	//��URL
	void slot_playermenu_openurl();
	//�����Ļ�ļ�
	void slot_playermenu_addsubtitle();
	//ȫ��
	void slot_playermenu_fullscreen();
	//�˳�ȫ��
	void slot_playermenu_exitfullscreen();
	//������ǰ
	void slot_playermenu_front();
	//ȡ����ǰ
	void slot_playermenu_neverfront();
	//��ͼ
	void slot_playermenu_snap();

public:
	void slot_toolbar_video_operation(int type);

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *pObject, QEvent *pEvent);
};
