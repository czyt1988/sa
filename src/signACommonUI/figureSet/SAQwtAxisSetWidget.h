#ifndef SAQWTAXISSETWIDGET_H
#define SAQWTAXISSETWIDGET_H

#include <QWidget>
class QwtPlot;
class QButtonGroup;
namespace Ui {
class SAQwtAxisSetWidget;
}

class SAQwtAxisSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAQwtAxisSetWidget(QWidget *parent = 0);
    ~SAQwtAxisSetWidget();
    QwtPlot *getChart() const;
    void setChart(QwtPlot *chart,int axisID);
    void updateAxisValue();
private slots:
    Q_SLOT void onEnableCheckBoxClicked(int state);
    Q_SLOT void onLineEditTextChanged(const QString& text);
    Q_SLOT void onAxisFontChanged(const QFont& font);
    Q_SLOT void onAxisLabelAligmentChanged(Qt::Alignment al);
    Q_SLOT void onAxisLabelRotationChanged(double v);
    Q_SLOT void onAxisMarginValueChanged(int v);
    Q_SLOT void onAxisMaxScaleChanged(double v);
    Q_SLOT void onAxisMinScaleChanged(double v);
    Q_SLOT void onCharDestroy(QObject* obj);
    Q_SLOT void onScaleDivChanged();
    Q_SLOT void onScaleStyleChanged(int id);
private:
    void updateUI();
private:
    enum ScaleStyle
    {
        NormalScale
        ,DateTimeScale
    };

    void connectChartAxis();
    void disconnectChartAxis();
private:
    Ui::SAQwtAxisSetWidget *ui;
    QwtPlot* m_chart;
    QButtonGroup* m_buttonGroup;
    int m_axisID;
};

#endif // SAQWTAXISSETWIDGET_H
