#include "ZPlayer.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QApplication::addLibraryPath("./plugins");
	ZPlayer w;
	w.show();
	return a.exec();
}
