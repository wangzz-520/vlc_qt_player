#ifndef JM_LISTTREEWIDGET_H
#define JM_LISTTREEWIDGET_H

#include <QFont>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTextCodec>
#include <QScrollBar>
#include <QToolButton>
#include <QTreeWidget>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QWidget>
#include "SelfdefinedScrollbar.h"
#include "Global.h"
#include "VideoTreeItem.h"

class ListTreeWidget: public QTreeWidget
{
	Q_OBJECT

public:
	ListTreeWidget();

public:
	//�����;
	void AddGroup(QString GroupName);
	//��ӳ�Ա;
	void AddMember(int GroupOrder, QString MemberName, VIDEO_TREE_ITEM itemData);
};

#endif