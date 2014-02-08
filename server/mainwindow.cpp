
#include <QtGui/QFont>
#include <QTextEdit>
#include <QMessageBox>
#include <QValidator>
#include <QTableWidget>
#include <QMutexLocker>
#include <QFile>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), telnetPPC( new TelnetPPC), 
	  connectPPCTimer(new QTimer), occupyPPCTimer(new QTimer), 
	  updateServerIPStatusTimer(new QTimer), takebackPPC(false), occupying(false)
{
	setupUi(this);

	occupyPPCTimer->setInterval(3000);
    updateServerIPStatusTimer->setInterval(1000);
	connectPPCTimer->setInterval(3000);

    //Initialize IPList
    iniIPList();

	iteratorTelnetList = telnetList.begin();

    //Configure textEdit
    QFont fnt = textEdit->font();
    fnt.setFamily("Fixed");
    textEdit->setFont(fnt);
    textEdit->setReadOnly(true);

    //Configure ipEdit
    ipEdit->setMaxLength(15);
    //QValidator *validator = new QIntValidator(0, 999, this);
    //ipEdit->setValidator(validator);

	connect(startConnectButton, SIGNAL(clicked()), connectPPCTimer, SLOT(start()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetPPC()));
	connect(resetAllButton, SIGNAL(clicked()), this, SLOT(resetAllPPC()));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(closeMainWindow()));


	connect(connectPPCTimer, SIGNAL(timeout()), this, SLOT(connectPPC()));
    

	connect(occupyPPCTimer, SIGNAL(timeout()), this, SLOT(occupyPPC()));
	connect(updateServerIPStatusTimer, SIGNAL(timeout()), this, SLOT(updateServerIPStatusList()));    

    //signal-slot connection that is relevant to telnetPPC
	connect(telnetPPC, SIGNAL(telnetResult(const QString &)), this, SLOT(processConnectPPCResult(const QString &)));
	connect(telnetPPC, SIGNAL(receivedMsg(const QString&)), textEdit, SLOT(setText(const QString&)));

    //signal-slot connection that is relevant to server
	server = new Server;
	QHostAddress serverAddress("172.16.20.2");
	if (!server->listen(serverAddress, 5555)) 
	{
		QMessageBox::critical(this, tr("Server"),
			                  tr("Unable to start the server: %1.").arg(server->errorString()));
		close();
		return;
	}
    updateServerIPStatusTimer->start();
	statusLabel->setText(tr("The server is running on port %1.\n").arg(server->serverPort()));
	connect(server, SIGNAL(applyPPCMessage(QString, QString)), this, SLOT(processApplyPPC(QString, QString)));
	connect(server, SIGNAL(giveBackPPCMessage(QString, QString)), this, SLOT(processGiveBackPPC(QString, QString)));
	connect(server, SIGNAL(giveBackPPCMessageResult(QString, QString)), this, SLOT(sendGiveBackPPCResult()));
	
}

void MainWindow::connectPPC()
{
    qDebug() << "(*iteratorIPtatusList).IP: " << (*iteratorIPtatusList).IP;
    
    if (++iteratorIPtatusList == IPStatusList.end())
    {
        iteratorIPtatusList = IPStatusList.begin();
    }
    
    //Finding PPC that is not occupied
    qDebug() << "(*iteratorIPtatusList).IP: " << (*iteratorIPtatusList).IP;
    for (int i = 0; OCCUPY == (*iteratorIPtatusList).status && i < IPStatusList.size(); i++ )
    {
        if (iteratorIPtatusList == IPStatusList.end()-1 )
            iteratorIPtatusList = IPStatusList.begin();
        else
            iteratorIPtatusList++;
    }
    
    if (OCCUPY != (*iteratorIPtatusList).status)
    {
        qDebug() << "Connecting to " << (*iteratorIPtatusList).IP << "status" << (*iteratorIPtatusList).status;
        textEdit->append(QString("Connect to %1").arg((*iteratorIPtatusList).IP));
        telnetPPC->connectPPC((*iteratorIPtatusList).IP);
    }
    else
    {
        //QMessageBox::warning(this, tr("Warning"),tr("No PPC available to connect"));
    }

    connectPPCTimer->stop();
}

void MainWindow::resetAllPPC()
{
	connectPPCTimer->stop();

	QList<IPStatus>::iterator tempIteratorIPStatusList;
	for (tempIteratorIPStatusList = IPStatusList.begin(); tempIteratorIPStatusList != IPStatusList.end(); tempIteratorIPStatusList++)
	{
		resetPPC((*tempIteratorIPStatusList).IP);
	}
	connectPPCTimer->start();
}


void MainWindow::resetPPC()
{
	connectPPCTimer->stop();
	
	QList<IPStatus>::iterator tempIteratorIPStatusList;
	qDebug() << ipEdit->text();

	if (!ipEdit->text().isEmpty())
	{
		for (tempIteratorIPStatusList = IPStatusList.begin(); tempIteratorIPStatusList != IPStatusList.end(); tempIteratorIPStatusList++)
		{
			if (((*tempIteratorIPStatusList).IP == ipEdit->text()) && 
				((*tempIteratorIPStatusList).status == OCCUPY))
			{
				qDebug() << "(*tempIteratorIPStatusList).IP:" << (*tempIteratorIPStatusList).IP;
                connectStatusMap[(*tempIteratorIPStatusList).IP]->disconnectPPC();
				delete connectStatusMap.value((*tempIteratorIPStatusList).IP);
                telnetList.removeAll(connectStatusMap[(*tempIteratorIPStatusList).IP]);
                (connectStatusMap.remove((*tempIteratorIPStatusList).IP));

				(*tempIteratorIPStatusList).status = UNKNOWN;
				(*tempIteratorIPStatusList).owner = UNKNOWN;
				ipTable->setItem(IPStatusList.indexOf(*tempIteratorIPStatusList), 1, new QTableWidgetItem(IDLE));
			}
		}
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"),tr("Please input IP firstly"));
	}

	connectPPCTimer->start();
}

void MainWindow::resetPPC(const QString &ppcIP)
{
	QList<IPStatus>::iterator tempIteratorIPStatusList;
	qDebug() << ppcIP;

	for (tempIteratorIPStatusList = IPStatusList.begin(); tempIteratorIPStatusList != IPStatusList.end(); tempIteratorIPStatusList++)
	{
		if (((*tempIteratorIPStatusList).IP == ppcIP) && 
			((*tempIteratorIPStatusList).status == OCCUPY))
		{
			connectStatusMap[(*tempIteratorIPStatusList).IP]->disconnectPPC();
			delete connectStatusMap.value((*tempIteratorIPStatusList).IP);
			telnetList.removeAll(connectStatusMap[(*tempIteratorIPStatusList).IP]);
			(connectStatusMap.remove((*tempIteratorIPStatusList).IP));

			(*tempIteratorIPStatusList).status = UNKNOWN;
			(*tempIteratorIPStatusList).owner = UNKNOWN;
			ipTable->setItem(IPStatusList.indexOf(*tempIteratorIPStatusList), 1, new QTableWidgetItem(IDLE));
		}
	}

}

void MainWindow::occupyPPC()
{
	occupyPPCTimer->stop();

	if (!connectStatusMap.contains((*iteratorIPtatusList).IP))
	{
		TelnetPPC *telnetIdlePPC = new TelnetPPC;
		connect(telnetIdlePPC, SIGNAL(telnetResult(const QString &)), this, SLOT(processConnectPPCResult(const QString &)));
		connect(telnetIdlePPC, SIGNAL(receivedMsg(const QString&)), textEdit, SLOT(setText(const QString&)));

		telnetList.append(telnetIdlePPC);
		connectStatusMap.insert((*iteratorIPtatusList).IP, telnetIdlePPC);
	}
	//telnetList.last()->connectPPC((*iteratorIPtatusList).IP);
	(connectStatusMap.value((*iteratorIPtatusList).IP))->connectPPC((*iteratorIPtatusList).IP);

}

void MainWindow::processConnectPPCResult(const QString &telnetResult)
{
	qDebug() << "Ip: " << (*iteratorIPtatusList).IP << "Status: " << telnetResult;

    if (!takebackPPC)
    {
        if (IDLE == telnetResult)
        {        
        
            if (occupying) //finish occupying
            {
                occupying = false;
                (*iteratorIPtatusList).status = OCCUPY;
                (*iteratorIPtatusList).owner = "Server";
                connectPPCTimer->start();
        
            }
            else //begin occupying
            {
                telnetPPC->disconnectPPC();
                occupying = true;
                occupyPPCTimer->start();
            }
        }
        else if (BUSY == telnetResult || TIMEOUT == telnetResult || ERROR == telnetResult)
        {
            (*iteratorIPtatusList).status = telnetResult;
            ipTable->setItem(IPStatusList.indexOf(*iteratorIPtatusList), 1, new QTableWidgetItem(telnetResult));
            telnetPPC->disconnectPPC();
            connectPPCTimer->start();
        }
        else
        {
        }
    }
    else
    {
		if (IDLE == telnetResult)
        {        
            occupying = false;
			giveBackPPCResultMap[(*iteratorIPtatusList).IP] = true;
			(*iteratorIPtatusList).status = OCCUPY;
			(*iteratorIPtatusList).owner = "Server";

        }
        else
        {
            giveBackPPCResultMap[(*iteratorIPtatusList).IP] = false;

        }
    }

}

void MainWindow::iniIPList()
{
	QList<IPStatus>::iterator  tempIteratorIPStatusList;

	QFile file("ppc.cfg");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, tr("Warning"),tr("Cann't open ppc.cfg"));
		return;
	}

	while (!file.atEnd()) 
	{
		QString line = QString(file.readLine()).remove(" ");
		line = line.remove('\n');
		line = line.remove('\r');

		if (line.startsWith("172.16"))
			IPStatusList << *iniIPStatus(line);
	} 

	iteratorIPtatusList = IPStatusList.begin();
    updateTableWidget();
}

IPStatus* MainWindow::iniIPStatus(const QString IP)
{
    IPStatus *ipStatus = new IPStatus;

    ipStatus->IP = IP;
    ipStatus->status = "Unknown";
    ipStatus->owner = "Unknown";

    return ipStatus;
}

void MainWindow::updateTableWidget()
{
	QTableWidgetItem *itemIP;
	QTableWidgetItem *itemStatus;
	QTableWidgetItem *itemOwner;
    
	QList<IPStatus>::iterator  tempIteratorIPStatusList;

	qDebug() << "IPStatusList.size(): " << IPStatusList.size();
	ipTable->setRowCount(IPStatusList.size());
	ipTable->setColumnCount(3);

	for (tempIteratorIPStatusList = IPStatusList.begin(); tempIteratorIPStatusList != IPStatusList.end(); tempIteratorIPStatusList++)
	{        
		itemIP = new QTableWidgetItem((*tempIteratorIPStatusList).IP);
		itemStatus = new QTableWidgetItem((*tempIteratorIPStatusList).status);
		itemOwner = new QTableWidgetItem((*tempIteratorIPStatusList).owner);

		qDebug() << "IPStatusList.indexOf(*tempIteratorIPStatusList): " << IPStatusList.indexOf(*tempIteratorIPStatusList);

		ipTable->setItem(IPStatusList.indexOf(*tempIteratorIPStatusList), 0, itemIP);
		ipTable->setItem(IPStatusList.indexOf(*tempIteratorIPStatusList), 1, itemStatus);
		ipTable->setItem(IPStatusList.indexOf(*tempIteratorIPStatusList), 2, itemOwner);

	}
}


void MainWindow::processApplyPPC(const QString clientName, const QString ppcIP)
{
	QList<IPStatus>::iterator  tempIteratorIPStatusList;

	qDebug() << "clientName" << clientName << "ppcIP" << ppcIP;
    for (tempIteratorIPStatusList = IPStatusList.begin(); tempIteratorIPStatusList != IPStatusList.end(); tempIteratorIPStatusList++)
    {
		if (ppcIP == (*tempIteratorIPStatusList).IP)
		{
			
			if (IDLE == (*tempIteratorIPStatusList).status)
			{
				QMessageBox::warning(this, tr("Oh, My God!"),
							QString("%1 is idle, but the server don't occupy it.\n" 
									   "This is a bug").arg(ppcIP));
			}
			else if (OCCUPY == (*tempIteratorIPStatusList).status)
			{
				if ("Server" == (*tempIteratorIPStatusList).owner)
				{
					(connectStatusMap.value(ppcIP))->disconnectPPC();
					(*tempIteratorIPStatusList).owner = clientName;
					server->sendMsg(APPLY_SUCCESS, QString("Apply %1 Success").arg(ppcIP));
					updateTableWidget();
				}
				else
				{
					server->sendMsg(APPLY_FAILED, QString("Sorry!\nThe PPC has been occupied by %1").arg((*tempIteratorIPStatusList).owner));
				}

			}
			else if (BUSY == (*tempIteratorIPStatusList).status)
			{
				server->sendMsg(APPLY_FAILED, QString("Sorry!\n%1 has been occupied by somebody").arg(ppcIP));
			}
			else if (TIMEOUT == (*tempIteratorIPStatusList).status)
			{
				server->sendMsg(APPLY_FAILED, QString("%1 can't be connected").arg(ppcIP));
			}
			else if (UNKNOWN == (*tempIteratorIPStatusList).status)
			{
				server->sendMsg(APPLY_FAILED, QString("%1 status is unknown").arg(ppcIP));
			}
			else
			{
			}
			break;
		}
    }

   if (tempIteratorIPStatusList == IPStatusList.end())
   {
	   server->sendMsg(APPLY_FAILED, QString("Are you kidding?\n%1 is not defined in server").arg(ppcIP));
   }
}


void MainWindow::processGiveBackPPC(const QString clientName, const QString ppcIP)
{
	QList<IPStatus>::iterator  tempIteratorIPStatusList;

	qDebug() << "clientName" << clientName << "ppcIP" << ppcIP;
	for (tempIteratorIPStatusList = IPStatusList.begin(); tempIteratorIPStatusList != IPStatusList.end(); tempIteratorIPStatusList++)
	{
		if (ppcIP == (*tempIteratorIPStatusList).IP)
		{

			if (IDLE == (*tempIteratorIPStatusList).status)
			{
				QMessageBox::warning(this, tr("Oh, My God!"),
					QString("%1 is idle, but the server don't occupy it.\n" 
					"This is a bug").arg(ppcIP));
			}
			else if (OCCUPY == (*tempIteratorIPStatusList).status)
			{
				if (clientName == (*tempIteratorIPStatusList).owner)
				{
                    takebackPPC = true;
					//Using iteratorIPtatusList to transfer data that will be used in processConnectPPCResult()
					connectPPCTimer->stop();
                    telnetPPC->disconnectPPC();
					iteratorIPtatusList = tempIteratorIPStatusList;
					giveBackPPCResultMap[ppcIP] = false;


					occupyPPCTimer->start();
				}
				else
				{
					server->sendMsg(GIVE_BACK_FAILED, QString("Are you kidding?\nYou do not occupy %1").arg(ppcIP));
				}

			}
			else if (BUSY == (*tempIteratorIPStatusList).status)
			{
				server->sendMsg(GIVE_BACK_FAILED, QString("Are you kidding?\nYou do not occupy %1").arg(ppcIP));
			}
			else if (TIMEOUT == (*tempIteratorIPStatusList).status)
			{
				server->sendMsg(GIVE_BACK_FAILED, QString("%1 can't be connected").arg(ppcIP));
			}
			else if (UNKNOWN == (*tempIteratorIPStatusList).status)
			{
				server->sendMsg(GIVE_BACK_FAILED, QString("%1 status is unknown").arg(ppcIP));
			}
			else
			{
			}
			break;
		}
		else
		{
		}
	}

	if (tempIteratorIPStatusList == IPStatusList.end())
	{
		server->sendMsg(APPLY_FAILED, QString("Are you kidding?\n%1 is not defined in server").arg(ppcIP));
	}
}

void MainWindow::sendGiveBackPPCResult()
{
	if (giveBackPPCResultMap.contains((*iteratorIPtatusList).IP))
	{
		if (giveBackPPCResultMap[(*iteratorIPtatusList).IP])
			server->sendMsg(GIVE_BACK_SUCCESS, QString("Give Back %1 Success").arg((*iteratorIPtatusList).IP));
		else
			server->sendMsg(GIVE_BACK_FAILED, QString("Give Back %1 Failed").arg((*iteratorIPtatusList).IP));
		
		giveBackPPCResultMap.remove((*iteratorIPtatusList).IP);

	}
}

void MainWindow::updateServerIPStatusList()
{
	server->updatePPCStatus(IPStatusList);
	updateTableWidget();
}


void MainWindow::closeMainWindow()
{
	qDebug() << telnetList.size();
	QList<TelnetPPC *>::iterator iteratorTelnetListTemp;

	occupyPPCTimer->stop();

	for (iteratorTelnetListTemp = telnetList.begin(); iteratorTelnetListTemp != telnetList.end() && !telnetList.isEmpty(); iteratorTelnetListTemp++)
	{
		qDebug() << (*iteratorTelnetListTemp);
		(*iteratorTelnetListTemp)->disconnect();
	}
	
	close();
}


MainWindow::~MainWindow()
{

}

bool operator ==(const IPStatus &lIP, const IPStatus &rIP)
{
	if (lIP.IP == rIP.IP && lIP.status == rIP.status && lIP.owner == rIP.owner)
		return true;
	else
		return false;
}


bool operator <(const IPStatus &lIP, const IPStatus &rIP)
{
    if (lIP.IP < rIP.IP)
        return true;
    else
        return false;
}

