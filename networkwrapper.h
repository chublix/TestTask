#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include "json.h"

#define ONE_MINUTES 60000

struct MediaListItem
{
    QString aurl;
    QUrl iurl;
    QString genre;
    QString artist;
    QString title;
};


typedef QList<MediaListItem*> QMediaList;

class NetworkWrapper: public QObject
{
    Q_OBJECT
public:
    NetworkWrapper( QObject *parent = 0 );
    virtual ~NetworkWrapper();
    void setLogin( const QString& login ) { mLogin = login; }
    void setPassword( const QString& password ) { mPassword = password; }
    const QMediaList& mediaList() { return mMediaList; }
    void login();

signals:
    void loadedMedia();

private slots:
    void received( QNetworkReply *reply );
    void mediaUpdate();

private:
    void parseMediaList( const QString& jsonAnswer );
    void load();

private:
    QTimer *mUpdateTimer;
    QString mLogin;
    QString mPassword;
    QString mToken;
    QNetworkRequest *mMediaRequest;
    QNetworkRequest *mAuthRequest;

    QMediaList mMediaList;
    QNetworkAccessManager *mNetworkManager;
};

