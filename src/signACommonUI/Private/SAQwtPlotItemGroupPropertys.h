#ifndef SAQWTPLOTITEMGROUPPROPERTYS_H
#define SAQWTPLOTITEMGROUPPROPERTYS_H
#include <QList>
class QtProperty;
class SAQwtPlotItemGroupPropertys
{
public:
    SAQwtPlotItemGroupPropertys()
    {
        isValid = false;
        topGroupItem = visibleItem = titleItem = nullptr;
    }

    bool isValid;
    QtProperty* topGroupItem;
    QtProperty* visibleItem;
    QtProperty* titleItem;
    QList<QtProperty*> otherItem;
};
#endif // SAQWTPLOTITEMGROUPPROPERTYS_H
