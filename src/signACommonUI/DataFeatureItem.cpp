#include "DataFeatureItem.h"
#include <QWidget>
#include <QApplication>
#include <algorithm>
#include <czyMath.h>
#include "SAChart2D.h"
#define STR_WDF_PointCounts QObject::tr("number of data")//数据点数
#define STR_WDF_min_value QObject::tr("minimum value")//最小值
#define STR_WDF_max_value QObject::tr("maximum value")//最大值
#define STR_WDF_mid_value QObject::tr("mid-value")//中值
#define STR_WDF_min_point QObject::tr("minimum point")//最小点
#define STR_WDF_max_point QObject::tr("maximum point")//最大点
#define STR_WDF_mid_point QObject::tr("mid-point")//中值点
#define STR_WDF_PeekPeekValue QObject::tr("peak-to-peak value")//峰峰值
#define STR_WDF_Mean QObject::tr("mean value")//均值
#define STR_WDF_Sum QObject::tr("sum")//和
#define STR_WDF_Var QObject::tr("variance")//方差
#define STR_WDF_StdVar QObject::tr("standard deviation")//标准差
#define STR_WDF_Skewness QObject::tr("斜度")//斜度
#define STR_WDF_Kurtosis QObject::tr("kurtosis")//峭度
#define STR_WDF_Top QObject::tr("top n")//最大值序列
#define STR_WDF_Low QObject::tr("bottom n")//最小值序列
#define STR_WDF_SharpPeakTop QObject::tr("maximum peak value")//最大尖峰值
#define STR_WDF_SharpPeakLow QObject::tr("minimum peak value")//最小尖峰值
#define STR_WDF_TEMP_ORDER QObject::tr("order")


AbstractDataFeatureItem::AbstractDataFeatureItem():QStandardItem()
{

}

AbstractDataFeatureItem::AbstractDataFeatureItem(const QString &text):QStandardItem(text)
{

}

AbstractDataFeatureItem::AbstractDataFeatureItem(const QIcon &icon, const QString &text):QStandardItem(icon,text)
{

}

AbstractDataFeatureItem::~AbstractDataFeatureItem()
{

}

QVariant AbstractDataFeatureItem::displayRole() const
{
    return data(Qt::DisplayRole);
}

QIcon AbstractDataFeatureItem::icon() const
{
    return data(Qt::DecorationRole).value<QIcon>();
}
///
/// \brief 获取最顶层条目
/// \return
/// \note 需要保证AbstractDataFeatureItem的子对象都是基于AbstractDataFeatureItem的子类
///
AbstractDataFeatureItem *AbstractDataFeatureItem::topParent() const
{
    AbstractDataFeatureItem* cur = const_cast<AbstractDataFeatureItem *>(this);
    AbstractDataFeatureItem* par = static_cast<AbstractDataFeatureItem *>(cur->parent());
    while(par != nullptr)
    {
        cur = par;
        par = static_cast<AbstractDataFeatureItem *>(cur->parent());
    }
    return cur;
}

//===================================================================

DataFeaturePlotInfoItem::DataFeaturePlotInfoItem():AbstractDataFeatureItem()
  ,m_chart(nullptr)
{

}
DataFeaturePlotInfoItem::DataFeaturePlotInfoItem(const QString &text):AbstractDataFeatureItem(text)
  ,m_chart(nullptr)
{

}

DataFeaturePlotInfoItem::DataFeaturePlotInfoItem(const QIcon &icon, const QString &text)
:AbstractDataFeatureItem(icon,text)
,m_chart(nullptr)
{

}

void DataFeaturePlotInfoItem::setChartWidget(QWidget *chart)
{
    m_chart = chart;
    SAChart2D* chart2d = qobject_cast<SAChart2D*>(chart);
    if(chart2d)
    {
        QString str = chart2d->title().text();
        if(str.isEmpty())
        {
            str = QApplication::translate("DataFeaturePlotInfoItem", "chart", 0);
        }
        setText(str);
    }
}
//===================================================================

DataFeatureDescribeItem::DataFeatureDescribeItem():AbstractDataFeatureItem()
{

}

DataFeatureDescribeItem::DataFeatureDescribeItem(const QString &text):AbstractDataFeatureItem(text)
{

}

DataFeatureDescribeItem::DataFeatureDescribeItem(const QIcon &icon, const QString &text)
:AbstractDataFeatureItem(icon,text)
{
    
}



DataFeatureValueItem::DataFeatureValueItem():AbstractDataFeatureItem()
{

}

DataFeatureValueItem::DataFeatureValueItem(double data)
    :AbstractDataFeatureItem(QString::number(data))
    ,m_data(data)
{

}

///
/// \brief 设置值
/// \param data
///
void DataFeatureValueItem::setValueData(double data)
{
    setData(data,Qt::DisplayRole);
    m_data = data;
}

double DataFeatureValueItem::getValueData() const
{
    return m_data;
}
///
/// \brief 获取左边的描述信息，如果没有，返回nullptr
/// \return
///
DataFeatureDescribeItem *DataFeatureValueItem::getDescribeItem() const
{
    int cl = column();
    if(0 == cl)
        return nullptr;
    QStandardItem* par = parent();
    if(!par)
        return nullptr;
    int r = row();

    QStandardItem* item = par->child(r,0);
    if(!item)
        return nullptr;
    return dynamic_cast<DataFeatureDescribeItem *>(item);
}

DataFeaturePointItem::DataFeaturePointItem():AbstractDataFeatureItem()
{

}

DataFeaturePointItem::DataFeaturePointItem(const QPointF &data)
    :AbstractDataFeatureItem(toString(data))
    ,m_data(data)
{

}

void DataFeaturePointItem::setPointData(const QPointF &data)
{
    setData(toString(data),Qt::DisplayRole);
    m_data = data;
}

QPointF DataFeaturePointItem::getPointData() const
{
    return m_data;
}

QString DataFeaturePointItem::toString(const QPointF &data)
{
    return QString("(%1,%2)").arg(data.x()).arg(data.y());
}

DataFeatureVectorValueItem::DataFeatureVectorValueItem():AbstractDataFeatureItem()
{

}

DataFeatureVectorValueItem::DataFeatureVectorValueItem(const QString &name):AbstractDataFeatureItem(name)
{

}

DataFeatureVectorValueItem::DataFeatureVectorValueItem(const QIcon &icon, const QString &name)
:AbstractDataFeatureItem(icon,name)
{

}

void DataFeatureVectorValueItem::setVectorValueDatas(const QVector<double> &datas,const QStringList& des)
{
    m_datas = datas;
    DataFeatureDescribeItem* item1 = nullptr;
    DataFeatureValueItem* item2 = nullptr;
    for(int i=0;i<datas.size();++i)
    {
        item1 = new DataFeatureDescribeItem((i < des.size()) ? des[i] : "");
        item2 = new DataFeatureValueItem(datas[i]);
        appendRow(QList<QStandardItem*>()<<item1<<item2);
    }
}

void DataFeatureVectorValueItem::getVectorValueDatas(QVector<double> &datas) const
{
    datas = m_datas;
}

void DataFeatureVectorValueItem::append(double data,const QString& des)
{
    m_datas.append(data);
    DataFeatureDescribeItem* item1 = new DataFeatureDescribeItem(des);
    DataFeatureValueItem* item2 = new DataFeatureValueItem(data);
    appendRow(QList<QStandardItem*>()<<item1<<item2);
}

void DataFeatureVectorValueItem::clear()
{
    m_datas.clear();
    removeRows(0,rowCount());
}

double DataFeatureVectorValueItem::getAtData(int index) const
{
    return m_datas[index];
}

DataFeatureValueItem *DataFeatureVectorValueItem::getAtItem(int index) const
{
    return static_cast<DataFeatureValueItem *>(child(index));
}

DataFeatureVectorPointItem::DataFeatureVectorPointItem():AbstractDataFeatureItem()
{

}

DataFeatureVectorPointItem::DataFeatureVectorPointItem(const QString &name):AbstractDataFeatureItem(name)
{

}

DataFeatureVectorPointItem::DataFeatureVectorPointItem(const QIcon &icon, const QString &name)
:AbstractDataFeatureItem(icon,name)
{

}

void DataFeatureVectorPointItem::setVectorPointDatas(const QVector<QPointF> &datas, const QStringList &des)
{
    m_datas = datas;
    DataFeatureDescribeItem* item1 = nullptr;
    DataFeaturePointItem* item2 = nullptr;
    for(int i=0;i<datas.size();++i)
    {
        item1 = new DataFeatureDescribeItem((i < des.size()) ? des[i] : "");
        item2 = new DataFeaturePointItem(datas[i]);
        appendRow(QList<QStandardItem*>()<<item1<<item2);
    }
}

void DataFeatureVectorPointItem::getVectorPointDatas(QVector<QPointF> &datas) const
{
    datas = m_datas;
}

void DataFeatureVectorPointItem::append(QPointF data, const QString &des)
{
    m_datas.append(data);
    DataFeatureDescribeItem* item1 = new DataFeatureDescribeItem(des);
    DataFeaturePointItem* item2 = new DataFeaturePointItem(data);
    appendRow(QList<QStandardItem*>()<<item1<<item2);
}

void DataFeatureVectorPointItem::clear()
{
    m_datas.clear();
    removeRows(0,rowCount());
}

QPointF DataFeatureVectorPointItem::getAtData(int index) const
{
    return m_datas[index];
}

DataFeaturePointItem *DataFeatureVectorPointItem::getAtItem(int index) const
{
    return static_cast<DataFeaturePointItem *>(child(index,1));
}



DataFeatureItem::DataFeatureItem():AbstractDataFeatureItem()
{

}

DataFeatureItem::DataFeatureItem(const QString &name):AbstractDataFeatureItem(name)
{

}

DataFeatureItem::DataFeatureItem(const QString &name, const QIcon &icon)
    :AbstractDataFeatureItem(icon,name)
{

}

DataFeatureItem::~DataFeatureItem()
{

}

///
/// \brief 添加值信息
/// \param item 值信息指针
/// \param name 名字
/// \param icon 图标
///
void DataFeatureItem::addValueItem(DataFeatureValueItem *item, const QString &name, const QIcon &icon)
{
    addItemWithDescribe(item,name,icon);
}
///
/// \brief 添加值信息
/// \param data 值
/// \param name 名字
/// \param icon 图标
///
void DataFeatureItem::addValueItem(double data, const QString &name, const QIcon &icon)
{
    DataFeatureDescribeItem* desItem = new DataFeatureDescribeItem(icon,name);
    DataFeatureValueItem* valItem = new DataFeatureValueItem(data);
    appendRow((QList<QStandardItem*>()<<desItem<<valItem));
}
///
/// \brief 添加点信息
/// \param item 点信息条目
/// \param name 描述名字
/// \param icon 图标
///
void DataFeatureItem::addPointItem(DataFeaturePointItem *item, const QString &name, const QIcon &icon)
{
    addItemWithDescribe(item,name,icon);
}
///
/// \brief 添加点信息
/// \param data 值
/// \param name 名字
/// \param icon 图标
///
void DataFeatureItem::addPointItem(const QPointF &data, const QString &name, const QIcon &icon)
{
    DataFeatureDescribeItem* desItem = new DataFeatureDescribeItem(icon,name);
    DataFeaturePointItem* valItem = new DataFeaturePointItem(data);
    appendRow(QList<QStandardItem*>()<<desItem<<valItem);
}

void DataFeatureItem::addVectorValueItem(DataFeatureVectorValueItem *item)
{
    appendRow(item);
}

void DataFeatureItem::addVectorValueItem(QVector<double> &datas, const QString &name, const QStringList &des)
{
    DataFeatureVectorValueItem *item = new DataFeatureVectorValueItem(name);
    item->setVectorValueDatas(datas,des);
    appendRow(item);
}

void DataFeatureItem::addVectorPointItem(DataFeatureVectorPointItem *item)
{
    appendRow(item);
}

void DataFeatureItem::addVectorPointItem(QVector<QPointF> &datas,const QString& name,const QStringList& des)
{
    DataFeatureVectorPointItem *item = new DataFeatureVectorPointItem(name);
    item->setVectorPointDatas(datas,des);
    appendRow(item);
}

QwtPlotItem *DataFeatureItem::plotItem() const
{
    return nullptr;
}



///
/// \brief 添加单值信息，名字和icon将作为describe
/// \param item 条目接口
/// \param name 描述名字
/// \param icon 图标
///
void DataFeatureItem::addItemWithDescribe(AbstractDataFeatureItem* item,const QString& name,const QIcon & icon)
{
    DataFeatureDescribeItem* desItem = new DataFeatureDescribeItem(icon,name);
    appendRow((QList<QStandardItem*>()<<desItem<<item));
}

CurveDataFeatureItem::CurveDataFeatureItem():DataFeatureItem()
  ,m_cur(nullptr),m_sortCount(20)
{

}

CurveDataFeatureItem::CurveDataFeatureItem(QwtPlotCurve *cur)
    :DataFeatureItem(cur->title().text())
    ,m_sortCount(20)
{
    setCurvePtr(cur);
}

void CurveDataFeatureItem::setCurvePtr(QwtPlotCurve *cur)
{
    m_cur = cur;
    setText(m_cur->title().text());
    analysisData();
}

void CurveDataFeatureItem::getCurData(QwtPlotCurve *cur, QVector<QPointF> &datas)
{
    size_t n = cur->data()->size();
    datas.reserve(n);
    for(size_t i=0;i<n;++i)
    {
        datas.append(cur->data()->sample(i));
    }
}

void CurveDataFeatureItem::getVectorPointY(const QVector<QPointF> &points, QVector<double> &ys)
{
    ys.resize(points.size());
    std::transform(points.begin(),points.end(),ys.begin()
                   ,[](const QPointF& f)->double{
        return f.y();
    });
}

void CurveDataFeatureItem::clear()
{
    removeRows(0,rowCount());
}

void CurveDataFeatureItem::setSortCount(size_t n)
{
    m_sortCount = n;
}

size_t CurveDataFeatureItem::getSortCount() const
{
    return m_sortCount;
}

QwtPlotItem *CurveDataFeatureItem::plotItem() const
{
    return m_cur;
}

QwtPlotCurve *CurveDataFeatureItem::plotCurvePtr() const
{
    return m_cur;
}

void CurveDataFeatureItem::analysisData()
{
    clear();
    QVector<QPointF> datas;
    QVector<double> y;
    if(!m_cur)
    {
        return;
    }
    getCurData(m_cur,datas);
    getVectorPointY(datas,y);
    if(datas.size()<=0 || y.size()<=0)
    {
        return;
    }
    double Sum;
    double Mean;
    double Var;
    double StdVar;
    double Skewness;
    double Kurtosis;
    size_t n = datas.size();
    czy::Math::get_statistics(y.begin(),y.end(),Sum,Mean,Var
        ,StdVar,Skewness,Kurtosis);
    std::sort(datas.begin(),datas.end()
              ,[](const QPointF& a,const QPointF& b)->bool{
        return (a.y() < b.y());
    });
    double min = datas.begin()->y();//最小
    double max = (datas.end()-1)->y();//最大
    double mid = n>1 ? (datas.begin() + int(n/2))->y() : min;//中位数
    double peak2peak = max - min;
    QPointF minPoint = *datas.begin();
    QPointF maxPoint = *(datas.end()-1);
    QPointF midPoint = n>1 ? *(datas.begin() + int(n/2)) : minPoint;//中位数

    addValueItem(double(datas.size()),STR_WDF_PointCounts);
    addValueItem(min,STR_WDF_min_value);
    addValueItem(max,STR_WDF_max_value);
    addValueItem(mid,STR_WDF_mid_value);
    addValueItem(peak2peak,STR_WDF_PeekPeekValue);
    addValueItem(Sum,STR_WDF_Sum);
    addValueItem(Mean,STR_WDF_Mean);
    addValueItem(Var,STR_WDF_Var);
    addValueItem(StdVar,STR_WDF_StdVar);
    addValueItem(Skewness,STR_WDF_Skewness);
    addValueItem(Kurtosis,STR_WDF_Kurtosis);

    addPointItem(minPoint,STR_WDF_min_point);
    addPointItem(maxPoint,STR_WDF_max_point);
    addPointItem(midPoint,STR_WDF_mid_point);
    QVector<QPointF> ser;
    QStringList des;
    for (size_t i=0;i<m_sortCount && i<n;++i)
    {
        ser.append(datas[i]);
        des.append(QString::number(i+1));
    }
    addVectorPointItem(ser,STR_WDF_Low,des);
    ser.clear();
    for (size_t i=datas.size()-1;i>(datas.size()-m_sortCount-1);--i)
    {
        ser.append(datas[i]);
    }
    addVectorPointItem(ser,STR_WDF_Top,des);
}





