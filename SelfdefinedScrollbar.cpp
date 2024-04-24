#include "SelfdefinedScrollbar.h"

//自定义滚动条
SelfdefinedScrollbar::SelfdefinedScrollbar(int Flags)
{
	switch(Flags)
	{
	case HORIZONTAL_SCROLLBAR_TYPE:
		SetHorizontalScrollBarStyle();
		break;
	case VERTICAL_SCROLLBAR_TYPE:
		SetVerticalScrollBarStyle();
		break;
	default:
		break;
	}
}

SelfdefinedScrollbar::~SelfdefinedScrollbar(void)
{
}

void SelfdefinedScrollbar::SetHorizontalScrollBarStyle()
{
	this->setStyleSheet("QScrollBar:horizontal"
		"{"
		"width:16px;"
		"border-image:url(:/images/scrollbarbg.png);"
		"border:1px solid black;"
		"border-color:black;"
		"margin:0px,0px,0px,0px;"
		"padding-top:15px;"
		"padding-bottom:15px;"
		"}"
		"QScrollBar::handle:horizontal"
		"{"
		"width:16px;"
		"border-image:url(:/images/handle01.png);"
		"min-height:50;"
		"}"
		"QScrollBar::handle:horizontal:hover"
		"{"
		"width:16px;"
		"border-image:url(:/images/handle01.png);"
		"min-height:50;"
		"}"
		"QScrollBar::add-line:horizontal"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/03.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:horizontal"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/01.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-line:horizontal:hover"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/04.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:horizontal:hover"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/02.png);"
		"subcontrol-position:top;"
		"}"

		"QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal"
		"{"
		"background:rgba(0,0,0,10%);"
		"border-radius:25%;"
		"}"
		);
}

void SelfdefinedScrollbar::SetVerticalScrollBarStyle()
{
	this->setStyleSheet("QScrollBar:vertical"
		"{"
		"width:16px;"
		"border-image:url(:/images/tiao01.png);"
		"margin:0px,0px,0px,0px;"
		"padding-top:15px;"
		"padding-bottom:15px;"
		"}"
		"QScrollBar::handle:vertical"
		"{"
		//"width:15px;"
		//"border-image:url(:/images/ScrollBarHandle_high.png);"
		//"min-height:40;"
		"width:15px;"
		"background:rgba(128, 128, 128, 25 % );"
		"border-radius:4px;"   // 滚动条两端变成椭圆
		"min-height:20;"
		"}"
		"QScrollBar::handle:vertical:hover"
		"{"
		/*"width:15px;"
		"border-image:url(:/images/ScrollBarHandle_high.png);"
		"min-height:40;"*/
		"width:15px;"
		"background:rgba(128, 128, 128, 25 % );"
		"border-radius:4px;"   // 滚动条两端变成椭圆
		"min-height:20;"
		"}"
		"QScrollBar::add-line:vertical"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/03.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/01.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-line:vertical:hover"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/04.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical:hover"
		"{"
		"height:15px;width:16px;"
		"border-image:url(:/images/02.png);"
		"subcontrol-position:top;"
		"}"

		"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
		"{"
		"background:rgba(32,32,32,10%);"
		"border-radius:25%;"
		"}"
		);
}