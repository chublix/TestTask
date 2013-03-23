#include "vlcwidget.h"


QVlcWidget::QVlcWidget( QWidget *parent ):
    QWidget( parent ), mFullScreen(false)
{
#if defined(Q_OS_WIN)
    mVideoWidget = new QFrame( this );
#else
    mVideoWidget = new QX11EmbedContainer( this );
#endif

    mMediaList = new QListWidget( this );

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

#if defined(Q_OS_WIN)
    //create a new libvlc instance
    mVlcInstance = libvlc_new(0,NULL);
    // Create a media player playing environement
    mMediaPlayer = libvlc_media_player_new ( mVlcInstance );
#else
    libvlc_exception_init( &mVlcExcep );
    //create a new libvlc instance
    mVlcInstance = libvlc_new( 0, NULL, &mVlcExcep );
    raise( &mVlcExcep );
    // Create a media player playing environement
    mMediaPlayer = libvlc_media_player_new ( mVlcInstance, &mVlcExcep );
    raise( &mVlcExcep );
#endif

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
#if defined(Q_OS_WIN)
    libvlc_media_player_stop( mMediaPlayer );
#else
    libvlc_media_player_stop( mMediaPlayer, &mVlcExcep );
    raise( &mVlcExcep );
#endif
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

#if !defined(Q_OS_WIN)
void QVlcWidget::raise( libvlc_exception_t *ex )
{
    if (libvlc_exception_raised (ex))
    {
        fprintf (stderr, "error: %s\n", libvlc_exception_get_message(ex));
        exit (-1);
    }
}
#endif

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
        #if defined(Q_OS_WIN)
            libvlc_media_player_pause( mMediaPlayer );
        #else
            libvlc_media_player_pause( mMediaPlayer, &mVlcExcep );
            raise( &mVlcExcep );
        #endif
        mPauseButton->setText(" > ");

    }
    else
    {
        #if defined(Q_OS_WIN)
            libvlc_media_player_play( mMediaPlayer );
        #else
            libvlc_media_player_play( mMediaPlayer, &mVlcExcep );
            raise( &mVlcExcep );
        #endif
        mPauseButton->setText("| |");
    }
    mIsPlaying = !mIsPlaying;
}

void QVlcWidget::stopClicked()
{
    if ( mIsPlaying )
    {
        #if defined(Q_OS_WIN)
            libvlc_media_player_stop( mMediaPlayer );
        #else
            libvlc_media_player_stop( mMediaPlayer, &mVlcExcep );
            raise( &mVlcExcep );
        #endif
        mPositionSlider->setValue( 0 );
        mIsPlaying = false;
    }
}

void QVlcWidget::playFile( QString file )
{
#if defined(Q_OS_WIN)
     mMedia = libvlc_media_new_location( mVlcInstance, file.toAscii() );
     libvlc_media_player_set_media( mMediaPlayer, mMedia );
#else
    mMedia = libvlc_media_new( mVlcInstance, file.toAscii(), &mVlcExcep );
    raise( &mVlcExcep );

    libvlc_media_player_set_media( mMediaPlayer, mMedia, &mVlcExcep );
    raise( &mVlcExcep );
 #endif

#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd( mMediaPlayer, mVideoWidget->winId() );
    libvlc_media_player_play( mMediaPlayer );
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject( mMediaPlayer, mVideoWidget->winId() );
#else //Linux
    libvlc_media_player_set_xwindow( mMediaPlayer, mVideoWidget->winId(), &mVlcExcep );
    raise( &mVlcExcep );

    libvlc_media_player_play( mMediaPlayer, &mVlcExcep );
    raise( &mVlcExcep );
#endif

     mIsPlaying = true;
}

void QVlcWidget::updateInterface()
{
    if ( !mIsPlaying )
        return;

    // It's possible that the vlc doesn't play anything
    // so check before
    #if defined(Q_OS_WIN)
        libvlc_media_t *curMedia = libvlc_media_player_get_media( mMediaPlayer );
    #else
        libvlc_media_t *curMedia = libvlc_media_player_get_media( mMediaPlayer, &mVlcExcep );
        raise( &mVlcExcep );
    #endif
    if (curMedia == NULL)
        return;

    #if defined(Q_OS_WIN)
        float pos = libvlc_media_player_get_position( mMediaPlayer );
    #else
        float pos = libvlc_media_player_get_position ( mMediaPlayer, &mVlcExcep );
        raise( &mVlcExcep );
    #endif


    int siderPos = (int)(pos * (float)(POSITION_RESOLUTION));

    mPositionSlider->setValue( siderPos );

#if defined(Q_OS_WIN)
    int volume = libvlc_audio_get_volume( mMediaPlayer );
#else
    int volume = libvlc_audio_get_volume( mVlcInstance, &mVlcExcep );
    raise( &mVlcExcep );
#endif

    mVolumeSlider->setValue( volume );
}

void QVlcWidget::changeVolume( int newVolume )
{
#if defined(Q_OS_WIN)
    libvlc_audio_set_volume( mMediaPlayer, newVolume );
#else
    libvlc_audio_set_volume( mVlcInstance, newVolume, &mVlcExcep );
    raise( &mVlcExcep );
#endif
}

void QVlcWidget::changePosition( int newPosition )
{
#if defined(Q_OS_WIN)
    libvlc_media_t *curMedia = libvlc_media_player_get_media( mMediaPlayer );
#else
    libvlc_media_t *curMedia = libvlc_media_player_get_media( mMediaPlayer, &mVlcExcep );
    raise( &mVlcExcep );
#endif

    if ( curMedia == NULL )
        return;

    float pos = (float)(newPosition) / (float)POSITION_RESOLUTION;

#if defined(Q_OS_WIN)
    libvlc_media_player_set_position( mMediaPlayer, pos );
#else
    libvlc_media_player_set_position( mMediaPlayer, pos, &mVlcExcep );
    raise( &mVlcExcep );
#endif
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


