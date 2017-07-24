#ifndef SAQWTPLOTCURVEITEMSETWIDGET_H
#define SAQWTPLOTCURVEITEMSETWIDGET_H
#include "SAQwtPlotItemSetWidget.h"
#include "qwt_symbol.h"
class QwtPlotCurve;
class SAPenSetWidget;
class QCheckBox;
class SAQwtSymbolComboBox;
class QPushButton;
class QLabel;
class SAQwtPlotCurveItemSetWidget : public SAQwtPlotItemSetWidget
{
    Q_OBJECT
public:
    SAQwtPlotCurveItemSetWidget(QwtPlotCurve *plotItem, QWidget* par = nullptr);
    virtual void retranslateUi();
    virtual void upDateData(bool downLoad = true);
    void showAll(bool enable=true);
private slots:
    Q_SLOT void onPenChenged(const QPen& pen);
    Q_SLOT void onSPlineCheckBoxStateChanged(int state);
    Q_SLOT void onBaseLineSpinBoxValueChanged(double v);
    Q_SLOT void onSymbolComboBoxChanged(QwtSymbol::Style style);
    Q_SLOT void onSymbolSetButtonClicked();
private:
    bool m_showAll;
    QwtPlotCurve *m_curveItem;
    SAPenSetWidget* m_PenStyle;
    SADoubleSpinBoxPropertyItem* m_baseLineSpinBox;
    QCheckBox* m_isSPlineCheckBox;///< 样条线
    SAQwtSymbolComboBox* m_symbolComboBox;///< 符号选择复选框
    QPushButton* m_symbolSetButton;///< 符号的详细设置按钮
    QLabel* m_labelSymbolSet;///<
};

#endif // SAQWTPLOTCURVEITEMSETWIDGET_H
