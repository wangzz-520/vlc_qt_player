#pragma once

#include <QWidget>
#include "ui_PlayActivePageView.h"
#include "Global.h"

using namespace zz;

class PlayActivePageView : public QWidget
{
	Q_OBJECT

public:
	PlayActivePageView(QWidget *parent = Q_NULLPTR);
	~PlayActivePageView();

private:
	Ui::PlayActivePageView ui;

private:
	//��ʼ��ui
	void InitUi();

public:
	void SetActiveMode(bool active);

signals:
	//���ļ���ť
	void signal_openvideobtn_clicked();
	//�򿪲˵�
	void signal_openmenubtn_clicked(int menuType);

private slots:
	//�򿪱����ļ�
	void slot_click_openfile_btn();
	//�򿪲˵�
	void slot_click_openmenu_btn();
	void slot_click_openurl_btn();
	void slot_click_opendir_btn();

protected:
	void showEvent(QShowEvent *event);

private:
	PLAYACTIVE_OPENMENU_TYPE m_type;
};
