#include "QwtPlotPropertySetDialog.h"
#include <QBrush>
#include <QPen>
#include <qwt_scale_draw.h>
#include <qwt_date_scale_draw.h>
#include <qwt_interval.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_item.h>
#define PROPERTY_ATTRIBUTE_CURVE_PTR QString("curve_ptr")
#define ID_GroupPlot 1
//"图表参数"
#define ID_PlotTitle 2
//"图表标题"
#define ID_PlotFooter 3
//"脚标"

#define ID_XBottomTitle 4
//"X底轴标题"
#define ID_XTopTitle 5
//"X顶轴标题"
#define ID_YLeftTitle 6
//"Y左轴标题"
#define ID_YRightTitle 7
//"Y右轴标题"

#define ID_XBottomScale 8
//"X底轴Scale样式"
#define ID_XTopScale 9
//"X顶轴Scale样式"
#define ID_YLeftScale 10
//"Y左轴Scale样式"
#define ID_YRightScale 11
//"Y右轴Scale样式"

#define ID_XBottomDateScaleType 12
//"X底轴DateScale样式"
#define ID_XTopDateScaleType 13
//"X顶轴DateScale样式"
#define ID_YLeftDateScaleType 14
//"Y左轴DateScale样式"
#define ID_YRightDateScaleType 15
//"Y右轴DateScale样式"

#define ID_PlotCanvasBackground 16
//"画布背景"
#define ID_PlotEnableZoomerScroll 17
//"缩放滚动条"
#define ID_PlotAxisSet 18
//"坐标轴设置"

#define ID_GroupXBottomSet 19
//"x底轴设置"
#define ID_GroupXTopSet 20
//"x顶轴设置"
#define ID_GroupyLeftSet 21
//"y左轴设置"
#define ID_GroupyRightSet 22
//"y右轴设置"

#define ID_XBottomInterval 23
//"X底轴IntervalMin
#define ID_XTopInterval 24
//"X顶轴IntervalMin
#define ID_YLeftInterval 25
//"Y左轴IntervalMin
#define ID_YRightInterval 26
//"Y右轴IntervalMin

#define ID_XBottomLabelRotation  27
//"X底轴IntervalMin
#define ID_XTopLabelRotation  28
//"X顶轴IntervalMin
#define ID_YLeftLabelRotation  29
//"Y左轴IntervalMin
#define ID_YRightLabelRotation  30
//"Y右轴IntervalMin
//////////////////////////////////////////////
#define ID_CurveIDBase 0x800
//记录曲线id的基本位 ，在位数大于ID_CurveIDBase的为曲线id
#define ID_CurvePropertyIDBase 0x7FF

//用于与出曲线的属性位
#define ID_CurveTitle 300
//曲线标题
#define ID_CurveColore 301
//曲线颜色
#define ID_CurvePenStyle 302
//曲线线宽
#define ID_CurveWidth 303
//曲线线形

#define xBottomFlag 0x1
#define xTopFlag 0x2
#define yLeftFlag 0x4
#define yRightFlag 0x8

int QwtPlotPropertySetDialog::makeCurvePropertyID(int CurveBaseID,int curvePropertyID)
{
    return (CurveBaseID|curvePropertyID);
}

int QwtPlotPropertySetDialog::getCurveID(int curvePropertyID)
{
    return (curvePropertyID&(~ID_CurvePropertyIDBase));
}

int QwtPlotPropertySetDialog::makeCurveBaseID(QwtPlotCurve *cur)
{
    static int s_curve_id = ID_CurveIDBase;
    s_curve_id+=ID_CurveIDBase;
    m_baseID2curve[s_curve_id] = cur;
    m_curve2baseID[cur] = s_curve_id;
    return s_curve_id;
}

int QwtPlotPropertySetDialog::getCurvePropertyID(int id)
{
    return (ID_CurvePropertyIDBase&id);
}

QwtPlotPropertySetDialog::QwtPlotPropertySetDialog(QWidget *parent,SA2DGraph* plot)
    : QWidget(parent)
    ,m_plot(plot)
    ,m_enableSet(false)
{
    createUI();
    buildConnect();
}

QwtPlotPropertySetDialog::~QwtPlotPropertySetDialog()
{

}

void QwtPlotPropertySetDialog::createUI()
{
    m_mainHorizontalLayout = new QHBoxLayout(this);
    m_mainHorizontalLayout->setSpacing(1);
    m_mainHorizontalLayout->setContentsMargins(1, 1, 1, 1);
    m_mainHorizontalLayout->setObjectName(QString("mainHorizontalLayout"));
    m_mainHorizontalLayout->setContentsMargins(1, 1, 1, 1);

    m_property = new QtTreePropertyBrowser(this);
    m_mainHorizontalLayout->addWidget(m_property);

    m_propertyGroup = new QtGroupPropertyManager(m_property);
    m_variantManager = new QtVariantPropertyManager(m_property);
    m_variantFactory = new QtVariantEditorFactory(m_property);
    m_property->setFactoryForManager(m_variantManager,m_variantFactory);
    connect(m_variantManager,&QtVariantPropertyManager::valueChanged
            ,this,&QwtPlotPropertySetDialog::onPropertyValueChanged);
    addPlotSet(m_plot);
    addCurveSet(m_plot);
}

void QwtPlotPropertySetDialog::updateAllValue()
{
    m_property_id.setVarPropertyData(ID_PlotTitle,m_plot->title().text());
    m_property_id.setVarPropertyData(ID_PlotFooter,m_plot->footer().text());
    m_property_id.setVarPropertyData(ID_PlotCanvasBackground,m_plot->canvasBackground().color());
    //m_property_id.setVarPropertyData(ID_PlotEnableZoomerScroll,m_plot->isEnableZoomerScroll());
    m_property_id.setVarPropertyData(ID_PlotAxisSet,getAxisEnable(m_plot));
    updateAxisValue(QwtPlot::xBottom);
    updateAxisValue(QwtPlot::yLeft);
    updateAxisValue(QwtPlot::xTop);
    updateAxisValue(QwtPlot::yRight);
    QList<QwtPlotCurve*> curs = m_plot->getCurveList();
    for (auto ite = curs.begin();ite != curs.end();++ite)
    {
        updateCurveValue(*ite);
    }
}

void QwtPlotPropertySetDialog::buildConnect()
{
    connect(m_plot,&QwtPlot::itemAttached,this,&QwtPlotPropertySetDialog::itemAttached);
    QwtScaleWidget* aw = m_plot->axisWidget(QwtPlot::xBottom);
    if(aw)
        connect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedXBottom);
    aw = m_plot->axisWidget(QwtPlot::xTop);
    if(aw)
        connect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedXTop);
    aw = m_plot->axisWidget(QwtPlot::yLeft);
    if(aw)
        connect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedYLeft);
    aw = m_plot->axisWidget(QwtPlot::yRight);
    if(aw)
        connect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedYRight);
}
void QwtPlotPropertySetDialog::disChartConnect()
{
    QwtScaleWidget* aw = m_plot->axisWidget(QwtPlot::xBottom);
    if(aw)
        disconnect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedXBottom);
    aw = m_plot->axisWidget(QwtPlot::xTop);
    if(aw)
        disconnect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedXTop);
    aw = m_plot->axisWidget(QwtPlot::yLeft);
    if(aw)
        disconnect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedYLeft);
    aw = m_plot->axisWidget(QwtPlot::yRight);
    if(aw)
        disconnect(aw,&QwtScaleWidget::scaleDivChanged,this,&QwtPlotPropertySetDialog::onScaleDivChangedYRight);
}
void QwtPlotPropertySetDialog::addPlotSet(SA2DGraph* plot)
{
    m_enableSet = false;
    //图表参数
    QtVariantProperty *pro = nullptr;
    QtProperty *groupItem = m_propertyGroup->addProperty(tr("figure information"));//图表参数
    m_property_id.rememberTheProperty(ID_GroupPlot,groupItem);
    m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::String
                                            ,groupItem,tr("figure title"),ID_PlotTitle,m_plot->title().text());//图表标题
    m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::String
                                            ,groupItem,tr("figure footer"),ID_PlotFooter,m_plot->footer().text());//脚标
    m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::Color
                                            ,groupItem,tr("canvas background"),ID_PlotCanvasBackground,m_plot->canvasBackground().color());//画布背景
//    m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::Bool
//                                            ,groupItem,tr("enable zoomer scroll"),ID_PlotEnableZoomerScroll,m_plot->isEnableZoomerScroll());//缩放滚动条

    //坐标轴设置

    pro = m_property_id.addVariantPropertyInGroup(m_variantManager,QtVariantPropertyManager::flagTypeId()
                                                  ,groupItem
                                                  ,tr("axis set")//坐标轴设置
                                                  ,ID_PlotAxisSet,getAxisEnable(plot));
    pro->setAttribute(QLatin1String("flagNames"), QStringList()
                      <<tr("x bottom")//x底
                      <<tr("x top")//x顶
                      <<tr("y left")
                      <<tr("y right")
                      );//在setAttribute过程中会触发onPropertyValueChanged，而onPropertyValueChanged的值是0,需要使用m_enableAxisEnableSet来抑制
    pro->setValue(getAxisEnable(plot));

    //轴详细设置
    addAxisSet(groupItem,plot,QwtPlot::xBottom);
    addAxisSet(groupItem,plot,QwtPlot::yLeft);
    addAxisSet(groupItem,plot,QwtPlot::xTop);
    addAxisSet(groupItem,plot,QwtPlot::yRight);
    m_property->addProperty(groupItem);
    m_enableSet = true;
}

void QwtPlotPropertySetDialog::addAxisSet(QtProperty* parentGroup, SA2DGraph* plot,QwtPlot::Axis axis)
{
    m_enableSet = false;
    QtVariantProperty *pro = nullptr;
    //QwtScaleWidget* scaleWidget = plot->axisWidget(axis);
    QwtScaleDraw *scaleDraw = plot->axisScaleDraw(axis);
    QwtDateScaleDraw* dateScale = dynamic_cast<QwtDateScaleDraw*>(scaleDraw);
    //轴详细设置
    QtProperty *childGroup = m_property_id.addGroupInGroup(m_variantManager,parentGroup
                                                           ,tr("%1 axis set").arg(axisString(axis))//%1轴设置
                                                           ,axisPropertyID(axis,0));
    childGroup->setEnabled(plot->axisEnabled(axis));//在轴允许时才让设置

    QwtInterval inv= plot->axisInterval(axis);

    m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::PointF
                                            ,childGroup
                                            ,tr("axis range")//轴范围
                                            ,axisPropertyID(axis,4)
                                            ,QPointF(inv.minValue(),inv.maxValue()));

    m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::String
                                            ,childGroup,tr("title")//
                                            ,axisPropertyID(axis,1)
                                            ,m_plot->axisTitle(axis).text());
    //
    m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::Double
                                            ,childGroup,tr("label angle")//刻度文字角度
                                            ,axisPropertyID(axis,5)
                                            ,scaleDraw->labelRotation());
    pro = m_property_id.addVariantPropertyInGroup(m_variantManager,QtVariantPropertyManager::enumTypeId()
                                                  ,childGroup
                                                  ,tr("%1 axis style").arg(axisString(axis))//%1轴样式
                                                  ,axisPropertyID(axis,2)
                                                  ,0);
    pro->setAttribute(QLatin1String("enumNames"),QStringList()
                      <<tr("normal")//正常坐标轴
                      <<tr("date time scale")//时间坐标轴
                      );
    ScaleDraw sd = NormalScale;
    sd = ((dateScale == nullptr) ? NormalScale : DateScale);
    pro->setValue(int(sd));

    pro = m_property_id.addVariantPropertyInGroup(m_variantManager,QtVariantPropertyManager::enumTypeId()
                                                  ,childGroup
                                                  ,tr("date time format")
                                                  ,axisPropertyID(axis,3)
                                                  ,int(SA2DGraph::hh_mm_ss));
    pro->setAttribute(QLatin1String("enumNames"), QStringList()
                      <<QString("h_m")
                      <<QString("hh_mm")
                      <<QString("h_m_s")
                      <<QString("hh_mm_ss")
                      <<QString("yyyy_M_d")
                      <<QString("yyyy_M_d_h_m")
                      <<QString("yyyy_M_d_h_m_s")
                      <<QString("yyyy_MM_dd")
                      <<QString("yyyy_MM_dd_hh_mm")
                      <<QString("yyyy_MM_dd_hh_mm_ss")
                      );//在setAttribute过程中会触发onPropertyValueChanged，而onPropertyValueChanged的值是0,需要使用m_enableAxisEnableSet来抑制
    pro->setValue(int(SA2DGraph::hh_mm_ss));
    pro->setEnabled(sd == DateScale);

    m_enableSet = true;
}

QString QwtPlotPropertySetDialog::axisString(QwtPlot::Axis axis) const
{
    switch(axis)
    {
    case QwtPlot::xBottom:return tr("x bottom");//x底
    case QwtPlot::xTop:return tr("x top");
    case QwtPlot::yLeft:return tr("y left");
    case QwtPlot::yRight:return tr("y right");
    default:
        break;
    }
    return QString("");
}

int QwtPlotPropertySetDialog::axisPropertyID(QwtPlot::Axis axis, int order) const
{
    if(0 == order)
    {
        switch(axis)
        {
            case QwtPlot::xBottom:return ID_GroupXBottomSet;
            case QwtPlot::xTop:return ID_GroupXTopSet;
            case QwtPlot::yLeft:return ID_GroupyLeftSet;
            case QwtPlot::yRight:return ID_GroupyRightSet;
            default:break;
        }
    }
    else if(1 == order)
    {
        switch(axis)
        {
            case QwtPlot::xBottom:return ID_XBottomTitle;
            case QwtPlot::xTop:return ID_XTopTitle;
            case QwtPlot::yLeft:return ID_YLeftTitle;
            case QwtPlot::yRight:return ID_YRightTitle;
            default:break;
        }
    }
    else if(2 == order)
    {
        switch(axis)
        {
            case QwtPlot::xBottom:return ID_XBottomScale;
            case QwtPlot::xTop:return ID_XTopScale;
            case QwtPlot::yLeft:return ID_YLeftScale;
            case QwtPlot::yRight:return ID_YRightScale;
            default:break;
        }
    }
    else if(3 == order)
    {
        switch(axis)
        {
            case QwtPlot::xBottom:return ID_XBottomDateScaleType;
            case QwtPlot::xTop:return ID_XTopDateScaleType;
            case QwtPlot::yLeft:return ID_YLeftDateScaleType;
            case QwtPlot::yRight:return ID_YRightDateScaleType;
            default:break;
        }
    }
    else if(4==order)
    {
        switch(axis)
        {
            case QwtPlot::xBottom:return ID_XBottomInterval;
            case QwtPlot::xTop:return ID_XTopInterval;
            case QwtPlot::yLeft:return ID_YLeftInterval;
            case QwtPlot::yRight:return ID_YRightInterval;
            default:break;
        }
    }
    else if(5==order)
    {
        switch(axis)
        {
            case QwtPlot::xBottom:return ID_XBottomLabelRotation;
            case QwtPlot::xTop:return ID_XTopLabelRotation;
            case QwtPlot::yLeft:return ID_YLeftLabelRotation;
            case QwtPlot::yRight:return ID_YRightLabelRotation;
            default:break;
        }
    }
    return -1;
}

void QwtPlotPropertySetDialog::addCurveSet(SA2DGraph* plot)
{
    QList<QwtPlotCurve*> curs = plot->getCurveList();
    for (auto ite = curs.begin();ite != curs.end();++ite)
    {
        addCurveSet(*ite);
    }
}

QString QwtPlotPropertySetDialog::penStyle2Chinese(const Qt::PenStyle penStyle)
{
    switch(penStyle){
    case Qt::SolidLine:
        return tr("solid line");//实线
    case Qt::DashLine:
        return tr("dash line");//虚线
    case Qt::DotLine:
        return tr("dot line");//DotLine
    case Qt::DashDotLine:
        return tr("dash dot line");//点划线
    case Qt::DashDotDotLine:
        return tr("dash dot dot line");//双点划线
    default:break;
    }
    return QString();
}
Qt::PenStyle QwtPlotPropertySetDialog::Chinese2PenStyle(const QString str)
{
    if (str == tr("Solid Line"))//实线
    {
        return Qt::SolidLine;
    }
    else if(str == tr("Dash Line"))
    {
        return Qt::DashLine;
    }
    else if (str == tr("Dot Line"))//点线
    {
        return Qt::DotLine;
    }
    else if (str == tr("Dash Dot Line"))//点划线
    {
        return Qt::DashDotLine;
    }
    else if (str == tr("Dash Dot Dot Line"))//双点划线
    {
        return Qt::DashDotDotLine;
    }
    return Qt::NoPen;
}



void QwtPlotPropertySetDialog::setChartPtr(SA2DGraph *chart)
{
    if(m_plot)//如果已经关联了图表，需要先接触连接
        disChartConnect();
    m_plot = chart;
    buildConnect();
}

void QwtPlotPropertySetDialog::updateAxisValue(QwtPlot::Axis axis)
{
    QwtInterval inv= m_plot->axisInterval(axis);
    //QwtScaleWidget* scaleWidget = m_plot->axisWidget(axis);
    QwtScaleDraw *scaleDraw = m_plot->axisScaleDraw(axis);
    QwtDateScaleDraw* dateScale = dynamic_cast<QwtDateScaleDraw*>(scaleDraw);
    m_property_id.setVarPropertyData(axisPropertyID(axis,4)
                                     ,QPointF(inv.minValue(),inv.maxValue()));
    m_property_id.setVarPropertyData(axisPropertyID(axis,1)
                                     ,m_plot->axisTitle(axis).text());
    ScaleDraw sd = NormalScale;
    sd = ((dateScale == nullptr) ? NormalScale : DateScale);
    m_property_id.setVarPropertyData(axisPropertyID(axis,2),int(sd));
    m_property_id.setVarPropertyData(axisPropertyID(axis,5),scaleDraw->labelRotation());
}

void QwtPlotPropertySetDialog::updateCurveValue(QwtPlotCurve *cur)
{
    int baseID = getCurveBaseID(cur);
    m_property_id.setVarPropertyData(makeCurvePropertyID(baseID,ID_CurveTitle)
                                     ,cur->title().text());
    const QPen pen = cur->pen();
    m_property_id.setVarPropertyData(makeCurvePropertyID(baseID,ID_CurveColore)
                                     ,pen.color());
    m_property_id.setVarPropertyData(makeCurvePropertyID(baseID,ID_CurveWidth)
                                     ,pen.width());
    m_property_id.setVarPropertyData(makeCurvePropertyID(baseID,ID_CurvePenStyle)
                                     ,penStyle2Order(pen.style()));
}

void QwtPlotPropertySetDialog::addCurveSet(QwtPlotCurve* curve)
{
    m_enableSet = false;
    int curveID = makeCurveBaseID(curve);
    QString name = tr("%1-figure information").arg(curve->title().text());//%1-图表参数
    QtProperty *groupItem = m_propertyGroup->addProperty(name);
    QtVariantProperty *pro = nullptr;

    m_property_id.rememberTheProperty(curveID,groupItem);
    pro = m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::String
                                                  ,groupItem,tr("title")//标题
                                                  ,makeCurvePropertyID(curveID,ID_CurveTitle)
                                                  ,curve->title().text());

    const QPen pen = curve->pen();
    pro=  m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::Color
                                                  ,groupItem,tr("color")//颜色
                                                  ,makeCurvePropertyID(curveID,ID_CurveColore)
                                                  ,pen.color());

    pro = m_property_id.addVariantPropertyInGroup(m_variantManager,QVariant::Int
                                                  ,groupItem,tr("line width")//线宽
                                                  ,makeCurvePropertyID(curveID,ID_CurveWidth)
                                                  ,pen.width());
    pro->setAttribute(QString("singleStep"), 1);

    pro = m_property_id.addVariantPropertyInGroup(m_variantManager,QtVariantPropertyManager::enumTypeId()
                                                  ,groupItem,tr("line style")//线形
                                                  ,makeCurvePropertyID(curveID,ID_CurvePenStyle)
                                                  ,penStyle2Order(pen.style()));
    pro->setAttribute(QLatin1String("enumNames"),QStringList()
                      <<tr("Solid Line")//实线
                      <<tr("Dash Line")//虚线
                      <<tr("Dot Line")//点线
                      <<tr("Dash Dot Line")//点划线
                      <<tr("Dash Dot Dot Line")//双点划线
                      );
    m_property->addProperty(groupItem);
    m_enableSet = true;
}



int QwtPlotPropertySetDialog::penStyle2Order(Qt::PenStyle style) const
{
    switch (style) {
        case Qt::SolidLine:return 0;
        case Qt::DashLine:return 1;
        case Qt::DotLine:return 2;
        case Qt::DashDotLine:return 3;
        case Qt::DashDotDotLine:return 4;
        default:
            return 0;
    }
    return 0;
}

Qt::PenStyle QwtPlotPropertySetDialog::order2penStyle(int order) const
{
    switch (order) {
        case 0:return Qt::SolidLine;
        case 1:return Qt::DashLine;
        case 2:return Qt::DotLine;
        case 3:return Qt::DashDotLine;
        case 4:return Qt::DashDotDotLine;
        default:
            return Qt::SolidLine;
    }
    return Qt::SolidLine;
}
///
/// \brief 获取坐标轴设置对应的flagTypeId，QtVariantPropertyManager里，flagTypeId用位来标示
/// \param plot
/// \return
///
int QwtPlotPropertySetDialog::getAxisEnable(QwtPlot* plot) const
{
    int res = 0;
    if(plot->axisEnabled(QwtPlot::xBottom))
        res |= xBottomFlag;
    if(plot->axisEnabled(QwtPlot::xTop))
        res |= xTopFlag;
    if(plot->axisEnabled(QwtPlot::yLeft))
        res |= yLeftFlag;
    if(plot->axisEnabled(QwtPlot::yRight))
        res |= yRightFlag;
    return res;
}







QwtPlotCurve* QwtPlotPropertySetDialog::getCurvePtr(int id)
{
    int curid = getCurveID(id);
    auto i = m_baseID2curve.find(curid);
    if(i == m_baseID2curve.end())
        return nullptr;
    return i.value();
}

int QwtPlotPropertySetDialog::getCurveBaseID(QwtPlotCurve *cur)
{
    auto i = m_curve2baseID.find(cur);
    if(i == m_curve2baseID.end())
        return -1;
    return i.value();
}

void QwtPlotPropertySetDialog::setAxisScaleDrawStyle(QwtPlot::Axis axis, const QVariant& value)
{
    ScaleDraw sType = ScaleDraw( value.toInt());//强制转换，因为已经设置好枚举的值，所以这里可以进行强制转换
    QwtScaleDraw *scaleDraw = m_plot->axisScaleDraw(axis);
    QwtDateScaleDraw* dateScale = dynamic_cast<QwtDateScaleDraw*>(scaleDraw);
    QtProperty * property = m_property_id.getProperty(axisPropertyID(axis,3));

    if(NormalScale == sType)
    {
        if(property)
            property->setEnabled(false);
        if(dateScale)
        {//说明当前是时间坐标轴
            QwtScaleDraw* scale = nullptr;
            scale = new QwtScaleDraw;
            m_plot->setAxisScaleDraw(axis,scale);
        }
    }
    else if(DateScale == sType)
    {
        SA2DGraph::AxisDateScaleType st(SA2DGraph::hh_mm_ss);
        if(property)
        {
            property->setEnabled(true);
            st = SA2DGraph::AxisDateScaleType( static_cast<QtVariantProperty*>(property)->value().toInt() );
        }
        if(!dateScale)
        {//说明当前不是时间坐标轴
            m_plot->setAxisDateTimeScale(st,axis,QwtDate::Second);
        }
    }
}

void QwtPlotPropertySetDialog::itemAttached(QwtPlotItem *plotItem, bool on)
{
    if(plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
    {
        QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(plotItem);
        //说明曲线有删减或添加
        if(on)
        {
            //曲线添加
            addCurveSet(cur);
        }
        else
        {
            //曲线删除
            int id = getCurveBaseID(cur);
            if(-1 == id)
                return;
            m_property_id.deleteProperty(id);
        }
    }
}

void QwtPlotPropertySetDialog::setAxisEnable(int axisSet)
{
    m_plot->enableAxis(QwtPlot::xBottom,axisSet & xBottomFlag);
    m_property_id.getProperty(ID_GroupXBottomSet)->setEnabled(axisSet & xBottomFlag);

    m_plot->enableAxis(QwtPlot::xTop,axisSet & xTopFlag);
    m_property_id.getProperty(ID_GroupXTopSet)->setEnabled(axisSet & xTopFlag);

    m_plot->enableAxis(QwtPlot::yLeft,axisSet & yLeftFlag);
    m_property_id.getProperty(ID_GroupyLeftSet)->setEnabled(axisSet & yLeftFlag);

    m_plot->enableAxis(QwtPlot::yRight,axisSet & yRightFlag);
    m_property_id.getProperty(ID_GroupyRightSet)->setEnabled(axisSet & yRightFlag);
}

void QwtPlotPropertySetDialog::setAxisScale(int id, QPointF minMax, double step)
{
    switch (id) {
        case ID_XBottomInterval:
            m_plot->setAxisScale(QwtPlot::xBottom,minMax.x(),minMax.y(),step);
            return;
        case ID_XTopInterval:
            m_plot->setAxisScale(QwtPlot::xTop,minMax.x(),minMax.y(),step);
            return;
        case ID_YLeftInterval:
            m_plot->setAxisScale(QwtPlot::yLeft,minMax.x(),minMax.y(),step);
            return;
        case ID_YRightInterval:
            m_plot->setAxisScale(QwtPlot::yRight,minMax.x(),minMax.y(),step);
            return;
    }
}

void QwtPlotPropertySetDialog::setAxisTitle(int id, const QString& title)
{
    switch (id) {
        case ID_XBottomTitle:
            m_plot->setAxisTitle(QwtPlot::xBottom,title);
            return;
        case ID_XTopTitle:
            m_plot->setAxisTitle(QwtPlot::xTop,title);
            return;
        case ID_YLeftTitle:
            m_plot->setAxisTitle(QwtPlot::yLeft,title);
            return;
        case ID_YRightTitle:
            m_plot->setAxisTitle(QwtPlot::yRight,title);
            return;
    }
}

void QwtPlotPropertySetDialog::setAxislabelRotation(int id, double r)
{
    switch (id) {
        case ID_XBottomLabelRotation:
            m_plot->setAxisLabelRotation(QwtPlot::xBottom,r);
            return;
        case ID_XTopLabelRotation:
            m_plot->setAxisLabelRotation(QwtPlot::xTop,r);
            return;
        case ID_YLeftLabelRotation:
            m_plot->setAxisLabelRotation(QwtPlot::yLeft,r);
            return;
        case ID_YRightLabelRotation:
            m_plot->setAxisLabelRotation(QwtPlot::yRight,r);
            return;
    }
}
void QwtPlotPropertySetDialog::onScaleDivChangedXBottom()
{
    updateAxisValue(QwtPlot::xBottom);
}
void QwtPlotPropertySetDialog::onScaleDivChangedXTop()
{
   updateAxisValue(QwtPlot::xTop);
}

void QwtPlotPropertySetDialog::onScaleDivChangedYLeft()
{
    updateAxisValue(QwtPlot::yLeft);
}

void QwtPlotPropertySetDialog::onScaleDivChangedYRight()
{
    updateAxisValue(QwtPlot::yRight);
}



void QwtPlotPropertySetDialog::onPropertyValueChanged(QtProperty * property, const QVariant & value)
{
    if(!m_enableSet)
        return;
    int id = m_property_id.getPropertyID(property);

    if (id < ID_CurveIDBase)
    {
        switch(id)
        {
            case ID_PlotTitle:
                m_plot->setTitle(value.toString());
                return;
            case ID_PlotFooter:
                m_plot->setFooter(value.toString());
                return;
            case ID_PlotCanvasBackground:
                m_plot->setCanvasBackground(QBrush(value.value<QColor>()));
                return;
//            case ID_PlotEnableZoomerScroll:
//                m_plot->enableZoomerScroll(value.toBool());
//                return;
            case ID_PlotAxisSet:
                setAxisEnable(value.toInt());
                return;
            case ID_XBottomScale:
                setAxisScaleDrawStyle(QwtPlot::xBottom,value);
                return;
            case ID_XTopScale:
                setAxisScaleDrawStyle(QwtPlot::xTop,value);
                return;
            case ID_YLeftScale:
                setAxisScaleDrawStyle(QwtPlot::yLeft,value);
                return;
            case ID_YRightScale:
                setAxisScaleDrawStyle(QwtPlot::yRight,value);
                return;
            case ID_XBottomDateScaleType:
                m_plot->setAxisDateFormat(QwtPlot::xBottom,SA2DGraph::AxisDateScaleType(value.toInt()));
                return;
            case ID_XTopDateScaleType:
                m_plot->setAxisDateFormat(QwtPlot::xTop,SA2DGraph::AxisDateScaleType(value.toInt()));
                return;
            case ID_YLeftDateScaleType:
                m_plot->setAxisDateFormat(QwtPlot::yLeft,SA2DGraph::AxisDateScaleType(value.toInt()));
                return;
            case ID_YRightDateScaleType:
                m_plot->setAxisDateFormat(QwtPlot::yRight,SA2DGraph::AxisDateScaleType(value.toInt()));
                return;
            case ID_XBottomInterval:
            case ID_XTopInterval:
            case ID_YLeftInterval:
            case ID_YRightInterval:
                setAxisScale(id,value.toPointF());
                return;
            case ID_XBottomTitle:
            case ID_XTopTitle:
            case ID_YLeftTitle:
            case ID_YRightTitle:
                setAxisTitle(id,value.toString());
                return;
            case ID_XBottomLabelRotation:
            case ID_XTopLabelRotation:
            case ID_YLeftLabelRotation:
            case ID_YRightLabelRotation:
                setAxislabelRotation(id,value.toDouble());
                return;

        }
    }
    else
    {
        QwtPlotCurve* cur = getCurvePtr(id);
        if(!cur)
            return;
        const int propertyID = getCurvePropertyID(id);

        if (ID_CurveTitle == propertyID)
        {
            cur->setTitle(value.toString());
            return;
        }

        QPen newPen(cur->pen());
        switch (propertyID) {
            case ID_CurveColore:
                newPen.setColor(value.value<QColor>());
                break;
            case ID_CurveWidth:
                newPen.setWidth(value.toInt());
                break;
            case ID_CurvePenStyle:
                newPen.setStyle(order2penStyle(value.toInt()));
                break;
        }
        cur->setPen(newPen);
    }
}
