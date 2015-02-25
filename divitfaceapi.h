#ifndef DIVITFACEAPI_H
#define DIVITFACEAPI_H

#include <QObject>
#include <QtNetwork/QtNetwork>
#include "httprequestworker.h"
#include "face.h"
namespace divitfaceapi {
class DivitFaceApi : public QObject
{
    Q_OBJECT
public:
    explicit DivitFaceApi(QObject *parent = 0);
    void getRequest( const QString &url );
    void postRequest( const QString &url, QHttpMultiPart *multiPart );
    void postRequest( const QString &url,const QByteArray &data );
    void postRequest(const QString &url, QList<QString> &keys, QList<QString> &filepaths, QList<QString> &contentTypes);
    void postRequest(QString &url, QString &key, QString &filepath, QString &contentType);
    QList<FaceClass> parseFaces(QByteArray &faceAsJSON);
    QVariant parseObject(QByteArray &objectAsJSON);
signals:

    void faceReady( const QByteArray &faceAsJSON );
    void objectReady(const QByteArray &objectAsJSON );
    void networkError( QNetworkReply::NetworkError err );
    void networkError( QString err );
public slots:
    void parseNetworkResponse( QNetworkReply *finished );
    void parseNetworkResponse( HttpRequestWorker *finished );
private:
    QNetworkAccessManager networkManager;

    };
}
#endif // DIVITFACEAPI_H
