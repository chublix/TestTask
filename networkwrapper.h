#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "json.h"

struct MediaListItem
{
    QUrl aurl;
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
    void update();
    void setLogin( const QString& login ) { mLogin = login; }
    void setPassword( const QString& password ) { mPassword = password; }
    const QMediaList& getMediaList() { return mMediaList; }
    void tokenize();

signals:
    void loadedMedia();

private:
    void mediaList();
    void parseMediaList( const QString& jsonAnswer );

private slots:
    void received( QNetworkReply *reply );

private:
    QString mLogin;
    QString mPassword;
    QString mToken;

    QMediaList mMediaList;
    QNetworkAccessManager *mNetworkManager;

};

