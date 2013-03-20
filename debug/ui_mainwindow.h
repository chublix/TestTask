/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed 20. Mar 19:09:25 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWebView *webView;
    QWidget *loginWidget;
    QPushButton *loginButton;
    QLabel *loginLabel;
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QLabel *label;
    QListWidget *listWidget;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        MainWindow->setMouseTracking(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMouseTracking(true);
        webView = new QWebView(centralWidget);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setGeometry(QRect(0, 0, 640, 480));
        webView->setMouseTracking(true);
        webView->setFocusPolicy(Qt::NoFocus);
        webView->setAcceptDrops(false);
        webView->setLayoutDirection(Qt::LeftToRight);
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));
        loginWidget = new QWidget(centralWidget);
        loginWidget->setObjectName(QString::fromUtf8("loginWidget"));
        loginWidget->setGeometry(QRect(225, 195, 190, 90));
        loginWidget->setMouseTracking(false);
        loginWidget->setFocusPolicy(Qt::StrongFocus);
        loginWidget->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 0);\n"
"background-color: rgb(218, 218, 218);\n"
"border-color: rgb(81, 81, 81);\n"
""));
        loginButton = new QPushButton(loginWidget);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(130, 60, 51, 23));
        loginLabel = new QLabel(loginWidget);
        loginLabel->setObjectName(QString::fromUtf8("loginLabel"));
        loginLabel->setGeometry(QRect(31, 10, 31, 16));
        loginEdit = new QLineEdit(loginWidget);
        loginEdit->setObjectName(QString::fromUtf8("loginEdit"));
        loginEdit->setGeometry(QRect(70, 8, 113, 20));
        loginEdit->setStyleSheet(QString::fromUtf8(""));
        passwordEdit = new QLineEdit(loginWidget);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setGeometry(QRect(70, 35, 113, 20));
        label = new QLabel(loginWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 37, 51, 16));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setEnabled(true);
        listWidget->setGeometry(QRect(440, 0, 200, 480));
        listWidget->setMouseTracking(true);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "TestTask", 0, QApplication::UnicodeUTF8));
        loginButton->setText(QApplication::translate("MainWindow", "Login", 0, QApplication::UnicodeUTF8));
        loginLabel->setText(QApplication::translate("MainWindow", "Login:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Password:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
