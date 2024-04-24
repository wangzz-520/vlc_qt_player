#include "VideoTreeItem.h"

VideoTreeItem::VideoTreeItem(QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{
}

VideoTreeItem::VideoTreeItem(QTreeWidgetItem *parent, const QStringList &strings, int type)
	: QTreeWidgetItem(parent, strings, type)
{
}

VideoTreeItem::~VideoTreeItem()
{
}

void VideoTreeItem::SetIpcData(VIDEO_TREE_ITEM itemData)
{
	mItemData = itemData;
}

void VideoTreeItem::GetIpcData(VIDEO_TREE_ITEM &itemData)
{
	itemData = mItemData;
}