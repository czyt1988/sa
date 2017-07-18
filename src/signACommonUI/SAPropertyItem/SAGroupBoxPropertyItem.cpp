#include "SAGroupBoxPropertyItem.h"
#include <QApplication>
#if QT_VERSION >= 0x040600
#include "ctkProxyStyle.h"
const QString groupbox_tree_mode_style =
        "QGroupBox::indicator {"
            "width: 16px;"
            "height: 16px;"
        "}"
        "QGroupBox::indicator:unchecked {"
            "image: url(:/icon/icons/colled.png);"
        "}"
        "QGroupBox::indicator:checked {"
            "image: url(:/icon/icons/expand.png);"
        "}"
    ;
//-----------------------------------------------------------------------------
class vCollapsibleGroupBoxStyle:public ctkProxyStyle
{
public:


    enum ShowMode{
        TreeMode
        ,CheckBoxMode
    };
    ShowMode groupBoxIndicatorMode;
    typedef ctkProxyStyle Superclass;
    vCollapsibleGroupBoxStyle(ShowMode mode,QStyle* style = 0, QObject* parent =0)
    : Superclass(style, parent)
    {
      groupBoxIndicatorMode = mode;
    }
    virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption * opt, QPainter * p, const QWidget * widget = 0) const
    {
    if (pe == QStyle::PE_IndicatorCheckBox)
      {
      const ctkCollapsibleGroupBox* groupBox= qobject_cast<const ctkCollapsibleGroupBox*>(widget);
      if (groupBox)
        {
          if(CheckBoxMode == groupBoxIndicatorMode)
          {
              this->Superclass::drawPrimitive(QStyle::PE_IndicatorCheckBox, opt, p, widget);
          }
          else
          {
            this->Superclass::drawPrimitive(groupBox->isChecked()
                                            ? QStyle::PE_IndicatorArrowDown : QStyle::PE_IndicatorArrowRight, opt, p, widget);
          }
          return;
        }
      }
    this->Superclass::drawPrimitive(pe, opt, p, widget);
    }
    virtual int pixelMetric(PixelMetric metric, const QStyleOption * option, const QWidget * widget) const
    {
    if (metric == QStyle::PM_IndicatorHeight)
      {
      const ctkCollapsibleGroupBox* groupBox= qobject_cast<const ctkCollapsibleGroupBox*>(widget);
      if (groupBox)
        {
        return groupBox->fontMetrics().height();
        }
      }
    return this->Superclass::pixelMetric(metric, option, widget);
    }
};
#endif

SAGroupBoxPropertyItem::SAGroupBoxPropertyItem(QWidget* par):ctkCollapsibleGroupBox(par)
{

}

SAGroupBoxPropertyItem::SAGroupBoxPropertyItem(const QString &title, QWidget *par)
    :ctkCollapsibleGroupBox(title,par)
{

}

SAGroupBoxPropertyItem::~SAGroupBoxPropertyItem()
{

}

void SAGroupBoxPropertyItem::setTreeMode(bool b)
{
#if QT_VERSION >= 0x040600
    vCollapsibleGroupBoxStyle::ShowMode mode = b ? vCollapsibleGroupBoxStyle::TreeMode : vCollapsibleGroupBoxStyle::CheckBoxMode;
    QWidget* parent = parentWidget();
    QStyle* parentStyle = (parent) ? parent->style() : qApp->style();
    QStyle* groupBoxStyle = new vCollapsibleGroupBoxStyle(mode,parentStyle, qApp);
    setStyle(groupBoxStyle);
#else
    if(b)
    {
        setStyleSheet(groupbox_tree_mode_style);
    }
    else
    {
        setStyleSheet("");
    }
#endif
}

