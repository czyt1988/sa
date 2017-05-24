#ifndef DATAFEATUREITEM_H
#define DATAFEATUREITEM_H
#include <QStandardItem>
#include <QVector>
#include "qwt_plot_curve.h"
#include "SACommonUIGlobal.h"
class SAChart2D;
///
/// \brief DataFeatureItem接口
///
class SA_COMMON_UI_EXPORT AbstractDataFeatureItem : public QStandardItem
{
public:
    AbstractDataFeatureItem();
    AbstractDataFeatureItem(const QString & text);
    AbstractDataFeatureItem(const QIcon & icon, const QString & text);
    virtual ~AbstractDataFeatureItem();
    virtual int rtti() const = 0;
    enum{
        UnKnow
        ,TopPlotItem///<
        ,DescribeItem///< 用于描述的条目，和值项目关联
        ,ValueItem///< 值项目
        ,PointItem///< 点项目
        ,VectorValueItem///< 值系列项目
        ,VectorPointItem///< 点系列项目项目
        ,DataFeature///用于保存信息的顶层条目
        ,CurveDataFeatureItem///<针对线性的默认条目
    };
    QVariant displayRole() const;
    QIcon icon() const;
    //获取最顶层条目
    AbstractDataFeatureItem* topParent() const;
};

///
/// \brief 顶层的plot相关信息的item，用当前plot的title作为描述
///
class SA_COMMON_UI_EXPORT DataFeaturePlotInfoItem : public AbstractDataFeatureItem
{
public:
    DataFeaturePlotInfoItem();
    DataFeaturePlotInfoItem(const QString & text);
    DataFeaturePlotInfoItem(const QIcon & icon, const QString & text);
    virtual int rtti() const{return AbstractDataFeatureItem::TopPlotItem;}
    QWidget* chartWidget() const{return m_chart;}
    void setChartWidget(QWidget* chart);
private:
    QWidget* m_chart;
};

///
/// \brief 保存描述信息
///
class SA_COMMON_UI_EXPORT DataFeatureDescribeItem : public AbstractDataFeatureItem
{
public:
    DataFeatureDescribeItem();
    DataFeatureDescribeItem(const QString & text);
    DataFeatureDescribeItem(const QIcon & icon, const QString & text);
    virtual int rtti() const{return AbstractDataFeatureItem::DescribeItem;}
};

///
/// \brief 保存值信息
///
class SA_COMMON_UI_EXPORT DataFeatureValueItem : public AbstractDataFeatureItem
{
public:
    DataFeatureValueItem();
    DataFeatureValueItem(double data);
    virtual int rtti() const{return AbstractDataFeatureItem::ValueItem;}
    void setValueData(double data);
    double getValueData() const;
    DataFeatureDescribeItem* getDescribeItem() const;
private:
    double m_data;
};

///
/// \brief 保存点信息
///
class SA_COMMON_UI_EXPORT DataFeaturePointItem : public AbstractDataFeatureItem
{
public:
    DataFeaturePointItem();
    DataFeaturePointItem(const QPointF& data);
    virtual int rtti() const{return AbstractDataFeatureItem::PointItem;}
    void setPointData(const QPointF& data);
    QPointF getPointData() const;
    static QString toString(const QPointF& data);
private:
    QPointF m_data;
};

///
/// \brief 保存值系列信息
///
class SA_COMMON_UI_EXPORT DataFeatureVectorValueItem : public AbstractDataFeatureItem
{
public:
    DataFeatureVectorValueItem();
    DataFeatureVectorValueItem(const QString& name);
    DataFeatureVectorValueItem(const QIcon & icon, const QString & name);
    virtual int rtti() const{return AbstractDataFeatureItem::VectorValueItem;}
    void setVectorValueDatas(const QVector<double>& datas,const QStringList& des = QStringList());
    void getVectorValueDatas(QVector<double>& datas) const;
    void append(double data,const QString& des = QString());
    void clear();
    double getAtData(int index) const;
    DataFeatureValueItem* getAtItem(int index) const;
private:
    QVector<double> m_datas;
};

///
/// \brief 保存点系列信息
///
class SA_COMMON_UI_EXPORT DataFeatureVectorPointItem : public AbstractDataFeatureItem
{
public:
    DataFeatureVectorPointItem();
    DataFeatureVectorPointItem(const QString& name);
    DataFeatureVectorPointItem(const QIcon & icon, const QString & name);
    virtual int rtti() const{return AbstractDataFeatureItem::VectorPointItem;}
    void setVectorPointDatas(const QVector<QPointF>& datas,const QStringList& des = QStringList());
    void getVectorPointDatas(QVector<QPointF>& datas) const;
    void append(QPointF data,const QString& des = QString());
    void clear();
    QPointF getAtData(int index) const;
    DataFeaturePointItem* getAtItem(int index) const;
private:
    QVector<QPointF> m_datas;
};


///
/// \brief 保存点信息
///
class SA_COMMON_UI_EXPORT DataFeatureItem : public AbstractDataFeatureItem
{
public:
    DataFeatureItem();
    DataFeatureItem(const QString& name);
    DataFeatureItem(const QString& name,const QIcon& icon);
    virtual ~DataFeatureItem();
    virtual int rtti() const{return AbstractDataFeatureItem::DataFeature;}
    //添加值信息
    void addValueItem(DataFeatureValueItem* item,const QString& name,const QIcon & icon = QIcon());
    void addValueItem(double data,const QString& name,const QIcon & icon = QIcon());
    //添加点信息
    void addPointItem(DataFeaturePointItem* item,const QString& name,const QIcon & icon = QIcon());
    void addPointItem(const QPointF& data,const QString& name,const QIcon & icon = QIcon());
    //添加序列值信息
    void addVectorValueItem(DataFeatureVectorValueItem* item);
    void addVectorValueItem(QVector<double>& datas,const QString& name,const QStringList& des = QStringList());
    //添加序列点信息
    void addVectorPointItem(DataFeatureVectorPointItem* item);
    void addVectorPointItem(QVector<QPointF>& datas,const QString& name,const QStringList& des = QStringList());
    virtual QwtPlotItem *plotItem() const;
private:
    void addItemWithDescribe(AbstractDataFeatureItem* item,const QString& name,const QIcon & icon = QIcon());
};

///
/// \brief 自动解析QwtPlotCurve的DataFeature
///
class SA_COMMON_UI_EXPORT CurveDataFeatureItem : public DataFeatureItem
{
public:
    CurveDataFeatureItem();
    CurveDataFeatureItem(QwtPlotCurve* cur);
    virtual int rtti() const{return AbstractDataFeatureItem::CurveDataFeatureItem;}
    void setCurvePtr(QwtPlotCurve* cur);
    static void getCurData(QwtPlotCurve* cur,QVector<QPointF>& datas);
    static void getVectorPointY(const QVector<QPointF>& points, QVector<double>& ys);
    void clear();
    void setSortCount(size_t n);
    size_t getSortCount() const;
    virtual QwtPlotItem *plotItem() const;
    QwtPlotCurve* plotCurvePtr() const;
private:
    void analysisData();
private:
    QwtPlotCurve* m_cur;
    size_t m_sortCount;
};

#endif // DATAFEATUREITEM_H
