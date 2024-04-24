#include "TitleView.h"
#include <QPoint>
#include <QDesktopWidget>

const int const_mainwnd_height = 40;
const int const_menubtn_width = 39;
const int const_menubtn_height = 26;
const int const_modebtn_width = 81;
const int const_modebtn_height = 30;
const int const_operabtn_width = 20;
const int const_operabtn_height = 20;

TitleView::TitleView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QDesktopWidget desktop;
	mAvailableWidth = desktop.availableGeometry().width();
	mAvailabelHeight = desktop.availableGeometry().height();

	//设置ui界面
	setWindowFlags(Qt::FramelessWindowHint);

	setFixedHeight(const_mainwnd_height);
	ui.logo_btn->setFixedSize(const_modebtn_height, const_modebtn_height);
	ui.min_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.max_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.restore_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.close_btn->setFixedSize(const_operabtn_width, const_operabtn_height);

	ui.min_btn->setToolTip(QStringLiteral("最小化"));
	ui.max_btn->setToolTip(QStringLiteral("最大化"));
	ui.restore_btn->setToolTip(QStringLiteral("还原"));
	ui.close_btn->setToolTip(QStringLiteral("关闭"));

	ui.logo_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/log_min.png);}"
		"QToolButton:hover{border-image: url(:/images/log_min.png);}"
		"QToolButton:pressed{border-image: url(:/images/log_min.png);}"
	);
	ui.min_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_min_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_min_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_min_press.png);}"
	);
	ui.max_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_max_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_max_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_max_press.png);}"
	);
	ui.restore_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_restore_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_restore_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_restore_press.png);}"
	);
	ui.close_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_close_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_close_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_close_press.png);}"
	);

	ui.restore_btn->setVisible(false);

	connect(ui.min_btn, SIGNAL(clicked()), this, SLOT(slot_click_minimum_btn()));
	connect(ui.max_btn, SIGNAL(clicked()), this, SLOT(slot_click_maximum_btn()));
	connect(ui.restore_btn, SIGNAL(clicked()), this, SLOT(slot_click_normal_btn()));
	connect(ui.close_btn, SIGNAL(clicked()), this, SLOT(slot_click_close_btn()));
}

TitleView::~TitleView()
{
}

void TitleView::slot_click_minimum_btn()
{
	emit signal_show_minimum();
}

void TitleView::slot_click_maximum_btn()
{
	ui.max_btn->setVisible(false);
	ui.restore_btn->setVisible(true);
	emit signal_show_maximised();
}

void TitleView::slot_click_normal_btn()
{
	ui.max_btn->setVisible(true);
	ui.restore_btn->setVisible(false);
	emit signal_show_normal();
}

void TitleView::slot_click_close_btn()
{
	emit signal_close_wnd();
}

void TitleView::showEvent(QShowEvent *event)
{
	QPalette t_BgPalette;
	t_BgPalette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/title_bg.png").
		scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	this->setAutoFillBackground(true);
	this->setPalette(t_BgPalette);
}

void TitleView::resizeEvent(QResizeEvent *event)
{
	QPalette t_BgPalette;
	t_BgPalette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/title_bg.png").
		scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	this->setAutoFillBackground(true);
	this->setPalette(t_BgPalette);
}