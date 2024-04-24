#include "PlayActivePageView.h"

const int openfile_btn_width = 88;
const int openfile_btn_height = 16;
const int openmenu_btn_width = 29;
const int openmenu_btn_height = 45;
const int menu_btn_width = 158;
const int menu_btn_height = 22;
const int ctrl_height =291;
const int ctrl_width = 336;

PlayActivePageView::PlayActivePageView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	InitUi();

	ui.open_dir_btn->setVisible(false);
	ui.open_url_btn->setVisible(false);

	connect(ui.open_file_btn, SIGNAL(clicked()), this, SLOT(slot_click_openfile_btn()));
	connect(ui.open_menu_btn, SIGNAL(clicked()), this, SLOT(slot_click_openmenu_btn()));
	connect(ui.open_url_btn, SIGNAL(clicked()), this, SLOT(slot_click_openurl_btn()));
	connect(ui.open_dir_btn, SIGNAL(clicked()), this, SLOT(slot_click_opendir_btn()));
}

PlayActivePageView::~PlayActivePageView()
{
}

void PlayActivePageView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	ui.widget->setFixedSize(menu_btn_width, openmenu_btn_height);
	ui.widget_2->setFixedSize(ctrl_width, ctrl_height);
	ui.widget_4->setFixedSize(menu_btn_width + openmenu_btn_width, openmenu_btn_height);
	ui.open_file_btn->setFixedSize(openfile_btn_width, openfile_btn_height);
	ui.open_menu_btn->setFixedSize(openmenu_btn_width, openmenu_btn_height);
	ui.open_url_btn->setFixedSize(menu_btn_width + openmenu_btn_width + 1, menu_btn_height);
	ui.open_dir_btn->setFixedSize(menu_btn_width + openmenu_btn_width + 1, menu_btn_height);

	ui.open_file_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_open_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_open_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_open_press.png);}"
		);
	ui.open_menu_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/combo_open_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/combo_open_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/combo_open_press.png);}"
		);
	ui.open_url_btn->setStyleSheet(
		"QToolButton{background-color: rgb(7,7,7,60);}"
		"QToolButton:hover{background-color: rgb(20,52,91);}"
		"QToolButton:pressed{background-color: rgb(20,52,91);}"
		"QToolButton{color:rgb(154,154,154); font: 75 16px;}"
		);
	ui.open_dir_btn->setStyleSheet(
		"QToolButton{background-color: rgb(7,7,7,60);}"
		"QToolButton:hover{background-color: rgb(20,52,91);}"
		"QToolButton:pressed{background-color: rgb(20,52,91);}"
		"QToolButton{color:rgb(154,154,154); font: 75 16px;}"
		);
}

void PlayActivePageView::SetActiveMode(bool active)
{
	ui.widget_2->setVisible(!active);
}

void PlayActivePageView::slot_click_openfile_btn()
{
	emit signal_openvideobtn_clicked();
}

void PlayActivePageView::slot_click_openmenu_btn()
{
	ui.open_dir_btn->setVisible(true);
	ui.open_url_btn->setVisible(true);
	m_type = PLAYACTIVE_OPEN_DIR;
}

void PlayActivePageView::slot_click_openurl_btn()
{
	ui.open_dir_btn->setVisible(false);
	ui.open_url_btn->setVisible(false);

	m_type = PLAYACTIVE_OPEN_URL;

	emit signal_openmenubtn_clicked(m_type);
}

void PlayActivePageView::slot_click_opendir_btn()
{
	ui.open_dir_btn->setVisible(false);
	ui.open_url_btn->setVisible(false);

	m_type = PLAYACTIVE_OPEN_DIR;
	emit signal_openmenubtn_clicked(m_type);
}

void PlayActivePageView::showEvent(QShowEvent *event)
{
	QPalette t_BgPalette;
	t_BgPalette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/bk1.png").\
		scaled(ui.widget->width(), ui.widget->height(),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	ui.widget->setAutoFillBackground(true);
	ui.widget->setPalette(t_BgPalette);

	QPalette pal(palette());
	pal.setBrush(QPalette::Background, QBrush(QPixmap(":/images/bk_logo.png").\
		scaled(ui.logo_label->width(), ui.logo_label->height(),
			Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	ui.logo_label->setAutoFillBackground(true);
	ui.logo_label->setPalette(pal);

	m_type = PLAYACTIVE_OPEN_DIR;
}