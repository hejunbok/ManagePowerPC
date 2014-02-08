#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QAbstractSocket>
#include <QTimer>
#include "ui_client.h"
#include "client.h"

class QTextEdit;
class QLineEdit;
class TelnetPPC;

class MainWindow : public QMainWindow, public Ui::managePPCclientClass
{
	Q_OBJECT

public:
	MainWindow(const QString cName = "", QWidget *parent = 0, Qt::WFlags flags = 0);

    
	~MainWindow();

    
private slots:
    //client
	void applyPPC();
	void giveBackPPC();
	void getGiveBackPPCResult();
	void updatePPCStatus();
	void updateTableWidget(QList<IPStatus> &IPStatusList);
    void connectedToServer();
	void dealWithResultFromServer(const QString &result, const QString &resultContent);
	void connectToServerError(const QString &errorMsg);

    
private:
	void insertNewOccupiedPPC(const QString &ppcIP);
	void enableAllButton();
	void disableAllButton();

	QString clientName;
	QStringList occupiedPPCList;
    
    ManagePPCClient *client;

	QTimer *getGiveBackPPCResultTimer;




    
};

#endif // MAINWINDOW_H
