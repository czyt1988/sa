#ifndef SASELECTREGIONEDITOR_H
#define SASELECTREGIONEDITOR_H
#include "SAAbstractPlotEditor.h"
class SAChart2D;
class SASelectRegionEditor : public SAAbstractPlotEditor
{
public:
    enum RTTI
    {
        RTTISelectRegionEditor = (SAAbstractPlotEditor::RTTIUserDefine + 13)
    };

    SASelectRegionEditor(SAChart2D *parent);
    //是否启用编辑器
    virtual void setEnabled( bool on );
    virtual bool isEnabled() const;
    virtual int rtti() const;
};

#endif // SASELECTREGIONEDITOR_H
