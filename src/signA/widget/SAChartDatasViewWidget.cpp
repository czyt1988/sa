#include "SAChartDatasViewWidget.h"
#include "ui_SAChartDatasViewWidget.h"
#include "SAPlotItemTreeModel.h"
#include "SAPlotDataModel.h"
#include "SAChart2D.h"
#include "SAFigureWindow.h"
#include "SAFigureOptCommands.h"
#include <QClipboard>
#include <QMessageBox>
#include "SAWaitCursor.h"
#include "SALog.h"
#include "SAFigureOptCommands.h"
#include "SAFigureTableCommands.h"
#include <QMenu>

SAChartDatasViewWidget::SAChartDatasViewWidget(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::SAChartDatasViewWidget)
    ,m_figure(nullptr)
  ,m_menu(nullptr)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,3);
    m_treeModel = new SAPlotItemTreeModel(this);
    ui->treeView->setModel(m_treeModel);
    m_tableModel = new SAPlotDataModel(this);
    ui->tableView->setModel(m_tableModel);

    ui->tableView->onCtrlVFun = [this](SATableView* obj)
    {
        Q_UNUSED(obj);
        this->onActionPasteTriggered();
    };

    ui->tableView->onBackspaceFun
    = ui->tableView->onDeleteFun
    = [this](SATableView* obj)
    {
        Q_UNUSED(obj);
        this->onActionDeleteTriggered();
    };

    connect(ui->tableView,&QTableView::customContextMenuRequested
            ,this,&SAChartDatasViewWidget::onTableViewCustomContextMenuRequested);
    // - TreeView CurPlotItem slots(曲线条目树形窗口)
    connect(ui->treeView,&QTreeView::clicked,this,&SAChartDatasViewWidget::onTreeViewCurPlotItemClicked);
}

SAChartDatasViewWidget::~SAChartDatasViewWidget()
{
    delete ui;
}

QwtPlotItemTreeModel *SAChartDatasViewWidget::getTreeModel() const
{
    return m_treeModel;
}
///
/// \brief 设置一个fig，并显示figure的内容
/// \param fig
///
void SAChartDatasViewWidget::setFigure(SAFigureWindow *fig)
{
    if(m_figure)
    {
        if(m_figure == fig)
        {
            return;
        }
        disconnect(m_figure,&SAFigureWindow::chartAdded
                   ,this,&SAChartDatasViewWidget::onChartAdded);
        disconnect(m_figure,&SAFigureWindow::destroyed
                   ,this,&SAChartDatasViewWidget::onFigureDestroy);
    }
    m_figure = fig;
    m_tableModel->setFigure(fig);
    if(fig)
    {
        QList<SAChart2D*> plotWidgets = fig->get2DPlots();
        QList<QwtPlot*> qwtChart;
        std::for_each(plotWidgets.begin(),plotWidgets.end(),[&](SAChart2D* p){
            qwtChart.append(static_cast<QwtPlot*>(p));
        });
        m_treeModel->setPlots(qwtChart);
        ui->treeView->expandAll();
        connect(m_figure,&SAFigureWindow::chartAdded
                   ,this,&SAChartDatasViewWidget::onChartAdded);
        connect(m_figure,&SAFigureWindow::destroyed
                   ,this,&SAChartDatasViewWidget::onFigureDestroy);
    }
    else
    {
        m_treeModel->clear();
    }
    m_tableModel->clear();

}

SAPlotDataModel *SAChartDatasViewWidget::getPlotModel() const
{
    return m_tableModel;
}
///
/// \brief TreeView CurPlotItem slots(曲线条目树形窗口)
/// \param index
///
void SAChartDatasViewWidget::onTreeViewCurPlotItemClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QItemSelectionModel* sel = ui->treeView->selectionModel();
    QModelIndexList indexList = sel->selectedRows();
    QList<QwtPlotItem*> items;
    for(int i = 0;i<indexList.size ();++i)
    {
        if(!indexList[i].parent().isValid())
        {//说明点击的是父窗口，就是qwtplot,这时显示所有
            items.clear();
            int childIndex = 0;
            while (indexList[i].child(childIndex,0).isValid()) {
                items.append(m_treeModel->getQwtPlotItemFromIndex (
                                 indexList[i].child(childIndex,0)));
                ++childIndex;
            }
            break;
        }
        if(indexList[i].column () != 0)
        {
            indexList[i] = indexList[i].parent().child(indexList[i].row(),0);
        }
        items.append (m_treeModel->getQwtPlotItemFromIndex (indexList[i]));
    }

    m_tableModel->setPlotItems (items);
}



void SAChartDatasViewWidget::onChartAdded(QwtPlot *plot)
{
    m_treeModel->addPlot(plot);
}

void SAChartDatasViewWidget::onFigureDestroy(QObject *obj)
{
    Q_UNUSED(obj);
    setFigure(nullptr);
}

void SAChartDatasViewWidget::onTableViewCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    if(nullptr == m_menu)
    {
        createMenu();
    }
    m_pasteAction->setEnabled(QApplication::clipboard()->ownsClipboard());
    m_menu->exec (QCursor::pos());
}

void SAChartDatasViewWidget::onActionInsertTriggered()
{
    SAWaitCursor waitCursor;
    QItemSelectionModel* selModel = ui->tableView->selectionModel();
    QModelIndexList indexs = selModel->selectedIndexes ();
    QMap<QwtPlotItem *, int > res;
    analysisSelectModelIndex(indexs,res);
    for(auto i=res.begin();i!=res.end();++i)
    {
        SAChart2D* chart = qobject_cast<SAChart2D*>(i.key()->plot());
        if(nullptr == chart)
        {
            continue;
        }
        QScopedPointer<SAFigureTableInsertCommand> cmd(new SAFigureTableInsertCommand(
                    chart
                    ,i.key()
                    ,i.value()
                    ,tr("insert plot datas")
                    ));
        if(!(cmd->isValid()))
        {
            continue;
        }
        chart->appendCommand(cmd.take());
    }
    SAPlotDataModel* model = getPlotModel();
    if(model)
    {
        model->updateRow();
    }
}

void SAChartDatasViewWidget::onActionDeleteTriggered()
{
    SAWaitCursor waitCursor;
    QItemSelectionModel* selModel = ui->tableView->selectionModel();
    QModelIndexList indexs = selModel->selectedIndexes ();
    QMap<QwtPlotItem *, QVector<QPoint> > res;
    analysisSelectModelIndex(indexs,res);

    for(auto i=res.begin();i!=res.end();++i)
    {
        SAChart2D* chart = qobject_cast<SAChart2D*>(i.key()->plot());
        if(nullptr == chart)
        {
            continue;
        }
        QScopedPointer<SAFigureTableDeleteCommand> cmd(new SAFigureTableDeleteCommand(
                    chart
                    ,i.key()
                    ,i.value()
                    ,tr("delete plot datas")
                    ));
        if(!(cmd->isValid()))
        {
            continue;
        }
        chart->appendCommand(cmd.take());
    }
    SAPlotDataModel* model = getPlotModel();
    if(model)
    {
        model->updateRow();
    }
}

void SAChartDatasViewWidget::onActionPasteTriggered()
{
    SAWaitCursor waitCursor;
    SAPlotDataModel* model = getPlotModel();
    QItemSelectionModel* selModel = ui->tableView->selectionModel();
    if(nullptr == selModel || nullptr == model)
        return;
    //获取当前选择的列
    QModelIndexList selColIndexs = selModel->selectedIndexes();
    //说明只选择了一个单元格
    if(selColIndexs.isEmpty())
    {
        return;
    }
    int col = selColIndexs[0].column();
    int row = selColIndexs[0].row();
    //获取对应的item
    QwtPlotItem* item = model->getItemFromCol(col);
    if(nullptr == item)
        return;
    //获取这个数据在表格里的起始列和终止列
    int startCol,endCol;
    model->getItemColumnRange(item,&startCol,&endCol);
    if(-1 == startCol || -1 == endCol)
        return;

    QList<QStringList> stringClipboardTable;
    getClipboardTextTable(stringClipboardTable);
    SAChart2D* chart = qobject_cast<SAChart2D*>(item->plot());
    if(chart)
    {
        QScopedPointer<SAFigureTablePasteInSeriesCommand> cmd(new SAFigureTablePasteInSeriesCommand(
                    chart
                    ,item
                    ,stringClipboardTable
                    ,row
                    ,col-startCol
                    ,tr("paste in figure chart table")
                    ));
        if(!cmd->isValid())
        {
            return;
        }

        chart->appendCommand(cmd.take());
        model->updateRow();
    }
}

void SAChartDatasViewWidget::onActionCopyTriggered()
{
    ui->tableView->copySelectedCellInClipboard();
}

QSize SAChartDatasViewWidget::getClipboardTextTable(QList<QStringList> &res)
{
    QClipboard *clipboard = QApplication::clipboard();
    QString clipboardText = clipboard->text();
    if(clipboardText.isEmpty())
        return QSize();
    //解析复制的数据
    QTextStream ts(&clipboardText);
    int maxColumn = 0;
    int maxRow = 0;
    while(!ts.atEnd())
    {
        QString line = ts.readLine();
        QStringList listStr = line.split('\t');
        if(listStr.size() > maxColumn)
            maxColumn = listStr.size();
        res.append(listStr);
        ++maxRow;
    }
    return QSize(maxColumn,maxRow);
}




void SAChartDatasViewWidget::createMenu()
{
    if(m_menu)
    {
        return;
    }
    m_menu = new QMenu(this);
    //////////////////////////////////////////////////////

    m_insertAction = new QAction(tr("insert"),this);
    m_menu->addAction(m_insertAction);
    connect(m_insertAction,&QAction::triggered,this,&SAChartDatasViewWidget::onActionInsertTriggered);

    m_deleteAction = new QAction(tr("delete"),this);
    m_menu->addAction(m_deleteAction);
    connect(m_deleteAction,&QAction::triggered,this,&SAChartDatasViewWidget::onActionDeleteTriggered);

    m_copyAction = new QAction(tr("copy"),this);
    m_menu->addAction(m_copyAction);
    connect(m_copyAction,&QAction::triggered,this,&SAChartDatasViewWidget::onActionCopyTriggered);

    m_pasteAction = new QAction(tr("paste"),this);
    m_menu->addAction(m_pasteAction);
    connect(m_pasteAction,&QAction::triggered,this,&SAChartDatasViewWidget::onActionPasteTriggered);

    /////////////////////////////////////////////////////
//    m_menu->addSeparator();

//    QMenu* m = m_menu->addMenu (tr("export select datas"));
//    m->addAction(ui->actionToLinerData);
//    m->addAction(ui->actionToPointFVectorData);

}
///
/// \brief 根据选中的表格单元，分析数据对应的有用表格
/// \param indexLists 表格选中的索引index
/// \param res 分析的结果
///
void SAChartDatasViewWidget::analysisSelectModelIndex(const QModelIndexList &indexLists, QMap<QwtPlotItem *, QVector<QPoint> > &res)
{
    SAPlotDataModel* model = getPlotModel();
    const QList<QwtPlotItem*>& ployItems = model->getPlotItems();
    struct _tmp_struct
    {
        _tmp_struct()
        {
            colStart = -1;
            colEnd = -1;
            item = nullptr;
        }
        int colStart;
        int colEnd;
        QwtPlotItem* item;
    };
    QList<_tmp_struct> dataRanges;
    for(int i=0;i<ployItems.size();++i)
    {
        _tmp_struct st;
        st.item = ployItems[i];
        model->getItemColumnRange(st.item,&st.colStart,&st.colEnd);
        if(st.colStart >= 0 || st.colEnd >= 0)
        {
            dataRanges.append(st);
        }
    }
    for(int i=0;i<indexLists.size();++i)
    {
        int col = indexLists[i].column ();
        int row = indexLists[i].row ();
        for(int j=0;j<dataRanges.size();++j)
        {
            if(col >= dataRanges[j].colStart && col <= dataRanges[j].colEnd)
            {
                res[dataRanges[j].item].push_back(QPoint(col,row));
            }
        }
    }
}

void SAChartDatasViewWidget::analysisSelectModelIndex(const QModelIndexList &indexLists, QMap<QwtPlotItem *, int> &res)
{
    SAPlotDataModel* model = getPlotModel();
    const QList<QwtPlotItem*>& ployItems = model->getPlotItems();
    struct _tmp_struct
    {
        _tmp_struct()
        {
            colStart = -1;
            colEnd = -1;
            item = nullptr;
        }
        int colStart;
        int colEnd;
        QwtPlotItem* item;
    };
    QList<_tmp_struct> dataRanges;
    for(int i=0;i<ployItems.size();++i)
    {
        _tmp_struct st;
        st.item = ployItems[i];
        model->getItemColumnRange(st.item,&st.colStart,&st.colEnd);
        if(st.colStart >= 0 || st.colEnd >= 0)
        {
            dataRanges.append(st);
        }
    }
    for(int i=0;i<indexLists.size();++i)
    {
        int col = indexLists[i].column ();
        int row = indexLists[i].row ();
        for(int j=0;j<dataRanges.size();++j)
        {
            if(col >= dataRanges[j].colStart && col <= dataRanges[j].colEnd)
            {
                if(res.contains(dataRanges[j].item))
                {
                    if(row < res[dataRanges[j].item])
                    {
                        res[dataRanges[j].item] = row;
                    }
                }
                else
                {
                    res[dataRanges[j].item] = row;
                }
            }
        }
    }
}
