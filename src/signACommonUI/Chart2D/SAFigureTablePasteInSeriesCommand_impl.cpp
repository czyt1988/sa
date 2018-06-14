#include "SAFigureTableCommands.h"
#include "QwtPlotItemDataModel.h"
#include "SAChart.h"
///
/// \brief The SAAbstractFiguresTablePasteInSeriesCommandPrivate class
/// 通用impl接口
///
class SAFigureTablePasteInSeriesCommandPrivate
{
public:
    SAFigureTablePasteInSeriesCommandPrivate(SAChart2D *chart, QwtPlotItem *item)
        :m_chart(chart)
        ,m_item(item)
    {}
    virtual ~SAFigureTablePasteInSeriesCommandPrivate(){}
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
    static QSize getClipboardTableSize(const QList<QVariantList>& clipboardTable);
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;
};


QSize SAFigureTablePasteInSeriesCommandPrivate::getClipboardTableSize(const QList<QVariantList> &clipboardTable)
{
    int maxColumn = 0;
    const int row = clipboardTable.size();
    for(int i=0;i<row;++i)
    {
        const QVariantList& colVar = clipboardTable.at(i);
        if(colVar.size() > maxColumn)
        {
            maxColumn = colVar.size();
        }
    }
    return QSize(maxColumn,row);
}

////////////////////////////////////////////////////////////////
// SAFigureDataCommandEditSeriesPrivate_SeriesStoreItem
// 适用于多个数据的编辑，适用于复制粘贴
////////////////////////////////////////////////////////////////

template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
class SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem : public SAFigureTablePasteInSeriesCommandPrivate
{
public:
    SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem(
            SAChart2D *chart
            , QwtPlotItem *item
            , const QList<QStringList>& stringClipboardTable
            , int indexRow
            , int indexCol
            , FpSetFun fpSetVal
            , FpSetSeriesSampleFun fpSetSeries
            );
    void init(const QList<QStringList>& stringClipboardTable,int indexRow,int indexCol);
    //api isValid
    virtual bool isValid() const;
    //api redo
    virtual void redo();
    //api undo
    virtual void undo();
    //判断是否尺寸有变
    virtual bool isSizeChanged() const;
private:
    bool m_isValid;
    bool m_isSizeChanged;///< 记录是否尺寸发生了变化
    FpSetFun m_fpSetValue;///< fun(T& inputoutput,int indexCol,double val);
    FpSetSeriesSampleFun m_fpSetSample;///< fun(QwtPlotItem* item,cosnt QVector<T>& series);
    PlotItemType* m_plotItem;
    //索引，旧值，新值
    QVector<std::tuple<int,T,T> > m_replaceInfos;
    QVector<std::pair<int,T> > m_appendInfos;
    int m_oldDataSize;///< 记录原来的长度
    int m_newDataSize;///< 记录原来的长度
};

template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QList<QStringList> &stringClipboardTable
        , int indexRow
        , int indexCol
        , FpSetFun fpSetVal
        , FpSetSeriesSampleFun fpSetSeries)
    :SAFigureTablePasteInSeriesCommandPrivate(chart,item)
    ,m_isValid(false)
    ,m_isSizeChanged(false)
    ,m_fpSetValue(fpSetVal)
    ,m_fpSetSample(fpSetSeries)
    ,m_oldDataSize(0)
    ,m_newDataSize(0)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    init(stringClipboardTable,indexRow,indexCol);
}
template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
void SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::init(const QList<QStringList> &stringClipboardTable, int indexRow, int indexCol)
{
    int dataColumnSize = QwtPlotItemDataModel::getItemColumnCount(item());
    m_newDataSize = m_oldDataSize = (int)(m_plotItem->dataSize());
    const int clipBoardRowCount = stringClipboardTable.size();

    //线提取旧的数据，生成新的数据
    for(int r=0;r<clipBoardRowCount;++r)
    {
        const QStringList& rowValue = stringClipboardTable.at(r);
        const int realDataRowIndex = indexRow + r;
        if(realDataRowIndex < m_oldDataSize)
        {
            //说明这是替换原来的数据
            if(rowValue.size() <= 0)
                continue;
            T oldData = m_plotItem->sample(indexRow);
            T newData = oldData;
            for(int c=0;c<rowValue.size();++c)
            {
                int realDataCol = c + indexCol;
                if(realDataCol < dataColumnSize)
                {
                    m_fpSetValue(newData,realDataCol,rowValue[c].toDouble());//此函数指针用来根据QVariant和对应的列设置T的值
                }
            }
            m_replaceInfos.append(std::make_tuple(realDataRowIndex,oldData,newData));
        }
        else
        {
            //说明插入新数据
            //此句在if(rowValue.size() <= 0)之前的意义是允许插入空行，空行会以默认数据替代
            m_newDataSize = realDataRowIndex+1;
            if(rowValue.size() <= 0)
                continue;
            T newData;
            for(int c=0;c<rowValue.size();++c)
            {
                int realDataCol = c + indexCol;
                if(realDataCol < dataColumnSize)
                    m_fpSetValue(newData,realDataCol,rowValue[c].toDouble());//此函数指针用来根据QVariant和对应的列设置T的值
            }
            m_appendInfos.append(std::make_pair(realDataRowIndex,newData));
        }
    }
    m_isValid = ((m_replaceInfos.size()+m_appendInfos.size())>0);
    m_isSizeChanged = (m_appendInfos.size() > 0);
}
template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
bool SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::isValid() const
{
    return m_isValid;
}
template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
void SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::redo()
{
    if(!isValid())
        return;
    QVector<T> vecDatas;
    vecDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(vecDatas,m_plotItem);
    if(m_newDataSize > m_oldDataSize)
    {
        vecDatas.resize(m_newDataSize);
        for(auto i = m_appendInfos.begin();i!=m_appendInfos.end();++i)
        {
            int index = std::get<0>((*i));
            if(index < vecDatas.size())
                vecDatas[index] = std::get<1>((*i));
        }
    }
    for(auto i = m_replaceInfos.begin();i!=m_replaceInfos.end();++i)
    {
        int index = std::get<0>((*i));
        if(index < vecDatas.size())
            vecDatas[index] = std::get<2>((*i));
    }
    m_fpSetSample(item(),vecDatas);
}
template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
void SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::undo()
{
    if(!isValid())
        return;
    QVector<T> vecDatas;
    vecDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(vecDatas,m_plotItem);
    if(m_newDataSize > m_oldDataSize)
    {
        vecDatas.resize(m_oldDataSize);
    }
    for(auto i = m_replaceInfos.begin();i!=m_replaceInfos.end();++i)
    {
        int index = std::get<0>((*i));
        if(index < vecDatas.size())
            vecDatas[index] = std::get<1>((*i));
    }
    m_fpSetSample(item(),vecDatas);
}
template<typename T,typename PlotItemType,typename FpSetFun,typename FpSetSeriesSampleFun>
bool SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem<T, PlotItemType, FpSetFun,FpSetSeriesSampleFun>
::isSizeChanged() const
{
    return m_isSizeChanged;
}

//////////////////////////////////////////////////////////////////////////////////



SAFigureTablePasteInSeriesCommand::SAFigureTablePasteInSeriesCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QList<QStringList> &stringClipboardTable
        , int indexRow
        , int indexCol
        , const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,d_ptr(nullptr)
{
    switch(item->rtti ())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        d_ptr = new SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem
                <QPointF
                ,QwtPlotCurve
                ,decltype(&QwtPlotItemDataModel::setSeriesPointFValue)
                ,decltype(&SAChart::setPlotCurveSample)
                >
                (chart,item,stringClipboardTable,indexRow,indexCol
                 ,QwtPlotItemDataModel::setSeriesPointFValue
                 ,SAChart::setPlotCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        d_ptr = new SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotHistogram
                ,decltype(&QwtPlotItemDataModel::setSeriesIntervalValue)
                ,decltype(&SAChart::setPlotHistogramSample)
                >
                (chart,item,stringClipboardTable,indexRow,indexCol
                 ,QwtPlotItemDataModel::setSeriesIntervalValue
                 ,SAChart::setPlotHistogramSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        d_ptr = new SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotIntervalCurve
                ,decltype(&QwtPlotItemDataModel::setSeriesIntervalValue)
                ,decltype(&SAChart::setPlotIntervalCurveSample)
                >
                (chart,item,stringClipboardTable,indexRow,indexCol
                 ,QwtPlotItemDataModel::setSeriesIntervalValue
                 ,SAChart::setPlotIntervalCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        d_ptr = new SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem
                <QwtSetSample
                ,QwtPlotMultiBarChart
                ,decltype(&QwtPlotItemDataModel::setSeriesSetsampleValue)
                ,decltype(&SAChart::setPlotMultiBarChartSample)
                >
                (chart,item,stringClipboardTable,indexRow,indexCol
                 ,QwtPlotItemDataModel::setSeriesSetsampleValue
                 ,SAChart::setPlotMultiBarChartSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        d_ptr = new SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem
                <QwtOHLCSample
                ,QwtPlotTradingCurve
                ,decltype(&QwtPlotItemDataModel::setSeriesOHLCsampleValue)
                ,decltype(&SAChart::setPlotTradingCurveSample)
                >
                (chart,item,stringClipboardTable,indexRow,indexCol
                 ,QwtPlotItemDataModel::setSeriesOHLCsampleValue
                 ,SAChart::setPlotTradingCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        d_ptr = new SAFigureTablePasteInSeriesCommandPrivate_SeriesStoreItem
                <QwtPoint3D
                ,QwtPlotSpectroCurve
                ,decltype(&QwtPlotItemDataModel::setSeriesPoint3dValue)
                ,decltype(&SAChart::setPlotSpectroCurveSample)
                >
                (chart,item,stringClipboardTable,indexRow,indexCol
                 ,QwtPlotItemDataModel::setSeriesPoint3dValue
                 ,SAChart::setPlotSpectroCurveSample);
        break;
    }
    default:
        break;
    }
}

SAFigureTablePasteInSeriesCommand::~SAFigureTablePasteInSeriesCommand()
{
    if(d_ptr)
        delete d_ptr;
}

void SAFigureTablePasteInSeriesCommand::redo()
{
    if(d_ptr)
        d_ptr->redo();
}

void SAFigureTablePasteInSeriesCommand::undo()
{
    if(d_ptr)
        d_ptr->undo();
}

bool SAFigureTablePasteInSeriesCommand::isValid() const
{
    if(d_ptr)
        return d_ptr->isValid();
    return false;
}

bool SAFigureTablePasteInSeriesCommand::isSizeChanged() const
{
    if(d_ptr)
        return d_ptr->isSizeChanged();
    return false;
}










