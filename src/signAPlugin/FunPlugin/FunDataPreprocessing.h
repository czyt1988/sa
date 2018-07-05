#ifndef DATA_PREPROCESSING_H
#define DATA_PREPROCESSING_H
#include <QString>
class SAUIInterface;
class QtVariantProperty;
class SAPropertySetDialog;
class SAFigureWindow;
class FunDataPreprocessing {
public:

private:
    static void pointSmoothInValue();
};
//sigma异常值判断
void sigmaDetect(SAUIInterface* ui);
   //m点n次滤波
void pointSmooth(SAUIInterface* ui);
#endif // DATA_PREPROCESSING_H
