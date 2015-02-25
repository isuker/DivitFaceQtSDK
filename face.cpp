#include "face.h"
using namespace divitfaceapi;

QStringList variantListToStringList(QVariantList vlist)
{
    QStringList result;
    foreach ( QVariant v, vlist )
    {
        result << v.toString();
    }
    return result;
}

FaceClass::FaceClass()
{
    faceRect = NULL;
}

FaceClass::~FaceClass(){
    delete faceRect;
}

FaceClass::FaceClass(const QByteArray &faceAsJSON)
{
    QJson::Parser parser;
    bool success;

    if(!faceAsJSON.isEmpty()){
        qDebug() <<faceAsJSON;
    }
    QVariant result = parser.parse(faceAsJSON, &success);
    QList< QVariant> resultList =  result.toList();
    if ( success && resultList.size() != 0 )
    {
        //get the first element of facelist ( detect only single face)
        QVariantMap faceDataMap =  resultList.at(0).toMap();
        QVariantMap faceData = faceDataMap["analysisResult"].toMap();
        int face_x1 =  faceData["face_x1"].toInt();
        int face_y1 =  faceData["face_y1"].toInt();
        int face_width =  faceData["face_width"].toInt();
        int face_height =  faceData["face_height"].toInt();
        faceRect = new cv::Rect(face_x1,face_y1, face_width, face_height);

        leftEye_x1 =  faceData["leftEye_x1"].toInt();
        leftEye_y1 =  faceData["leftEye_y1"].toInt();
        leftEye_x2 =  faceData["leftEye_x2"].toInt();
        leftEye_y2 =  faceData["leftEye_y2"].toInt();

        rightEye_x1 =  faceData["rightEye_x1"].toInt();
        rightEye_y1 =  faceData["rightEye_y1"].toInt();
        rightEye_x2 =  faceData["rightEye_x2"].toInt();
        rightEye_y2 =  faceData["rightEye_y2"].toInt();
    }else{
        faceRect = NULL;
    }
}

void FaceClass::parseFace(QVariantMap &faceData){
    int face_x1 =  faceData["face_x1"].toInt();
    int face_x2 =  faceData["face_x2"].toInt();
    int face_width =  faceData["face_width"].toInt();
    int face_height =  faceData["face_height"].toInt();
    faceRect = new cv::Rect(face_x1,face_x2, face_width, face_height);

    leftEye_x1 =  faceData["leftEye_x1"].toInt();
    leftEye_y1 =  faceData["leftEye_y1"].toInt();
    leftEye_x2 =  faceData["leftEye_x2"].toInt();
    leftEye_y2 =  faceData["leftEye_y2"].toInt();

    rightEye_x1 =  faceData["rightEye_x1"].toInt();
    rightEye_y1 =  faceData["rightEye_y1"].toInt();
    rightEye_x2 =  faceData["rightEye_x2"].toInt();
    rightEye_y2 =  faceData["rightEye_y2"].toInt();
}

cv::Rect* FaceClass::getFaceRect(){
    return this->faceRect;
}

cv::Point& FaceClass::getLeftEyeCenter(cv::Point& destPoint){
    destPoint.x = (leftEye_x1 + leftEye_x2) / 2;
    destPoint.y = (leftEye_y1 + leftEye_y2) / 2;
    return destPoint;
}

cv::Point& FaceClass::getRightEyeCenter(cv::Point& destPoint){
    destPoint.x = (rightEye_x1 + rightEye_x2) / 2;
    destPoint.y = (rightEye_y1 + rightEye_y2) / 2;
    return destPoint;
}
