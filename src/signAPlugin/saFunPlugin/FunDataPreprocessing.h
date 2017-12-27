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
    static void pointSmoothInValue();
    static bool getSigmaDetectPorperty(double &sigma, bool *isMark, bool *isChangPlot);
    static void sigmaDetectInValue();
    static void sigmaDetectPlotInNewFigure();
    static bool getPointSmoothPorperty(int &m, int& n);
};

#endif // DATA_PREPROCESSING_H
