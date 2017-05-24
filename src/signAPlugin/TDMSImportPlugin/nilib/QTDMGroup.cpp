#include "QTDMGroup.h"
#include <vector>
#include <algorithm>
QTDMGroup::QTDMGroup():m_group(nullptr)
  ,m_isAutoCloseChannel(true)
{
}

QTDMGroup::QTDMGroup(DDCChannelGroupHandle handle):m_group(nullptr)
    ,m_isAutoCloseChannel(true)
{
    setHandle (handle);
}

QTDMGroup::~QTDMGroup()
{
    if(m_isAutoCloseChannel)
    {
       closeChannel();
    }
}

DDCChannelGroupHandle QTDMGroup::getHandle() const
{
    return m_group;
}

void QTDMGroup::setHandle(DDCChannelGroupHandle handle)
{
    m_group = handle;
    getChannelHandlesInGroup();
}
///
/// \brief 获取通道数
/// \return
///
unsigned int QTDMGroup::getChannelNums() const
{
    unsigned int channelNums(0);
    DDC_GetNumChannels(m_group, &channelNums);//获取通道的数目
    return channelNums;
}

QString QTDMGroup::getName() const
{
    return getStringProperty(DDC_CHANNELGROUP_NAME);
}

QString QTDMGroup::getDescription() const
{
    return getStringProperty(DDC_CHANNELGROUP_DESCRIPTION);
}

QList<DDCChannelHandle> QTDMGroup::getChannelHandles() const
{
    return std::move(m_channelHandles);
}

QList<QTDMChannel*> QTDMGroup::getChannels() const
{
    QList<QTDMChannel*> channels;
    for(int i=0;i<m_channels.size ();++i)
    {
        channels.push_back (m_channels[i].get ());
    }
    return channels;
}

bool QTDMGroup::isAutoCloseChannel() const
{
    return m_isAutoCloseChannel;
}

void QTDMGroup::setAutoCloseChannel(bool enable)
{
    m_isAutoCloseChannel = enable;
}
///
/// \brief 关闭此组下的所有通道
///
void QTDMGroup::closeChannel()
{
    std::for_each(m_channelHandles.begin (),m_channelHandles.end ()
                  ,[](DDCChannelHandle c){
        DDC_CloseChannel (c);
    });
    m_channelHandles.clear ();
    m_channels.clear ();
}
///
/// \brief 重新获取通道
/// \param closeOldChannel 是否关闭原有的通道，如果关闭了，那么原有通道句柄将失效，
/// 如果原来的句柄在此类外使用，那么请把closeOldChannel设置为false
///
///
void QTDMGroup::reGetChannel(bool closeOldChannel)
{
    if(closeOldChannel)
        closeChannel ();
    getChannelHandlesInGroup();
}

QString QTDMGroup::getStringProperty(const char* propertyName) const
{
    unsigned int length(0);
    //获取文件属性
    int errCode = DDC_GetChannelGroupStringPropertyLength (m_group
        , propertyName
        , &length);
    char* property = nullptr;
    length = (8)*(length+1);
    property = new char[length];
    memset(property,0,sizeof(char)*length);
    errCode = DDC_GetChannelGroupProperty (m_group, propertyName, property, length);
    QString str = QString::fromLocal8Bit(property);
    if(property)
        delete[] property;
    return str;
}

void QTDMGroup::getChannelHandlesInGroup()
{
    m_channelHandles.clear ();
    m_channels.clear ();
    unsigned int channelNums = getChannelNums();//获取通道数
    if(channelNums <= 0)
        return;
    std::vector<DDCChannelHandle> arr_c(channelNums,nullptr);
    DDC_GetChannels(m_group,arr_c.data(),channelNums);
    for(unsigned int i = 0;i<arr_c.size();++i){
        m_channelHandles.append(arr_c[i]);
        m_channels.push_back (makeChannelPtr(arr_c[i]));
    }
}

QTDMGroup::QTDMChannelPtr QTDMGroup::makeChannelPtr(DDCChannelHandle handle)
{
    return std::make_shared<QTDMChannel>(handle);
}
