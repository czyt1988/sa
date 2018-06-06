#include "headers/DImagePatternConver.h"
#include "headers/DImageAlgorithm.h"
#include <QMutexLocker>
#include <stdlib.h>
#include <stdio.h>
#include <QDebug>
//#define DEBUG_OUT_PUT

const unsigned char yellowDSBBuffer12[]={192,20,15,160,15,15,192,5,20,160,0,20,128,10,10,224,20,0,160,10,10,128,10,20,224,20,20,128,10,20,160,10,10,192,20,10};//黄色逆向12针
const unsigned char yellowDSBBuffer12r[]={160,20,10,192,10,10,224,10,20,128,20,20,224,10,20,192,10,10,128,20,0,224,10,10,192,0,20,160,5,20,192,15,15,160,20,15};//黄色正向12针
const unsigned char yellowDSBBuffer11[]={160,20,10,192,10,10,224,10,20,128,20,20,224,10,20,192,10,10,128,20,0,224,10,10,192,0,20,224,10,10,160,20,10};//黄色正向11针
const unsigned char yellowDSBBuffer11r[]={192,20,10,128,10,10,160,0,20,192,10,10,160,20,0,224,10,10,128,10,20,224,20,20,128,10,20,160,10,10,192,20,10};//黄色逆向11针
const unsigned char yellowDSBBuffer10[]={160,20,10,192,10,10,224,10,20,128,20,20,224,10,20,192,10,10,128,10,10,160,0,20,192,10,20,160,20,20};//黄色正向10针
const unsigned char yellowDSBBuffer10r[]={192,20,20,160,10,20,128,0,20,224,10,10,160,10,10,128,10,20,224,20,20,128,10,20,160,10,10,192,20,10};//黄色逆向10针
const unsigned char yellowDSBBuffer9[]={160,20,10,192,10,10,224,10,20,128,20,20,224,10,20,192,10,10,128,20,0,192,20,5,160,20,15};//黄色正向9针
const unsigned char yellowDSBBuffer9r[]={192,20,15,160,20,5,224,20,0,160,10,10,128,10,20,224,20,20,128,10,20,160,10,10,192,20,10};//黄色逆向9针
const unsigned char yellowDSBBuffer8[]={160,20,10,192,10,10,224,10,20,128,20,20,224,10,20,128,10,10,192,20,5,160,20,15};//黄色正向8针
const unsigned char yellowDSBBuffer8r[]={192,20,15,160,20,5,224,10,10,128,10,20,224,20,20,128,10,20,160,10,10,192,20,10};//黄色逆向8针
const unsigned char yellowDSBBuffer7[]={160,20,10,192,10,10,224,10,20,128,20,20,224,10,20,192,10,10,160,20,10};//黄色正向7针
const unsigned char yellowDSBBuffer7r[]={192,20,10,160,10,10,128,10,20,224,20,20,128,10,20,160,10,10,192,20,10};//黄色逆向7针
const unsigned char yellowDSBBuffer6[]={160,10,20,192,10,10,128,20,0,224,20,10,128,10,20,160,10,20};//黄色正向6针
const unsigned char yellowDSBBuffer6r[]={192,10,20,224,10,20,128,20,10,224,20,0,160,10,10,192,10,20};//黄色逆向6针
const unsigned char yellowDSBBuffer5[]={160,10,20,128,10,20,224,20,20,128,10,20,160,10,20};//黄色正向5针
const unsigned char yellowDSBBuffer5r[]={192,10,20,224,10,20,128,20,20,224,10,20,192,10,20};//黄色逆向5针
const unsigned char yellowDSBBuffer4[]={160,10,20,128,10,20,224,20,10,160,20,10};//黄色正向4针
const unsigned char yellowDSBBuffer4r[]={192,20,10,128,20,10,224,10,20,192,10,20};//黄色逆向4针
const unsigned char yellowDSBBuffer3[]={160,10,20,192,0,20,160,10,20};//黄色正向3针
const unsigned char yellowDSBBuffer3r[]={192,10,20,160,0,20,192,10,20};//黄色逆向3针
const unsigned char yellowDSBBuffer2[]={160,0,20,128,20,0,};//黄色正向2针
const unsigned char yellowDSBBuffer2r[]={192,20,0,128,0,20};//黄色逆向2针
const unsigned char yellowDSBBuffer1[]={160,20,20};//黄色正向1针
const unsigned char yellowDSBBuffer1r[]={192,20,20};//黄色逆向1针

const unsigned char magentaDSBBuffer12[]={128,20,20,224,10,20,192,10,10,128,10,10,160,0,20,192,10,20,160,20,10,224,20,0,160,20,5,224,15,5,128,10,20,160,5,20};//红色正向12针
const unsigned char magentaDSBBuffer12r[]={192,5,20,224,10,20,128,15,5,192,20,5,128,20,0,192,20,10,160,10,20,128,0,20,224,10,10,160,10,10,128,10,20,224,20,20};//红色逆向12针
const unsigned char magentaDSBBuffer11[]={128,20,20,224,10,20,192,10,10,128,10,10,160,0,20,192,10,20,160,20,10,224,20,0,160,15,10,192,5,20,160,10,20};//红色正向11针
const unsigned char magentaDSBBuffer11r[]={192,10,20,160,5,20,192,15,10,160,20,0,192,20,10,160,10,20,128,0,20,224,10,10,160,10,10,128,10,20,224,20,20};//红色逆向11针
const unsigned char magentaDSBBuffer10[]={128,20,20,224,10,20,192,10,10,128,10,10,160,0,20,128,10,10,192,20,10,160,5,20,128,10,20,160,5,20};//红色正向10针
const unsigned char magentaDSBBuffer10r[]={192,5,20,224,10,20,192,5,20,160,20,10,224,10,10,128,0,20,224,10,10,160,10,10,128,10,20,224,20,20};//红色逆向10针
const unsigned char magentaDSBBuffer9[]={128,20,20,224,10,20,192,10,10,128,10,10,160,0,20,128,10,10,192,20,0,128,15,10,160,5,20};//红色正向9针
const unsigned char magentaDSBBuffer9r[]={192,5,20,224,15,10,160,10,10,128,0,20,224,10,10,160,20,0,224,10,10,128,10,20,224,20,20};//红色逆向9针
const unsigned char magentaDSBBuffer8[]={128,20,20,224,10,20,192,10,10,128,10,10,160,0,20,128,10,10,192,20,10,160,20,20};//红色正向8针
const unsigned char magentaDSBBuffer8r[]={192,20,20,160,20,10,224,10,10,128,0,20,224,10,10,160,10,10,128,10,20,224,20,20};//红色逆向8针
const unsigned char magentaDSBBuffer7[]={128,20,20,224,10,20,192,10,10,128,10,10,160,10,10,192,20,10,160,20,20};//红色正向7针
const unsigned char magentaDSBBuffer7r[]={192,20,20,160,20,10,192,10,10,224,10,10,160,10,10,128,10,20,224,20,20};//红色逆向7针
const unsigned char magentaDSBBuffer6[]={128,20,20,224,10,20,192,10,20,160,20,10,192,20,0,160,20,10};//红色正向6针
const unsigned char magentaDSBBuffer6r[]={192,20,10,128,20,0,192,20,10,160,10,20,128,10,20,224,20,20};//红色逆向6针
const unsigned char magentaDSBBuffer5[]={128,10,20,224,0,20,128,10,20,224,20,10,160,20,10};//红色正向5针
const unsigned char magentaDSBBuffer5r[]={192,20,10,128,20,10,224,10,20,128,0,20,224,10,20};//红色逆向5针
const unsigned char magentaDSBBuffer4[]={128,20,20,224,10,20,192,10,20,160,20,20};//红色正向4针
const unsigned char magentaDSBBuffer4r[]={192,20,20,160,10,20,128,10,20,224,20,20};//红色逆向4针
const unsigned char magentaDSBBuffer3[]={128,20,10,192,20,10,160,20,20};//红色正向3针
const unsigned char magentaDSBBuffer3r[]={192,20,20,160,20,10,224,20,10};//红色逆向3针
const unsigned char magentaDSBBuffer2[]={128,10,20,160,10,20};//红色正向2针
const unsigned char magentaDSBBuffer2r[]={192,10,20,224,10,20};//红色逆向2针
const unsigned char magentaDSBBuffer1[]={128,20,0};//红色正向1针
const unsigned char magentaDSBBuffer1r[]={224,20,0};//红色逆向1针

const unsigned char cyanDSBBuffer12[]={128,10,20,224,10,10,160,20,10,192,20,20,160,20,10,224,10,10,128,0,20,160,10,10,224,20,5,128,15,15,224,10,20,128,15,20};//兰色正向12针
const unsigned char cyanDSBBuffer12r[]={224,15,20,128,10,20,224,15,15,128,20,5,224,10,10,128,0,20,160,10,10,192,20,10,160,20,20,192,20,10,128,10,10,224,10,20};//兰色逆向12针
const unsigned char cyanDSBBuffer11[]={128,10,20,224,10,10,160,20,10,192,20,20,160,20,10,224,10,10,128,0,20,160,10,10,224,20,0,160,10,10,128,10,20};//兰色正向11针
const unsigned char cyanDSBBuffer11r[]={224,10,20,192,10,10,160,20,10,192,20,20,160,20,10,224,10,10,128,0,20,160,10,10,224,20,0,128,10,10,224,10,20};//兰色逆向11针
const unsigned char cyanDSBBuffer10[]={128,10,20,224,10,10,160,20,10,192,20,20,160,20,10,224,10,10,128,0,20,160,10,10,224,15,10,128,15,20};//兰色正向10针
const unsigned char cyanDSBBuffer10r[]={224,15,20,128,15,10,192,10,10,160,0,20,128,10,10,192,20,10,160,20,20,192,20,10,128,10,10,224,10,20};//兰色逆向10针
const unsigned char cyanDSBBuffer9[]={128,10,20,224,10,10,160,20,10,192,20,20,160,20,10,224,10,10,128,0,20,224,5,20,128,15,20};//兰色正向9针
const unsigned char cyanDSBBuffer9r[]={224,15,20,128,5,20,160,0,20,128,10,10,192,20,10,160,20,20,192,20,10,128,10,10,224,10,20};//兰色逆向9针
const unsigned char cyanDSBBuffer8[]={128,10,20,224,10,10,160,20,10,192,20,20,160,20,10,192,10,10,160,0,20,128,10,20};//兰色正向8针
const unsigned char cyanDSBBuffer8r[]={224,10,20,128,0,20,160,10,10,192,20,10,160,20,20,192,20,10,128,10,10,224,10,20};//兰色逆向8针
const unsigned char cyanDSBBuffer7[]={128,10,20,224,10,10,160,20,10,192,20,20,160,20,10,224,10,10,128,10,20};//兰色正向7针
const unsigned char cyanDSBBuffer7r[]={224,10,20,128,10,10,192,20,10,160,20,20,192,20,10,128,10,10,224,10,20};//兰色逆向7针
const unsigned char cyanDSBBuffer6[]={128,20,10,192,20,10,160,10,20,192,0,20,224,10,10,128,20,10};//兰色正向6针
const unsigned char cyanDSBBuffer6r[]={224,20,10,128,10,10,160,0,20,192,10,20,160,20,10,224,20,10};//兰色逆向6针
const unsigned char cyanDSBBuffer5[]={128,20,10,192,20,10,128,20,0,224,20,10,128,20,10};//兰色正向5针
const unsigned char cyanDSBBuffer5r[]={224,20,10,128,20,10,224,20,0,160,20,10,224,20,10};//兰色逆向5针
const unsigned char cyanDSBBuffer4[]={128,10,20,160,10,20,192,20,10,128,20,10};//兰色正向4针
const unsigned char cyanDSBBuffer4r[]={224,20,10,160,20,10,192,10,20,224,10,20};//兰色逆向4针
const unsigned char cyanDSBBuffer3[]={128,20,10,224,20,0,128,20,10};//兰色正向3针
const unsigned char cyanDSBBuffer3r[]={224,20,10,128,20,0,224,20,10};//兰色逆向3针
const unsigned char cyanDSBBuffer2[]={128,0,20,128,20,0};//兰色正向2针
const unsigned char cyanDSBBuffer2r[]={224,20,0,224,0,20};//兰色逆向2针
const unsigned char cyanDSBBuffer1[]={128,20,20};//兰色正向1针
const unsigned char cyanDSBBuffer1r[]={224,20,20};//兰色逆向1针

const unsigned char blackDSBBuffer12[]={128,20,20,224,20,10,160,20,10,192,10,20,224,10,20,128,20,10,224,10,10,192,10,20,160,20,10,192,10,10,160,5,20,192,15,20};//黑色正向12针
const unsigned char blackDSBBuffer12r[]={160,15,20,192,5,20,160,10,10,192,20,10,160,10,20,128,10,10,224,20,10,128,10,20,160,10,20,192,20,10,128,20,10,224,20,20};//黑色逆向12针
const unsigned char blackDSBBuffer11[]={128,20,10,192,10,10,224,10,10,160,10,10,128,10,10,192,20,10,160,10,20,128,10,20,224,20,10,160,20,10,192,20,20};//黑色正向11针
const unsigned char blackDSBBuffer11r[]={160,20,20,192,20,10,128,20,10,224,10,20,192,10,20,160,20,10,224,10,10,192,10,10,128,10,10,160,10,10,224,20,10};//黑色逆向11针
const unsigned char blackDSBBuffer10[]={128,10,20,160,0,20,128,10,10,224,20,0,128,20,10,224,20,20,128,20,10,192,10,10,160,10,20,192,20,20};//黑色正向10针
const unsigned char blackDSBBuffer10r[]={160,20,20,192,10,20,160,10,10,224,10,10,128,0,20,224,10,20,128,20,20,224,20,10,128,20,0,224,20,10};//黑色逆向10针
const unsigned char blackDSBBuffer9[]={128,20,20,224,20,10,128,20,0,224,10,10,128,0,20,160,10,20,192,20,10,160,10,10,192,10,20};//黑色正向9针
const unsigned char blackDSBBuffer9r[]={160,10,20,192,10,10,160,20,10,192,10,20,160,0,20,128,10,10,192,20,0,128,20,10,224,20,20};//黑色逆向9针
const unsigned char blackDSBBuffer8[]={128,20,20,224,20,10,160,20,10,192,10,20,160,0,20,192,10,10,160,20,0,192,20,10};//黑色正向8针
const unsigned char blackDSBBuffer8r[]={160,20,10,192,20,0,160,10,10,128,0,20,160,10,20,192,20,10,128,20,10,224,20,20};//黑色逆向8针
const unsigned char blackDSBBuffer7[]={128,20,20,224,20,10,160,10,10,128,10,10,192,10,10,160,10,20,192,20,20};//黑色正向7针
const unsigned char blackDSBBuffer7r[]={160,20,20,192,10,20,160,10,10,224,10,10,192,10,10,128,20,10,224,20,20};//黑色逆向7针
const unsigned char blackDSBBuffer6[]={128,20,20,224,20,10,160,20,10,192,10,20,160,0,20,192,10,20};//黑色正向6针
const unsigned char blackDSBBuffer6r[]={160,10,20,128,0,20,160,10,20,192,20,10,128,20,10,224,20,20};//黑色逆向6针
const unsigned char blackDSBBuffer5[]={128,20,10,224,20,0,128,20,10,224,10,20,192,10,20};//黑色正向5针
const unsigned char blackDSBBuffer5r[]={160,10,20,128,10,20,224,20,10,128,20,0,224,20,10};//黑色逆向5针
const unsigned char blackDSBBuffer4[]={128,0,10,128,20,10,160,0,20,192,20,20};//黑色正向4针
const unsigned char blackDSBBuffer4r[]={160,20,20,128,0,20,224,20,10,160,0,10};//黑色逆向4针
const unsigned char blackDSBBuffer3[]={128,20,20,224,10,20,192,10,20};//黑色正向3针
const unsigned char blackDSBBuffer3r[]={160,10,20,128,10,20,224,20,20};//黑色逆向3针
const unsigned char blackDSBBuffer2[]={128,20,10,192,20,10};//黑色正向2针
const unsigned char blackDSBBuffer2r[]={160,20,10,224,20,10};//黑色逆向2针
const unsigned char blackDSBBuffer1[]={128,0,20};//黑色正向1针
const unsigned char blackDSBBuffer1r[]={224,0,20};//黑色逆向1针

const unsigned char blackStartDataDSBBuffer[3] = {225,0,0};//黑色处理的第一针
const unsigned char cyanStartDataDSBBuffer[3] = {225,0,0};//青色处理的文件头
const unsigned char yellowStartDataDSBBuffer[6] = {225,0,0,128,0,20};//黄色色处理的文件头
const unsigned char magentaStartDataDSBBuffer[6] = {225,0,0};//品红色处理的文件头



static const unsigned char s_dsbEndCode[3] = {0xF8,0,0};//dsb文件的结束码
#define INIT_COLOR_VECTOR(colorName) \
m_##colorName##DSBBuffer<<colorName##DSBBuffer1\
    <<colorName##DSBBuffer2\
    <<colorName##DSBBuffer3\
    <<colorName##DSBBuffer4\
    <<colorName##DSBBuffer5\
    <<colorName##DSBBuffer6\
    <<colorName##DSBBuffer7\
    <<colorName##DSBBuffer8\
    <<colorName##DSBBuffer9\
    <<colorName##DSBBuffer10\
    <<colorName##DSBBuffer11\
    <<colorName##DSBBuffer12\
    ;\
m_##colorName##DSBBufferReverse<<colorName##DSBBuffer1r\
    <<colorName##DSBBuffer2r\
    <<colorName##DSBBuffer3r\
    <<colorName##DSBBuffer4r\
    <<colorName##DSBBuffer5r\
    <<colorName##DSBBuffer6r\
    <<colorName##DSBBuffer7r\
    <<colorName##DSBBuffer8r\
    <<colorName##DSBBuffer9r\
    <<colorName##DSBBuffer10r\
    <<colorName##DSBBuffer11r\
    <<colorName##DSBBuffer12r\
    ;

DImagePatternConver::DImagePatternConver(QObject* par):QObject(par)
  ,m_colorMode(CMYKMode)
  ,m_currentPresent(0)
  ,m_stop(false)
  ,m_needleCount(8)
  ,m_cmaxNeedleCountPerPix(12)
#ifdef D_IMAGE2PATTERN_IS_ENABLE_LOGO
  ,m_addLogo(false)
#endif
{
    INIT_COLOR_VECTOR(yellow)
    INIT_COLOR_VECTOR(magenta)
    INIT_COLOR_VECTOR(cyan)
    INIT_COLOR_VECTOR(black)
    m_transparentValues<<255<<254<<253<<252<<251;
    m_blackDSBFileStartData.append((const char*)blackStartDataDSBBuffer,3);
    m_yellowDSBFileStartData.append((const char*)yellowStartDataDSBBuffer,6);
    m_cyanDSBFileStartData.append((const char*)cyanStartDataDSBBuffer,3);
    m_magentaDSBFileStartData.append((const char*)magentaStartDataDSBBuffer,3);
}

void DImagePatternConver::setImage(const QImage &image)
{
    setStop();
    QMutexLocker locker(&m_mutex);
    m_image = image;
    m_imageC = QImage();
    m_imageM = QImage();
    m_imageY = QImage();
    m_imageK = QImage();
}
///
/// \brief 有别于setImage，使用此函数将不进行自动分割cmyk
/// \param c_image
/// \param m_image
/// \param y_image
/// \param k_image
///
void DImagePatternConver::setCMYKImage(const QImage &imageC, const QImage &imageM, const QImage &imageY, const QImage &imageK)
{
    setStop();
    QMutexLocker locker(&m_mutex);
    m_imageC = imageC;
    m_imageM = imageM;
    m_imageY = imageY;
    m_imageK = imageK;
    m_image = QImage();
}

DImagePatternConver::ColorMode DImagePatternConver::getColorMode() const
{
    return m_colorMode;
}

void DImagePatternConver::setColorMode(const ColorMode &colorMode)
{
    QMutexLocker locker(&m_mutex);
    m_colorMode = colorMode;
}

void DImagePatternConver::startConver()
{
    m_mutex.lock();
    m_stop = false;
    m_totalStitchCount = 0;
    ColorMode mode = m_colorMode;
    m_mutex.unlock();

    switch (mode) {
    case CMYKMode:
        dealCMYKModeImage();
        break;
    case MonoMode:
        dealMonoModeImage();
        break;
    default:
        break;
    }
}

int DImagePatternConver::getNeedleCountPerPix() const
{
    return m_needleCount;
}

void DImagePatternConver::setNeedleCountPerPix(int needleWay)
{
    m_needleCount = needleWay;
}

int DImagePatternConver::meanRgb(QRgb rgb)
{
    return (qRed(rgb) + qGreen(rgb) + qBlue(rgb))/3;
}
///
/// \brief 按照灰度转到针数
/// \param grayValue 灰度值0~255
/// \param totalNeedleCountPerPix 总针数目前只有8针的信息
/// \return 返回值为0时代表没有针
///
int DImagePatternConver::grayValueToNeedleCountCMYK(int grayValue, int totalNeedleCountPerPix)
{
#if 0
    int part = 255 / totalNeedleCountPerPix;
    return (255 - grayValue)/part + 1;
#else
    if(8 == totalNeedleCountPerPix)
    {
        if(grayValue<=255 && grayValue>224)
        {
            return 1;
        }
        else if(grayValue<=224 && grayValue>192)
        {
            return 2;
        }
        else if(grayValue<=192 && grayValue>160)
        {
            return 3;
        }
        else if(grayValue<=160 && grayValue>128)
        {
            return 4;
        }
        else if(grayValue<=128 && grayValue>96)
        {
            return 5;
        }
        else if(grayValue<=96 && grayValue>64)
        {
            return 6;
        }
        else if(grayValue<=64 && grayValue>32)
        {
            return 7;
        }
        else if(grayValue<=32 && grayValue>=0)
        {
            return 8;
        }
        else
        {
            return 0;
        }
    }

#endif
    return 0;
}

///
/// \brief 按照灰度转到针数
/// \param grayValue 灰度值0~255
/// \param totalNeedleCountPerPix 总针数目前只有8针的信息
/// \return 返回值为0时代表没有针
///
int DImagePatternConver::grayValueToNeedleCountMono(int grayValue, int totalNeedleCountPerPix)
{
#if 0
    int part = 255 / totalNeedleCountPerPix;
    return (255 - grayValue)/part + 1;
#else
    if(8 == totalNeedleCountPerPix)
    {
        if(grayValue<=255 && grayValue>228)
        {
            return 1;
        }
        else if(grayValue<=228 && grayValue>203)
        {
            return 2;
        }
        else if(grayValue<=203 && grayValue>175)
        {
            return 3;
        }
        else if(grayValue<=175 && grayValue>147)
        {
            return 4;
        }
        else if(grayValue<=147 && grayValue>116)
        {
            return 5;
        }
        else if(grayValue<=116 && grayValue>85)
        {
            return 6;
        }
        else if(grayValue<=85 && grayValue>48)
        {
            return 7;
        }
        else if(grayValue<=48 && grayValue>=0)
        {
            return 8;
        }
        else
        {
            return 0;
        }
    }

#endif
    return 0;
}


void DImagePatternConver::setStop()
{
    QMutexLocker locker(&m_mutex);
    m_stop = true;
}

///
/// \brief 处理彩色针法的走向
///
void DImagePatternConver::dealCMYKModeImage()
{
    m_mutex.lock();
    m_stop = false;
    m_currentPresent = 0;
    QImage imageC;
    QImage imageM;
    QImage imageY;
    QImage imageK;
    int imageWidth,imageHeight;
    if(m_image.isNull())
    {
        imageC = m_imageC;
        imageM = m_imageM;
        imageY = m_imageY;
        imageK = m_imageK;
        imageWidth = qMin(qMin(qMin(imageC.width(),imageM.width()),imageY.width()),imageK.width());
        imageHeight = qMin(qMin(qMin(imageC.height(),imageM.height()),imageY.height()),imageK.height());
    }
    else
    {
        DImageAlgorithm::splitToCMYKChannel(m_image
                                            ,imageC
                                            ,imageM
                                            ,imageY
                                            ,imageK);
        imageWidth = m_image.width();
        imageHeight = m_image.height();
    }
    m_mutex.unlock();
    if(imageC.isNull() || imageM.isNull() || imageY.isNull() || imageK.isNull())
    {
        m_lastErrString = tr("图片格式错误");
        emit finish(QByteArray(),false);
        return;
    }
    //DSB文件的内存
    QByteArray datas;
    //填充dsb文件头
    const int headerLen = 512;
    char headerBuffer[headerLen];
    memset(headerBuffer,0x20,headerLen);
    datas.append(headerBuffer,headerLen);

    bool b = false;
    bool direction = false;//记录最后一针的走向的变量
#if 1
    b = dealYColorChannel(datas,imageY,&direction);
    if(!b)
    {
        m_lastErrString = tr("黄色通道数据处理出错");
        emit finish(QByteArray(),false);
        return;
    }
    //换色
    stitchChangColor(datas);
    //黄板针法走完，位置在右上角，需要跳针到红版针法的开始位置(左上角)
    if(direction)
    {
        //说明最后一针是斜向上结束
        stitchSkipLeft(datas,20 * imageWidth);
    }
    else
    {
        //说明最后一针是斜向下结束
        //stitchSkipUp(datas,20);//黄板针法结束时位于2*2的右下角，红版针法开始位置在2*2的左上角，因此要有20的偏移
        stitchSkipLeft(datas,20 * imageWidth);
    }
#endif

#if 1
    b = dealMColorChannel(datas,imageM);
    if(!b)
    {
        m_lastErrString = tr("品红色通道数据处理出错");
        emit finish(QByteArray(),false);
        return;
    }
    //换色
    stitchChangColor(datas);
    //品红板针法走完，位置在右上角或右下角，需要跳针到青色版针法的开始位置(左上角)

    if(0 != imageWidth % 2)
    {
        //如果宽度不是偶数，红色针法走完会在右下角
        stitchSkipLeft(datas,20 * imageWidth);
        stitchSkipUp(datas,20 * (imageHeight-1));
    }
    else
    {
        stitchSkipLeft(datas,20 * imageWidth);
        stitchSkipDown(datas, 20);
    }
#endif

#if 1
    b = dealCColorChannel(datas,imageC,&direction);
    if(!b)
    {
        m_lastErrString = tr("青色通道数据处理出错");
        emit finish(QByteArray(),false);
        return;
    }
    //换色
    stitchChangColor(datas);
    //青色板针法走完，位置在右下角，需要跳针到黑色版针法的开始位置(左下角)
    if(direction)
    {
        //正方向，斜向上结束
        stitchSkipLeft(datas,20 * imageWidth);
        //stitchSkipDown(datas,20);
    }
    else
    {
        //反方向，斜向下结束
        stitchSkipLeft(datas,20 * imageWidth);
    }
#endif


#if 1
    b = dealKColorChannel(datas,imageK);
    if(!b)
    {
        m_lastErrString = tr("黑色通道数据处理出错");
        emit finish(QByteArray(),false);
        return;
    }
#endif
#ifdef D_IMAGE2PATTERN_IS_ENABLE_LOGO
    if (m_addLogo)
    {
        //换色
        stitchChangColor(datas);
        //添加logo
        //此时位于左上角，需要跳针到右下角
        if(0 == imageHeight%2)
        {
            stitchSkipRight(datas,imageWidth * 20);
        }
        stitchSkipDown(datas,imageHeight * 20);
        //写死目前logo的长度为3X3
        stitchSkipLeft(datas,300);
        stitchSkipUp(datas,300);
        //

        datas += m_logoDsbDatas;

        qDebug() << "add logo:"
                 << "\n logo data size:" << m_logoDsbDatas.size()
                    ;
    }
#endif
    //结束码
    stitchEndCode(datas);

    emit finish(datas,true);
}

///
/// \brief 处理黑白色的花样
///
void DImagePatternConver::dealMonoModeImage()
{
    //多线程支持
    m_mutex.lock();
    m_stop = false;
    m_currentPresent = 0;
    QImage image = m_image;//为了避免多线的竞争，这里先把图片传递给临时变量
    m_mutex.unlock();

    if(image.isNull())
    {
        m_lastErrString = tr("图片格式错误");
        emit finish(QByteArray(),false);
        return;
    }
    //把图片处理为灰度
    DImageAlgorithm::toGray(image);
    //DSB文件的内存
    QByteArray datas;

    //填充dsb文件头
    const int headerLen = 512;
    char headerBuffer[headerLen];
    memset(headerBuffer,0x20,headerLen);
    datas.append(headerBuffer,headerLen);
    bool b = false;
    b = dealMonoColor(datas,image);
    if(!b)
    {
        m_lastErrString = tr("黑色通道数据处理出错");
        emit finish(QByteArray(),false);
        return;
    }
#ifdef D_IMAGE2PATTERN_IS_ENABLE_LOGO
    if (m_addLogo)
    {
        //换色
        stitchChangColor(datas);
        QMargins region;
        QPoint lastPoint;
        calcRegion(datas,region,lastPoint);
        //添加logo
        //此时位于左上角，需要跳针到右下角
        int skipH = region.right() - 300 - lastPoint.x();
        if(skipH>0)
        {
            stitchSkipRight(datas,skipH);
        }
        else
        {
            stitchSkipLeft(datas,-skipH);
        }
        int skipV = region.bottom()  + 300 - lastPoint.y();
        if(skipV>0)
        {
            stitchSkipUp(datas,skipV);
        }
        else
        {
            stitchSkipDown(datas,-skipV);
        }
        datas += m_logoDsbDatas;

    }
#endif
    //结束码
    stitchEndCode(datas);
    emit finish(datas,true);
}

bool DImagePatternConver::dealKColorChannel(QByteArray& dsbBuffer, const QImage &image)
{
    //成员变量预先赋值，防止多线程冲突
    m_mutex.lock();
    int totalNeedlePerPix = m_needleCount;
    int maxNeedleCount = m_cmaxNeedleCountPerPix;
    m_mutex.unlock();

    unsigned int totalNeedleCount = 0;
    //换色，黑色默认为4针位
    // dsbBuffer.append(0x68|4);//设置一个换色
    const int width = image.width();
    const int height = image.height();
    //黑色文件头
    dsbBuffer.append(m_blackDSBFileStartData);
    bool bDirect = true;//true为向正方向迭代

    for(int y = height-1;y>=0;--y)//黑色是从左下角开始处理
    {
        if(bDirect)
        {
            for(int x = 0;x<width;++x)
            {
                QRgb rgb = image.pixel(x,y);
                //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
                int grayValue = meanRgb(rgb);
                //获取针数
                int needle = grayValueToNeedleCountCMYK(grayValue,totalNeedlePerPix);
                //限定范围
                needle = qBound(1,needle,maxNeedleCount);
                //根据针数获取dsb文件对应的字节
                const unsigned char* p = m_blackDSBBuffer[needle-1];
                //把对应的字节插入缓冲区
#ifdef DEBUG_OUT_PUT
                for(int i=0;i<needle*3;++i)
                {
                    fprintf(stderr,"%3d,",(int)p[i]);
                }
                fprintf(stderr,"\n");
#endif
                dsbBuffer.append((const char*)(p),needle*3);
                totalNeedleCount += needle;
            }
            //走到最右需要向上走20
            stitchMoveUp(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else
        {
            for(int x = width-1;x>=0;--x)
            {
                QRgb rgb = image.pixel(x,y);
                //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
                int grayValue = meanRgb(rgb);
                //获取针数
                int needle = grayValueToNeedleCountCMYK(grayValue,totalNeedlePerPix);
                //限定范围
                needle = qBound(1,needle,maxNeedleCount);
                //根据针数获取dsb文件对应的字节
                const unsigned char* p = m_blackDSBBufferReverse[needle-1];
#ifdef DEBUG_OUT_PUT
                for(int i=0;i<needle*3;++i)
                {
                    fprintf(stderr,"%3d,",(int)p[i]);
                }
                fprintf(stderr,"\n");
#endif
                //把对应的字节插入缓冲区
                dsbBuffer.append((const char*)(p),needle*3);
                totalNeedleCount += needle;
            }
            stitchMoveUp(dsbBuffer,20);
            bDirect = !bDirect;
        }
    }
    //dsbBuffer.append((const char *)(s_dsbEndCode),3);//写入结束码
    qDebug() << "k color total needle:" << totalNeedleCount;
    m_mutex.lock();
    m_totalStitchCount += totalNeedleCount;
    m_mutex.unlock();
    return true;
}

bool DImagePatternConver::dealYColorChannel(QByteArray& dsbBuffer, const QImage &image,bool* direction)
{
    //成员变量预先幅值，防止多线程冲突
    m_mutex.lock();
    int totalNeedlePerPix = m_needleCount;
    int maxNeedleCount = m_cmaxNeedleCountPerPix;
    m_mutex.unlock();
    //换色，黑色默认为4针位
    // dsbBuffer.append(0x68|4);//设置一个换色
    const int width = image.width();
    const int height = image.height();
    const int totalPixel = width * height;
    //黄色色文件头
    dsbBuffer.append(m_yellowDSBFileStartData);
    bool bDirect = true;//true为向正方向迭代,黄色正方向为斜向上,此变量必须设置为true，第一次循环会从1变到2，方向变更为false

    unsigned int totalNeedleCount = 0;
    int i=0;
    int x = 0;
    int y = height-1;
    const unsigned char* p = NULL;
    while(i < totalPixel)
    {
        //qDebug() << "(" << x << "," << y << ")";
        if(x<0 || x>width || y<0 || y>height)
        {
            break;
        }
        QRgb rgb = image.pixel(x,y);
        //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
        int grayValue = meanRgb(rgb);
        //获取针数
        int needle = grayValueToNeedleCountCMYK(grayValue,totalNeedlePerPix);
        //限定范围
        needle = qBound(1,needle,maxNeedleCount);
        //根据针数获取dsb文件对应的字节
        if(bDirect)
        {
            p = m_yellowDSBBuffer[needle-1];
        }
        else
        {
            p = m_yellowDSBBufferReverse[needle-1];
        }
        //把对应的字节插入缓冲区
        dsbBuffer.append((const char*)(p),needle*3);
        totalNeedleCount += needle;
        if(bDirect)
        {
            --x;
            --y;
        }
        else
        {
            ++x;
            ++y;
        }
        ++i;
        //边界条件处理
        if(x<0 && y>=0 && y<height)
        {
            //x等于0时，需要向上移动,同时方向改变
            x = 0;
            stitchMoveUp(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x<0 && y<0)
        {
            //左上角特殊点，这时需要向右移动
            x=1;
            y=0;
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x>=0 && x<width && y>=height)
        {
            //这时需要向右移动
            y = height - 1;
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x>=width && y>=height)
        {
            //右下特殊点，这时需要向上移动
            y = height - 2;
            x = width -1;
            stitchMoveUp(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x >=width && y >=0 && y<height)
        {
            y -= 2;
            x = width - 1;
            stitchMoveUp(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if (x >= 0 && x < width && y<0)
        {
            x += 2;
            y = 0;
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
    }
    if(direction)
    {
        *direction = bDirect;//记录最后的方向，最后一个方向虽然改变但没有作用，真正的方向是!bDirect
    }
    //dsbBuffer.append((const char *)(s_dsbEndCode),3);//写入结束码
    qDebug() << "y color total needle:" << totalNeedleCount;
    m_mutex.lock();
    m_totalStitchCount += totalNeedleCount;
    m_mutex.unlock();
    return true;
}

///
/// \brief 红色
/// \param dsbBuffer
/// \param image
/// \return
///
bool DImagePatternConver::dealMColorChannel(QByteArray& dsbBuffer, const QImage &image)
{
    //成员变量预先赋值，防止多线程冲突
    m_mutex.lock();
    int totalNeedlePerPix = m_needleCount;
    int maxNeedleCount = m_cmaxNeedleCountPerPix;
    m_mutex.unlock();

    unsigned int totalNeedleCount = 0;
    //换色，黑色默认为4针位
    // dsbBuffer.append(0x68|4);//设置一个换色
    const int width = image.width();
    const int height = image.height();
    //红色文件头
    dsbBuffer.append(m_magentaDSBFileStartData);
    bool bDirect = true;//true为向正方向迭代

    for(int x = 0;x<width;++x)//红色色是从左上角开始处理
    {
        if(bDirect)
        {
            for(int y = 0;y<height;++y)
            {
                QRgb rgb = image.pixel(x,y);
                //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
                int grayValue = meanRgb(rgb);
                //获取针数
                int needle = grayValueToNeedleCountCMYK(grayValue,totalNeedlePerPix);
                //限定范围
                needle = qBound(1,needle,maxNeedleCount);
                //根据针数获取dsb文件对应的字节
                const unsigned char* p = m_magentaDSBBufferReverse[needle-1];
                //把对应的字节插入缓冲区
                dsbBuffer.append((const char*)(p),needle*3);
                totalNeedleCount += needle;

            }
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else
        {
            for(int y = height-1;y>=0;--y)
            {
                QRgb rgb = image.pixel(x,y);
                //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
                int grayValue = meanRgb(rgb);
                //获取针数
                int needle = grayValueToNeedleCountCMYK(grayValue,totalNeedlePerPix);
                //限定范围
                needle = qBound(1,needle,maxNeedleCount);
                //根据针数获取dsb文件对应的字节
                const unsigned char* p = m_magentaDSBBuffer[needle-1];
                //把对应的字节插入缓冲区
                dsbBuffer.append((const char*)(p),needle*3);
                totalNeedleCount += needle;
            }
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
    }

    //dsbBuffer.append((const char *)(s_dsbEndCode),3);//写入结束码
    qDebug() << "m color total needle:" << totalNeedleCount;
    m_mutex.lock();
    m_totalStitchCount += totalNeedleCount;
    m_mutex.unlock();

    return true;
}
///
/// \brief 青色
/// \param dsbBuffer
/// \param image
/// \return
///
bool DImagePatternConver::dealCColorChannel(QByteArray &dsbBuffer, const QImage &image, bool *direction)
{
    //成员变量预先幅值，防止多线程冲突
    m_mutex.lock();
    int totalNeedlePerPix = m_needleCount;
    int maxNeedleCount = m_cmaxNeedleCountPerPix;
    m_mutex.unlock();
    //换色，黑色默认为4针位
    // dsbBuffer.append(0x68|4);//设置一个换色
    const int width = image.width();
    const int height = image.height();
    const int totalPixel = width * height;
    //黄色色文件头
    dsbBuffer.append(m_cyanDSBFileStartData);
    bool bDirect = true;//true为向正方向迭代,黄色正方向为斜向上,此变量必须设置为true，第一次循环会从1变到2，方向变更为false

    unsigned int totalNeedleCount = 0;
    int i=0;
    int x = 0;
    int y = 0;
    const unsigned char* p = NULL;
    while(i < totalPixel)
    {
        //qDebug() << "(" << x << "," << y << ")";
        if(x<0 || x>width || y<0 || y>height)
        {
            break;
        }
        QRgb rgb = image.pixel(x,y);
        //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
        int grayValue = meanRgb(rgb);
        //获取针数
        int needle = grayValueToNeedleCountCMYK(grayValue,totalNeedlePerPix);
        //限定范围
        needle = qBound(1,needle,maxNeedleCount);
        //根据针数获取dsb文件对应的字节
        if(bDirect)
        {
            p = m_cyanDSBBuffer[needle-1];
        }
        else
        {
            p = m_cyanDSBBufferReverse[needle-1];
        }
        //把对应的字节插入缓冲区
        dsbBuffer.append((const char*)(p),needle*3);
        totalNeedleCount += needle;
        if(bDirect)
        {
            ++x;
            --y;
        }
        else
        {
            --x;
            ++y;
        }
        ++i;
        //边界条件处理
        if(x>=0 && x<width && y<0)
        {
            //上边缘
            y = 0;
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x < 0 && y >= 0 && y < height)
        {
            //左边缘
            x = 0;
            stitchMoveDown(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x >= width && y < 0)
        {
            //右上角
            x = width - 1;
            y = 1;
            stitchMoveDown(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x < 0 && y >= height)
        {
            //左下角
            x = 1;
            y = height - 1;
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x>=0 && x<width && y>=height)
        {
            //下边缘
            x += 2;
            y = height - 1;
            stitchMoveRight(dsbBuffer,20);
            bDirect = !bDirect;
        }
        else if(x >= width && y >= 0 && y<height)
        {
            //右边缘
            x = width - 1;
            y += 2;
            stitchMoveDown(dsbBuffer,20);
            bDirect = !bDirect;
        }
    }
    if(direction)
    {
        *direction = !bDirect;//记录最后的方向，最后一个方向虽然改变但没有作用，真正的方向是!bDirect
    }
    //dsbBuffer.append((const char *)(s_dsbEndCode),3);//写入结束码
    qDebug() << "c color total needle:" << totalNeedleCount;
    m_mutex.lock();
    m_totalStitchCount += totalNeedleCount;
    m_mutex.unlock();
    return true;
}

///
/// \brief 处理黑白色数据
/// \param dsbBuffer dsb文件内存
/// \param image 已经进行灰度化的图片
/// \return 成功返回true
///
bool DImagePatternConver::dealMonoColor(QByteArray &dsbBuffer, const QImage &image)
{
    //成员变量预先幅值，防止多线程冲突
    m_mutex.lock();
    int totalNeedlePerPix = m_needleCount;
    int maxNeedleCount = m_cmaxNeedleCountPerPix;
    QSet<unsigned int> transparentValues = m_transparentValues;
    m_mutex.unlock();
    //换色，黑色默认为4针位
    // dsbBuffer.append(0x68|4);//设置一个换色
    const int width = image.width();
    const int height = image.height();
    //黑色文件头
    dsbBuffer.append(m_blackDSBFileStartData);
    bool bDirect = true;//true为向正方向迭代
    bool bFindFirstNeedleFlag = false;
    int x_position = 0;

    unsigned int totalNeedleCount = 0;

    for(int y = height-1;y>=0;--y)//黑色是从左下角开始处理
    {
        int x_max = width - 1;
        int x_min = 0;

        for (int x = width -1; x >= 0; x--) {
            QRgb rgb = image.pixel(x,y);
            //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
            int grayValue = meanRgb(rgb);
            if (!transparentValues.contains(grayValue)) {
                x_max = x;
                break;
            }
            if (x == 0) {
                x_max = 0;
            }
        }
        for (int x = 0; x <= width -1; x++) {
            QRgb rgb = image.pixel(x,y);
            //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
            int grayValue = meanRgb(rgb);
            if (!transparentValues.contains(grayValue)) {
                x_min = x;
                break;
            }
            if (x == (width - 1)) {
                x_min = width -1;
            }
        }
        if (x_max < x_min) {
         //   break;
            continue;
        }


        if(bDirect)
        {
            if (x_position > x_min) {
                while(x_position > x_min) {
                    if (!bFindFirstNeedleFlag) {
                        dsbBuffer.append(224+1);
                    } else {
                        dsbBuffer.append(224);
                    }
                    dsbBuffer.append(char (0));
                    dsbBuffer.append(20);
                    x_position--;
                    totalNeedleCount += 1;
                }
            }else {
                while(x_position < x_min) {
                    if (!bFindFirstNeedleFlag) {
                        dsbBuffer.append(128 + 1);
                    } else {
                        dsbBuffer.append(128);
                    }
                    dsbBuffer.append(char (0));
                    dsbBuffer.append(20);
                    x_position++;
                    totalNeedleCount += 1;
                }
            }

            for(int x = x_min;x<=x_max;++x)
            {
                QRgb rgb = image.pixel(x,y);
                //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
                int grayValue = meanRgb(rgb);
                if (255 != grayValue ) {
                    bFindFirstNeedleFlag = true;
                }
                //获取针数
                int needle = grayValueToNeedleCountMono(grayValue,totalNeedlePerPix);
                //限定范围
                needle = qBound(1,needle,maxNeedleCount);
                //根据针数获取dsb文件对应的字节
                const unsigned char* p = m_blackDSBBuffer[needle-1];
                //把对应的字节插入缓冲区
                if (!bFindFirstNeedleFlag) {
                    dsbBuffer.append(128+1);
                    dsbBuffer.append((char)0);
                    dsbBuffer.append(20);
                    totalNeedleCount += 1;
                } else {
                    dsbBuffer.append((const char*)(p),needle*3);
                    totalNeedleCount += needle;
                }
                x_position = x;
            }

            if (!bFindFirstNeedleFlag) {
                dsbBuffer.append(128+1);
                dsbBuffer.append(20);
                dsbBuffer.append(char(0));
            } else {
                //走到最右需要向上走20
                stitchMoveUp(dsbBuffer,20);
                totalNeedleCount += 1;
            }
            bDirect = !bDirect;
        }
        else
        {
            if (x_position > x_max) {
                while(x_position > x_max) {
                    if (!bFindFirstNeedleFlag) {
                        dsbBuffer.append(224+1);
                    } else {
                        dsbBuffer.append(224);
                    }
                    dsbBuffer.append(char (0));
                    dsbBuffer.append(20);
                    x_position--;
                    totalNeedleCount += 1;
                }

            } else {
                while(x_position < x_max) {
                    if (!bFindFirstNeedleFlag) {
                        dsbBuffer.append(128+1);
                    } else {
                        dsbBuffer.append(128);
                    }
                    dsbBuffer.append(char (0));
                    dsbBuffer.append(20);
                    x_position++;
                    totalNeedleCount += 1;
                }
            }

            for(int x = x_max;x>=x_min;--x)
            {
                QRgb rgb = image.pixel(x,y);
                //获取灰度值，理论此时图片已经是灰度图，rgb三个都是一样的
                int grayValue = meanRgb(rgb);

                if (255 != grayValue ) {
                    bFindFirstNeedleFlag = true;
                }
                //获取针数
                int needle = grayValueToNeedleCountMono(grayValue,totalNeedlePerPix);
                //限定范围
                needle = qBound(1,needle,maxNeedleCount);
                //根据针数获取dsb文件对应的字节
                const unsigned char* p = m_blackDSBBufferReverse[needle-1];
    #if 0
                for(int i=0;i<needle*3;++i)
                {
                    fprintf(stderr,"%3d,",(int)p[i]);
                }
                fprintf(stderr,"\n");
    #endif
                //把对应的字节插入缓冲区
                if (!bFindFirstNeedleFlag) {
                    dsbBuffer.append(224+1);
                    dsbBuffer.append(char(0));
                    dsbBuffer.append(20);

                    totalNeedleCount += 1;
                } else {
                    dsbBuffer.append((const char*)(p),needle*3);
                    totalNeedleCount += needle;
                }
                x_position = x;
            }

            if (!bFindFirstNeedleFlag) {
                dsbBuffer.append(128+1);
                dsbBuffer.append(20);
                dsbBuffer.append(char(0));
                totalNeedleCount += 1;
            } else {
                //走到最右需要向上走20
                stitchMoveUp(dsbBuffer,20);
                totalNeedleCount += 1;
            }
            bDirect = !bDirect;
        }
    }
    qDebug() << "total needle:" << totalNeedleCount;
    return true;
}

///
/// \brief dsb文件写入向下移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DImagePatternConver::stitchMoveDown(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {224,distance,0};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief dsb文件写入向上移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DImagePatternConver::stitchMoveUp(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {128,distance,0};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief dsb文件写入向右移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DImagePatternConver::stitchMoveRight(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {128,0,distance};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief dsb文件写入向左移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DImagePatternConver::stitchMoveLeft(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {224,0,distance};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 向下跳针
/// \param dsbBuffer
/// \param distance
///
void DImagePatternConver::stitchSkipDown(QByteArray &dsbBuffer, unsigned int distance)
{
    unsigned char buffer[3] = {0xE1,0,0};
    int count = distance / 127;
    int dis = distance % 127;
    for(int i = 0;i < count; ++i)
    {
        buffer[1] = 127;
        buffer[2] = 0;
        dsbBuffer.append((const char*)(buffer),3);
    }
    buffer[1] = dis;
    buffer[2] = 0;
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 向上跳针
/// \param dsbBuffer
/// \param distance
///
void DImagePatternConver::stitchSkipUp(QByteArray &dsbBuffer, unsigned int distance)
{
    unsigned char buffer[3] = {0x81,0,0};
    int count = distance / 127;
    int dis = distance % 127;
    for(int i = 0;i < count; ++i)
    {
        buffer[1] = 127;
        buffer[2] = 0;
        dsbBuffer.append((const char*)(buffer),3);
    }
    buffer[1] = dis;
    buffer[2] = 0;
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 向右跳针
/// \param dsbBuffer
/// \param distance
///
void DImagePatternConver::stitchSkipRight(QByteArray &dsbBuffer, unsigned int distance)
{
    unsigned char buffer[3] = {0x81,0,0};
    int count = distance / 127;
    int dis = distance % 127;
    for(int i = 0;i < count; ++i)
    {
        buffer[1] = 0;
        buffer[2] = 127;
        dsbBuffer.append((const char*)(buffer),3);
    }
    buffer[1] = 0;
    buffer[2] = dis;
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 向左跳针
/// \param dsbBuffer
/// \param distance
///
void DImagePatternConver::stitchSkipLeft(QByteArray &dsbBuffer, unsigned int distance)
{
    unsigned char buffer[3] = {0xE1,0,0};
    int count = distance / 127;
    int dis = distance % 127;
    for(int i = 0;i < count; ++i)
    {
        buffer[1] = 0;
        buffer[2] = 127;
        dsbBuffer.append((const char*)(buffer),3);
    }
    buffer[1] = 0;
    buffer[2] = dis;
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 写入结束码
/// \param dsbBuffer
///
void DImagePatternConver::stitchEndCode(QByteArray &dsbBuffer)
{
    const unsigned char buffer[3] = {0x98,0,0};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 写入换色码
/// \param dsbBuffer
///
void DImagePatternConver::stitchChangColor(QByteArray &dsbBuffer)
{
    const unsigned char buffer[3] = {0x89,0,0};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 计算dsb文件的范围和最后一帧的坐标
/// \param dsbBuffer dsb文件，带文件头
/// \param region 范围
/// \param lastPoint 最后一帧的坐标
///
void DImagePatternConver::calcRegion(const QByteArray &dsbBuffer, QMargins &region, QPoint &lastPoint)
{
    lastPoint.setX(0);
    lastPoint.setY(0);
    int xmin,xmax,ymin,ymax;
    xmin = ymin = xmax = ymax = 0;
    const int size = dsbBuffer.size();
    int index = 512;
    char byte1,byte2,byte3;
    int x=0,y=0;
    int dx=0,dy=0;
    while((index+2) < size)
    {
        byte1 = dsbBuffer[index];//(&byte1,1);
        ++index;
        byte2 = dsbBuffer[index];//(&byte1,1);
        ++index;
        byte3 = dsbBuffer[index];//(&byte1,1);
        ++index;
        dx = byte3;
        dy = byte2;
        if ((byte1&0x20) == 0x20)
            dx = -dx;
        if ((byte1&0x40) == 0x40)
            dy = -dy;
        lastPoint.rx() += dx;
        lastPoint.ry() += dy;
        if(lastPoint.x() < xmin)
        {
            xmin = lastPoint.x();
        }
        if(lastPoint.x() > xmax)
        {
            xmax = lastPoint.x();
        }
        if(lastPoint.y() < ymin)
        {
            ymin = lastPoint.y();
        }
        if(lastPoint.y() > ymax)
        {
            ymax = lastPoint.y();
        }
    }
    region.setLeft(xmin);
    region.setRight(xmax);
    region.setTop(ymax);
    region.setBottom(ymin);
}

#ifdef D_IMAGE2PATTERN_IS_ENABLE_LOGO
void DImagePatternConver::getLogoDsbDatas(QByteArray &datas) const
{
    datas = m_logoDsbDatas;
}

void DImagePatternConver::setLogoDsbDatas(const QByteArray &logoDsbDatas)
{
    m_logoDsbDatas = logoDsbDatas;
}

bool DImagePatternConver::isAddLogo() const
{
    return m_addLogo;
}

void DImagePatternConver::setAddLogo(bool addLogo)
{
    m_addLogo = addLogo;
}
#endif
QSet<unsigned int> &DImagePatternConver::getTransparentValues()
{
    return m_transparentValues;
}
const QSet<unsigned int> &DImagePatternConver::getTransparentValues() const
{
    return m_transparentValues;
}
void DImagePatternConver::setTransparentValues(const QSet<unsigned int> &transparentValues)
{
    m_transparentValues = transparentValues;
}

unsigned int DImagePatternConver::getTotalStitchCount() const
{
    return m_totalStitchCount;
}


