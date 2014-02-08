#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class Thread: public QThread
{
public:
	Thread(QObject *parent = 0);
	void run();

};

#endif