#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent)
{
	QGridLayout *gl = new QGridLayout;
	setLayout(gl);

	QLabel *l;

	l = new QLabel("&Login:", this);
	gl->addWidget(l, 0, 0);
	lle = new QLineEdit(this);
	gl->addWidget(lle, 0, 1);
	l->setBuddy(lle);


	QWidget *hbox = new QWidget(this);
	QHBoxLayout *hboxlayout = new QHBoxLayout;

	gl->addWidget(hbox, 2, 0, 1, 2);

	QPushButton *pb;
	pb = new QPushButton("&Ok");
	connect(pb, SIGNAL(clicked()), this, SLOT(accept()));
	hboxlayout->addWidget(pb);

	pb = new QPushButton("&Cancel");
	connect(pb, SIGNAL(clicked()), this, SLOT(reject()));
	hboxlayout->addWidget(pb);

	hbox->setLayout(hboxlayout);
}

QString LoginDialog::login() const 
{ 
	return lle->text(); 
}
