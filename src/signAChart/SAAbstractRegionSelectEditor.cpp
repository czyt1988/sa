#include "SAAbstractRegionSelectEditor.h"


SAAbstractRegionSelectEditor::SAAbstractRegionSelectEditor(QwtPlot *parent)
    :SAAbstractPlotEditor(parent)
    ,m_selectionMode(AdditionalSelection)//SingleSelection
{

}

SAAbstractRegionSelectEditor::~SAAbstractRegionSelectEditor()
{

}

SAAbstractRegionSelectEditor::SelectionMode SAAbstractRegionSelectEditor::getSelectionMode() const
{
    return m_selectionMode;
}

void SAAbstractRegionSelectEditor::setSelectionMode(const SelectionMode &selectionMode)
{
    m_selectionMode = selectionMode;
}
