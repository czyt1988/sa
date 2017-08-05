#ifndef SAQWTSYMBOLSETWIDGET_H
#define SAQWTSYMBOLSETWIDGET_H

#include <QWidget>
#include "qwt_symbol.h"
#include <QScopedPointer>
namespace Ui {
class SAQwtSymbolSetWidget;
}
///
/// \brief QwtSymbol的设置
/// 通过函数getSymbol获取最终的symbol样式
///
class SAQwtSymbolSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAQwtSymbolSetWidget(QWidget *parent = 0);
    explicit SAQwtSymbolSetWidget(const QwtSymbol* symbol,QWidget *parent = 0);
    ~SAQwtSymbolSetWidget();
    const QwtSymbol& getSymbol();
signals:
    void symbolSetChanged(const QwtSymbol& symbol);
private slots:
    Q_SLOT void onSymbolSelectChanged(QwtSymbol::Style style);
    Q_SLOT void onSpinBoxSizeXValueChanged(int v);
    Q_SLOT void onSpinBoxSizeYValueChanged(int v);
    Q_SLOT void onEdgePenChanged(const QPen &pen);
    Q_SLOT void onBrushColorChanged(const QColor &clr);
    Q_SLOT void onBrushAlphaSliderValueChanged(int v);
private:
    void init();
    void updateSymbol();
private:
    Ui::SAQwtSymbolSetWidget *ui;
    bool m_signalLock;
    QwtSymbol m_symbol;
};

#endif // SAQWTSYMBOLSETWIDGET_H
