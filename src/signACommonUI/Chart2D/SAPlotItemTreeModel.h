#ifndef SAPLOTITEMTREEMODEL_H
#define SAPLOTITEMTREEMODEL_H
#include "QwtPlotItemTreeModel.h"
#include "SACommonUIGlobal.h"
class SA_COMMON_UI_EXPORT SAPlotItemTreeModel : public QwtPlotItemTreeModel
{
    Q_OBJECT
public:
    SAPlotItemTreeModel(QObject * parent = 0);
protected:
    virtual QList<QwtPlotItem*> getItems(QwtPlot* plot) const;
};

#endif // SAPLOTITEMTREEMODEL_H
