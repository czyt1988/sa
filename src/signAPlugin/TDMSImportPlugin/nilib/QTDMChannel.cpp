#include "QTDMChannel.h"
#include <memory>
#include <QDateTime>
#include <QVariant>
#include <vector>
//#pragma comment(lib, "user32.lib")
//#pragma comment(lib,"BufferOverflowU.lib")
QTDMChannel::QTDMChannel():m_channel(nullptr)
{

}

QTDMChannel::QTDMChannel(DDCChannelHandle handle)
    :m_channel(handle)
{
}

DDCChannelHandle QTDMChannel::getHandle() const
{
    return m_channel;
}

void QTDMChannel::setHandle(DDCChannelHandle handle)
{
    m_channel = handle;
}

///
/// \brief 获取通道的数据类型
/// \return
///
DDCDataType QTDMChannel::getDataType() const
{
    DDCDataType dataType;
    DDC_GetDataType(m_channel,&dataType);
    return dataType;
}

QString QTDMChannel::getName() const
{
    return getStringProperty(DDC_CHANNEL_NAME);
}

QString QTDMChannel::getDescription() const
{
    return getStringProperty(DDC_CHANNEL_DESCRIPTION);
}

QString QTDMChannel::getUnit() const
{
    return getStringProperty(DDC_CHANNEL_UNIT_STRING);
}

QString QTDMChannel::getDataTypeString() const
{
    return dataTypeToString(getDataType ());
}
///
/// \brief 获取数据个数
/// \return
///
unsigned long long QTDMChannel::getDataNums() const
{
    unsigned __int64 dataSize;
    DDC_GetNumDataValues(m_channel,&dataSize);
    return dataSize;
}
///
/// \brief QTDMChannel::getDoubleData
/// \param datas
/// \param firstIndex
/// \param length
/// \return 返回获取的正确性验证，如果不是DDC_NoError说明有错误发生
///
int QTDMChannel::getDoubleData(double* datas, size_t firstIndex, size_t length) const
{
    return DDC_GetDataValuesDouble(m_channel,firstIndex,length,datas);
}

int QTDMChannel::getFloatData(float* datas, size_t firstIndex, size_t length) const
{
    return DDC_GetDataValuesFloat (m_channel,firstIndex,length,datas);
}

int QTDMChannel::getStringData(QString* datas, size_t firstIndex, size_t length) const
{
    char **values = new char*[length];
    int res = DDC_GetDataValuesString(m_channel,firstIndex,length,values);
    if(res == DDC_NoError)
    {
        for(size_t i=0;i<length;++i)
        {
            datas[i] = QString::fromLocal8Bit(values[i]);
        }
        //释放lib自己创建的内存
        for(size_t i = 0; i < length; ++i)
        {
            DDC_FreeMemory (values[i]);
        }
        if(values)
            delete[] values;
    }
    return res;
}

int QTDMChannel::getLongData(long* datas, size_t firstIndex, size_t length) const
{
    return DDC_GetDataValuesInt32(m_channel,firstIndex,length,datas);
}

int QTDMChannel::getShortData(short* datas, size_t firstIndex, size_t length) const
{
    return DDC_GetDataValuesInt16(m_channel,firstIndex,length,datas);
}

int QTDMChannel::getUCharData(unsigned char* datas, size_t firstIndex, size_t length) const
{
    return DDC_GetDataValuesUInt8(m_channel,firstIndex,length,datas);
}

int QTDMChannel::getDateTimeData(QDateTime* datas, size_t firstIndex, size_t length) const
{
    int res;
    std::vector<unsigned int> year,month,day,hour,minute,second,weekDay;
    std::vector<double> milliSecond;
    res = DDC_GetDataValuesTimestampComponents(m_channel,firstIndex,length
                                                               ,year.data (),month.data (),day.data (),hour.data ()
                                                               ,minute.data (),second.data (),milliSecond.data (),weekDay.data ());
    for(size_t i=0;i<length;++i)
    {
        datas[i] = QDateTime(QDate(year[i],month[i],day[i])
                               ,QTime(hour[i],minute[i],second[i]));
    }
    return res;
}

#define GET_DATA(type,FunName)\
    std::vector<type> p;\
    p.resize (length);\
    r = FunName (p.data(),firstIndex,length);\
    array_to_qvariantarray(p.begin (),p.end (),datas);

int QTDMChannel::getVariantData(QVariant* datas, size_t firstIndex, size_t length) const
{
    DDCDataType type = getDataType();
    int r=0;
    switch(type)
    {
    case DDCDataType::DDC_Float:
    {
        GET_DATA(float,getFloatData);
    }
        break;
    case DDCDataType::DDC_Double:
    {
        GET_DATA(double,getDoubleData);
    }
        break;
    case DDCDataType::DDC_UInt8:
    {
        GET_DATA(unsigned char,getUCharData);
    }
        break;
    case DDCDataType::DDC_Int16:
    {
        GET_DATA(short,getShortData);
    }
        break;
    case DDCDataType::DDC_Int32:
    {
        GET_DATA(long,getLongData);
    }
        break;
    case DDCDataType::DDC_String:
    {
        GET_DATA(QString,getStringData);
    }
        break;
    case DDCDataType::DDC_Timestamp:
    {
        GET_DATA(QDateTime,getDateTimeData);
    }
        break;
    default:
        r = DDC_InvalidDataType;
    }
    return r;
}
///
/// \brief 获取单个数据
/// \param index
/// \return
///
QVariant QTDMChannel::getVariantData(size_t index) const
{
    DDCDataType type = getDataType();
    switch(type)
    {
    case DDCDataType::DDC_Float:
        {
            float d;
            DDC_GetDataValuesFloat(m_channel,index,1,&d);
            return d;
        }
    case DDCDataType::DDC_Double:
        {
            double d;
            DDC_GetDataValuesDouble(m_channel,index,1,&d);
            return d;
        }
    case DDCDataType::DDC_UInt8:
        {
            unsigned char d;
            DDC_GetDataValuesUInt8(m_channel,index,1,&d);
            return d;
        }
    case DDCDataType::DDC_Int16:
        {
            short d;
            DDC_GetDataValuesInt16(m_channel,index,1,&d);
            return d;
        }
    case DDCDataType::DDC_Int32:
        {
            long d;
            DDC_GetDataValuesInt32(m_channel,index,1,&d);
            return qlonglong(d);
        }
    case DDCDataType::DDC_String:
        {
            char **values = new char*[1];
            DDC_GetDataValuesString(m_channel,index,1,values);
            QString d;
            d = QString::fromLocal8Bit(values[0]);
            //释放lib自己创建的内存
            DDC_FreeMemory (values[0]);
            if(values)
                delete[] values;
            return d;
        }
    case DDCDataType::DDC_Timestamp:
        {
            QDateTime d;
            unsigned int year,month,day,hour,minute,second,weekDay;
            double milliSecond;
            DDC_GetDataValuesTimestampComponents(m_channel,index,1
                ,&year,&month,&day,&hour
                ,&minute,&second,&milliSecond,&weekDay);
            return QDateTime(QDate(year,month,day),QTime(hour,minute,second));
        }
    }
    return QVariant();
}

QString QTDMChannel::getStringProperty(const char* propertyName) const
{
    unsigned int length(0);
    //获取文件属性 - DDC_FILE_NAME
    int errCode = DDC_GetChannelStringPropertyLength (m_channel, propertyName, &length);
    char* property = nullptr;
    length = (8)*(length+1);
    property = new char[length];
    memset(property,0,sizeof(char)*length);
    errCode = DDC_GetChannelProperty (m_channel, propertyName, property, length);
    QString str = QString::fromLocal8Bit(property);
    if(property)
        delete[] property;
    return str;
}

QString QTDMChannel::dataTypeToString(const DDCDataType dataType)
{
    switch(dataType)
    {
    case DDCDataType::DDC_UInt8:
        return QStringLiteral("UInt8");
    case DDCDataType::DDC_Int16:
        return QStringLiteral("Int16");
    case DDCDataType::DDC_Int32:
        return QStringLiteral("Int32");
    case DDCDataType::DDC_Float:
        return QStringLiteral("Float");
    case DDCDataType::DDC_Double:
        return QStringLiteral("Double");
    case DDCDataType::DDC_String:
        return QStringLiteral("String");
    case DDCDataType::DDC_Timestamp:
        return QStringLiteral("Timestamp");
    }
    return QStringLiteral("UnKnow");
}

