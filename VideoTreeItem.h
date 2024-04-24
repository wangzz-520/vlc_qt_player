#ifndef VIDEOTREEITEM_H
#define VIDEOTREEITEM_H

#include <QTreeWidgetItem>
#include "Global.h"

using namespace zz;

class VideoTreeItem : public QTreeWidgetItem
{
public:
	VideoTreeItem(QTreeWidgetItem *parent);
	VideoTreeItem(QTreeWidgetItem *parent, const QStringList &strings, int type = Type);
	~VideoTreeItem();

	//设置、获取节点数据
	void SetIpcData(VIDEO_TREE_ITEM itemData);
	void GetIpcData(VIDEO_TREE_ITEM &itemData);

private:
	//IPC参数
	VIDEO_TREE_ITEM	mItemData;
};

#endif