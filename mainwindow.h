#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <QNetworkReply>
#include <QTimer>
#include <QListWidget>
#include "networkwrapper.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

protected:
    void keyPressEvent( QKeyEvent *event );

private:
    void fullScreen();

private slots:
    void loadedMedia();
    void timeout();
    void on_loginButton_clicked();

    void on_listWidget_itemDoubleClicked( QListWidgetItem *item );

private:
    Ui::MainWindow *ui;
    bool mFullScreen;
    QTimer* mTimer;
    NetworkWrapper *mWrapper;
};
