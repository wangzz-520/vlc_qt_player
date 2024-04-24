#include "ListTreeWidget.h"
#include "VideoTreeItem.h"

ListTreeWidget::ListTreeWidget()
{
	//设备列表皮肤
	QFont serifFont(tr("微软雅黑"),10);
	this->setFont(serifFont);

	//设备列表
	int t_width1 = this->width();
	this->setColumnWidth(0,50);
	this->setColumnWidth(1,t_width1-50);

	this->setRootIsDecorated(false);
	this->setFocusPolicy(Qt::NoFocus);
	this->setFrameStyle(QFrame::Panel);
	this->setStyleSheet("QTreeWidget{background: rgb(32, 32, 32);}"
		"QTreeWidget::item{height: 30px;}"
		"QTreeWidget::branch{image: none;}"
		"QTreeWidget::item:!selected{padding-left: 0px;}"
		"QTreeWidget::item:selected{border-image: url(:/images/itemteambackground.png)}"
		"QTreeWidget::item:!selected{border-image: url(:/images/itemteambackground2.png)}"
		"QTreeWidget::item:selected{color: white;}"
		"QTreeWidget::item:!selected{color: gray;}"
		);
	//设置设备列表表头
	this->headerItem()->setText(0,"");
	this->header()->setStyleSheet("QHeaderView::section"
		"{"
		"padding-left:10px;"
		"min-height:30;"
		"border-image: url(:/images/deviceList_bg.png);"
		"border:1px solid #1b1b1b;"
		"}"
		);

	SelfdefinedScrollbar *pVerticalScrollbar = new SelfdefinedScrollbar(VERTICAL_SCROLLBAR_TYPE);
	this->setVerticalScrollBar(pVerticalScrollbar);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void ListTreeWidget::AddGroup(QString GroupName)
{
	QTreeWidgetItem *t_NewTreeWidgetItem = new QTreeWidgetItem(this, QStringList(GroupName));
	t_NewTreeWidgetItem->setBackground(0, QBrush(QPixmap(":/images/GroupMemberBg.png")));
	t_NewTreeWidgetItem->setForeground(0, QBrush(QColor(Qt::gray)));
	this->addTopLevelItem(t_NewTreeWidgetItem);
}

void ListTreeWidget::AddMember(int GroupOrder, QString MemberName, VIDEO_TREE_ITEM itemData)
{
	if(topLevelItemCount() == 0)
	{
		return;
	}
	else
	{
		QTreeWidgetItem *t_CurrentTreeItem = this->topLevelItem(GroupOrder);
		VideoTreeItem *t_newItem = new VideoTreeItem(t_CurrentTreeItem, QStringList(MemberName));
		t_newItem->setBackground(0, QBrush(QPixmap(":/images/itemteambackground2.png")));
		t_newItem->setForeground(0, QBrush(QColor(Qt::gray)));
		t_newItem->setIcon(0, QIcon("images/camera.png"));
		t_newItem->SetIpcData(itemData);
		t_newItem->setToolTip(0, QString::fromLocal8Bit(itemData.videoFilePath.c_str()));
		t_CurrentTreeItem->insertChild(GroupOrder, t_newItem);
		setCurrentItem(t_CurrentTreeItem);
	}
	this->expandAll();
}