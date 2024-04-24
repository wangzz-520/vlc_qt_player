#ifndef	JM_SELFDEFINEDSCROLLBAR_H
#define JM_SELFDEFINEDSCROLLBAR_H

#include "qscrollbar.h"
#include "Global.h"

/**
 * 自定义滚动条.
 */
class SelfdefinedScrollbar : public QScrollBar
{
	Q_OBJECT

public:
	SelfdefinedScrollbar(int Flags);
	~SelfdefinedScrollbar(void);

private:
	//设置横向的风格
	void SetHorizontalScrollBarStyle();

	//设置竖向的风格
	void SetVerticalScrollBarStyle();
};

#endif
