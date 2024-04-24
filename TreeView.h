#pragma once

#include <QWidget>
#include "ui_TreeView.h"
#include "Global.h"
#include "ListTreeWidget.h"

using namespace zz;
using namespace std;

class TreeView : public QWidget
{
	Q_OBJECT

public:
	TreeView(QWidget *parent = Q_NULLPTR);
	~TreeView();

private:
	void InitUi();
	void RefreshUi();
	bool FindItem(QString filePath);

public:
	//获取当前要播放的项
	VIDEO_TREE_ITEM GetSelTreeItem();
	//获取下一个要播放的项
	VIDEO_TREE_ITEM GetNextItem();
	//获取上一个要播放的项
	VIDEO_TREE_ITEM GetPreviousItem();
	//获取当前项的索引
	int GetCurrentItemIndex();
	//设置当前播放的项索引
	void SetCurrentItem(int itemIndex);
	//添加播放项
	void AddVideoItem(VIDEO_TREE_ITEM tvshowItemInfo);
	//获取当前播放列表
	void GetVecVideoList(vector<VIDEO_TREE_ITEM> &vecVideoList);
	//放入到播放队列
	void PushVideoItem(VIDEO_TREE_ITEM item);;

signals:
	void signal_playvideo();

private slots:
	//打开文件项
	void slot_click_addfile_btn();
	//删除列表项
	void slot_click_delfile_btn();
	//树节点点击
	void slot_treeItemClicked(QTreeWidgetItem* selItem, int column);
	//树节点双击
	void slot_treeItemDbClicked(QTreeWidgetItem* selItem, int column);

protected:
	void showEvent(QShowEvent *event);

private:
	Ui::TreeView ui;

private:
	//树列表
	ListTreeWidget *m_pVideoTree = nullptr;
	//当前视频列表
	vector<VIDEO_TREE_ITEM> mVecVideoList;
	//当前选择播放的项
	VIDEO_TREE_ITEM mSelVideoItem;
	//当前选择播放的项
	QTreeWidgetItem*	mSelTreeItem = nullptr;
	//当前播放的Item
	QTreeWidgetItem*	mPlayingItem = nullptr;
	int mCurIndexOfTree = 0;
};
