#pragma once

#include <QWidget>
#include <vlc/vlc.h>
#include <QBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QFrame>
#include <QSlider>
#include <QListWidget>
#include <QKeyEvent>
#include "networkwrapper.h"
#include "form.h"

#define POSITION_RESOLUTION 10000

class QVlcWidget : public QWidget
{
    Q_OBJECT
    QListWidget *mMediaList;
    QPushButton *mStopButton;
    QPushButton *mPauseButton;
    QSlider *mPositionSlider;
    QSlider *mVolumeSlider;
    QFrame *mVideoWidget;
    QTimer *mPoller;

    bool mIsPlaying;
    libvlc_instance_t *mVlcInstance;
    libvlc_media_player_t *mMediaPlayer;
    libvlc_media_t *mMedia;

public:
     QVlcWidget( QWidget *parent = 0 );
    ~QVlcWidget();
     void apiConnect( const QString& login, const QString& password);
     void playFile( QString file );

protected:
    void keyPressEvent( QKeyEvent *event );

private:
    void fullScreen();

private slots:
    void mediaSelected( QListWidgetItem *item );
    void showFullScreen();
    void showNormal();
    void pauseClicked();
    void stopClicked();
    void updateInterface();
    void changeVolume(int newVolume);
    void changePosition(int newPosition);

    void loadedMedia();
    void loginClicked( const QString& login, const QString& pass );

private:
    Form *mForm;
    bool mFullScreen;
    NetworkWrapper *mWrapper;
    QMediaList mInfoList;
};
