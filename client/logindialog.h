#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include <QString>

class LoginDialog : public QDialog
{
public:
	LoginDialog(QWidget *parent = 0);


	QString login() const;


private:
	QLineEdit *lle;
};

#endif