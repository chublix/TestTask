#include "vlcwidget.h"


QVlcWidget::QVlcWidget( QWidget *parent ):
    QWidget( parent ), mFullScreen(false)
{
    mMediaList = new QListWidget( this );
    mVideoWidget = new QFrame( this );
    mForm = new Form(this);
    mVolumeSlider = new QSlider( Qt::Horizontal,this );
    mVolumeSlider->setMaximum( 100 ); //the volume is between 0 and 100
    mVolumeSlider->setToolTip( "Audio slider" );

    // Note: if you use streaming, there is no ability to use the position slider
    mPositionSlider = new QSlider( Qt::Horizontal, this );
    mPositionSlider->setMaximum( POSITION_RESOLUTION );
    mPositionSlider->setToolTip("Seeker");

    mStopButton = new QPushButton( this );
    mStopButton->resize(10,8);
    mStopButton->setText("Stop");

    mPauseButton = new QPushButton( this );
    mPauseButton->resize(10,8);
    mPauseButton->setText("| |");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin( 0 );

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVBoxLayout *uiLayout = new QVBoxLayout();
    uiLayout->setMargin( 2 );

    mainLayout->addWidget( mVideoWidget, 5);
    mainLayout->addWidget( mMediaList, 2 );
    mainLayout->setDirection( QVBoxLayout::LeftToRight );

    uiLayout->addWidget( mPauseButton );
    uiLayout->addWidget( mStopButton );
    uiLayout->addWidget( mPositionSlider, 10 );
    uiLayout->addWidget( mVolumeSlider );
    uiLayout->setDirection( QVBoxLayout::LeftToRight );

    layout->addLayout( mainLayout );
    layout->addLayout( uiLayout );

    setLayout( layout );

    mIsPlaying = false;
    mPoller = new QTimer( this );
    mWrapper = new NetworkWrapper( this );

    //create a new libvlc instance
    mVlcInstance = libvlc_new(0,NULL);

    // Create a media player playing environement
    mMediaPlayer = libvlc_media_player_new ( mVlcInstance );

    //connect the two sliders to the corresponding slots (uses Qt's signal / slots technology)
    connect( mPauseButton, SIGNAL( clicked() ), this, SLOT( pauseClicked() ) );
    connect( mStopButton, SIGNAL( clicked() ), this, SLOT( stopClicked() ) );

    connect( mPoller, SIGNAL( timeout() ), this, SLOT( updateInterface() ) );

    connect( mPositionSlider, SIGNAL( sliderMoved( int ) )
             , this, SLOT( changePosition ( int ) ) );

    connect( mVolumeSlider, SIGNAL( sliderMoved( int ) )
             , this, SLOT( changeVolume( int ) ) );

    connect ( mMediaList, SIGNAL( itemDoubleClicked( QListWidgetItem* ) )
              , this, SLOT( mediaSelected( QListWidgetItem* ) ) );

    connect( mWrapper, SIGNAL( loadedMedia() ), this, SLOT( loadedMedia() ) );

    connect( mForm, SIGNAL ( click( QString, QString ) )
             , this, SLOT ( loginClicked( QString, QString ) ) );

    mPoller->start( 100 ); //start timer to trigger every 100 ms the updateInterface slot
}

QVlcWidget::~QVlcWidget()
{
    libvlc_media_player_stop( mMediaPlayer );
    libvlc_media_player_release( mMediaPlayer );
    libvlc_release( mVlcInstance );
}

void QVlcWidget::apiConnect(const QString &login, const QString &password)
{
    mWrapper->setLogin( login );
    mWrapper->setPassword( password );
    mWrapper->login();
    // mInfoList = mWrapper->mediaList();
}

void QVlcWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
    {
        fullScreen();
    }
}

void QVlcWidget::fullScreen()
{
    mFullScreen = !mFullScreen;

    if (mFullScreen)
        showFullScreen();
    else
        showNormal();
}

void QVlcWidget::mediaSelected( QListWidgetItem *item )
{
    QMediaList list = mWrapper->mediaList();
    foreach(MediaListItem *mediaInfo, list )
    {
        if ( item->text() == ( mediaInfo->artist + " - " +  mediaInfo->title ) )
        {
            playFile( mediaInfo->aurl );
            break;
        }
    }
}

void QVlcWidget::showFullScreen()
{
    mMediaList->setVisible(false);
    QWidget::showFullScreen();
}

void QVlcWidget::showNormal()
{
    mMediaList->setVisible(true);
    QWidget::showNormal();
}

void QVlcWidget::pauseClicked()
{
    if ( mIsPlaying )
    {
        libvlc_media_player_pause( mMediaPlayer );
        mPauseButton->setText(" > ");

    }
    else
    {
        libvlc_media_player_play( mMediaPlayer );
        mPauseButton->setText("| |");
    }
    mIsPlaying = !mIsPlaying;
}

void QVlcWidget::stopClicked()
{
    if ( mIsPlaying )
    {
        libvlc_media_player_stop( mMediaPlayer );
        mPositionSlider->setValue(0);
        mIsPlaying = false;
    }
}

void QVlcWidget::playFile( QString file )
{

     mMedia = libvlc_media_new_location( mVlcInstance, file.toAscii() );
     libvlc_media_player_set_media( mMediaPlayer, mMedia );

#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd( mMediaPlayer, mVideoWidget->winId() );
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject( mMediaPlayer, mVideoWidget->winId() );
#else //Linux
    libvlc_media_player_set_xwindow( mMediaPlayer, mVideoWidget->winId() );
#endif
     libvlc_media_player_play( mMediaPlayer );
     mIsPlaying = true;
}

void QVlcWidget::updateInterface()
{
    if ( !mIsPlaying )
        return;

    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media( mMediaPlayer );
    if (curMedia == NULL)
        return;

    float pos = libvlc_media_player_get_position (mMediaPlayer);

    int siderPos = (int)(pos * (float)(POSITION_RESOLUTION));

    mPositionSlider->setValue( siderPos );

    int volume = libvlc_audio_get_volume( mMediaPlayer );

    mVolumeSlider->setValue( volume );
}

void QVlcWidget::changeVolume( int newVolume )
{
    libvlc_audio_set_volume( mMediaPlayer, newVolume );
}

void QVlcWidget::changePosition( int newPosition )
{
    libvlc_media_t *curMedia = libvlc_media_player_get_media( mMediaPlayer );
    if ( curMedia == NULL )
        return;

    float pos = (float)(newPosition) / (float)POSITION_RESOLUTION;
    libvlc_media_player_set_position( mMediaPlayer, pos );
}

void QVlcWidget::loadedMedia()
{
    QMediaList list = mWrapper->mediaList();
    foreach( MediaListItem *item, list )
    {
        mMediaList->addItem( item->artist + " - " + item->title );
    }
}

void QVlcWidget::loginClicked(const QString &login, const QString &pass)
{
    apiConnect( login, pass );
    mForm->close();
}


