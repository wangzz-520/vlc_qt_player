#include "TreeView.h"
#include "Global.h"
#include <QFileDialog>

QString gCurrentTreeItemPath = "";

const int const_treeview_width = 290;
const int const_fontbtn_width_height = 27;
const int const_searchbtn_width_height = 18;
const int const_toolbtn_width = 286;
const int const_toolbtn_height = 32;
const int const_toolpanel_height = 41;
const int const_combo_width = 65;
const int const_combo_height = 28;
const int const_smallbtn_width = 18;
const int const_smallbtn_height = 18;
const int const_videotree_item_height = 20;


TreeView::TreeView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pVideoTree = new ListTreeWidget;
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(m_pVideoTree);
	hLayout->setContentsMargins(0, 0, 0, 0);
	ui.playlist_tree->setLayout(hLayout);
	m_pVideoTree->setFixedWidth(const_treeview_width);
	m_pVideoTree->setHeaderHidden(true);
	m_pVideoTree->setIndentation(0);
	m_pVideoTree->AddGroup(QStringLiteral("本地媒体文件"));

	connect(ui.addfile_btn, SIGNAL(clicked()), this, SLOT(slot_click_addfile_btn()));
	connect(ui.delfile_btn, SIGNAL(clicked()), this, SLOT(slot_click_delfile_btn()));

	connect(m_pVideoTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slot_treeItemClicked(QTreeWidgetItem *, int)));
	connect(m_pVideoTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slot_treeItemDbClicked(QTreeWidgetItem *, int)));
}

TreeView::~TreeView()
{
}

void TreeView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	setFixedWidth(const_treeview_width);
	ui.playlist_btn->setFixedSize(const_toolbtn_width, const_toolbtn_height);
	ui.playlist_tree_2->setFixedSize(const_toolbtn_width, const_toolpanel_height);
	ui.listtype_combo->setFixedSize(const_combo_width, const_combo_height);
	ui.addfile_btn->setFixedSize(const_smallbtn_width, const_smallbtn_height);
	ui.delfile_btn->setFixedSize(const_smallbtn_width, const_smallbtn_height);
	ui.playlist_tree->setFixedWidth(const_toolbtn_width);

	ui.addfile_btn->setToolTip(QStringLiteral("添加文件"));
	ui.delfile_btn->setToolTip(QStringLiteral("删除文件"));

	ui.playlist_btn->SetUiStyleSheet("QToolButton#playlist_btn{border-image:url(:/images/treeview_header.png);color: rgb(233, 233, 233); font-size: 75 16px;}",
		"QToolButton#playlist_btn{border-image:url(:/images/treeview_header_expand.png);color: rgb(233, 233, 233); font-size: 75 16px;}", ENUM_TOOLBUTTON_SELECT);
	ui.addfile_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_add_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_add_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_add_press.png);}"
		);
	ui.delfile_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_delete_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_delete_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_delete_press.png);}"
		);
	ui.listtype_combo->setStyleSheet(
		"QComboBox{border: 0px solid gray; padding: 1px 18px 1px 3px;}"
		"QComboBox:!editable, QComboBox::drop-down:!editable{background: rgba(25,25,25);}"
		"QComboBox:!editable:on, QComboBox::drop-down:!editable:on{background: rgba(47,47,47);}"
		"QComboBox::down-arrow{border-image: url(:/images/combo_right_normal.png);width:20; height:20;}"
		"QComboBox::down-arrow:hover{border-image: url(:/images/combo_right_hover.png);width:20; height:20;}"
		"QComboBox::down-arrow:pressed{border-image: url(:/images/combo_right_press.png);width:20; height:20;}"
		"QComboBox::drop-down{width: 20px; height: 20px;}"
		"QComboBox{color: white;}"
		"QComboBox QAbstractItemView{selection-color: white; background: rgba(32,32,32,60);}"
		"QComboBox QAbstractItemView{color: white; border: 1px solid darkgray; selection-background-color: rgb(64, 64, 64);}");

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(32, 32, 32)));
	setAutoFillBackground(true);
	setPalette(pal);

	ui.playlist_btn->setText(QStringLiteral("播放列表"));

	QStringList typecomboList;
	ui.listtype_combo->clear();
	typecomboList << QStringLiteral("全部");
	ui.listtype_combo->addItems(typecomboList);
}

void TreeView::RefreshUi()
{
	if (mVecVideoList.size() > 0)
	{
		vector<VIDEO_TREE_ITEM>::iterator it;
		for (it = mVecVideoList.begin(); it != mVecVideoList.end(); it++)
		{
			m_pVideoTree->AddMember(0, QString::fromLocal8Bit(it->videoName.c_str()), *it);
		}
	}
	if (m_pVideoTree)
		m_pVideoTree->expandAll();
	VideoTreeItem *itemSel = (VideoTreeItem *)m_pVideoTree->topLevelItem(0)->child(0);
	if (itemSel)
	{
		itemSel->GetIpcData(mSelVideoItem);
		mCurIndexOfTree = 0;
		mSelTreeItem = m_pVideoTree->topLevelItem(0)->child(0);
	}
	else
	{
		memset(&mSelVideoItem, 0, sizeof(VIDEO_TREE_ITEM));
		mCurIndexOfTree = -1;
		mSelTreeItem = NULL;
	}
}

bool TreeView::FindItem(QString filePath)
{
	vector<VIDEO_TREE_ITEM>::iterator it;
	for (it = mVecVideoList.begin(); it != mVecVideoList.end(); it++)
	{
		QString filePath2 = QString::fromLocal8Bit(it->videoFilePath.c_str());
		if (filePath2.compare(filePath) == 0)
			return true;
	}
	return false;
}

void TreeView::AddVideoItem(VIDEO_TREE_ITEM videoInfo)
{
	if (!FindItem(QString::fromLocal8Bit(videoInfo.videoFilePath.c_str())))
	{
		m_pVideoTree->AddMember(0, QString::fromLocal8Bit(videoInfo.videoName.c_str()), videoInfo);
		SetCurrentItem(m_pVideoTree->topLevelItem(0)->childCount()-1);
	}
	else
	{
		if (m_pVideoTree)
		{
			for (int i = 0; i < m_pVideoTree->topLevelItem(0)->childCount(); i++)
			{
				QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(i);
				if (item)
				{
					VideoTreeItem* truelyItem = (VideoTreeItem*)item;
					if (truelyItem)
					{
						VIDEO_TREE_ITEM itemParam;
						truelyItem->GetIpcData(itemParam);
						if (videoInfo.videoFilePath.compare(itemParam.videoFilePath) == 0)
						{
							SetCurrentItem(i);
							break;
						}
					}
				}
			}
		}
	}
}


void TreeView::GetVecVideoList(vector<VIDEO_TREE_ITEM> &vecVideoList)
{
	vecVideoList = mVecVideoList;
}


void TreeView::PushVideoItem(VIDEO_TREE_ITEM item)
{
	mVecVideoList.push_back(item);
}

int TreeView::GetCurrentItemIndex()
{
	int curIdx = m_pVideoTree->currentIndex().row();
	if (curIdx != mCurIndexOfTree)
		return mCurIndexOfTree;
	return curIdx;
}


VIDEO_TREE_ITEM TreeView::GetSelTreeItem()
{
	return mSelVideoItem;
}

VIDEO_TREE_ITEM TreeView::GetNextItem()
{
	VIDEO_TREE_ITEM videoItem;
	if (m_pVideoTree && m_pVideoTree->topLevelItem(0)->childCount() > mCurIndexOfTree+1)
	{
		QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(mCurIndexOfTree+1);
		VideoTreeItem* item2 = (VideoTreeItem*)item;
		if (item2)
		{
			item2->GetIpcData(videoItem);
		}
	}
	return videoItem;
}

VIDEO_TREE_ITEM TreeView::GetPreviousItem()
{
	VIDEO_TREE_ITEM videoItem;
	if (m_pVideoTree && mCurIndexOfTree>0)
	{
		QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(mCurIndexOfTree - 1);
		VideoTreeItem* item2 = (VideoTreeItem*)item;
		if (item2)
		{
			item2->GetIpcData(videoItem);
		}
	}
	return videoItem;
}

void TreeView::SetCurrentItem(int itemIndex)
{
	if (m_pVideoTree && itemIndex < m_pVideoTree->topLevelItem(0)->childCount())
	{
		QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(itemIndex);
		VideoTreeItem* videoItem = (VideoTreeItem*)item;
		if (videoItem && mSelTreeItem && item && m_pVideoTree)
		{
			mSelTreeItem->setSelected(false);
			videoItem->GetIpcData(mSelVideoItem);
			mCurIndexOfTree = itemIndex;
			item->setSelected(true);
			mSelTreeItem = item;
			m_pVideoTree->scrollToItem(item);
		}
	}
}

void TreeView::slot_click_addfile_btn()
{
	QStringList fileList = QFileDialog::getOpenFileNames(this, QStringLiteral("打开"), "../", QStringLiteral("媒体文件（所有文件）") + ";*.*");
	if (fileList.empty())
	{
		return;
	}

	QString dirPath = fileList.at(0);
	QFileInfo fileDev(dirPath);
	dirPath = fileDev.dir().path();

	int videoIdStart = mVecVideoList.size();
	for (int i = 0; i < fileList.size(); i++)
	{
		QString filePath = fileList.at(i);

		VIDEO_TREE_ITEM videoInfo;
		QFileInfo fileInfo(filePath);
		QString fileName = fileInfo.fileName();

		QString videoIdQ = "video_" + QString::number(videoIdStart);
		QString videoSet = "videoset_0";
		videoInfo.videoId = std::string(videoIdQ.toLocal8Bit().data());
		videoInfo.videoName = std::string(fileName.toLocal8Bit().data());
		videoInfo.videoFilePath = std::string(filePath.toLocal8Bit().data());
		videoInfo.videoPlayTimeStamp = 0;
		videoInfo.setId = std::string(videoSet.toLocal8Bit().data());

		AddVideoItem(videoInfo);
		mVecVideoList.push_back(videoInfo);
	}
	if (m_pVideoTree)
		m_pVideoTree->expandAll();
}

void TreeView::slot_click_delfile_btn()
{
	QList<QTreeWidgetItem*> listSelectedItems = m_pVideoTree->selectedItems();
	if (listSelectedItems.size() == 0)
	{
		return;
	}

	for (int i = 0; i < listSelectedItems.size(); i++)
	{
		QTreeWidgetItem* item = listSelectedItems.at(i);
		if (item->parent() == NULL)
			continue;

		if (item)
		{
			VideoTreeItem *arItem = (VideoTreeItem*)item;

			VIDEO_TREE_ITEM itemParams;
			if (arItem)
				arItem->GetIpcData(itemParams);

			//再删除列表
			m_pVideoTree->topLevelItem(0)->takeChild(m_pVideoTree->currentIndex().row());

			for (int i = 0; i < mVecVideoList.size(); i++)
			{
				if (mVecVideoList[i].videoFilePath == itemParams.videoFilePath)
				{
					mVecVideoList.erase(mVecVideoList.begin() + i);
					i--;
				}
			}
		}
	}
}

void TreeView::slot_treeItemClicked(QTreeWidgetItem* selItem, int column)
{
	QTreeWidgetItem* t_parentItem = selItem->parent();
	if (!t_parentItem)	return;

	VideoTreeItem *videoItem = (VideoTreeItem*)selItem;
	if (!videoItem)	return;

	mSelTreeItem = selItem;
	mCurIndexOfTree = m_pVideoTree->topLevelItem(0)->indexOfChild(selItem);
	videoItem->GetIpcData(mSelVideoItem);
	gCurrentTreeItemPath = QString::fromLocal8Bit(mSelVideoItem.videoFilePath.c_str());
}

void TreeView::slot_treeItemDbClicked(QTreeWidgetItem* selItem, int column)
{
	QTreeWidgetItem* t_parentItem = selItem->parent();
	if (!t_parentItem)	
		return;

	VideoTreeItem *videoItem = (VideoTreeItem*)selItem;
	if (!videoItem)
		return;

	mSelTreeItem = selItem;
	videoItem->GetIpcData(mSelVideoItem);

	emit signal_playvideo();
	mPlayingItem = selItem;
}


void TreeView::showEvent(QShowEvent *event)
{
	InitUi();
}