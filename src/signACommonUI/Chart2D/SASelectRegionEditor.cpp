#include "SASelectRegionEditor.h"
#include "SAChart2D.h"
SASelectRegionEditor::SASelectRegionEditor(SAChart2D *parent)
    :SAAbstractPlotEditor(parent)
{

}

void SASelectRegionEditor::setEnabled(bool on)
{

}

bool SASelectRegionEditor::isEnabled() const
{

}

int SASelectRegionEditor::rtti() const
{
    return RTTISelectRegionEditor;
}
