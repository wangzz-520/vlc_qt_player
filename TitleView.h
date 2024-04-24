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
	//���
	void signal_show_maximised();
	//��С��
	void signal_show_minimum();
	//������С
	void signal_show_normal();
	//�ر�
	void signal_close_wnd();

private slots:
	//��С��
	void slot_click_minimum_btn();
	//���
	void slot_click_maximum_btn();
	//������С
	void slot_click_normal_btn();
	//�ر�
	void slot_click_close_btn();

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	Ui::TitleView ui;
	
private:
	//ȫ����ʾ����
	int mAvailableWidth = 0;
	int mAvailabelHeight = 0;
};
