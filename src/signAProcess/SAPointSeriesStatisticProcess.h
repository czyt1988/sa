#ifndef SAPOINTSERIESSTATISTICPROCESS_H
#define SAPOINTSERIESSTATISTICPROCESS_H
#include "SAAbstractProcess.h"
class SAPointSeriesStatisticProcessPrivate;
class SA_PROCESS_EXPORT SAPointSeriesStatisticProcess : public SAAbstractProcess
{
    Q_OBJECT
    SA_IMPL(SAPointSeriesStatisticProcess)
public:
    SAPointSeriesStatisticProcess(QObject* par=nullptr);
    ~SAPointSeriesStatisticProcess();
    virtual void run();
};

#endif // SAPOINTSERIESSTATISTICPROCESS_H
