#include "headers/DImageAlgorithm.h"
#include <QDebug>
#include <QElapsedTimer>
DImageAlgorithm::DImageAlgorithm(QObject *obj):QObject(obj)
{

}
///
/// \brief 把图片转为灰度图
///
///  (R * 11 + G * 16 + B * 5)/32;
/// \note same as int qGray(QRgb rgb)
/// \see qGray
/// \param image 图片
///
void DImageAlgorithm::toGray(QImage &image)
{
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            int gray = qGray(image.pixel(i,j));
            image.setPixelColor(i, j,QColor(gray,gray,gray));
        }
    }
}

///
/// \brief 对图片进行锐化处理
/// \param image 图片
///
void DImageAlgorithm::sharpen(QImage &imageOutPut, float factor)
{
    const QImage imageInput(imageOutPut);
    const int kernelSize = 3;
    float kernel [3][3] = {
        {0,-1,0}
        ,{-1,5+factor,-1}
        ,{0,-1,0}
    };
    int sumKernel = 1;
    int r=0,g=0,b=0;
    QRgb color;
    for(int x = kernelSize/2;x<imageInput.width()-(kernelSize/2);++x){
        for(int y = kernelSize/2;y<imageInput.height()-(kernelSize/2);++y){
            r=g=b=0;
            for(int i=-kernelSize/2;i<=kernelSize/2;++i){
                for(int j=-kernelSize/2;j<=kernelSize/2;++j){
                    color = imageInput.pixel(x+i,y+j);
                    r += qRed(color)*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += qGreen(color)*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += qBlue(color)*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }
            r = qBound(0,r/sumKernel,255);
            g = qBound(0,g/sumKernel,255);
            b = qBound(0,b/sumKernel,255);
            imageOutPut.setPixel(x,y,qRgb(r,g,b));
        }
    }
}

///
/// \brief 设置饱和度
/// \param image
/// \param saturationLevel 饱和度-255~255
///
void DImageAlgorithm::setSaturation(QImage &image, int saturationLevel)
{
    QColor oldClr;
    QColor newClr;
    int s = 0;
    for(int x=0;x<image.width();++x){
        for(int y=0;y<image.height();++y){
            oldClr = image.pixel(x,y);
            newClr = oldClr.toHsl();
            s = newClr.saturation()+saturationLevel;
            s = qBound(0,s,255);
            newClr.setHsl(newClr.hue(),s,newClr.lightness());
            image.setPixel(x,y,qRgb(newClr.red(),newClr.green(),newClr.blue()));
        }
    }
}
///
/// \brief 设置对比度
/// \param image
/// \param contrastLevel 0~100
///
void DImageAlgorithm::setContrast(QImage &image, int contrastLevel)
{
    const int w = image.width();
    const int h = image.height();
    float param = 1;

    param = contrastLevel/100.0;
    param = qBound(float(0.0),param,float(1.0));

    int r,g,b;
    QRgb rgb;
    QRgb meanRgb = rgbMean(image);
    for(int x=0;x<w;++x){
        for(int y=0;y<h;++y){
            rgb = image.pixel(x,y);
            r = qRed(rgb) - qRed(meanRgb);
            g = qGreen(rgb) - qGreen(meanRgb);
            b = qBlue(rgb) - qBlue(meanRgb);
            r = r * param;
            g = g * param;
            b = b * param;
            r = qBound(0,r,255);
            g = qBound(0,g,255);
            b = qBound(0,b,255);
            image.setPixel(x,y,qRgb(r,g,b));
        }
    }
}
///
/// \brief 求取图片的rgb均值
/// \param image
/// \return QRgb均值
///
QRgb DImageAlgorithm::rgbMean(const QImage &image)
{
    int r,g,b;
    int total = rgbSum(image,r,g,b);
    return qRgb(r/total,g/total,b/total);
}
///
/// \brief 求取图片的像素rgb和
/// \param image 图片
/// \param rSum red的和
/// \param gSum green和
/// \param bSum blue和
/// \return 返回像素个数
///
int DImageAlgorithm::rgbSum(const QImage &image, int &rSum, int &gSum, int &bSum)
{
    const int w = image.width();
    const int h = image.height();
    QRgb rgb;
    rSum = gSum = bSum = 0;
    for(int x=0;x<w;++x){
        for(int y=0;y<h;++y){
            rgb = image.pixel(x,y);
            rSum += qRed(rgb);
            gSum = qGreen(rgb);
            bSum = qBlue(rgb);
        }
    }
    return w*h;
}

QRgb DImageAlgorithm::subColor(const QRgb &clr, unsigned char clrCount)
{
    int r = qRed(clr);
    int g = qGreen(clr);
    int b = qBlue(clr);
    int clrSection = 255 / clrCount;
    int res = r % clrSection;
    if(0 != res)
    {
        int d = r / clrCount;
        int tmp = r - d * clrSection;
        if(tmp > clrSection/2)
        {
            r = (d+1) * clrSection;
        }
        else
        {
            r = d * clrSection;
        }
    }
    res = g % clrSection;
    if(0 != res)
    {
        int d = g / clrCount;
        int tmp = g - d * clrSection;
        if(tmp > clrSection/2)
        {
            g = (d+1) * clrSection;
        }
        else
        {
            g = d * clrSection;
        }
    }
    res = b % clrSection;
    if(0 != res)
    {
        int d = b / clrCount;
        int tmp = b - d * clrSection;
        if(tmp > clrSection/2)
        {
            b = (d+1) * clrSection;
        }
        else
        {
            b = d * clrSection;
        }
    }
    return qRgb(qBound(0,r,255),qBound(0,g,255),qBound(0,b,255));
}

void DImageAlgorithm::subColor(QImage &image, unsigned char clrCount)
{
    const int w = image.width();
    const int h = image.height();
    for(int x=0;x<w;++x){
        for(int y=0;y<h;++y){
            QRgb clr = image.pixel(x,y);
            clr = subColor(clr,clrCount);
            image.setPixel(x,y,clr);
        }
    }
}

void DImageAlgorithm::splitToCMYKChannel(const QImage &raw, QImage &imageC, QImage &imageM, QImage &imageY, QImage &imageK)
{
    imageC = QImage(raw.size(),QImage::Format_RGB32);
    imageM = QImage(raw.size(),QImage::Format_RGB32);
    imageY = QImage(raw.size(),QImage::Format_RGB32);
    imageK = QImage(raw.size(),QImage::Format_RGB32);

#if 0
    for (int x = 0; x < raw.width(); ++x) {
        for (int y = 0; y < raw.height(); ++y) {
            QColor clr = raw.pixel(x,y);
            QColor clrCMYK = clr.convertTo(QColor::Cmyk);
            QColor tmpClr = clrCMYK;

            tmpClr = tmpClr.convertTo(QColor::Cmyk);
            tmpClr.setCmyk(clrCMYK.cyan(),0,0,0);
            imageC.setPixel(x,y,tmpClr.convertTo(QColor::Rgb).rgb());

            tmpClr = tmpClr.convertTo(QColor::Cmyk);
            tmpClr.setCmyk(0,clrCMYK.magenta(),0,0);
            imageM.setPixel(x,y,tmpClr.convertTo(QColor::Rgb).rgb());

            tmpClr = tmpClr.convertTo(QColor::Cmyk);
            tmpClr.setCmyk(0,0,clrCMYK.yellow(),0);
            imageY.setPixel(x,y,tmpClr.convertTo(QColor::Rgb).rgb());

            tmpClr = tmpClr.convertTo(QColor::Cmyk);
            tmpClr.setCmyk(0,0,0,clrCMYK.black());
            imageK.setPixel(x,y,tmpClr.convertTo(QColor::Rgb).rgb());
        }
    }
    toGray(imageC);
    toGray(imageM);
    toGray(imageY);
    toGray(imageK);
#endif


#if 0
    for (int x = 0; x < raw.width(); ++x) {
        for (int y = 0; y < raw.height(); ++y) {
            QColor clr = raw.pixel(x,y);
            QColor clrCMYK = clr.convertTo(QColor::Cmyk);
            QColor tmpClr = clrCMYK;
            int gv = tmpClr.cyan();
            imageC.setPixel(x,y,qRgb(255-gv,255-gv,255-gv));

            gv = tmpClr.magenta();
            imageM.setPixel(x,y,qRgb(255-gv,255-gv,255-gv));

            gv = tmpClr.yellow();
            imageY.setPixel(x,y,qRgb(255-gv,255-gv,255-gv));

            gv = tmpClr.black();
            imageK.setPixel(x,y,qRgb(255-gv,255-gv,255-gv));
        }
    }
#endif


#if 1
    for (int x = 0; x < raw.width(); ++x) {
        for (int y = 0; y < raw.height(); ++y) {
            QColor clr = raw.pixel(x,y);
            QColor clrCMYK = clr.convertTo(QColor::Cmyk);
            QColor tmpClr = clrCMYK;
            int gv = tmpClr.cyan();
            imageC.setPixel(x,y,qRgb(255-gv,255-gv,255-gv));

            gv = tmpClr.magenta();
            imageM.setPixel(x,y,qRgb(255-gv,255-gv,255-gv));

            gv = tmpClr.yellow();
            imageY.setPixel(x,y,qRgb(255-gv,255-gv,255-gv));

            gv = tmpClr.black();
            imageK.setPixel(x,y,qRgb((255-gv*200/255),(255-gv*200/255),(255-gv*200/255)));
        }
    }
#endif

#if 0
    for (int x = 0; x < raw.width(); ++x) {
        for (int y = 0; y < raw.height(); ++y) {
            QColor clr = raw.pixel(x,y);
            QColor clrCMYK = clr.convertTo(QColor::Cmyk);
            QColor tmpClr = clrCMYK;
            tmpClr.setCmyk(clrCMYK.cyan(),0,0,0);
            imageC.setPixel(x,y,tmpClr.toRgb().rgb());

            tmpClr.setCmyk(0,clrCMYK.magenta(),0,0);
            imageM.setPixel(x,y,tmpClr.toRgb().rgb());

            tmpClr.setCmyk(0,0,clrCMYK.yellow(),0);
            imageY.setPixel(x,y,tmpClr.toRgb().rgb());

            tmpClr.setCmyk(0,0,0,clrCMYK.black());
            imageK.setPixel(x,y,tmpClr.toRgb().rgb());
        }
    }
    toGray(imageC);
    toGray(imageM);
    toGray(imageY);
    toGray(imageK);
#endif
}

void DImageAlgorithm::splitToRGBChannel(const QImage &raw, QImage &imageR, QImage &imageG, QImage &imageB)
{
    imageR = QImage(raw.size(),QImage::Format_RGB32);
    imageG = QImage(raw.size(),QImage::Format_RGB32);
    imageB = QImage(raw.size(),QImage::Format_RGB32);

    for (int x = 0; x < raw.width(); ++x) {
        for (int y = 0; y < raw.height(); ++y) {
            QColor clr = raw.pixel(x,y);

            imageR.setPixel(x,y,qRgb(clr.red(),clr.red(),clr.red()));

            imageG.setPixel(x,y,qRgb(clr.green(),clr.green(),clr.green()));

            imageB.setPixel(x,y,qRgb(clr.blue(),clr.blue(),clr.blue()));
        }
    }
}

///
/// \brief rgb颜色转cmyk颜色
/// \param r rgb:r
/// \param g rgb:g
/// \param b rgb:b
/// \param c cmyk:c
/// \param m cmyk:m
/// \param y cmyk:y
/// \param k cmyk:k
///
void DImageAlgorithm::rgbToCmyk(int r, int g, int b, int &c, int &m, int &y, int &k)
{
    c = 255-r;
    m = 255-g;
    y = 255-b;
    k = qMin(c,m);
    k = qMin(k,y);
    c = c - k;
    m = m - k;
    y = y - k;
}

void DImageAlgorithm::stop()
{
    m_stop = true;
}

///
/// \brief 对图片进行灰度处理
/// \param image 图片
/// \param args 用户自定义传参，会原封不动通过finish槽函数返回
///
void DImageAlgorithm::toGray(const QImage &image,const QVariant& args)
{
    m_stop = false;
    QImage img(image);
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            if(m_stop)
                return;
            int gray = qGray(img.pixel(i,j));
            img.setPixelColor(i, j,QColor(gray,gray,gray));
        }
    }
    emit finish(img,args);
}
///
/// \brief 对图片进行锐化处理
/// \param image 图片
/// \param args 用户自定义传参，会原封不动通过finish槽函数返回
///
void DImageAlgorithm::sharpen(const QImage &image,float factor,const QVariant& args)
{
    m_stop = false;
    QImage imageOutPut(image);
    const int kernelSize = 3;
    float kernel [3][3] = {
        {0,-1,0}
        ,{-1,5+factor,-1}
        ,{0,-1,0}
    };
    int sumKernel = 1;
    int r=0,g=0,b=0;
    QRgb color;
    if(m_stop)
        return;
    for(int x = kernelSize/2;x<image.width()-(kernelSize/2);++x){
        for(int y = kernelSize/2;y<image.height()-(kernelSize/2);++y){
            r=g=b=0;
            for(int i=-kernelSize/2;i<=kernelSize/2;++i){
                for(int j=-kernelSize/2;j<=kernelSize/2;++j){
                    if(m_stop)
                        return;
                    color = image.pixel(x+i,y+j);
                    r += qRed(color)*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += qGreen(color)*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += qBlue(color)*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }
            r = qBound(0,r/sumKernel,255);
            g = qBound(0,g/sumKernel,255);
            b = qBound(0,b/sumKernel,255);
            imageOutPut.setPixel(x,y,qRgb(r,g,b));
        }
    }
    emit finish(imageOutPut,args);
}
///
/// \brief 设置饱和度
/// \param image 图片
/// \param saturationLevel 饱和度值-255~255
/// \param args 用户自定义传参，会原封不动通过finish槽函数返回
///
void DImageAlgorithm::setSaturation(const QImage &image, int saturationLevel,const QVariant& args)
{
    m_stop = false;
    QImage img(image);
    QColor oldClr;
    QColor newClr;
    int s = 0;
    if(m_stop)
        return;
    for(int x=0;x<img.width();++x){
        for(int y=0;y<img.height();++y){
            if(m_stop)
                return;
            oldClr = img.pixel(x,y);
            newClr = oldClr.toHsl();
            s = newClr.saturation()+saturationLevel;
            s = qBound(0,s,255);
            newClr.setHsl(newClr.hue(),s,newClr.lightness());
            img.setPixel(x,y,qRgb(newClr.red(),newClr.green(),newClr.blue()));
        }
    }
    emit finish(img,args);
}

void DImageAlgorithm::setContrast(const QImage &image, int contrastLevel, const QVariant &args)
{
    m_stop = false;
    QImage img(image);
    const int w = img.width();
    const int h = img.height();
    float param = 1;

    param = contrastLevel/100.0;
    //param = qBound(float(0.0),param,float(1.0));

    int r,g,b;
    QRgb meanRgb = rgbMean(image);
    if(m_stop)
        return;
    QRgb rgb;
    for(int x=0;x<w;++x){
        for(int y=0;y<h;++y){
            if(m_stop)
                return;
            rgb = img.pixel(x,y);
            r = qRed(rgb) - qRed(meanRgb);
            g = qGreen(rgb) - qGreen(meanRgb);
            b = qBlue(rgb) - qBlue(meanRgb);
            r = r * param;
            g = g * param;
            b = b * param;
            r = qBound(0,r,255);
            g = qBound(0,g,255);
            b = qBound(0,b,255);
            img.setPixel(x,y,qRgb(r,g,b));
        }
    }
    emit finish(img,args);
}


