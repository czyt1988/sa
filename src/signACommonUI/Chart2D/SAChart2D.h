#ifndef SACHART2D_H
#define SACHART2D_H
#include "SA2DGraph.h"
#include "SACommonUIGlobal.h"
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
    using SA2DGraph::removeDataInRang;
    void removeDataInRang(QList<QwtPlotCurve *> curves);

    //开始选择模式
    void startSelectMode(SelectionMode selectMode = SingleSelection);
private:
    void addDatas(const QList<SAAbstractDatas*>& datas);
protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
private:

    bool m_isSelectionMode; ///< 是否在选框模式

};

#endif // SACHART2D_H
