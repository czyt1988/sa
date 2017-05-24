#ifndef QTDMGROUP_H
#define QTDMGROUP_H
#include "nilibddc.h"
#include <memory>
#include <QString>
#include <QList>
#include <iterator>
#include "QTDMChannel.h"
#include "SALibGlobal.h"
class SALIB_EXPORT QTDMGroup
{
public:
    QTDMGroup();
    QTDMGroup(DDCChannelGroupHandle handle);
    ~QTDMGroup();
    DDCChannelGroupHandle getHandle() const;
    void setHandle(DDCChannelGroupHandle handle);

    unsigned int getChannelNums() const;
    /// {@
    /// 数据属性操作
    QString getName() const;
    QString getDescription() const;
    /// }@
    QList<DDCChannelHandle> getChannelHandles() const;
    QList<QTDMChannel*> getChannels() const;

    bool isAutoCloseChannel() const;
    void setAutoCloseChannel(bool enable=false);
    void closeChannel();
    void reGetChannel(bool closeOldChannel = true);
private:
    typedef std::shared_ptr<QTDMChannel> QTDMChannelPtr;
    QString getStringProperty(const char* propertyName) const;
    void getChannelHandlesInGroup();
    QTDMChannelPtr makeChannelPtr(DDCChannelHandle handle);
private:
    DDCChannelGroupHandle m_group;
    QList<DDCChannelHandle> m_channelHandles;
    QList<QTDMChannelPtr> m_channels;
    bool m_isAutoCloseChannel;
};

#endif // QTDMGROUP_H
