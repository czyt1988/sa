#ifndef QWTCHARTSETDIALOG_H
#define QWTCHARTSETDIALOG_H
#include "SAChartGlobals.h"

#include <QWidget>
#include "qttreepropertybrowser.h"
#include <QtWidgets/QHBoxLayout>
#include "SAChart2D.h"
#include "qtpropertymanager.h"
#include <QtGroupPropertyManager>
#include <QtVariantPropertyManager>
#include <QtVariantEditorFactory>
#include <QtColorPropertyManager>
#include <QList>
#include <QMap>
#include <QSizeF>
#include "QtPropertyIDStorage.h"
class QwtPlotItem;
class SA_CHART_EXPORT QwtPlotPropertySetDialog : public QWidget
{
	Q_OBJECT
public:
    QwtPlotPropertySetDialog(QWidget *parent,SAChart2D* plot);
    void updateAllValue();
    void setChartPtr(SAChart2D * chart);
    ~QwtPlotPropertySetDialog();
private:
    enum ScaleDraw{
        NormalScale=0
        ,DateScale=1
    };
    void updateAxisValue(QwtPlot::Axis axis);
    void updateCurveValue(QwtPlotCurve* cur);
	SAChart2D *m_plot;
	QtTreePropertyBrowser *m_property;
	QHBoxLayout *m_mainHorizontalLayout;
	QtGroupPropertyManager* m_propertyGroup;
	QtVariantPropertyManager *m_variantManager;
	QtVariantEditorFactory *m_variantFactory;

    QtPropertyIdStorage<int> m_property_id;
    QMap<int,QwtPlotCurve*> m_baseID2curve;
    QMap<QwtPlotCurve*,int> m_curve2baseID;
	void createUI();
    void buildConnect();
    void disChartConnect();
	void addPlotSet(SAChart2D* plot);
	void addCurveSet(SAChart2D* plot);
	void addCurveSet(QwtPlotCurve* curve);
    void addAxisSet(QtProperty *parentGroup,SAChart2D* plot,QwtPlot::Axis axis);
    QString axisString(QwtPlot::Axis axis) const;
    int axisPropertyID(QwtPlot::Axis axis,int order) const;
    int penStyle2Order(Qt::PenStyle style) const;
    Qt::PenStyle order2penStyle(int order) const;


    int getAxisEnable(QwtPlot* plot) const;
    static QString penStyle2Chinese(const Qt::PenStyle penStyle);
    static Qt::PenStyle Chinese2PenStyle(const QString str);
    bool m_enableSet;


    void setAxisScale(int id, QPointF minMax, double step=0);
    void setAxisTitle(int id, const QString& title);
    void setAxislabelRotation(int id, double r);
    int getCurvePropertyID(int id);
    int getCurveID(int curvePropertyID);
    int makeCurveBaseID(QwtPlotCurve* cur);
    int makeCurvePropertyID(int CurveBaseID,int curvePropertyID);
    QwtPlotCurve* getCurvePtr(int id);
    int getCurveBaseID(QwtPlotCurve* cur);
private slots:
	void onPropertyValueChanged(QtProperty * property, const QVariant & value);

    void setAxisScaleDrawStyle(QwtPlot::Axis axis,const QVariant & value);
    void itemAttached (QwtPlotItem *plotItem, bool on);
    void setAxisEnable(int axisSet);

    void onScaleDivChangedXBottom();
    void onScaleDivChangedXTop();
    void onScaleDivChangedYLeft();
    void onScaleDivChangedYRight();
};

#endif // QWTCHARTSETDIALOG_H
