#ifndef FACEAPI_H
#define FACEAPI_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <QtCore/QtCore>
#include <qjson/parser.h>

namespace divitfaceapi{
class FaceClass
{

public:
    FaceClass();
    ~FaceClass();
    FaceClass( const QByteArray &faceAsJSON);
    void parseFace(QVariantMap &faceData);
    cv::Rect* getFaceRect();
    cv::Point& getLeftEyeCenter(cv::Point& destPoint);
    cv::Point& getRightEyeCenter(cv::Point& destPoint);
signals:
    
public slots:

private:
    //face area
    cv::Rect *faceRect;

    //coordinates of left eye
    int leftEye_x1;
    int leftEye_y1;
    int leftEye_x2;
    int leftEye_y2;

    //coordinates of right eye
    int rightEye_x1;
    int rightEye_y1;
    int rightEye_x2;
    int rightEye_y2;

    /**
    further properties of face detection result can be added to this class
    **/

    };
}

#endif // FACEAPI_H
