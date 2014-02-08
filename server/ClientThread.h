/****************************************************************************************
 ** Server is an application to manage several clients inside a thread.
 ** Copyright (C) 2013  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/
 
#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "Client.h"

class ClientThread : public QThread
{
    Q_OBJECT

    signals:
        void newClientMessage(const QString &clientName);
        void applyPPCMessage(const QString &clientName, const QString &PPCIP);
        void giveBackPPCMessage(const QString &clientName, const QString &PPCIP);
		void giveBackPPCMessageResult(const QString &clientName, const QString &PPCIP);
		void updateClientPPCStatusMessage(const QString &clientName);
        void clientClosed(int id);

    public:
        explicit ClientThread(int _id, QObject *parent = 0);
        void run();
        void sendMessage(const QString &msg);
        Client * getClient() const { return client; }
		int getThreadId() const { return socketId; }
        
    public slots:
        void readMsg();
        void disconnected();

    private:
        Client *client;
		bool quit;
		QStringList msgToWrite;
        int socketId;

        
        quint16 nextBlockSize;
        
};

#endif // CLIENTTHREAD_H

