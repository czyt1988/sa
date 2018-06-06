#ifndef DIMAGEALGORITHM_H
#define DIMAGEALGORITHM_H
#include <QImage>
#include <QObject>
#include <QVariant>
#include "DImagePatternConverGlobal.h"


///
/// \brief 封装图片的一些基本算法，分为静态函数和槽函数，槽函数可把实例化用于多线程
///
class D_IMAGE2PATTERN_EXPORT DImageAlgorithm : public QObject
{
    Q_OBJECT
public:
    DImageAlgorithm(QObject* obj = NULL);
    //对图片进行灰度处理
    static void toGray(QImage& image);
    //对图片进行锐化处理
    static void sharpen(QImage& imageOutPut,float factor = 0 );
    //设置饱和度
    static void setSaturation(QImage& image,int saturationLevel);
    //设置对比度
    static void setContrast(QImage& image,int contrastLevel);
    //求取图片的rgb均值
    static QRgb rgbMean(const QImage& image);
    //求取图片的像素rgb和
    static int rgbSum(const QImage& image,int& rSum,int& gSum,int& bSum);
    //减少颜色
    static QRgb subColor(const QRgb& clr,unsigned char clrCount);
    static void subColor(QImage& image,unsigned char clrCount);
    //把图片转换为cmyk通道的四个灰度
    static void splitToCMYKChannel(const QImage& raw
                                   ,QImage& imageC
                                   ,QImage& imageM
                                   ,QImage& imageY
                                   ,QImage& imageK);
    static void splitToRGBChannel(const QImage& raw
                                   ,QImage& imageR
                                   ,QImage& imageG
                                   ,QImage& imageB);
    static void rgbToCmyk(int r,int g,int b,int &c,int &m, int& y,int &k);
public:
    // 停止线程使用的函数，如果不用在多线程，此函数无作用
    void stop();
signals:
    ///
    /// \brief 处理结束发出的信号
    /// \param outPutImage 结果图片
    /// \param args 传入处理函数的参数原样返回，用于标定此结果是哪个处理返回的
    ///
    void finish(const QImage& outPutImage,const QVariant& args);
public slots:
    //支持多线程操作的槽函数
    //对图片进行灰度处理
    Q_SLOT void toGray(const QImage& image,const QVariant& args = QVariant());
    //对图片进行锐化处理
    Q_SLOT void sharpen(const QImage& image,float factor = 0 ,const QVariant& args = QVariant());
    //设置饱和度
    Q_SLOT void setSaturation(const QImage& image,int saturationLevel,const QVariant& args = QVariant());
    //设置对比度
    Q_SLOT void setContrast(const QImage& image,int contrastLevel,const QVariant& args = QVariant());
private:
    bool m_stop;
};

#endif // DIMAGEALGORITHM_H
