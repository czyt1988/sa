#ifndef SA2DGRAPH_H
#define SA2DGRAPH_H
#include "SAChartGlobals.h"

//qt
#include <QMap>
#include <QPointF>
#include <qevent.h>
#include <qglobal.h>
#include <qscrollbar.h>
#include <QRectF>
#include <QList>
//qwt
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include <qwt_symbol.h>
#include <qwt_date.h>
#include <qwt_plot_legenditem.h>
#include <qwt_legend.h>
#include <qwt_date.h>
#include <qwt_plot_histogram.h>

#include <SAMarkSymbol.h>
#include "SAPlotZoomer.h"
//class ScrollBar;
//class ScrollZoomer;
class QwtDateScaleDraw;

class ScrollData;
class SAYDataTracker;
class SAXYDataTracker;
class SA2DGraphPrivate;

class SA_CHART_EXPORT ScrollBar: public QScrollBar
{
    Q_OBJECT
public:
    ScrollBar( QWidget *parent = NULL );
    ScrollBar( Qt::Orientation, QWidget *parent = NULL );
    ScrollBar( double minBase, double maxBase,
        Qt::Orientation o, QWidget *parent = NULL );

    void setInverted( bool );
    bool isInverted() const;

    double minBaseValue() const;
    double maxBaseValue() const;

    double minSliderValue() const;
    double maxSliderValue() const;

    int extent() const;

Q_SIGNALS:
    void sliderMoved( Qt::Orientation, double, double );
    void valueChanged( Qt::Orientation, double, double );

public Q_SLOTS:
    virtual void setBase( double min, double max );
    virtual void moveSlider( double min, double max );

protected:
    void sliderRange( int value, double &min, double &max ) const;
    int mapToTick( double ) const;
    double mapFromTick( int ) const;

private Q_SLOTS:
    void catchValueChanged( int value );
    void catchSliderMoved( int value );

private:
    void init();

    bool d_inverted;
    double d_minBase;
    double d_maxBase;
    int d_baseTicks;
};

class SA_CHART_EXPORT ScrollZoomer: public QwtPlotZoomer
{
    Q_OBJECT
public:
    enum ScrollBarPosition
    {
        AttachedToScale,
        OppositeToScale
    };
    ScrollZoomer (int xAxis, int yAxis, QWidget *);
    ScrollZoomer( QWidget * );
    virtual ~ScrollZoomer();

    ScrollBar *horizontalScrollBar() const;
    ScrollBar *verticalScrollBar() const;

    void setHScrollBarMode( Qt::ScrollBarPolicy );
    void setVScrollBarMode( Qt::ScrollBarPolicy );

    Qt::ScrollBarPolicy vScrollBarMode () const;
    Qt::ScrollBarPolicy hScrollBarMode () const;

    void setHScrollBarPosition( ScrollBarPosition );
    void setVScrollBarPosition( ScrollBarPosition );

    ScrollBarPosition hScrollBarPosition() const;
    ScrollBarPosition vScrollBarPosition() const;

    QWidget* cornerWidget() const;
    virtual void setCornerWidget( QWidget * );

    virtual bool eventFilter( QObject *, QEvent * );

    virtual void rescale();
    bool isEnableScrollBar() const;
public slots:
    void on_enable_scrollBar(bool enable);
protected:
    virtual ScrollBar *scrollBar( Qt::Orientation );
    virtual void updateScrollBars();
    virtual void layoutScrollBars( const QRect & );
private Q_SLOTS:
    void scrollBarMoved( Qt::Orientation o, double min, double max );

private:
    bool needScrollBar( Qt::Orientation ) const;
    int oppositeAxis( int ) const;

    QWidget *d_cornerWidget;

    ScrollData *d_hScrollData;
    ScrollData *d_vScrollData;

    bool d_inZoom;
    bool d_alignCanvasToScales[ QwtPlot::axisCnt ];
    bool d_isEnable;///< 标定是否显示滚动条
};


class SA_CHART_EXPORT Zoomer_qwt: public ScrollZoomer
{
    Q_OBJECT
    const unsigned int c_rangeMax;
public:
    Zoomer_qwt(int xAxis, int yAxis, QWidget* canvas):
        ScrollZoomer(xAxis,yAxis, canvas )
      ,c_rangeMax(1000)
    {
        setRubberBandPen( QColor( Qt::darkGreen ) );
    }

    Zoomer_qwt( QWidget *canvas ):
        ScrollZoomer( canvas )
      ,c_rangeMax(1000)
    {
        setRubberBandPen( QColor( Qt::darkGreen ) );
    }
    virtual ~Zoomer_qwt(){}
    virtual void rescale()
    {
        QwtScaleWidget *scaleWidget = plot()->axisWidget( yAxis() );
        QwtScaleDraw *sd = scaleWidget->scaleDraw();

        double minExtent = 0.0;
        if ( zoomRectIndex() > 0 )
        {
            // When scrolling in vertical direction
            // the plot is jumping in horizontal direction
            // because of the different widths of the labels
            // So we better use a fixed extent.

            minExtent = sd->spacing() + sd->maxTickLength() + 1;
            minExtent += sd->labelSize(
                scaleWidget->font(), c_rangeMax ).width();
        }

        sd->setMinimumExtent( minExtent );

        ScrollZoomer::rescale();
    }
    virtual QwtText trackerTextF(const QPointF & pos) const
    {
        Q_UNUSED(pos);
        return QwtText();
    }
    virtual QRect trackerRect(const QFont & font) const
    {
        Q_UNUSED(font);
        return QRect();
    }
};



class SA_CHART_EXPORT LegendItem: public QwtPlotLegendItem
{
public:
	LegendItem()
	{
		setRenderHint( QwtPlotItem::RenderAntialiased );

		QColor color( Qt::white );

		setTextPen( color );

		setBorderPen( color );

		QColor c( Qt::gray );
		c.setAlpha( 200 );

		setBackgroundBrush( c );

	}
};

//class SAYDataTracker: public QwtPlotPicker
//{
//public:
//    SAYDataTracker( QWidget * );

//protected:
//	virtual QwtText trackerTextF( const QPointF & ) const;
//	virtual QRect trackerRect( const QFont & ) const;

//private:
//	QString curveInfoAt( const QwtPlotCurve *, const QPointF & ) const;
//	QLineF curveLineAt( const QwtPlotCurve *, double x ) const;
//};
///
/// \brief 2d绘图 class
///
class SA_CHART_EXPORT SA2DGraph : public QwtPlot
{
    Q_OBJECT
    SA_IMPL(SA2DGraph)
public:
    SA2DGraph(QWidget *parent = nullptr);
    virtual ~SA2DGraph();
    ///
    /// \brief getCureList 获取所有曲线
    /// \return
    ///
    QList<QwtPlotCurve*> getCurveList();
    //获取所有标记
    QList<QwtPlotMarker*> getMakerList();
    //设置为时间坐标轴
    QwtDateScaleDraw *setAxisDateTimeScale(const QString& format, int axisID, QwtDate::IntervalType intType = QwtDate::Second);
    QwtDateScaleDraw *setAxisDateTimeScale(int axisID);
    //坐标的极值
    double axisXmin(int axisId = QwtPlot::xBottom) const;
    double axisXmax(int axisId = QwtPlot::xBottom) const;
    double axisYmin(int axisId = QwtPlot::yLeft) const;
    double axisYmax(int axisId = QwtPlot::yLeft) const;
    //清除所有editor，如zoom，panner，cross等
    virtual void setEnableAllEditor(bool enable);
public slots:
	//功能性语句
    void enableZoomer(bool enable = true );
    //回到放大的最底栈
    void setZoomBase();
    //重置放大的基准
    void setZoomReset();
    //放大1.6 相当于乘以0.625
    void zoomIn();
    //缩小1.6 相当于乘以1.6
    void zoomOut();
    //缩放到最适合比例，就是可以把所有图都能看清的比例
    void zoomInCompatible();

    void enablePicker(bool enable = true );
	void enableGrid(bool isShow = true);
	void enableGridX(bool enable = true);
	void enableGridY(bool enable = true);
	void enableGridXMin(bool enable = true);
	void enableGridYMin(bool enable = true);

	void enablePanner(bool enable = true);

	void enableLegend(bool enable = true);
	void enableLegendPanel(bool enable = true);

    void markYValue(double data,const QString& strLabel, QColor clr = Qt::black,int type = 0);



	void showItem( const QVariant &itemInfo, bool on );

    void enableYDataPicker(bool enable = true);

    void enableXYDataPicker(bool enable = true);
signals:
	void enableZoomerChanged(bool enable);
	void enablePickerChanged(bool enable);
	void enableGridChanged(bool enable);
	void enableGridXChanged(bool enable);
	void enableGridYChanged(bool enable);
	void enableGridXMinChanged(bool enable);
	void enableGridYMinChanged(bool enable);
	void enablePannerChanged(bool enable);
	void enableLegendChanged(bool enable);
	void enableLegendPanelChanged(bool enable);
    void enableYDataPickerChanged(bool enable);
    void enableXYDataPickerChanged(bool enable);
public:
    bool isEnableZoomer() const;
    //是否允许十字光标
    bool isEnablePicker() const;
    bool isEnableGrid() const;
    bool isEnableGridX() const;
    bool isEnableGridY() const;
    bool isEnableGridXMin() const;
    bool isEnableGridYMin() const;
    bool isEnablePanner() const;
    bool isEnableLegend() const;
    bool isEnableLegendPanel() const;
    bool isEnableYDataPicker() const;
    bool isEnableXYDataPicker() const;
protected:
    virtual void resizeEvent( QResizeEvent * );
public:
    QwtPlotZoomer * zoomer();
    QwtPlotZoomer * zoomerSecond();
	///
	/// \brief 返回网格指针
	/// \return
	///
    QwtPlotGrid * grid();
protected:
	///
	/// \brief 设置网格
	/// \param color 网格的颜色
	/// \param width 网格线条的宽度
	/// \param style 网格的样式
	/// \param bShowX 显示x坐标
	/// \param bShowY 显示y坐标
	/// \return
	///
    QwtPlotGrid* setupGrid(
            const QColor & 	color = Qt::gray
            ,qreal 	width = 1.0
            ,Qt::PenStyle style = Qt::DotLine);
	///
	/// \brief 移除网格
	///
	void deleteGrid();
    ///
    /// \brief 建立缩放模式
    ///
    void setupZoomer();
    void deleteZoomer();
    void enableZoomer(QwtPlotZoomer* zoomer, bool enable);
    ///
    /// \brief 建立一个内置的picker
    ///
    void setupPicker();
	///
	/// \brief 建立一个鼠标中间画布拖动
	///
	void setupPanner();
	void deletePanner();
	///
	/// \brief 建立一个图例r
	///
	void setupLegend();
	void setuplegendPanel();
	void deletelegendPanel();

    void setupYDataPicker();
    void deleteYDataPicker();

    void setupXYDataPicker();
    void deleteXYDataPicker();
};


/*
 *

enum QwtPlotItem::RttiValues



Runtime type information.

RttiValues is used to cast plot items, without having to enable runtime type information of the compiler.



Enumerator



Rtti_PlotItem


Unspecific value, that can be used, when it doesn't matter.



Rtti_PlotGrid


For QwtPlotGrid.



Rtti_PlotScale


For QwtPlotScaleItem.



Rtti_PlotLegend


For QwtPlotLegendItem.



Rtti_PlotMarker


For QwtPlotMarker.



Rtti_PlotCurve


For QwtPlotCurve.



Rtti_PlotSpectroCurve


For QwtPlotSpectroCurve.



Rtti_PlotIntervalCurve


For QwtPlotIntervalCurve.



Rtti_PlotHistogram


For QwtPlotHistogram.



Rtti_PlotSpectrogram


For QwtPlotSpectrogram.



Rtti_PlotSVG


For QwtPlotSvgItem.



Rtti_PlotTradingCurve


For QwtPlotTradingCurve.



Rtti_PlotBarChart


For QwtPlotBarChart.



Rtti_PlotMultiBarChart


For QwtPlotMultiBarChart.



Rtti_PlotShape


For QwtPlotShapeItem.



Rtti_PlotTextLabel


For QwtPlotTextLabel.



Rtti_PlotZone


For QwtPlotZoneItem.



Rtti_PlotUserItem


Values >= Rtti_PlotUserItem are reserved for plot items not implemented in the Qwt library.


 */


#endif // CHARTWAVE_QWT_H
