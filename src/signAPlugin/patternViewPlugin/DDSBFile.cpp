#include "DDSBFile.h"
#define DSB_HEAD_SIZE (512)
DDSBFile::DDSBFile(QObject *par):QFile(par)
{

}

DDSBFile::DDSBFile(const QString &name):QFile(name)
{

}

void DDSBFile::getPatternDatas(DPatternDatas &points)
{
    if(m_needleColorList.isEmpty())
    {
        points.appendColor(Qt::black);
    }
    else
    {
        points.appendColor(m_needleColorList[0]);
    }
    seek(dsbFileHeaderSize());
    char byte1,byte2,byte3;
    unsigned int tmp = 1;
    int x=0,y=0;
    int dx=0,dy=0;
    int colorChangIndex = 1;
    while(!atEnd())
    {
        tmp = read(&byte1,1);
        if(1 != tmp)
        {
            continue;
        }
        tmp = read(&byte2,1);
        if(1 != tmp)
        {
            break;
        }
        tmp = read(&byte3,1);
        if(1 != tmp)
        {
            break;
        }
        dx = byte3;
        dy = byte2;
        if (((byte1)&0x20) == 0x20)
            dx = -dx;
        if (((byte1)&0x40) == 0x40)
            dy = -dy;
        x += dx;
        y += dy;
        if ((byte1 & 0x98) == 0x88)	//换色
        {
            if(colorChangIndex < m_needleColorList.size())
            {
                points.appendColor(m_needleColorList[colorChangIndex]);
            }
            else
            {
                points.appendColor(Qt::black);
            }
            ++colorChangIndex;
        }
        if(1 != byte1&0x1)
        {
            points.appendPoint(QPoint(x,y));
        }
    }
}

void DDSBFile::getPatternDatas(const QByteArray &buffer, DPatternDatas &points, const QList<QColor> &needleColorList)
{
    if(needleColorList.isEmpty())
    {
        points.appendColor(Qt::black);
    }
    else
    {
        points.appendColor(needleColorList[0]);
    }

    if(buffer.size() < dsbFileHeaderSize())
    {
        return;
    }
    const int size = buffer.size();
    int index = DSB_HEAD_SIZE;
    char byte1,byte2,byte3;
    int x=0,y=0;
    int dx=0,dy=0;
    int colorChangIndex = 1;
    while((index+2) < size)
    {
        byte1 = buffer[index];//(&byte1,1);
        ++index;
        byte2 = buffer[index];//(&byte1,1);
        ++index;
        byte3 = buffer[index];//(&byte1,1);
        ++index;
        dx = byte3;
        dy = byte2;
        if ((byte1&0x20) == 0x20)
            dx = -dx;
        if ((byte1&0x40) == 0x40)
            dy = -dy;
        x += dx;
        y += dy;
        if ((byte1 & 0x98) == 0x88)	//换色
        {
            if(colorChangIndex < needleColorList.size())
            {
                points.appendColor(needleColorList[colorChangIndex]);
                ++colorChangIndex;
            }
            else
            {
                points.appendColor(Qt::black);
            }
        }
        if(1 != byte1&0x1)
        {
            points.appendPoint(QPoint(x,y));
        }
    }
}

unsigned int DDSBFile::dsbFileHeaderSize()
{
    return DSB_HEAD_SIZE;
}

QList<QColor> DDSBFile::getNeedleColorList() const
{
    return m_needleColorList;
}

void DDSBFile::setNeedleColorList(const QList<QColor>& needleColorList)
{
    m_needleColorList = needleColorList;
}

///
/// \brief 读取一个dsb的帧数据，如果最后一帧是结束码会忽略
/// \return
///
QByteArray DDSBFile::readStitchDatas()
{
    if(!isOpen())
    {
        return QByteArray();
    }
    qint64 fileSize = size();
    qint64 readSize = fileSize - 512;
    //先看最后3字节是否是结束码
    char byte1;
    seek(fileSize-3);
    read(&byte1,1);
    if((unsigned char)byte1 == 0x98)
    {
        //是结束码，读取的尺寸减3
        readSize -= 3;
    }
    seek(512);
    return read(readSize);
}



///
/// \brief dsb文件写入向下移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DDSBFile::stitchMoveDown(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {224,distance,0};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief dsb文件写入向上移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DDSBFile::stitchMoveUp(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {128,distance,0};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief dsb文件写入向右移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DDSBFile::stitchMoveRight(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {128,0,distance};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief dsb文件写入向左移动的平针
/// \param dsbBuffer 文件内存
/// \param distance 移动距离
///
void DDSBFile::stitchMoveLeft(QByteArray &dsbBuffer, unsigned char distance)
{
    unsigned char buffer[3] = {224,0,distance};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 向下跳针
/// \param dsbBuffer
/// \param distance
///
void DDSBFile::stitchSkipDown(QByteArray &dsbBuffer, unsigned int distance)
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
void DDSBFile::stitchSkipUp(QByteArray &dsbBuffer, unsigned int distance)
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
void DDSBFile::stitchSkipRight(QByteArray &dsbBuffer, unsigned int distance)
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
void DDSBFile::stitchSkipLeft(QByteArray &dsbBuffer, unsigned int distance)
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
void DDSBFile::stitchEndCode(QByteArray &dsbBuffer)
{
    const unsigned char buffer[3] = {0x98,0,0};
    dsbBuffer.append((const char*)(buffer),3);
}
///
/// \brief 写入换色码
/// \param dsbBuffer
///
void DDSBFile::stitchChangColor(QByteArray &dsbBuffer)
{
    const unsigned char buffer[3] = {0x89,0,0};
    dsbBuffer.append((const char*)(buffer),3);
}
