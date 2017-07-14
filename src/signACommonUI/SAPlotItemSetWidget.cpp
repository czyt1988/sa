#include "SAPlotItemSetWidget.h"

class SAPlotItemSetWidget::UI
{
public:
    void setupUI(SAPlotItemSetWidget* par)
    {

    }
};

SAPlotItemSetWidget::SAPlotItemSetWidget(QWidget *par)
    :QWidget(par)
    ,ui(new SAPlotItemSetWidget::UI)
{
    ui->setupUI(this);
}

SAPlotItemSetWidget::~SAPlotItemSetWidget()
{
    delete ui;
}
