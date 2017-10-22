#ifndef SARIBBONELEMENTMANAGER_H
#define SARIBBONELEMENTMANAGER_H
#include "SARibbonElementCreateDelegate.h"
///
/// \brief 此类是一个全局单例，用于管理SARibbonElementCreateDelegate
///
class SARibbonElementManager
{
protected:
    SARibbonElementManager();
public:
    virtual ~SARibbonElementManager();
    static SARibbonElementManager* instance();
    SARibbonElementCreateDelegate* delegate();
    void setupDelegate(SARibbonElementCreateDelegate* delegate);

private:
    static SARibbonElementManager* s_instance;
    SARibbonElementCreateDelegate* m_delegate;
};
#ifndef RibbonSubElementMgr
#define RibbonSubElementMgr SARibbonElementManager::instance()
#endif
#ifndef RibbonSubElementDelegate
#define RibbonSubElementDelegate SARibbonElementManager::instance()->delegate()
#endif
#endif // SARIBBONELEMENTMANAGER_H
