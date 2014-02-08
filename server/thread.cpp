
#include "thread.h"
#include <QDebug>
Thread::Thread(QObject *parent)
	:QThread(parent)
{
	//start();
}


void Thread::run()
{

	exec();
	qDebug() << "RUN" << thread();
}
