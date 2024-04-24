#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "ui_ToolbarView.h"


typedef enum{
	TOOLBAR_VIDEO_PLAY = 0,
	TOOLBAR_VIDEO_PAUSE,
	TOOLBAR_VIDEO_STOP,
	TOOLBAR_VIDEO_PREVIOUS,
	TOOLBAR_VIDEO_NEXT,
	TOOLBAR_VIDEO_AUDIO_OPEN,
	TOOLBAR_VIDEO_AUDIO_CLOSE,
	TOOLBAR_VIDEO_FULLSCREEN,
	TOOLBAR_VIDEO_NORMALSCREEN,
	TOOLBAR_VIDEO_FAST,
	TOOLBAR_VIDEO_SLOW,
	TOOLBAR_VIDEO_OPEN
}TOOLBAR_VIDEO_OPERATION;

class ToolbarView : public QWidget
{
	Q_OBJECT

public:
	ToolbarView(QWidget *parent = Q_NULLPTR);
	~ToolbarView();

private:
	Ui::ToolbarView ui;
	QTimer* m_pTimer;

	int mMaxValue;
	int mTotalTime;
	bool mSliderReleased;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

public:
	void SetPlayUi(bool success, bool pause = false);
	void SetVolumeUi(bool mute);
	void SetFullScreenUi(bool full);
	void SetVideoRange(int maxValue);
	int GetVideoRange(){ return mMaxValue; };
	int GetVideoSliderValue(){ return ui.video_slider->value(); };
	void SetVideoSliderValue(int value){ ui.video_slider->setValue(value); };
	void SetHideTreeBtn(bool hide){ ui.hide_treeview_btn->setVisible(!hide); ui.show_treeview_btn->setVisible(hide); };

signals:
	void signal_click_show_hide_treeview(bool showOrhide);
	void signal_toolbar_video_operation(int operation);
	void signal_voice_value_changed(int volume);
	void signal_video_value_changed(int volume);
	void signal_video_slider_behavior(int behave);

private slots:
	void slot_click_slow_btn();
	void slot_click_fast_btn();
	void slot_click_showtreeview_btn();
	void slot_click_hidetreeview_btn();
	void slot_click_close_btn();
	void slot_click_previous_video_btn();
	void slot_click_play_btn();
	void slot_click_pause_btn();
	void slot_click_next_video_btn();
	void slot_click_unmute_btn();
	void slot_click_mute_btn();
	void slot_click_openfile_btn();
	void slot_click_fullscreen_btn();
	void slot_click_normalscreen_btn();
	void slot_voice_value_changed(int volume);
	void slot_video_value_changed(int volume);
	void slot_timer();
	void slot_videoSliderPressed();
	void slot_videoSliderReleased();
	void slot_setVideo(int value);

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *e);
	bool eventFilter(QObject *obj, QEvent *event);
};
