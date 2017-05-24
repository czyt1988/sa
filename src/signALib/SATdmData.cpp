#include "SATdmData.h"
#include <assert.h>
#include <algorithm>
#include <qwt_date.h>
#include <czyQArrayEx.h>
SATdmData::SATdmData():SAAbstractDatas(),m_file(nullptr),m_isAutoClose(true)
{
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATdmData::SATdmData(const QIcon& icon, DDCFileHandle fileHandle)
:SAAbstractDatas()
,m_isAutoClose(true)
{
    setTDMSFile(fileHandle);
    setText (getName());
    setIcon (icon);
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATdmData::SATdmData(DDCFileHandle fileHandle):SAAbstractDatas()
,m_isAutoClose(true)
{
    setTDMSFile(fileHandle);
    setText (getName());
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATdmData::~SATdmData()
{
    if(m_isAutoClose)
        closeFile();
}
///
/// \brief 目前还没实现
/// \param name
///
void SATdmData::setName(const QString& name)
{
    setText (name);
}


int SATdmData::getType() const
{
    return SA::TdmsFile;
}

int SATdmData::getSize(int sizeType) const
{
    Q_UNUSED(sizeType);
    return -1;
}

QVariant SATdmData::getAt(size_t index, size_t index2) const
{
    Q_UNUSED(index);
    Q_UNUSED(index2);
    return QVariant();
}

bool SATdmData::toDoubleVector(QVector<double>& data, const QVector<int>* index) const
{
    Q_UNUSED(data);Q_UNUSED(index);
    return false;
}

bool SATdmData::toPointFVector(QVector<QPointF>& data, const QVector<int>* index) const
{
    Q_UNUSED(data);Q_UNUSED(index);
    return false;
}

bool SATdmData::isCanCast2DoubleVector() const
{
    return false;
}

bool SATdmData::isCanCast2PointFVector() const
{
    return false;
}


QTDMFile* SATdmData::getTDMSFile()
{
    return &m_file;
}

void SATdmData::setTDMSFile(DDCFileHandle file)
{
    m_file.setHandle (file);
    iter_child();
}

void SATdmData::closeFile()
{
    m_file.closeFile ();
}

QString SATdmData::getName() const
{
    return m_file.getName ();
}

QString SATdmData::getDescription() const
{
    return m_file.getDescription ();
}

QString SATdmData::getTitle() const
{
    return m_file.getTitle ();
}

QString SATdmData::getAuthor() const
{
    return m_file.getAuthor ();
}

QList<QTDMGroup*> SATdmData::getGroups() const
{
    return m_file.getGroups ();
}

QList<SATdmGroup*> SATdmData::getGroupsItem() const
{
    QList<SATdmGroup*> groups;
    int groupCounts = rowCount ();
    for(int i=0;i<groupCounts;++i)
    {
        SATdmGroup* item = static_cast<SATdmGroup*>(child (i,0));
        groups.append (item);
    }
    return groups;
}

bool SATdmData::isFileOpen() const
{
    return m_file.isFileOpen ();
}

void SATdmData::setAutoClose(bool enable)
{
    m_isAutoClose = enable;
}

bool SATdmData::isAutoClose() const
{
    return m_isAutoClose;
}

void SATdmData::iter_child()
{
    QList<QTDMGroup*> groups = getGroups();
    for(int i=0;i<groups.size ();++i)
    {
        SATdmGroup* group = new SATdmGroup(groups[i]);
        appendRow (group);
    }
}


SATdmGroup::SATdmGroup():SAAbstractDatas()
{
    setData (SA::TdmsGroup,SA_ROLE_DATA_TYPE);
}

SATdmGroup::SATdmGroup(QTDMGroup* group):SAAbstractDatas()
{
    setGroup(group);
    setData (SA::TdmsGroup,SA_ROLE_DATA_TYPE);
}

SATdmGroup::SATdmGroup(const QIcon& icon, QTDMGroup* group)
    :SAAbstractDatas()
{
    setGroup(group);
    setIcon (icon);
    setData (SA::TdmsGroup,SA_ROLE_DATA_TYPE);
}

SATdmGroup::~SATdmGroup()
{

}

QVariant SATdmGroup::getAt(size_t index,size_t index2) const
{
    Q_UNUSED(index);
    Q_UNUSED(index2);
    return QVariant();
}

bool SATdmGroup::toDoubleVector(QVector<double>& data, const QVector<int>* index) const
{
    Q_UNUSED(data);Q_UNUSED(index);
    return false;
}

bool SATdmGroup::toPointFVector(QVector<QPointF>& data, const QVector<int>* index) const
{
    Q_UNUSED(data);Q_UNUSED(index);
    return false;
}

bool SATdmGroup::isCanCast2DoubleVector() const
{
    return false;
}

bool SATdmGroup::isCanCast2PointFVector() const
{
    return false;
}

void SATdmGroup::setGroup(QTDMGroup* group)
{
    m_group = group;
    setText (group->getName ());
    QList<QTDMChannel*> channel = group->getChannels ();
    for(int i=0;i<channel.size ();++i)
    {
        SATdmChannel* item = new SATdmChannel(channel[i]);
        appendRow (item);
    }
}

size_t SATdmGroup::getChannelNums() const
{
    return m_group->getChannelNums ();
}

QTDMGroup* SATdmGroup::getGroup() const
{
    return m_group;
}

QList<QTDMChannel*> SATdmGroup::getChannels() const
{
    return m_group->getChannels ();
}

QList<SATdmChannel*> SATdmGroup::getChannelsItem() const
{
    QList<SATdmChannel*> channels;
    int channelsCounts = rowCount ();
    for(int i=0;i<channelsCounts;++i)
    {
        SATdmChannel* item = static_cast<SATdmChannel*>(child (i,0));
        channels.append (item);
    }
    return channels;
}

QString SATdmGroup::getDescription() const
{
    return m_group->getDescription ();
}

QString SATdmGroup::getGroupName() const
{
    return m_group->getName ();
}

QString SATdmGroup::getName() const
{
    return m_group->getName ();
}


SATdmChannel::SATdmChannel():SAAbstractDatas()
{
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATdmChannel::SATdmChannel(QTDMChannel* channel):SAAbstractDatas()
{
    setChannel(channel);
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATdmChannel::SATdmChannel(const QIcon& icon, QTDMChannel* channel):SAAbstractDatas()
{
    setChannel(channel);
    setIcon (icon);
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATdmChannel::~SATdmChannel()
{

}

QString SATdmChannel::getTypeName() const
{
    return QString("%1 tdm/tdms channel").arg(getDataTypeString());
}

QVariant SATdmChannel::getAt(size_t index,size_t index2) const
{
    Q_UNUSED(index2);
    return m_channel->getVariantData(index);
}

//TODO 在选择数量比较少时的获取方法
bool SATdmChannel::toDoubleVector(QVector<double>& data, const QVector<int>* index) const
{
    if(!index)
        return getDoubles(data,true);
    QVector<double> temp;
    bool isSussGet = getDoubles(temp,true);
    if(!isSussGet)
        return false;
    auto end = index->end ();
    data.reserve (index->size ());
    for(auto i=index->begin ();i!=end;++i)
    {
        data.push_back (temp[(*i)]);
    }
    return true;
}

bool SATdmChannel::toPointFVector(QVector<QPointF>& data, const QVector<int>* index) const
{
    Q_UNUSED(data);Q_UNUSED(index);
    return false;
}

bool SATdmChannel::isCanCast2DoubleVector() const
{
    int type =  getDataType ();
    if(type == DDC_Float || type == DDC_Double
       || type == DDC_UInt8 || type == DDC_Int16 || type == DDC_Int32
       || type == DDC_Timestamp)
    {
        return true;
    }
    if(type == DDC_String)
    {
        size_t size = getDataNums ();
        int r=DDC_NoError;
        QVector<QString> t;
        t.resize (size);
        r = m_channel->getStringData (t.data (),0,size);
        bool successCast;

        for(size_t i=0;i<size;++i)
        {
            t[i].toDouble (&successCast);
            if(successCast)
                return true;
        }
    }
    return false;
}

bool SATdmChannel::isCanCast2PointFVector() const
{
    return false;
}

int SATdmChannel::getSize(int sizeType) const
{
    Q_UNUSED(sizeType);
    return m_channel->getDataNums ();
}

void SATdmChannel::setChannel(QTDMChannel* channel)
{
    m_channel = channel;
    setText (channel->getName ());
}

QTDMChannel* SATdmChannel::getChannel() const
{
    return m_channel;
}

DDCChannelHandle SATdmChannel::getChannelHandle() const
{
    return getChannel()->getHandle ();
}
///
/// \brief 获取通道的类型
/// \return
///
DDCDataType SATdmChannel::getDataType() const
{
    return m_channel->getDataType ();
}

#define GET_CAST_DATA(type,typeBeCast,FunName)\
    QVector<type> t;\
    t.resize (size);\
    r = m_channel->FunName (t.data (),0,size);\
    czy::Array::transform_cast_type<type,typeBeCast>(t.begin (),t.end (),outputDatas.begin ());

bool SATdmChannel::getDoubles(QVector<double>& outputDatas, bool ifCast) const
{
    DDCDataType type = getDataType ();
    size_t size = getDataNums ();
    outputDatas.resize (size);
    int r=DDC_NoError;
    if(DDC_Double == type)
    {
        r = m_channel->getDoubleData (outputDatas.data (),0,size);
        return DDC_NoError == r;
    }
    if(!ifCast)
        return false;
    switch (type) {
    case DDC_Float:
    {
        GET_CAST_DATA(float,double,getFloatData);
    }
        break;
    case DDC_UInt8:
    {
        GET_CAST_DATA(unsigned char,double,getUCharData);
    }
       break;
    case DDC_Int16:
    {
        GET_CAST_DATA(short,double,getShortData);
    }
       break;
    case DDC_Int32:
    {
        GET_CAST_DATA(long,double,getLongData);
    }
       break;
    case DDC_String:
    {
        QVector<QString> t;
        t.resize (size);
        r = m_channel->getStringData (t.data (),0,size);
        std::transform(t.begin (),t.end (),outputDatas.begin (),[](const QString& str)->double{
            bool successCast;
            double d = str.toDouble (&successCast);
            if(successCast)
                return d;
            return 0;
        });
    }
       break;
    case DDC_Timestamp:
    {
        QVector<QDateTime> t;
        t.resize (size);
        r = m_channel->getDateTimeData (t.data (),0,size);
        std::transform(t.begin (),t.end (),outputDatas.begin (),[](const QDateTime& time)->double{
            return QwtDate::toDouble (time);
        });
    }
        break;
    default:
        return false;
    }
    return DDC_NoError == r;
}
#define GET_CAST_STRING_DATA(type,FunName)\
    QVector<type> t;\
    t.resize (size);\
    r = m_channel->FunName (t.data (),0,size);\
    std::transform(t.begin (),t.end (),outputDatas.begin (),[](const type& d)->QString{\
        return QString::number (d);\
     });

bool SATdmChannel::getStrings(QVector<QString>& outputDatas,bool ifCast) const
{
    DDCDataType type = getDataType ();
    size_t size = getDataNums ();
    outputDatas.resize (size);
    int r=DDC_NoError;
    if(DDC_String == type)
    {
         r = m_channel->getStringData (outputDatas.data (),0,size);
        return DDC_NoError == r;
    }
    if(!ifCast)
        return false;
    switch (type) {
    case DDC_Double:
    {
        GET_CAST_STRING_DATA(double,getDoubleData);
    }
        break;
    case DDC_Float:
    {
        GET_CAST_STRING_DATA(float,getFloatData);
    }
        break;
    case DDC_UInt8:
    {
        GET_CAST_STRING_DATA(unsigned char,getUCharData);
    }
        break;
    case DDC_Int16:
    {
        GET_CAST_STRING_DATA(short,getShortData);
    }
        break;
    case DDC_Int32:
    {
        GET_CAST_STRING_DATA(long,getLongData);
    }
        break;
    case DDC_Timestamp:
    {
        QVector<QDateTime> t;
        t.resize (size);
        r = m_channel->getDateTimeData (t.data (),0,size);
        std::transform(t.begin (),t.end (),outputDatas.begin (),[](const QDateTime& d)->QString{
            return d.toString();
         });\
    }
        break;
    default:
        return false;
    }
    return DDC_NoError == r;
}

bool SATdmChannel::getStringFromDateTimeType(QVector<QString>& outputDatas, const QString& format) const
{
    DDCDataType type = getDataType ();
    size_t size = getDataNums ();
    outputDatas.resize (size);
    int r=DDC_NoError;
    if(DDC_Timestamp == type)
    {
        QVector<QDateTime> t;
        t.resize (size);
        r = m_channel->getDateTimeData (t.data (),0,size);
        std::transform(t.begin (),t.end (),outputDatas.begin (),[&](const QDateTime& d)->QString{
            return d.toString(format);
         });\
        return DDC_NoError == r;
    }
    return false;
}

#define GET_CAST_TIME_T_DATA(type,FunName)\
    QVector<type> t;\
    t.resize (size);\
    r = m_channel->FunName (t.data (),0,size);\
    std::transform(t.begin (),t.end (),outputDatas.begin (),[](const type& d)->QDateTime{\
        return QDateTime::fromTime_t (uint(d));\
    });

bool SATdmChannel::getDateTimes(QVector<QDateTime>& outputDatas, bool ifCast) const
{
    DDCDataType type = getDataType ();
    size_t size = getDataNums ();
    outputDatas.resize (size);
    int r=DDC_NoError;
    if(DDC_Timestamp == type)
    {
         r = m_channel->getDateTimeData (outputDatas.data (),0,size);
        return DDC_NoError == r;
    }
    if(!ifCast)
        return false;
    switch (type) {
    case DDC_Double:
    {
        QVector<double> t;
        t.resize (size);
        r = m_channel->getDoubleData (t.data (),0,size);
        std::transform(t.begin (),t.end (),outputDatas.begin (),[](const double& d)->QDateTime{
            return QwtDate::toDateTime (d);
        });
    }
        break;
    case DDC_Float:
    {
        QVector<float> t;
        t.resize (size);
        r = m_channel->getFloatData (t.data (),0,size);
        std::transform(t.begin (),t.end (),outputDatas.begin (),[](const float& d)->QDateTime{
            return QwtDate::toDateTime (double(d));
        });
    }
        break;
    case DDC_UInt8:
    {
        GET_CAST_TIME_T_DATA(unsigned char,getUCharData);
    }
        break;
    case DDC_Int16:
    {
        GET_CAST_TIME_T_DATA(short,getShortData);
    }
        break;
    case DDC_Int32:
    {
        GET_CAST_TIME_T_DATA(long,getLongData);
    }
        break;
    default:
        return false;
    }
    return DDC_NoError == r;
}
///
/// \brief 获取数据
/// \param outputDatas
///
void SATdmChannel::getVariants(QVector<QVariant>& outputDatas) const
{
    size_t size = getDataNums ();
    outputDatas.resize (size);
    int r = m_channel->getVariantData (outputDatas.data (),0,size);
    if(r != DDC_NoError)
    {
        outputDatas.resize (0);
    }
}
///
/// \brief 通道的描述
/// \return
///
QString SATdmChannel::getDescription() const
{
    return m_channel->getDescription ();
}

QString SATdmChannel::getDataTypeString() const
{
    return m_channel->getDataTypeString ();
}

QString SATdmChannel::getUnit() const
{
    return m_channel->getUnit ();
}

QString SATdmChannel::getChannelName() const
{
    return m_channel->getName ();
}

QString SATdmChannel::getName() const
{
    return m_channel->getName ();
}

size_t SATdmChannel::getDataNums() const
{
    return m_channel->getDataNums ();
}
