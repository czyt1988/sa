#ifndef DIALOG_ADDCHART_H
#define DIALOG_ADDCHART_H

#include <QDialog>
#include <QStandardItemModel>
#include <qtableview.h>
#include <qtreeview.h>

#include <qtpropertymanager.h>
#include <qtvariantproperty.h>
#include "MyEasyProperty.h"
#include <qttreepropertybrowser.h>
#include <QList>
#include <qvector.h>
#include <QPointF>
#include <memory>
#include <qwt_plot_curve.h>
#include <QComboBox>

#include <qwt_plot.h>

class SAChart2D;
class SAValueManagerModel;

namespace Ui {
class Dialog_AddChart;
}
class SAAbstractDatas;
class Dialog_AddChart : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_AddChart(QWidget *parent = 0);
    ~Dialog_AddChart();
    //void setOriginalFolder(const SAModelItem* dataItem);
    QList<QwtPlotCurve*> getDrawCurveList();
    QString isAxisDateTime(bool* is,QwtPlot::Axis ax);
    const SAChart2D* chart();
private:
    Ui::Dialog_AddChart *ui;
	MyEasyProperty m_easyProperty;
	QtVariantPropertyManager* m_pVarManager_NotEdit;
    //QStandardItemModel* m_dataModel;
    QStandardItemModel* m_comboModel;
	int m_lineCount;
	double m_start;
	double m_detal;

private slots:
	void on_treeView_clicked(const QModelIndex & index);


	void on_pushButto_add_clicked();
	void on_pushButton_del_clicked();
	void on_pushButton_finish_clicked();

    void on_pushButton_clearXY_clicked();

    void on_checkBox_isSelDefX_toggled(bool checked);

    void on_checkBox_isSelDefY_toggled(bool checked);

private:
    SAValueManagerModel* getValueModel();
    void showProperty(QModelIndex index);
    QList<QtProperty*> getDataTreeProperty(SAAbstractDatas* baseData);
    void getBaseDataProperty(SAAbstractDatas* baseData,QList<QtProperty*>& propertys);
    void addTableRow(const QwtPlotCurve* cur
                     ,const QString& dataGroup
                     ,const QString& DataType
                     ,size_t dataNum
                     );


	static void vectorData2VectorPoint(const QVector<QVariant>& in_X,const QVector<QVariant>& in_Y, QVector<QPointF>& res);
	static QColor getRandColor();

    bool getSeries(SAAbstractDatas* series, QVector<double>& datas,bool isTime,const QString& timeFormat);
    QwtPlotCurve* drawChart(SAAbstractDatas* x,SAAbstractDatas* y);
    QwtPlotCurve* drawChart(SAAbstractDatas* data,bool isXSelDef,double start,double detal);
	void ui_setCombox_TimeFormat(QComboBox* comb);
	void noSelfDefineSeriesSelectDraw();
    //void haveSelfDefineSeriesSelectDraw();
    void haveXSelfDefineSeriesSelectDraw();
    void haveYSelfDefineSeriesSelectDraw();
};

#endif // DIALOG_ADDCHART_H
