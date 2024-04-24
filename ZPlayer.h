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
	//最小化
	void OnMinimumWnd();
	//最大化
	void OnMaximumWnd();
	//正常
	void OnNormalWnd();
	//关闭
	void OnCloseWnd();
	//隐藏侧边栏
	void OnShowHideTreeview(bool show);
	//双击播放文件
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
	//移动窗体
	QPoint mWindowPos;
	QPoint mMousePos;
	QPoint mPosDelta;

	bool mIsPressed = false;

	//获取程序运行目录
	QString mRuntimePath;

	//全屏显示窗口
	int mAvailableWidth = 0;
	int mAvailabelHeight = 0;
};
