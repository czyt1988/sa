#include "SAPointSeriesStatisticProcess.h"
class SAPointSeriesStatisticProcessPrivate
{
    SA_IMPL_PUBLIC(SAPointSeriesStatisticProcess)
public:
    SAPointSeriesStatisticProcessPrivate(SAPointSeriesStatisticProcess* p):q_ptr(p)
    {

    }
    ~SAPointSeriesStatisticProcessPrivate()
    {

    }

};

SAPointSeriesStatisticProcess::SAPointSeriesStatisticProcess(QObject *par):SAAbstractProcess(par)
  ,d_ptr(new SAPointSeriesStatisticProcessPrivate(this))
{

}

SAPointSeriesStatisticProcess::~SAPointSeriesStatisticProcess()
{

}

void SAPointSeriesStatisticProcess::run()
{
    //emit result();
}
