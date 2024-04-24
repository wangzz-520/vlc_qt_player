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
	//��ȡ��ǰҪ���ŵ���
	VIDEO_TREE_ITEM GetSelTreeItem();
	//��ȡ��һ��Ҫ���ŵ���
	VIDEO_TREE_ITEM GetNextItem();
	//��ȡ��һ��Ҫ���ŵ���
	VIDEO_TREE_ITEM GetPreviousItem();
	//��ȡ��ǰ�������
	int GetCurrentItemIndex();
	//���õ�ǰ���ŵ�������
	void SetCurrentItem(int itemIndex);
	//��Ӳ�����
	void AddVideoItem(VIDEO_TREE_ITEM tvshowItemInfo);
	//��ȡ��ǰ�����б�
	void GetVecVideoList(vector<VIDEO_TREE_ITEM> &vecVideoList);
	//���뵽���Ŷ���
	void PushVideoItem(VIDEO_TREE_ITEM item);;

signals:
	void signal_playvideo();

private slots:
	//���ļ���
	void slot_click_addfile_btn();
	//ɾ���б���
	void slot_click_delfile_btn();
	//���ڵ���
	void slot_treeItemClicked(QTreeWidgetItem* selItem, int column);
	//���ڵ�˫��
	void slot_treeItemDbClicked(QTreeWidgetItem* selItem, int column);

protected:
	void showEvent(QShowEvent *event);

private:
	Ui::TreeView ui;

private:
	//���б�
	ListTreeWidget *m_pVideoTree = nullptr;
	//��ǰ��Ƶ�б�
	vector<VIDEO_TREE_ITEM> mVecVideoList;
	//��ǰѡ�񲥷ŵ���
	VIDEO_TREE_ITEM mSelVideoItem;
	//��ǰѡ�񲥷ŵ���
	QTreeWidgetItem*	mSelTreeItem = nullptr;
	//��ǰ���ŵ�Item
	QTreeWidgetItem*	mPlayingItem = nullptr;
	int mCurIndexOfTree = 0;
};
