#pragma once

#include <QToolButton>
#include "Global.h"

using namespace zz;

class ToolButton2State : public QToolButton
{
	Q_OBJECT

public:
	ToolButton2State(QWidget *parent);
	~ToolButton2State();

	void SetUiStyleSheet(QString idleStyle, QString selectStyle, ENUM_TOOLBUTTON_STYLE_STATE styleState = ENUM_TOOLBUTTON_IDLE);
	void SetState(ENUM_TOOLBUTTON_STYLE_STATE styleState);

private:
	QString		mSelectStyle;
	QString		mIdleStyle;
};
