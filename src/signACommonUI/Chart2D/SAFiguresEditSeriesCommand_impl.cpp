#include "SAFigureOptCommands.h"
#include <QDebug>
#include "qwt_series_store.h"
#include "SAChart.h"
///
/// \brief The SAFigureEditSeriesDataCommandPrivate class
/// 通用impl接口
///
class SAFiguresEditSeriesCommandPrivate
{
public:
    SAFiguresEditSeriesCommandPrivate(SAChart2D *chart, QwtPlotItem *item)
        :m_chart(chart)
        ,m_item(item)
    {}
    virtual ~SAFiguresEditSeriesCommandPrivate(){}
    //获取chart
    SAChart2D *chart() const{return m_chart;}
    //获取item
    QwtPlotItem *item() const{return m_item;}
    //api isValid
    virtual bool isValid() const = 0;
    //api redo
    virtual void redo() = 0;
    //api undo
    virtual void undo() = 0;
    //api isSizeChanged
    virtual bool isSizeChanged() const = 0;
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;
};

///
/// \brief 适用于QwtSeriesStore<T>的接口
///
template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
class SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem : public SAFiguresEditSeriesCommandPrivate
{
public:
    SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem(
            SAChart2D *chart
            , QwtPlotItem *item
            , double val
            , int indexRow
            , int indexCol
            , FpSetFun fpSetVal
            , FpSetSeriesSampleFun fpSetSeries
            );
    void init(double val,int indexRow,int indexCol);
    //api isValid
    virtual bool isValid() const;
    //api redo
    virtual void redo();
    //api undo
    virtual void undo();
    //
    virtual bool isSizeChanged() const;
private:
    PlotItemType* m_plotItem;
    T m_oldData;
    T m_newData;
    int m_index;
    int m_oldDataSize;///< 旧数据的尺寸，只有在数据需要插入时有效
    bool m_isValid;
    bool m_isSizeChanged;///< 记录是否尺寸发生了变化
    FpSetFun m_fpSetValue;///< fun(T& inputoutput,int indexCol,double val);
    FpSetSeriesSampleFun m_fpSetSample;///< fun(QwtPlotItem* item,cosnt QVector<T>& series);
};


///
/// \brief SAFigureEditSeriesDataCommandSeriesStoreItemPrivate<T, FpSetFun>::SAFigureEditSeriesDataCommandSeriesStoreItemPrivate
/// \param chart
/// \param item
/// \param val
/// \param indexRow
/// \param indexCol
/// \param fp 设置数据函数类似于 void fp(T& val,int col,double v)
///
template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem<T,PlotItemType,FpSetFun,FpSetSeriesSampleFun>
    ::SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem(
        SAChart2D *chart
        , QwtPlotItem *item
        , double val
        , int indexRow
        , int indexCol
        , FpSetFun fpSetVal
        , FpSetSeriesSampleFun fpSetSeries
        )
    :SAFiguresEditSeriesCommandPrivate(chart,item)
    ,m_index(indexRow)
    ,m_isValid(false)
    ,m_isSizeChanged(false)
    ,m_oldDataSize(-1)
    ,m_fpSetValue(fpSetVal)
    ,m_fpSetSample(fpSetSeries)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    init(val,indexRow,indexCol);
}

template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
bool SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem<T,PlotItemType,FpSetFun,FpSetSeriesSampleFun>
::isValid() const
{
    return m_isValid;
}

template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
void SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem<T,PlotItemType,FpSetFun,FpSetSeriesSampleFun>
::init(double val, int indexRow, int indexCol)
{
    m_oldDataSize = (int)(m_plotItem->dataSize());
    //两种情况，第一种indexRow比size小，直接提取旧数据进行替换，第二种情况，indexRow比size大，插入到新数据处，期间补充默认构造
    if(indexRow < m_oldDataSize)
    {
        T data = m_plotItem->sample(indexRow);
        m_oldData = data;
        m_newData = data;
        m_fpSetValue(m_newData,indexCol,val);
        m_isValid = true;
    }
    else if(indexRow >= m_oldDataSize)
    {
        T data;
        m_oldData = m_newData = data;
        m_fpSetValue(m_newData,indexCol,val);
        m_isValid = true;
    }
    else
    {
        m_isValid = false;
    }

}


template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
void SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem<T,PlotItemType,FpSetFun,FpSetSeriesSampleFun>
::redo()
{
    if(!m_isValid)
        return;
    if(m_oldDataSize != m_plotItem->dataSize())
        return;
    QVector<T> vecDatas;
    vecDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(vecDatas,m_plotItem);
    if(m_index < m_oldDataSize)
    {
        vecDatas[m_index] = m_newData;
    }
    else if(m_index >= m_oldDataSize)
    {
        vecDatas.resize(m_index+1);
        vecDatas[m_index] = m_newData;
    }
    m_fpSetSample(item(),vecDatas);
}


template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
void SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::undo()
{
    if(!m_isValid)
        return;
    QVector<T> vecDatas;
    vecDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(vecDatas,m_plotItem);
    if(m_index < m_oldDataSize)
    {
        vecDatas[m_index] = m_oldData;
    }
    else if(m_index >= m_oldDataSize)
    {
        vecDatas.resize(m_oldDataSize);
    }
    m_fpSetSample(item(),vecDatas);
    //SAChart::setVectorSampleData(item(),vecDatas);
}

template<typename T, typename PlotItemType, typename FpSetFun, typename FpSetSeriesSampleFun>
bool SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::isSizeChanged() const
{
    return m_isSizeChanged;
}

////////////////////////////////////////////////////////////////
// SAFigureEditSeriesDataCommand
////////////////////////////////////////////////////////////////


void fun_set_series_pointf_value(QPointF& p,int col,double val)
{
    switch (col) {
    case 0:
        p.rx() = val;break;
    case 1:
        p.ry() = val;break;
    default:
        break;
    }
}

void fun_set_series_point3d_value(QwtPoint3D& p,int col,double val)
{
    switch(col)
    {
    case 0:
        p.setX(val);break;
    case 1:
        p.setY(val);break;
    case 2:
        p.setZ(val);break;
    default:
        break;
    }
}

void fun_set_series_interval_value(QwtIntervalSample& p,int col,double val)
{
    switch(col)
    {
    case 0:p.value = val;break;
    case 1:p.interval.setMinValue(val);break;
    case 2:p.interval.setMaxValue(val);break;
    default:
        break;
    }
}

void fun_set_series_setsample_value(QwtSetSample& p,int col,double val)
{
    if(0 == col)
    {
        p.value = val;
    }
    else
    {
        if(col-1 < p.set.size())
        {
            p.set[col-1] = val;
        }
    }
}

void fun_set_series_ohlcsample_value(QwtOHLCSample& p,int col,double val)
{
    switch(col)
    {
    case 0:
        p.time = val;break;
    case 1:
        p.open = val;break;
    case 2:
        p.high = val;break;
    case 3:
        p.low = val;break;
    case 4:
        p.close = val;break;
    default:
        break;
    }
}

void fun_set_QwtPlotCurve_series(QwtPlotItem* p,const QVector<QPointF>& datas)
{
    static_cast<QwtPlotCurve*>(p)->setSamples(datas);
}
void fun_set_QwtPlotBarChart_series(QwtPlotItem* p,const QVector<QPointF>& datas)
{
    static_cast<QwtPlotBarChart*>(p)->setSamples(datas);
}
void fun_set_QwtPlotHistogram_series(QwtPlotItem* p,const QVector<QwtIntervalSample>& datas)
{
    static_cast<QwtPlotHistogram*>(p)->setSamples(datas);
}
void fun_set_QwtPlotIntervalCurve_series(QwtPlotItem* p,const QVector<QwtIntervalSample>& datas)
{
    static_cast<QwtPlotIntervalCurve*>(p)->setSamples(datas);
}
void fun_set_QwtPlotMultiBarChart_series(QwtPlotItem* p,const QVector<QwtSetSample>& datas)
{
    static_cast<QwtPlotMultiBarChart*>(p)->setSamples(datas);
}
void fun_set_QwtPlotTradingCurve_series(QwtPlotItem* p,const QVector<QwtOHLCSample>& datas)
{
    static_cast<QwtPlotTradingCurve*>(p)->setSamples(datas);
}
void fun_set_QwtPlotSpectroCurve_series(QwtPlotItem* p,const QVector<QwtPoint3D>& datas)
{
    static_cast<QwtPlotSpectroCurve*>(p)->setSamples(datas);
}
///
/// \brief 编辑曲线数据的命令
/// \param chart 图指针
/// \param item item指针
/// \param val 编辑的值
/// \param indexRow 对应的行号，行号对应线性数据的index
/// \param indexCol 对应的列号，列号应该根据model的显示列号来定义
/// \param cmdName 命令名
/// \param parent 命令父级
///
SAFiguresEditSeriesCommand::SAFiguresEditSeriesCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , double val
        , int indexRow
        , int indexCol
        , const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,d_ptr(nullptr)
{
    //qDebug() << "rtti:" << item->rtti () << " row:" << indexRow << "indexCol" << indexCol;
    switch(item->rtti ())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        d_ptr = new SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
                <QPointF
                ,QwtPlotCurve
                ,decltype(&fun_set_series_pointf_value)
                ,decltype(&fun_set_QwtPlotCurve_series)
                >
                (chart,item,val,indexRow,indexCol,fun_set_series_pointf_value,fun_set_QwtPlotCurve_series);
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        d_ptr = new SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
                <QPointF
                ,QwtPlotBarChart
                ,decltype(&fun_set_series_pointf_value)
                ,decltype(&fun_set_QwtPlotBarChart_series)
                >
                (chart,item,val,indexRow,indexCol,fun_set_series_pointf_value,fun_set_QwtPlotBarChart_series);
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        d_ptr = new SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotHistogram
                ,decltype(&fun_set_series_interval_value)
                ,decltype(&fun_set_QwtPlotHistogram_series)
                >
                (chart,item,val,indexRow,indexCol,fun_set_series_interval_value,fun_set_QwtPlotHistogram_series);
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        d_ptr = new SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotIntervalCurve
                ,decltype(&fun_set_series_interval_value)
                ,decltype(&fun_set_QwtPlotIntervalCurve_series)
                >
                (chart,item,val,indexRow,indexCol,fun_set_series_interval_value,fun_set_QwtPlotIntervalCurve_series);
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        d_ptr = new SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
                <QwtSetSample
                ,QwtPlotMultiBarChart
                ,decltype(&fun_set_series_setsample_value)
                ,decltype(&fun_set_QwtPlotMultiBarChart_series)
                >
                (chart,item,val,indexRow,indexCol,fun_set_series_setsample_value,fun_set_QwtPlotMultiBarChart_series);
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        d_ptr = new SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
                <QwtOHLCSample
                ,QwtPlotTradingCurve
                ,decltype(&fun_set_series_ohlcsample_value)
                ,decltype(&fun_set_QwtPlotTradingCurve_series)
                >
                (chart,item,val,indexRow,indexCol,fun_set_series_ohlcsample_value,fun_set_QwtPlotTradingCurve_series);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        d_ptr = new SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
                <QwtPoint3D
                ,QwtPlotSpectroCurve
                ,decltype(&fun_set_series_point3d_value)
                ,decltype(&fun_set_QwtPlotSpectroCurve_series)
                >
                (chart,item,val,indexRow,indexCol,fun_set_series_point3d_value,fun_set_QwtPlotSpectroCurve_series);
        break;
    }
    default:
        break;
    }
}

SAFiguresEditSeriesCommand::~SAFiguresEditSeriesCommand()
{
    if(d_ptr)
        delete d_ptr;
}

void SAFiguresEditSeriesCommand::redo()
{
    if(d_ptr)
    {
       d_ptr->redo ();
    }
}

void SAFiguresEditSeriesCommand::undo()
{
    if(d_ptr)
    {
       d_ptr->undo ();
    }
}

bool SAFiguresEditSeriesCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid ();
    }
    return false;
}

bool SAFiguresEditSeriesCommand::isSizeChanged() const
{
    if(d_ptr)
    {
        return d_ptr->isSizeChanged ();
    }
    return false;
}

