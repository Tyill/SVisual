#include "stdafx.h"
#include "forms/mainWin.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
	QStringList paths = QCoreApplication::libraryPaths();
	paths.append(".");
	paths.append("plugins");
	QCoreApplication::setLibraryPaths(paths);
#endif

	QApplication a(argc, argv);

#ifdef SV_EN
	QTranslator translator;
    translator.load(":/SVViewer/svviewer_en.qm");
	a.installTranslator(&translator);
#endif

	MainWin w;
	w.showMaximized();
	return a.exec();
}
