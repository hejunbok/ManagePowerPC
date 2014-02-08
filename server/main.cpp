#include <QtGui/QApplication>
#include <QDir>
#include <QStringList>
#include "mainwindow.h"
#include "QLogger.h"

using namespace QLogger;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    
    QLoggerManager *manager = QLoggerManager::getInstance();
    qDebug()<<QDir::currentPath();
    QStringList moduleList;
    moduleList.append("Server");
    moduleList.append("ClientThread");
    manager->addDestination(QDir::currentPath() + "/logs/sever.log", QStringList("Server"),QLogger::TraceLevel);
    manager->addDestination(QDir::currentPath() + "/logs/client.log", QStringList("ClientThread"),QLogger::TraceLevel);

	MainWindow w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}

