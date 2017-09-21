#include "SAPlotPicker.h"
#include "qwt_picker_machine.h"
SAPlotPicker::SAPlotPicker(QWidget *canvas)
    :QwtPlotPicker(canvas)
{
    setRubberBand( QwtPlotPicker::CrossRubberBand );
    setStateMachine(  new QwtPickerDragPointMachine() );
}

SAPlotPicker::SAPlotPicker(int xAxis, int yAxis, QWidget *w)
    :QwtPlotPicker(xAxis,yAxis,w)
{
    setRubberBand(QwtPlotPicker::CrossRubberBand);
}

SAPlotPicker::SAPlotPicker(int xAxis, int yAxis, QwtPicker::RubberBand rubberBand, QwtPicker::DisplayMode trackerMode, QWidget *w)
    :QwtPlotPicker(xAxis,yAxis,rubberBand,trackerMode,w)
{

}


