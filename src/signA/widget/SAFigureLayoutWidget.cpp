#include "SAFigureLayoutWidget.h"
#include "ui_SAFigureLayoutWidget.h"
#include "SAPlotLayerTreeModel.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include <QColorDialog>
#include <QComboBox>
#include "SAChart.h"
SAFigureLayoutWidget::SAFigureLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAFigureLayoutWidget)
    , m_figure(nullptr)
{
    ui->setupUi(this);
    m_layoutModel = new SAPlotLayerTreeModel(m_figure, ui->treeView);

    ui->treeView->setModel(m_layoutModel);
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    auto hh = ui->treeView->header();

    hh->setSectionResizeMode(0, QHeaderView::Interactive);
    hh->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    hh->setStretchLastSection(true);
    connect(ui->treeView, &QAbstractItemView::pressed, this, &SAFigureLayoutWidget::onTreeViewPressed);
    connect(m_layoutModel, &SAPlotLayerTreeModel::modelReseted, this, &SAFigureLayoutWidget::onModelReseted);
    connect(ui->toolButtonDelete, &QToolButton::clicked, this, &SAFigureLayoutWidget::onToolButtonDeleteClicked);
}


SAFigureLayoutWidget::~SAFigureLayoutWidget()
{
    delete ui;
}


SAPlotLayerTreeModel *SAFigureLayoutWidget::getLayoutModel() const
{
    return (m_layoutModel);
}


void SAFigureLayoutWidget::setFigure(SAFigureWindow *fig)
{
    if (m_figure == fig) {
        return;
    }
    m_figure = fig;
    m_layoutModel->setFigure(fig);
    ui->treeView->expandAll();
}


SAFigureWindow *SAFigureLayoutWidget::currentFigure() const
{
    return (m_figure);
}


///
/// \brief 点击
/// \param index
///
void SAFigureLayoutWidget::onTreeViewPressed(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
    }
    m_lastPressedIndex = index;
    QColor rgb = index.data(Qt::BackgroundColorRole).value<QColor>();

    SAPlotLayerTreeModel *model = getLayoutModel();

    /**
     * QwtPlotItem *item = model->getPlotItemFromIndex(index);
     * SAChart2D *plot = qobject_cast<SAChart2D *>(model->getPlot());
     *
     * if (!plot) {
     *  return;
     * }
     * if (1 == index.column()) {              //可见性
     *  model->setData(index, !item->isVisible());
     * }else if (index.column() == 2) {        //颜色
     *  QColorDialog clrDlg;
     *  clrDlg.setCurrentColor(rgb);
     *  if (clrDlg.exec() == QDialog::Accepted) {
     *      QColor newClr = clrDlg.selectedColor();
     *      //在模型中设置颜色,信号通过onItemValueChanged槽函数触发
     *      model->setData(index, newClr, Qt::BackgroundColorRole);
     *  }
     * }
     *
     * //设置选中
     * QItemSelectionModel *selMode = ui->tableView->selectionModel();
     * QSet<QwtPlotItem *> itemSets;
     *
     * if (selMode) {
     *  QModelIndexList selIndex = selMode->selectedRows();
     *  for (int i = 0; i < selIndex.size(); ++i)
     *  {
     *      itemSets.insert(model->getPlotItemFromIndex(selIndex[i]));
     *  }
     *  plot->setCurrentSelectItems(itemSets.toList());
     * }
     **/
}


void SAFigureLayoutWidget::onToolButtonDeleteClicked(bool on)
{
    Q_UNUSED(on);
    QItemSelectionModel *selectModel = ui->treeView->selectionModel();
//    SAChart2D *plot = qobject_cast<SAChart2D *>(m_layoutModel->getPlot());

//    if (!selectModel || !plot) {
//        return;
//    }
//    QModelIndexList indexs = selectModel->selectedIndexes();

//    for (int i = 0; i < indexs.size(); ++i)
//    {
//        QwtPlotItem *item = m_layoutModel->getPlotItemFromIndex(indexs[i]);
//        if (SAChart::checkIsPlotChartItem(item)) {
//            plot->removeItem(item);
//            emit itemRemoved(plot, item);
//        }
//    }
//    m_layoutModel->updateModel();
}


void SAFigureLayoutWidget::onItemValueChanged(QwtPlotItem *plotItem, const QVariant& value, int type, const QModelIndex& index)
{
    /**
     * switch (type)
     * {
     * case SAPlotLayerModel::ItemVisible:
     *  emit itemVisibleChanged(plotItem, value.toBool());
     *  break;
     *
     * case SAPlotLayerModel::ItemColor:
     *  //通知其他界面颜色变更
     *  emit itemColorChanged(plotItem, value.value<QColor>());
     *  break;
     *
     * case SAPlotLayerModel::ItemTitle:
     *  emit itemTitleChanged(plotItem, value.toString());
     *  break;
     *
     * default:
     *  break;
     * }
     **/
}


void SAFigureLayoutWidget::onModelReseted()
{
    ui->treeView->expandAll();
    ui->treeView->scrollTo(m_lastPressedIndex);
}


void SAFigureLayoutWidget::updateModel()
{
    m_layoutModel->update();
}
