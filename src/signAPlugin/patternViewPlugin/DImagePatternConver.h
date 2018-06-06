#ifndef DIMAGEPATTERNCONVER_H
#define DIMAGEPATTERNCONVER_H
#include <QObject>
#include <QMutex>
#include <QImage>
#include <QVector>
#include <QByteArray>
#include <QSet>
#include <QMargins>
#include "DImagePatternConverGlobal.h"
/// \def 是否显示logo
#define D_IMAGE2PATTERN_IS_ENABLE_LOGO 1
///
/// \brief 图片转类花样
///
class D_IMAGE2PATTERN_EXPORT DImagePatternConver : public QObject
{
    Q_OBJECT
public:
    enum ColorMode{
        CMYKMode ///< CMYK模式
        ,MonoMode ///< 单色
    };
    DImagePatternConver(QObject* par = NULL);
    void setImage(const QImage& image);
    void setCMYKImage(const QImage& imageC,const QImage& imageM,const QImage& imageY,const QImage& imageK);
    //颜色模式
    ColorMode getColorMode() const;
    void setColorMode(const ColorMode &colorMode);
    //中断操作
    void setStop();
    //设置总针数，目前不能超过12针
    int getNeedleCountPerPix() const;
    void setNeedleCountPerPix(int needleWay);
    //获取rgb的均值
    static int meanRgb(QRgb rgb);
    //灰度值计算针数，灰度值为0~255，计算的针数
    static int grayValueToNeedleCountCMYK(int grayValue,int totalNeedleCountPerPix);
    static int grayValueToNeedleCountMono(int grayValue,int totalNeedleCountPerPix);
    unsigned int getTotalStitchCount() const;
    //
    const QSet<unsigned int>& getTransparentValues() const;
    QSet<unsigned int>& getTransparentValues();
    void setTransparentValues(const QSet<unsigned int> &transparentValues);
    //
#ifdef D_IMAGE2PATTERN_IS_ENABLE_LOGO
    bool isAddLogo() const;
    void setAddLogo(bool addLogo);
    void getLogoDsbDatas(QByteArray& datas) const;
    void setLogoDsbDatas(const QByteArray &logoDsbDatas);
#endif

public slots:
    //开始转换
    void startConver();
signals:
    ///
    /// \brief 成功转换好的点集
    /// \param points
    ///
    void finish(const QByteArray& buffer,bool isSuccess);
    ///
    /// \brief 转换的进度
    /// \param present
    ///
    void dealPresent(short present);
private:
    void dealCMYKModeImage();
    void dealMonoModeImage();
    //处理黑色通道
    bool dealKColorChannel(QByteArray& dsbBuffer,const QImage& image);
    bool dealYColorChannel(QByteArray& dsbBuffer,const QImage& image,bool* direction = NULL);
    bool dealMColorChannel(QByteArray& dsbBuffer,const QImage& image);
    bool dealCColorChannel(QByteArray& dsbBuffer,const QImage& image,bool* direction = NULL);
    //处理单色通道数据
    bool dealMonoColor(QByteArray& dsbBuffer,const QImage& image);
    //给dsb文件写入方向
    static void stitchMoveDown(QByteArray& dsbBuffer, unsigned char distance = 20);
    static void stitchMoveUp(QByteArray& dsbBuffer,unsigned char distance = 20);
    static void stitchMoveRight(QByteArray& dsbBuffer,unsigned char distance = 20);
    static void stitchMoveLeft(QByteArray& dsbBuffer,unsigned char distance = 20);
    //跳针
    static void stitchSkipDown(QByteArray& dsbBuffer,unsigned int distance);
    static void stitchSkipUp(QByteArray& dsbBuffer,unsigned int distance);
    static void stitchSkipRight(QByteArray& dsbBuffer,unsigned int distance);
    static void stitchSkipLeft(QByteArray& dsbBuffer,unsigned int distance);
    //写入结束码
    static void stitchEndCode(QByteArray& dsbBuffer);
    //写入换色码
    static void stitchChangColor(QByteArray& dsbBuffer);
    //计算dsb文件的范围和最后一帧的坐标
    static void calcRegion(const QByteArray& dsbBuffer,QMargins& region,QPoint& lastPoint);
private:
    volatile bool m_stop;
    short m_currentPresent;
    ColorMode m_colorMode;
    int m_needleCount;///< 允许的每个通道每像素最大针数
    QImage m_image;
    QImage m_imageC;
    QImage m_imageM;
    QImage m_imageY;
    QImage m_imageK;
    QMutex m_mutex;
    QString m_lastErrString;
    QVector<const unsigned char*> m_yellowDSBBuffer;
    QVector<const unsigned char*> m_yellowDSBBufferReverse;
    QVector<const unsigned char*> m_magentaDSBBuffer;
    QVector<const unsigned char*> m_magentaDSBBufferReverse;
    QVector<const unsigned char*> m_cyanDSBBuffer;
    QVector<const unsigned char*> m_cyanDSBBufferReverse;
    QVector<const unsigned char*> m_blackDSBBuffer;
    QVector<const unsigned char*> m_blackDSBBufferReverse;
    QByteArray m_blackDSBFileStartData;
    QByteArray m_yellowDSBFileStartData;
    QByteArray m_cyanDSBFileStartData;
    QByteArray m_magentaDSBFileStartData;
    QSet<unsigned int> m_transparentValues;///< 透明灰度值，默认为255
    unsigned int m_totalStitchCount;///< 记录总针数
    const int m_cmaxNeedleCountPerPix;///< 允许的图片每像素最大针数，图片是由4个CMYK通道组成，四个CMYK通道组成的一个像素的总针数不能超过此值
    //
#ifdef D_IMAGE2PATTERN_IS_ENABLE_LOGO
    bool m_addLogo;///< 是否显示logologo
    QByteArray m_logoDsbDatas;///< logo的文件数据
#endif

};

#endif // DIMAGEPATTERNCONVER_H
