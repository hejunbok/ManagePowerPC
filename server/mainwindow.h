#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QAbstractSocket>
#include <QTimer>
#include <QMap>
#include <QMutex>
#include "ui_mainwindow.h"
#include "telnetppc.h"
#include "Server.h"
#include "header.h"

class QTextEdit;
class QLineEdit;
class TelnetPPC;


class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

	~MainWindow();
private slots:
    //Interfaces that is related to connecting PPC
    void connectPPC();
	void resetPPC();
	void resetAllPPC();

	void occupyPPC();
	void processConnectPPCResult(const QString &telnetResult);

    //Interfaces that is related to communicating client 
    void processApplyPPC(const QString clientName, const QString ppcIP);
	void processGiveBackPPC(const QString clientName, const QString ppcIP);
	void sendGiveBackPPCResult();	
    void updateServerIPStatusList();

	void closeMainWindow();   
private:
	void resetPPC(const QString &ppcIP);

	//Initialize IPList
    void iniIPList();
    IPStatus* iniIPStatus(const QString IP);
    void updateTableWidget();

    TelnetPPC *telnetPPC;
	Server *server;
    
	QList<IPStatus> IPStatusList;
    QList<IPStatus>::iterator iteratorIPtatusList;

    //TelnetPPC *telnetIdlePPC;
	QList<TelnetPPC *> telnetList;
	QList<TelnetPPC *>::iterator iteratorTelnetList;

	QMap<QString, TelnetPPC*> connectStatusMap;

    QTimer *connectPPCTimer;    //Setting interval time that connect PPC
	QTimer *occupyPPCTimer;     //Setting delay time that occupy PPC
    QTimer *updateServerIPStatusTimer;  //Setting interval time that update server object's IPStatus
	

	QMap<QString, bool> giveBackPPCResultMap;

    bool takebackPPC;   //Whether server is in the process of taking back PPC
    bool occupying;     //Whether server is in the process of occupying PPC
};


#endif // MAINWINDOW_H

