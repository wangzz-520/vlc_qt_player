#pragma once

#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include "ui_ZPlayer.h"

class ZPlayer : public QWidget
{
    Q_OBJECT

public:
    ZPlayer(QWidget *parent = nullptr);
    ~ZPlayer();

private:
	void ExitApplication();
	void LoadVideoList();

public:
	TreeView* GetTreeViewInst() { return ui.TreeWidget; };

public slots:
	//��С��
	void OnMinimumWnd();
	//���
	void OnMaximumWnd();
	//����
	void OnNormalWnd();
	//�ر�
	void OnCloseWnd();
	//���ز����
	void OnShowHideTreeview(bool show);
	//˫�������ļ�
	void OnDbClickVideoFile();
	void OnPlayerMenu(int type);
	void OnPlayvideo(int type, int index);
	void OnPlayAudioDev(std::string dev);
	void OnPlayAudioModule(int index);
	void OnPlaySpeed(float speed);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *pObject, QEvent *pEvent);

private:
    Ui::ZPlayerClass ui;

private:
	//�ƶ�����
	QPoint mWindowPos;
	QPoint mMousePos;
	QPoint mPosDelta;

	bool mIsPressed = false;

	//��ȡ��������Ŀ¼
	QString mRuntimePath;

	//ȫ����ʾ����
	int mAvailableWidth = 0;
	int mAvailabelHeight = 0;
};
