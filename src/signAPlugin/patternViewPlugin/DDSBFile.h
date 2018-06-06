#ifndef DDSBFILE_H
#define DDSBFILE_H
#include "PatternViewGlobal.h"
#include <QFile>
#include <QList>
#include <QColor>
#include "DPatternDatas.h"
///
/// \brief 解析dsb文件
///
class D_PATTERN_VIEW_EXPORT DDSBFile : public QFile
{
    Q_OBJECT
public:
    DDSBFile(QObject* par = NULL);
    DDSBFile(const QString &name);
    //获取针迹
    void getPatternDatas(DPatternDatas& points);
    static void getPatternDatas(const QByteArray& buffer
                                , DPatternDatas& points
                                , const QList<QColor>& needleColorList = QList<QColor>()<<Qt::black);
    //dsb文件头的尺寸
    static unsigned int dsbFileHeaderSize();
    //预设换色，如果不设置，默认换色颜色为黑色
    QList<QColor> getNeedleColorList() const;
    void setNeedleColorList(const QList<QColor>& needleColorList);
    //读取针码，不包括结束符
    QByteArray readStitchDatas();
public:
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
private:
    QList<QColor> m_needleColorList;
};

#endif // DDSBFILE_H
