#include "networkwrapper.h"
#include <QRegExp>
#include <QDebug>

NetworkWrapper::NetworkWrapper( QObject *parent ):
    QObject( parent )
{
    mNetworkManager = new QNetworkAccessManager( parent );
    connect( mNetworkManager, SIGNAL( finished( QNetworkReply* ) )
            , this, SLOT( received( QNetworkReply* ) ) );
}

NetworkWrapper::~NetworkWrapper()
{
}

void NetworkWrapper::update()
{
    mediaList();
}


void NetworkWrapper::tokenize()
{
    QUrl url("http://vacancy.dev.telehouse-ua.net/auth/login?login="
            + mLogin + "&password=" + mPassword);

    mNetworkManager->get( QNetworkRequest( url ) );
}

void NetworkWrapper::mediaList()
{
    QUrl url( "http://vacancy.dev.telehouse-ua.net/media/list" );
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    //custom header
    QByteArray header( "X-Auth-Token" );
    QByteArray value( mToken.toStdString().c_str() );
    request.setRawHeader( header, value );

    mNetworkManager->get( request );
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
        media->aurl = fields["aurl"].toUrl();
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
    if ( reply->error() == QNetworkReply::NoError )
    {
        if ( data.size() > 64 )
            parseMediaList( data );
        else
        {
            mToken = data.section("\"",-2,-2);
            mediaList();
        }
    }
}
