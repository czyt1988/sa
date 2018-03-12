#ifndef SAPLOTDATAMODEL_H
#define SAPLOTDATAMODEL_H
#include "SACommonUIGlobal.h"
#include "QwtPlotItemDataModel.h"

class SA_COMMON_UI_EXPORT SAPlotDataModel : public QwtPlotItemDataModel
{
    Q_OBJECT
public:
    SAPlotDataModel(QObject *p = 0);
};

#endif // SAPLOTDATAMODEL_H
