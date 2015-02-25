#include "divitfaceapi.h"

using namespace divitfaceapi;

DivitFaceApi::DivitFaceApi(QObject *parent) :
    QObject(parent)
{
    QObject::connect(&networkManager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(parseNetworkResponse(QNetworkReply*)));
}

void DivitFaceApi::getRequest( const QString &urlString )
{
    QUrl url ( urlString );
    QNetworkRequest request ( url );
    this->networkManager.get( request );
}

void DivitFaceApi::postRequest( const QString &urlString, QHttpMultiPart *multiPart )
{
    QUrl url ( urlString );
    QNetworkRequest request ( url );
    QNetworkReply *reply = this->networkManager.post(request, multiPart);
    multiPart->setParent(reply);
    qDebug() << "URL: " << reply->url();
    //multiPart->deleteLater();
}

void DivitFaceApi::postRequest( const QString &urlString,const QByteArray &data )
{
    QUrl url ( urlString );
    QNetworkRequest request ( url );
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    this->networkManager.post(request, data);
    //multiPart->deleteLater();
}

void DivitFaceApi::parseNetworkResponse( QNetworkReply *finished )
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        QNetworkReply::NetworkError error = finished->error();
        emit networkError( error );
        delete finished;
        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    emit faceReady(data );
    delete finished;
}

//post request using HttpRequestWorker
void DivitFaceApi::postRequest(const QString &url, QList<QString> &keys, QList<QString> &filepaths, QList<QString> &contentTypes){
    HttpRequestInput input(url, "POST");

    int size = keys.size();
    for(int i=0; i<size; i++){
        input.add_file(keys.at(i), filepaths.at(i), NULL, contentTypes.at(i));
    }
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(parseNetworkResponse(HttpRequestWorker*)));
    worker->execute(&input);
}

//post request using HttpRequestWorker
void DivitFaceApi::postRequest(QString &url, QString &key, QString &filepath, QString &contentType){
    HttpRequestInput input(url, "POST");
    input.add_file(key, filepath, NULL, contentType);
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(parseNetworkResponse(HttpRequestWorker*)));
    worker->execute(&input);
}

void DivitFaceApi::parseNetworkResponse( HttpRequestWorker *finished )
{
    if ( finished->error_type != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->error_str );
        delete finished;
        return;
    }

    QByteArray data = finished->response;
    emit faceReady(data );
    finished->deleteLater();
    //delete finished;
}

QList<divitfaceapi::FaceClass> DivitFaceApi::parseFaces(QByteArray &faceAsJSON){
    QJson::Parser parser;
    bool success;
    QList<FaceClass> faces;
    if(!faceAsJSON.isEmpty()){
        qDebug() <<faceAsJSON;
    }
    QVariant result = parser.parse(faceAsJSON, &success);
    QList< QVariant> resultList =  result.toList();
    if ( success && resultList.size() != 0 )
    {
        for(int i = 0; i < resultList.size(); i++){
            FaceClass face;
            QVariantMap faceDataMap =  resultList.at(i).toMap();
            QVariantMap faceData = faceDataMap["analysisResult"].toMap();
            face.parseFace(faceData);
            faces.insert(faces.end(), face);
        }
        return faces;
    }else{
        return faces;
    }
}

QVariant DivitFaceApi::parseObject(QByteArray& objectAsJSON){
    QJson::Parser parser;
    return parser.parse(objectAsJSON);
}
