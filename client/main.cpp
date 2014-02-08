#include <QtGui/QApplication>
#include <QDir>
#include "mainwindow.h"
#include "logindialog.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QLoggerManager *manager = QLoggerManager::getInstance();
	qDebug() << QDir::currentPath();
	QStringList moduleList;
	moduleList.append("ManagePPCClient");
	manager->addDestination(QDir::currentPath() + "/logs/ManagePPCClient.log", QStringList("ManagePPCClient"),QLogger::TraceLevel);

	LoginDialog ld;
	ld.exec();

	if (ld.login() != "")
	{
		MainWindow mainWindow(ld.login());
		ld.close();
		mainWindow.show();
		return a.exec();
	}
}
