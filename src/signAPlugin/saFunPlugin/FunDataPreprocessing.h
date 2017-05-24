#ifndef DATA_PREPROCESSING_H
#define DATA_PREPROCESSING_H
#include <QString>
class QtVariantProperty;
class SAPropertySetDialog;
class SAFigureWindow;
class FunDataPreprocessing {
public:
    //sigma异常值判断
    static void sigmaDetect();
       //m点n次滤波
    static void pointSmooth();

private:
    static void sigmaDetectPlotInNewFigure();
};

#endif // DATA_PREPROCESSING_H
