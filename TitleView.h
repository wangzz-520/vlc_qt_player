#pragma once

#include <QWidget>
#include <QMenu>
#include "ui_TitleView.h"
#include "Global.h"

class TitleView : public QWidget
{
	Q_OBJECT

public:
	TitleView(QWidget *parent = Q_NULLPTR);
	~TitleView();

signals:
	//最大化
	void signal_show_maximised();
	//最小化
	void signal_show_minimum();
	//正常大小
	void signal_show_normal();
	//关闭
	void signal_close_wnd();

private slots:
	//最小化
	void slot_click_minimum_btn();
	//最大化
	void slot_click_maximum_btn();
	//正常大小
	void slot_click_normal_btn();
	//关闭
	void slot_click_close_btn();

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	Ui::TitleView ui;
	
private:
	//全屏显示窗口
	int mAvailableWidth = 0;
	int mAvailabelHeight = 0;
};
