#ifndef SAUIREFLECTION_H
#define SAUIREFLECTION_H
//Qt
#include <QObject>
#include <QVariantList>
#include <QHash>
//stl
#include <functional>

#include "SACommonUIGlobal.h"
#include "SAGlobals.h"
class SAUIInterface;
class QAction;
class QMenu;
class QMdiSubWindow;
class QProgressBar;
//SA
class MainWindow;
//SA Lib
class SAAbstractDatas;
//SA Chart
class SAChart2D;
//SA Common UI
class SAFigureWindow;
///
/// \brief UI放射器，涉及到和主界面相关的交互通过此类来进行处理
///
class SA_COMMON_UI_EXPORT SAUIReflection
{
private:
    explicit SAUIReflection();
    Q_DISABLE_COPY(SAUIReflection)
public:
    friend class MainWindow;
    ~SAUIReflection();
    static SAUIReflection *getInstance();
    SAUIInterface* uiInterface();
private:
    void setupUIInterface(SAUIInterface* ui);
private:
    SAUIInterface* m_uiInterface;
};
#ifndef saUIRef
#define saUIRef SAUIReflection::getInstance()
#endif
#ifndef saUI
#define saUI SAUIReflection::getInstance()->uiInterface()
#endif

#endif // SAUIREFLECTION_H
