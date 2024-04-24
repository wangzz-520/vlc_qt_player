#ifndef	JM_SELFDEFINEDSCROLLBAR_H
#define JM_SELFDEFINEDSCROLLBAR_H

#include "qscrollbar.h"
#include "Global.h"

/**
 * �Զ��������.
 */
class SelfdefinedScrollbar : public QScrollBar
{
	Q_OBJECT

public:
	SelfdefinedScrollbar(int Flags);
	~SelfdefinedScrollbar(void);

private:
	//���ú���ķ��
	void SetHorizontalScrollBarStyle();

	//��������ķ��
	void SetVerticalScrollBarStyle();
};

#endif
