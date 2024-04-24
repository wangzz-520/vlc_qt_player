#include "ToolButton2State.h"

ToolButton2State::ToolButton2State(QWidget *parent)
	: QToolButton(parent)
{
}

ToolButton2State::~ToolButton2State()
{
}

void ToolButton2State::SetUiStyleSheet(QString idleStyle, QString selectStyle, ENUM_TOOLBUTTON_STYLE_STATE styleState)
{
	mSelectStyle = selectStyle;
	mIdleStyle = idleStyle;
	SetState(styleState);
}

void ToolButton2State::SetState(ENUM_TOOLBUTTON_STYLE_STATE styleState)
{
	switch (styleState)
	{
	case ENUM_TOOLBUTTON_IDLE:
		setStyleSheet(mIdleStyle);
		break;
	case ENUM_TOOLBUTTON_SELECT:
		setStyleSheet(mSelectStyle);
		break;
	default:
		break;
	}
}