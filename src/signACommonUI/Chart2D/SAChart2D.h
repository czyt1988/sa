#ifndef SACHART2D_H
#define SACHART2D_H
#include "SA2DGraph.h"
#include "SACommonUIGlobal.h"
#include "SAPlotMarker.h"
#include <memory>
class QUndoStack;
class SAChart2DPrivate;
class SAAbstractRegionSelectEditor;
class SAAbstractDatas;
class SAXYSeries;
class SABarSeries;
class SAScatterSeries;
class SAFigureOptCommand;
class SAAbstractPlotEditor;
class SABoxSeries;
class SAHistogramSeries;
class QwtPlotBarChart;
///
/// \brief sa 2d 曲线绘图的基本窗口封装，包括支持SAAbstractDatas的处理
///
class SA_COMMON_UI_EXPORT SAChart2D : public SA2DGraph
{
    Q_OBJECT
    Q_DISABLE_COPY(SAChart2D)
    SA_IMPL(SAChart2D)
public:
    SAChart2D(QWidget *parent = nullptr);
    ~SAChart2D();
    enum SelectionMode{
        NoneSelection ///< 无选择
        ,RectSelection ///< 矩形选框
        ,EllipseSelection ///< 椭圆选框
        ,PolygonSelection ///< 多边形选框
    };
    //获取所有绘图的条目的rtti
    static QList<int> getPlotItemsRTTI();
    static QList<int> getXYSeriesItemsRTTI();
#if 0
    //判断是否是绘图条目
    static bool isPlotChartItem(const QwtPlotItem* item);
    //获取所有可支持的绘图条目
    static QwtPlotItemList getPlotChartItemList(const QwtPlot* chart);
    //获取所有xy点的绘图条目包括QwtPlotCurve和SAXYSeries,SABarSeries,QwtBarChart
    static QwtPlotItemList getPlotXYSeriesItemList(const QwtPlot* chart);
#endif
    //根据筛选set获取item list
    static QwtPlotItemList filterPlotItem(const QwtPlot* chart,const QSet<int>& enableRtti);
    //获取item的颜色,无法获取单一颜色就返回QColor()
    static QColor getItemColor(const QwtPlotItem *item,const QColor& defaultClr = QColor(0,0,0));
    //获取item的数据个数，-1为nan
    static int getItemDataSize(const QwtPlotItem* item);

    //根据曲线的size获取曲线的宽度，此函数结果通过setThinLineWidthPointSize设置
    static int getPlotCurWidth(int size);
    //设置细线画笔点数阈值，若大于这个值，默认画图的画笔将会为1，此函数设置会影响getPlotCurWidth函数结果
    static void setThinLineWidthPointSize(int size);

    //添加条目，-支持redo/undo
    void addItem(QwtPlotItem* item,const QString& des = QString());
    //移除一个对象-支持redo/undo
    void removeItem(QwtPlotItem* item);
    //添加曲线-支持redo/undo
    QwtPlotCurve* addCurve(const double *xData, const double *yData, int size);
    QwtPlotCurve* addCurve(const QVector<QPointF>& xyDatas);
    QwtPlotCurve* addCurve(const QVector< double > &xData, const QVector< double > &yData);
    SAXYSeries *addCurve(SAAbstractDatas* datas);
    SAXYSeries* addCurve(SAAbstractDatas* datas,double xStart,double xDetal);
    SAXYSeries* addCurve(SAAbstractDatas* x,SAAbstractDatas* y,const QString& name = QString());
    //添加Histogram-支持redo/undo
    SAHistogramSeries *addHistogram(SAAbstractDatas* datas);
    //添加Bar-支持redo/undo
    SABarSeries *addBar(SAAbstractDatas* datas);
    //绘制散点图-支持redo/undo
    SAScatterSeries* addScatter(SAAbstractDatas* datas);
    //绘制箱盒图-支持redo/undo
    SABoxSeries* addBox(SAAbstractDatas* datas);
    //添加样条线-支持redo/undo
    QwtPlotMarker* addVLine(double val);
    QwtPlotMarker* addHLine(double val);
    //移除范围内数据-支持redo/undo
    void removeDataInRang(const QList<QwtPlotItem *> &chartItems);
    void removeDataInRang();
    //获取选择范围内的数据,如果当前没有选区，返回false
    bool getXYDataInRange(QVector<QPointF>& xy,QVector<int>* index,const QwtPlotItem *cur);
    bool getXYDataInRange(QVector<double>* xs, QVector<double>* ys,QVector<int>* index,const QwtPlotItem *cur);
    //获取item对应的xy数据，如果可以转换的话
    bool getXYData(QVector<QPointF>& xy,const QwtPlotItem *cur);
    bool getXYData(QVector<double>* xs, QVector<double>* ys,const QwtPlotItem *cur);
    //开始选择模式
    void enableSelection(SelectionMode mode, bool on = true);
    //判断当前的选择模式
    bool isEnableSelection(SelectionMode mode) const;
    //清除所有选区以及对应的编辑器
    void clearAllSelectedRegion();
    //判断是否有选区
    bool isRegionVisible() const;
    //获取当前正在显示的选择区域
    SelectionMode currentSelectRegionMode() const;
    //获取当前选择编辑器
    SAAbstractRegionSelectEditor* getRegionSelectEditor();
    const SAAbstractRegionSelectEditor* getRegionSelectEditor() const;
    //获取当前可见的选区的范围
    QPainterPath getSelectionRange() const;
    QPainterPath getSelectionRange(int xaxis,int yaxis) const;
    void setSelectionRange(const QPainterPath& painterPath);
    //判断当前的条目的x，y轴关联是否和选区的一致
    bool isSelectionRangeAxisMatch(const QwtPlotItem* item);
    //

    //ctrl+z || ctrl + y
    void redo();
    void undo();
    //redo/undo 的command添加操作
    void appendCommand(SAFigureOptCommand* cmd);
    //设置一个编辑器，编辑器的内存交由SAChart2D管理，SAChart2D只能存在一个额外的编辑器
    void setEditor(SAAbstractPlotEditor* editor);
    SAAbstractPlotEditor* getEditor() const;
    //当前选择的条目
    const QList<QwtPlotItem*>& getCurrentSelectItems() const;
    //判断当前选中的条目是否有曲线
    bool isCurrentSelectItemsHaveChartItem() const;
    //设置当前选择的条目
    void setCurrentSelectItems(const QList<QwtPlotItem*>& items);
    void setCurrentSelectPlotCurveItems(const QList<QwtPlotCurve*>& items);

    //
    void unenableEditor();
    //添加标记 通过此函数添加的标记将会记录到一个列表中
    void addPlotMarker(SAAbstractPlotMarker* marker);
    const QList<SAAbstractPlotMarker*>& getPlotMarkers() const;
    void removePlotMarker(SAAbstractPlotMarker* marker);
    void removeAllPlotMarker();
protected:
    //开始矩形选框模式
    void startRectSelectMode();
    //开始椭圆选框模式
    void startEllipseSelectMode();
    //开始椭圆选框模式
    void startPolygonSelectMode();
    //结束选区模式但不清空
    void stopSelectMode();
signals:
    ///
    /// \brief 选区发生变化发送的信号
    /// \param shape
    ///
    void selectionRegionChanged(const QPainterPath& shape);
    ///
    /// \brief 当前选择的条目发生变化触发的信号
    /// \param items
    ///
    void currentSelectItemsChanged(const QList<QwtPlotItem*>& items);
    ///
    /// \brief 选区工具改变的信号
    /// \param mode 选区模式
    /// \param on
    ///
    void enableSelectionChanged(SelectionMode mode,bool on);
protected slots:
    // 选区选择完成
    void onSelectionFinished(const QPainterPath& shape);
private:
    QList<SAXYSeries *> addDatas(const QList<SAAbstractDatas*>& datas);
protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
private:
    static int s_size_pen_width2;///< 小于此尺寸将设置画笔为2，大于这个尺寸设置为1
};

#endif // SACHART2D_H
