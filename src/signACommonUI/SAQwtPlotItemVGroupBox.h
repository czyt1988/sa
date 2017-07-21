#ifndef SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#define SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#include "SAVCollapsibleGroupBox.h"
#include "qwt_symbol.h"
class QwtPlotItem;
class QwtPlotCurve;
class SAPenSetWidget;
class QCheckBox;
class SADoubleSpinBoxPropertyItem;
class SAQwtSymbolComboBox;
class QPushButton;
class QLabel;

///
/// \brief 适用于QwtPlotItem的VGroupBox
///
class SAQwtPlotItemVGroupBox : public SAVCollapsibleGroupBox
{
    Q_OBJECT
public:

    SAQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget* par = nullptr);
    ~SAQwtPlotItemVGroupBox();
    QwtPlotItem *getPlotItem() const;
    //生产QwtPlotItemVGroupBox的工厂函数
    static SAQwtPlotItemVGroupBox* createQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget* par=nullptr);
private:
    class UI;
    SAQwtPlotItemVGroupBox::UI* ui;
};


///
/// \brief 针对QwtCurveItem的group box
///
class SAQwtPlotCurveItemVGroupBox : public SAQwtPlotItemVGroupBox
{
    Q_OBJECT
public:
    SAQwtPlotCurveItemVGroupBox(QwtPlotCurve *plotItem, QWidget* par = nullptr);
    void showAll(bool enable=true);
protected:
    void retranslateUi();
    void updateData();
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

#endif // SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
