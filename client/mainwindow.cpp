
#include <QtGui/QFont>
#include <QTextEdit>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QProcess>
#include "mainwindow.h"
#include <stdio.h>

MainWindow::MainWindow(const QString cName, QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), clientName(cName), client(new ManagePPCClient(clientName)),
	getGiveBackPPCResultTimer(new QTimer)
{
	setupUi(this);

	getGiveBackPPCResultTimer->setInterval(5000);
	connect(getGiveBackPPCResultTimer, SIGNAL(timeout()), this, SLOT(getGiveBackPPCResult()));

	connect(applyPPCButton, SIGNAL(clicked()), this, SLOT(applyPPC()));
	connect(giveBackPPCButton, SIGNAL(clicked()), this, SLOT(giveBackPPC()));
	connect(updatePPCStatusButton, SIGNAL(clicked()), this, SLOT(updatePPCStatus()));

    //client's signal and slot connection to GUI
	connect(client, SIGNAL(connectedToServer()), this, SLOT(connectedToServer()));
	connect(client, SIGNAL(resultFromServer(const QString &, const QString &)), 
			this, SLOT(dealWithResultFromServer(const QString &, const QString &)));
    connect(client, SIGNAL(updateTableWidget(QList<IPStatus> &)), this, SLOT(updateTableWidget(QList<IPStatus> &)));
	connect(client, SIGNAL(connectToServerError(const QString &)), this, SLOT(connectToServerError(const QString &)));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

}

void MainWindow::connectedToServer()
{
    statusLabel->setText("Connected to server");
    ipLineEdit->setReadOnly(false);
	enableAllButton();
}

void MainWindow::applyPPC()
{
	disableAllButton();
	client->sendMsg(APPLY_PPC, ipLineEdit->text());

}

void MainWindow::giveBackPPC()
{
	disableAllButton();
	client->sendMsg(GIVE_BACK_PPC, ipLineEdit->text());
	getGiveBackPPCResultTimer->start();
}

void MainWindow::getGiveBackPPCResult()
{
	disableAllButton();
	client->sendMsg(GIVE_BACK_PPC_RESULT, ipLineEdit->text());
	getGiveBackPPCResultTimer->stop();
}


void MainWindow::updatePPCStatus()
{
	disableAllButton();
	client->sendMsg(UPDATE_PPC_STATUS);
}




void MainWindow::dealWithResultFromServer(const QString &result, const QString &resultContent)
{
	qDebug() << "result" << result << "resultContent" << resultContent;

	QMessageBox::information(this, result, resultContent);
	updatePPCStatus();
	enableAllButton();
}

void MainWindow::updateTableWidget(QList<IPStatus> &IPStatusList)
{
	QTableWidgetItem *itemIP;
	QTableWidgetItem *itemStatus;
	QTableWidgetItem *itemOwner;

	qDebug() << "IPStatusList.size(): " << IPStatusList.size();
	ipTableWidget->setRowCount(IPStatusList.size());
	ipTableWidget->setColumnCount(3);

	for (int i = 0; i < IPStatusList.size(); i++)
	{
		itemIP = new QTableWidgetItem(IPStatusList.at(i).IP);
		itemStatus = new QTableWidgetItem(IPStatusList.at(i).status);
		itemOwner = new QTableWidgetItem(IPStatusList.at(i).owner);

		ipTableWidget->setItem(i, 0, itemIP);
		ipTableWidget->setItem(i, 1, itemStatus);
		ipTableWidget->setItem(i, 2, itemOwner);
	}
	enableAllButton();
}

void MainWindow::connectToServerError(const QString &errorMsg)
{
	QMessageBox::information(this, tr("Client"), errorMsg);
	statusLabel->setText(errorMsg);

	disableAllButton();
}

void MainWindow::enableAllButton()
{
	applyPPCButton->setEnabled(true);
	giveBackPPCButton->setEnabled(true);
	updatePPCStatusButton->setEnabled(true);
	
}

void MainWindow::disableAllButton()
{
	applyPPCButton->setEnabled(false);
	giveBackPPCButton->setEnabled(false);
	updatePPCStatusButton->setEnabled(false);
}


MainWindow::~MainWindow()
{

}

