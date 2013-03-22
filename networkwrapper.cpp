#include "networkwrapper.h"

NetworkWrapper::NetworkWrapper( QObject *parent ):
    QObject( parent )
{
    mUpdateTimer = new QTimer( this ) ;
    mNetworkManager = new QNetworkAccessManager( parent );
    connect( mNetworkManager, SIGNAL( finished( QNetworkReply* ) )
            , this, SLOT( received( QNetworkReply* ) ) );

    connect (mUpdateTimer, SIGNAL ( timeout() ), this, SLOT( mediaUpdate() ) );
}

NetworkWrapper::~NetworkWrapper()
{
}

void NetworkWrapper::login()
{
    QUrl authUrl( "http://vacancy.dev.telehouse-ua.net/auth/login?login="
            + mLogin + "&password=" + mPassword );

    mAuthRequest = new QNetworkRequest( authUrl );

    mNetworkManager->get( *mAuthRequest );

    mUpdateTimer->start( 5 * ONE_MINUTES );
 }

void NetworkWrapper::load()
{
    QUrl mediaUrl( "http://vacancy.dev.telehouse-ua.net/media/list" );
    mMediaRequest = new QNetworkRequest( mediaUrl );
    mMediaRequest->setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    //custom header
    QByteArray header( "X-Auth-Token" );
    QByteArray value( mToken.toStdString().c_str() );
    mMediaRequest->setRawHeader( header, value );

    mNetworkManager->get( *mMediaRequest );
}

void NetworkWrapper::parseMediaList(const QString &jsonAnswer)
{
    QVariantMap root = QtJson::parse( jsonAnswer ).toMap();
    QVariantList rootList = root["list"].toList();
    for ( int i = 0; i < rootList.size(); ++i )
    {
        QVariant insideList = rootList[i];
        QVariantMap fields = insideList.toMap();

        MediaListItem *media = new MediaListItem();
        media->aurl = fields["aurl"].toString();
        media->iurl = fields["iurl"].toUrl();
        media->genre = fields["genre"].toString();
        media->artist = fields["artist"].toString();
        media->title = fields["title"].toString();

        mMediaList.push_back(media);
    }
    emit loadedMedia();
}

void NetworkWrapper::received( QNetworkReply *reply )
{
    QString data = reply->readAll();
    QVariantMap root = QtJson::parse( data ).toMap();
    if ( reply->error() == QNetworkReply::NoError )
    {
        if ( root["token"].isNull() )
            parseMediaList( data );
        else
        {
            mToken = root["token"].toString();
            load();
        }
    }
}

void NetworkWrapper::mediaUpdate()
{
   mNetworkManager->get( *mMediaRequest );
   mMediaList.clear();
}


