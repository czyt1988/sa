#ifndef SACHART2D_H
#define SACHART2D_H
#include "SA2DGraph.h"
#include "SACommonUIGlobal.h"
#include <memory>
class SAAbstractRegionSelectEditor;
class SAAbstractDatas;
class SAXYSeries;
class SABarSeries;
///
/// \brief sa 2d 曲线绘图的基本窗口封装，包括支持SAAbstractDatas的处理
///
class SA_COMMON_UI_EXPORT SAChart2D : public SA2DGraph
{
    Q_OBJECT
public:
    SAChart2D(QWidget *parent = nullptr);
    ~SAChart2D();
    enum SelectionMode{
        NoneSelection ///< 无选择
        ,RectSelection ///< 矩形选框
        ,EllipseSelection ///< 椭圆选框
        ,PolygonSelection ///< 多边形选框
    };

    //添加曲线
    using SA2DGraph::addCurve;
    SAXYSeries *addCurve(SAAbstractDatas* datas);
    SAXYSeries* addCurve(SAAbstractDatas* datas,double xStart,double xDetal);
    SAXYSeries* addCurve(SAAbstractDatas* x,SAAbstractDatas* y,const QString& name = QString());
    void addCurve(QwtPlotCurve* cur);
    //添加bar
    using SA2DGraph::addBar;
    SABarSeries *addBar(SAAbstractDatas* datas);
    //移除范围内数据
    void removeDataInRang(QList<QwtPlotCurve *> curves);

    //开始选择模式
    void startSelectMode(SelectionMode mode = RectSelection);
    void stopSelectMode();
    //清除所有选区
    void clearAllSelectedRegion();
    //判断是否有选区
    bool isRegionVisible() const;
    //获取当前正在显示的选择区域
    SelectionMode currentSelectRegionMode() const;
    //获取当前选择编辑器
    SAAbstractRegionSelectEditor* getRegionSelectEditor();
    const SAAbstractRegionSelectEditor* getRegionSelectEditor() const;
    //获取当前可见的选区的范围
    QPainterPath getVisibleRegion() const;
protected:
    //开始矩形选框模式
    void startRectSelectMode();
    //开始椭圆选框模式
    void startEllipseSelectMode();
    //开始椭圆选框模式
    void startPolygonSelectMode();
private:
    void addDatas(const QList<SAAbstractDatas*>& datas);
protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
private:
    SelectionMode m_selectMode;///< 选择模式
    SAAbstractRegionSelectEditor* m_chartSelectRigionEditor;///< 矩形选择编辑器
};

#endif // SACHART2D_H
