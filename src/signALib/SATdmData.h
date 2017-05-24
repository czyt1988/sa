#ifndef SATDMDATA_H
#define SATDMDATA_H

#include <memory>
#include "SALibGlobal.h"
#include "SAData.h"
#include "QTDMFile.h"
#include "QTDMGroup.h"
#include "QTDMChannel.h"

class SATdmGroup;
class SATdmChannel;
class SALIB_EXPORT SATdmData : public SAAbstractDatas
{
public:
    SATdmData();
    SATdmData(const QIcon & icon,DDCFileHandle fileHandle);
    SATdmData(DDCFileHandle fileHandle);
    virtual ~SATdmData();
    virtual void setName(const QString& name);
    virtual int getType() const;
    virtual QString getTypeName() const{return QString("tdm/tdms file");}
    virtual int getSize(int sizeType=SA_SIZE_TYPE_ROW) const;
    virtual QVariant getAt(size_t index,size_t index2=0) const;
    virtual bool toDoubleVector(QVector<double>& data,const QVector<int>* index=nullptr) const;
    virtual bool toPointFVector(QVector<QPointF>& data,const QVector<int>* index=nullptr) const;
    virtual bool isCanCast2DoubleVector() const;
    virtual bool isCanCast2PointFVector() const;
    QTDMFile* getTDMSFile();
    void setTDMSFile(DDCFileHandle file);
    void closeFile();

    /// {@ 文件属性操作
    QString getName() const;
    QString getDescription() const;
    QString getTitle() const;
    QString getAuthor() const;
    /// }@

    /// {@ 组操作
    QList<QTDMGroup*> getGroups() const;
    QList<SATdmGroup*> getGroupsItem() const;
    /// }@

    bool isFileOpen() const;
    void setAutoClose(bool enable);
    bool isAutoClose() const;

private:
    void iter_child();
private:
    QTDMFile m_file;
    //DDCFileHandle m_file;
    bool m_isAutoClose;
};

class SATdmGroup : public SAAbstractDatas
{
public:
    SATdmGroup();
    SATdmGroup(QTDMGroup* group);
    SATdmGroup(const QIcon & icon,QTDMGroup* group);
    virtual ~SATdmGroup();
    virtual int getType() const{return SA::TdmsGroup;}
    virtual QString getTypeName() const{return QString("tdm/tdms group");}
    virtual int getSize(int sizeType=SA_SIZE_TYPE_ROW) const{Q_UNUSED(sizeType);return -1;}
    virtual QVariant getAt(size_t index,size_t index2=0) const;
    virtual bool toDoubleVector(QVector<double>& data,const QVector<int>* index=nullptr) const;
    virtual bool toPointFVector(QVector<QPointF>& data,const QVector<int>* index=nullptr) const;
    virtual bool isCanCast2DoubleVector() const;
    virtual bool isCanCast2PointFVector() const;
    void setGroup(QTDMGroup* group);
    size_t getChannelNums() const;
    QTDMGroup* getGroup() const;
    QList<QTDMChannel*> getChannels() const;
    QList<SATdmChannel*> getChannelsItem() const;

    QString getDescription() const;
    QString getGroupName() const;
    QString getName() const;
private:
    QTDMGroup* m_group;
};

class SATdmChannel : public SAAbstractDatas
{
public:
    SATdmChannel();
    SATdmChannel(QTDMChannel* channel);
    SATdmChannel(const QIcon & icon,QTDMChannel* channel);
    virtual ~SATdmChannel();
    virtual int getType() const{return SA::TdmsChannel;}
    virtual QString getTypeName() const;
    virtual QVariant getAt(size_t index,size_t index2=0) const;
    virtual bool toDoubleVector(QVector<double>& data,const QVector<int>* index=nullptr) const;
    virtual bool toPointFVector(QVector<QPointF>& data,const QVector<int>* index=nullptr) const;
    virtual bool isCanCast2DoubleVector() const;
    virtual bool isCanCast2PointFVector() const;
    ///
    /// \brief 获取数据尺寸
    /// \return
    ///
    virtual int getSize(int sizeType=SA_SIZE_TYPE_ROW) const;
    ///{@ 句柄操作
    void setChannel(QTDMChannel* channel);
    QTDMChannel* getChannel() const;
    DDCChannelHandle getChannelHandle() const;
    ///}@

    ///{@ 数据操作
    DDCDataType getDataType() const;

    void getVariants(QVector<QVariant>& outputDatas) const;
    bool getDoubles(QVector<double>& outputDatas,bool ifCast = true) const;
    bool getStrings(QVector<QString>& outputDatas,bool ifCast = true) const;
    bool getStringFromDateTimeType(QVector<QString>& outputDatas,const QString & format) const;
    bool getDateTimes(QVector<QDateTime>& outputDatas,bool ifCast = true) const;
    ///}@

    ///{@ 数据属性操作
    QString getDescription() const;
    QString getDataTypeString() const;
    QString getUnit() const;
    QString getChannelName() const;
    QString getName() const;
    ///}@
    size_t getDataNums() const;
private:
    QTDMChannel* m_channel;
};
#endif // SATDMDATA_H
