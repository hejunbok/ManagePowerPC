#ifndef MANAGEPPCCLIENT_H
#define MANAGEPPCCLIENT_H

#include <QtGui/QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include "header.h"
#include "ui_client.h"


class ManagePPCClient : public QObject
{
	Q_OBJECT

public:
	ManagePPCClient(const QString cName, QObject *parent = 0);

public slots:
	void sendMsg(const QString &msgType, const QString &ppcIP = "");
    

private slots:
	void connected();
	void readMsg();
	void disconnected();
	void error(QAbstractSocket::SocketError socketError);

signals:
	void updateTableWidget(QList<IPStatus> &IPStatusList);
    void connectedToServer();
	void resultFromServer(const QString &result, const QString &resultContent);
	void connectionClosedByServer();
	void connectToServerError(const QString &errorMsg);


private:
	void closeConnection();
	void updatePPCStatusList(QDataStream &in);

	QTcpSocket *tcpSocket;
	QString clientName;
    QString ppcIP;
	quint16 nextBlockSize;

	QList<IPStatus> IPStatusList;


};

#endif // MANAGEPPCCLIENT_H
