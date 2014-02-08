/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_managePPCclientClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *vboxLayout;
    QVBoxLayout *vboxLayout1;
    QVBoxLayout *vboxLayout2;
    QTableWidget *ipTableWidget;
    QPushButton *updatePPCStatusButton;
    QVBoxLayout *vboxLayout3;
    QHBoxLayout *hboxLayout;
    QLabel *label_2;
    QLineEdit *ipLineEdit;
    QHBoxLayout *hboxLayout1;
    QPushButton *applyPPCButton;
    QPushButton *giveBackPPCButton;
    QVBoxLayout *vboxLayout4;
    QHBoxLayout *hboxLayout2;
    QPushButton *quitButton;
    QSpacerItem *spacerItem;
    QLabel *statusLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *managePPCclientClass)
    {
        if (managePPCclientClass->objectName().isEmpty())
            managePPCclientClass->setObjectName(QString::fromUtf8("managePPCclientClass"));
        managePPCclientClass->resize(406, 448);
        centralWidget = new QWidget(managePPCclientClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        vboxLayout = new QVBoxLayout(centralWidget);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setSpacing(6);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        ipTableWidget = new QTableWidget(centralWidget);
        if (ipTableWidget->columnCount() < 3)
            ipTableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        ipTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        ipTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        ipTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (ipTableWidget->rowCount() < 2)
            ipTableWidget->setRowCount(2);
        ipTableWidget->setObjectName(QString::fromUtf8("ipTableWidget"));
        ipTableWidget->setMidLineWidth(0);
        ipTableWidget->setRowCount(2);
        ipTableWidget->setColumnCount(3);

        vboxLayout2->addWidget(ipTableWidget);

        updatePPCStatusButton = new QPushButton(centralWidget);
        updatePPCStatusButton->setObjectName(QString::fromUtf8("updatePPCStatusButton"));

        vboxLayout2->addWidget(updatePPCStatusButton);


        vboxLayout1->addLayout(vboxLayout2);

        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setSpacing(6);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        hboxLayout->addWidget(label_2);

        ipLineEdit = new QLineEdit(centralWidget);
        ipLineEdit->setObjectName(QString::fromUtf8("ipLineEdit"));
        ipLineEdit->setReadOnly(false);

        hboxLayout->addWidget(ipLineEdit);


        vboxLayout3->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        applyPPCButton = new QPushButton(centralWidget);
        applyPPCButton->setObjectName(QString::fromUtf8("applyPPCButton"));
        applyPPCButton->setEnabled(true);

        hboxLayout1->addWidget(applyPPCButton);

        giveBackPPCButton = new QPushButton(centralWidget);
        giveBackPPCButton->setObjectName(QString::fromUtf8("giveBackPPCButton"));
        giveBackPPCButton->setEnabled(true);

        hboxLayout1->addWidget(giveBackPPCButton);


        vboxLayout3->addLayout(hboxLayout1);


        vboxLayout1->addLayout(vboxLayout3);


        vboxLayout->addLayout(vboxLayout1);

        vboxLayout4 = new QVBoxLayout();
        vboxLayout4->setSpacing(6);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        quitButton = new QPushButton(centralWidget);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));

        hboxLayout2->addWidget(quitButton);

        spacerItem = new QSpacerItem(261, 23, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem);


        vboxLayout4->addLayout(hboxLayout2);

        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        vboxLayout4->addWidget(statusLabel);


        vboxLayout->addLayout(vboxLayout4);

        managePPCclientClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(managePPCclientClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 406, 20));
        managePPCclientClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(managePPCclientClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        managePPCclientClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(managePPCclientClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        managePPCclientClass->setStatusBar(statusBar);
        toolBar = new QToolBar(managePPCclientClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        managePPCclientClass->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(managePPCclientClass);

        QMetaObject::connectSlotsByName(managePPCclientClass);
    } // setupUi

    void retranslateUi(QMainWindow *managePPCclientClass)
    {
        managePPCclientClass->setWindowTitle(QApplication::translate("managePPCclientClass", "managePPCclient", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = ipTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("managePPCclientClass", "IP", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = ipTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("managePPCclientClass", "Status", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = ipTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("managePPCclientClass", "Owner", 0, QApplication::UnicodeUTF8));
        updatePPCStatusButton->setText(QApplication::translate("managePPCclientClass", "Update PPC Status", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("managePPCclientClass", "PPC", 0, QApplication::UnicodeUTF8));
        ipLineEdit->setText(QApplication::translate("managePPCclientClass", "172.16.20.", 0, QApplication::UnicodeUTF8));
        applyPPCButton->setText(QApplication::translate("managePPCclientClass", "Apply PPC", 0, QApplication::UnicodeUTF8));
        giveBackPPCButton->setText(QApplication::translate("managePPCclientClass", "Give Back PPC", 0, QApplication::UnicodeUTF8));
        quitButton->setText(QApplication::translate("managePPCclientClass", "Quit", 0, QApplication::UnicodeUTF8));
        statusLabel->setText(QApplication::translate("managePPCclientClass", "Disconnected to server", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("managePPCclientClass", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class managePPCclientClass: public Ui_managePPCclientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
