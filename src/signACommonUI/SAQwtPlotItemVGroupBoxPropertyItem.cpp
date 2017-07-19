#include "SAQwtPlotItemVGroupBoxPropertyItem.h"
#include <QApplication>
#include <QVBoxLayout>
#include "qwt_plot_item.h"
#include "SALineEditPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include "SACheckBoxPropertyItem.h"
#include "SAAxisSelectComboBoxPropertyItem.h"
#include "SAChartAxisSetSelect.h"
#include <QHBoxLayout>
#include <QLabel>
#define TR(str) QApplication::translate("SAQwtPlotItemVGroupBoxPropertyItem", str, 0)

class SAQwtPlotItemVGroupBoxPropertyItem::UI
{
public:
    QwtPlotItem* itemPtr;
    bool isShowMoreSet;
    SACheckBoxPropertyItem* visibleItem;
    SALineEditPropertyItem* titleItem;
    SADoubleSpinBoxPropertyItem* zValueItem;
    SAChartAxisSetSelect* axisSetItem;
    QLabel* labelAxisSet;
    QHBoxLayout* hAxisSetlayout;
    SAQwtPlotItemVGroupBoxPropertyItem::ShowMode showMode;
    void setupUI(QwtPlotItem* plotItem,SAQwtPlotItemVGroupBoxPropertyItem* par,SAQwtPlotItemVGroupBoxPropertyItem::ShowMode mode)
    {
        showMode = mode;
        labelAxisSet = nullptr;
        QLayout* vlayout = par->layout();
        //visible
        visibleItem = new SACheckBoxPropertyItem(par);
        visibleItem->setObjectName("visible");
        par->connect(visibleItem,&SACheckBoxPropertyItem::stateChanged
                     ,par,[plotItem](int state){
            plotItem->setVisible(state==Qt::Checked);
        });
        vlayout->addWidget(visibleItem);
        //title
        titleItem = new SALineEditPropertyItem(par);
        titleItem->setObjectName("title");
        par->connect(titleItem,&SALineEditPropertyItem::textChanged
                     ,par,[par,plotItem](const QString& text){
            plotItem->setTitle(text);
            par->setTitle(text);
        });
        vlayout->addWidget(titleItem);
        if(SAQwtPlotItemVGroupBoxPropertyItem::ShowAll == showMode)
        {
            //
            zValueItem = new SADoubleSpinBoxPropertyItem(par);
            zValueItem->setObjectName("z");
            par->connect(zValueItem,&SADoubleSpinBoxPropertyItem::valueChanged
                         ,par,[plotItem](double z){
                plotItem->setZ(z);
            });
            vlayout->addWidget(zValueItem);

            //
            labelAxisSet = new QLabel(par);
            labelAxisSet->setAlignment(Qt::AlignTop|Qt::AlignLeft);
            hAxisSetlayout = new QHBoxLayout();
            axisSetItem = new SAChartAxisSetSelect(par);
            axisSetItem->setObjectName("axisSetItem");
            hAxisSetlayout->addWidget(labelAxisSet);
            hAxisSetlayout->addWidget(axisSetItem);
            hAxisSetlayout->addStretch();
            par->connect(axisSetItem,&SAChartAxisSetSelect::axisChanged
                         ,par,[plotItem](int x,int y){
                plotItem->setAxes(x,y);
            });
            vlayout->addItem(hAxisSetlayout);
        }
        //
        upDateItem(plotItem);
        retranslateUi(par);
    }
    void retranslateUi(SAQwtPlotItemVGroupBoxPropertyItem *w)
    {
        w->setTitle(titleItem->getEditText());
        visibleItem->setText(TR("Visible"));
        titleItem->setText(TR("Title"));
        zValueItem->setText(TR("Z Value"));
        if(labelAxisSet)
        {
            labelAxisSet->setText(TR("Axis Set"));
        }
    }

    void upDateItem(const QwtPlotItem* plotItem)
    {
        this->itemPtr = const_cast<QwtPlotItem*>(plotItem);
        visibleItem->setCheckState(plotItem->isVisible() ? Qt::Checked : Qt::Unchecked);
        titleItem->setEditText(plotItem->title().text());
        if(SAQwtPlotItemVGroupBoxPropertyItem::ShowAll == showMode)
        {
            zValueItem->setValue(plotItem->z());
            axisSetItem->setCurrentSelectAxis(plotItem->xAxis(),plotItem->yAxis());
        }
    }
};



SAQwtPlotItemVGroupBoxPropertyItem::SAQwtPlotItemVGroupBoxPropertyItem(QwtPlotItem *plotItem, QWidget* par,ShowMode mode)
    :SAVGroupBoxPropertyItem(par)
    ,ui(new SAQwtPlotItemVGroupBoxPropertyItem::UI)
{
    ui->setupUI(plotItem,this,mode);
}

SAQwtPlotItemVGroupBoxPropertyItem::~SAQwtPlotItemVGroupBoxPropertyItem()
{
    delete ui;
}
