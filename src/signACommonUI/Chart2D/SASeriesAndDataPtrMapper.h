#ifndef SASERIESANDDATAPTRMAPPER_H
#define SASERIESANDDATAPTRMAPPER_H
#include <QSet>
class SAAbstractDatas;
class SASeriesAndDataPtrMapper
{
public:
    SASeriesAndDataPtrMapper();
    void insertData(SAAbstractDatas* d);
    void removeLinkDataPtr(const SAAbstractDatas* ptr);
    //判断此曲线是否和某数据相关
    bool isContainData(SAAbstractDatas* d);
    //获取此曲线相关的数据
    const QSet<SAAbstractDatas *>& linkDatas() const;
    //获取此曲线相关的数据
    QSet<SAAbstractDatas *>& linkDatas();
    //
    void clearDataPtrLink();
private:
    QSet<SAAbstractDatas*> m_linkDatas;
};

#endif // SASERIESANDDATAPTRMAPPER_H
