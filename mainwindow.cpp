#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ), ui( new Ui::MainWindow )
  , mFullScreen( false ), mTimer( new QTimer )
{
    ui->setupUi( this );
    mWrapper = new NetworkWrapper( this );
    mTimer->setInterval( 60000 );
    connect( mWrapper, SIGNAL( loadedMedia() ), this, SLOT( loadedMedia() ) );
    connect( mTimer, SIGNAL( timeout() ), this, SLOT( timeout() ) );
    ui->webView->settings()->setAttribute( QWebSettings::PluginsEnabled, true );
    ui->listWidget->setVisible( false );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent( QKeyEvent *event )
{
    if (event->key() == Qt::Key_F11)
    {
        fullScreen();
    }
}


void MainWindow::fullScreen()
{
    mFullScreen = !mFullScreen;

    if (mFullScreen)
        showFullScreen();
    else
        showNormal();

    ui->listWidget->setVisible(!mFullScreen);
    ui->webView->resize( geometry().width(), geometry().height() );
}

void MainWindow::loadedMedia()
{
    QMediaList list = mWrapper->getMediaList();
    foreach( MediaListItem *item, list)
    {
        ui->listWidget->addItem( item->artist + " - " + item->title );
    }
    ui->listWidget->setVisible(true);
}

void MainWindow::timeout()
{
    mWrapper->update();
}

void MainWindow::on_loginButton_clicked()
{
    if ( !ui->loginEdit->text().isEmpty() && !ui->passwordEdit->text().isEmpty() )
    {
        mWrapper->setLogin( ui->loginEdit->text() );
        mWrapper->setPassword( ui->passwordEdit->text() );
        mWrapper->tokenize();
        ui->loginWidget->close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Fields must not be empty!");
        msgBox.exec();
    }
}

void MainWindow::on_listWidget_itemDoubleClicked( QListWidgetItem *item )
{
    QMediaList list = mWrapper->getMediaList();
    foreach( MediaListItem *listItem, list )
    {
        QString text = listItem->artist + " - " + listItem->title;
        if ( item->text() == text )
        {
            ui->webView->load( listItem->aurl );
            //ui->listWidget->setVisible( false );
            this->setWindowTitle( "TestTask | " + text
                                  + " | Fullscreen - F11" );
            break;
        }
    }
}
