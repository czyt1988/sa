#include "SAValueTableWidget.h"
#include "ui_SAValueTableWidget.h"
#include "SADataTableModel.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <algorithm>
#include <vector>
#include <set>
#include <QWheelEvent>
#include <QHeaderView>
#include "SAValueManager.h"
#include "SAData.h"
#include <QDebug>
#include <QClipboard>
#include "SAWaitCursor.h"
#include "SACsvStream.h"
#include "SAValueTableOptCommands.h"
#include "SAUIHelper.h"
#include "SALog.h"
#include "SADataConver.h"
#include <QShortcut>
#include "SAUIInterface.h"
#include "SAUIReflection.h"
/////////////////////////////////////////////////

SAValueTableWidget::SAValueTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAValueTableWidget)
  ,m_countNewData(0)
  ,m_menu(nullptr)
  ,m_insertAction(nullptr)
  ,m_deleteAction(nullptr)
  ,m_copyAction(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::ClickFocus);
    setFocusProxy(ui->tableView);
    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(20);
    m_undo = m_undoStack->createUndoAction(m_menu,tr("undo"));
    m_redo = m_undoStack->createRedoAction(m_menu,tr("redo"));
    m_undo->setIcon(QIcon(":/icons/icons/undo.png"));
    m_redo->setIcon(QIcon(":/icons/icons/redo.png"));
    SADataTableModel* model = new SADataTableModel(ui->tableView);
    SAColorList clrs;
    clrs.setColorRange(QColor(99,190,123),QColor(248,105,107),20);
    model->setColorList(clrs);
    ui->tableView->setModel (model);

    //设置model的setData的操作
    model->onSetDataFun = [&](int r,int c,const QVariant& v)->bool
    {
        return this->onTableViewSetData(r,c,v);
    };
    //设置tableView的Ctrl + v的操作
    ui->tableView->onCtrlVFun = [&](SATableView* obj)
    {
        Q_UNUSED(obj);
        this->onActionPasteTriggered();
    };
    //设置tableView的delete和backspace的操作
    auto deleteFunPtr = [&](SATableView* obj)
    {
        Q_UNUSED(obj);
        this->onActionDeleteTriggered();
    };
    ui->tableView->onDeleteFun = deleteFunPtr;
    ui->tableView->onBackspaceFun = deleteFunPtr;

    connect(ui->tableView,&QTableView::customContextMenuRequested
            ,this,&SAValueTableWidget::onTableViewCustomContextMenuRequested);
    connect(ui->actionToLinerData,&QAction::triggered
            ,this,&SAValueTableWidget::onActionToLinerDataTriggered);
    connect(ui->actionToPointFVectorData,&QAction::triggered
            ,this,&SAValueTableWidget::onActionToPointFVectorDataTriggered);
    connect(saValueManager,&SAValueManager::dataRemoved
            ,this,&SAValueTableWidget::onDataRemoved);
}

SAValueTableWidget::~SAValueTableWidget()
{
    delete ui;
}
///
/// \brief 右键菜单的创建
///
/// 菜单再使用时才会创建
///
void SAValueTableWidget::createMenu()
{
    if(m_menu)
    {
        return;
    }
    m_menu = new QMenu(this);
    m_menu->addAction(m_undo);
    m_menu->addAction(m_redo);
    //////////////////////////////////////////////////////
    m_menu->addSeparator();

    m_insertAction = new QAction(tr("insert"),this);
    m_menu->addAction(m_insertAction);
    connect(m_insertAction,&QAction::triggered,this,&SAValueTableWidget::onActionInsertTriggered);

    m_deleteAction = new QAction(tr("delete"),this);
    m_menu->addAction(m_deleteAction);
    connect(m_deleteAction,&QAction::triggered,this,&SAValueTableWidget::onActionDeleteTriggered);

    m_copyAction = new QAction(tr("copy"),this);
    m_menu->addAction(m_copyAction);
    connect(m_copyAction,&QAction::triggered,this,&SAValueTableWidget::onActionCopyTriggered);

    m_pasteAction = new QAction(tr("paste"),this);
    m_menu->addAction(m_pasteAction);
    connect(m_pasteAction,&QAction::triggered,this,&SAValueTableWidget::onActionPasteTriggered);

    /////////////////////////////////////////////////////
    m_menu->addSeparator();

    QMenu* m = m_menu->addMenu (tr("export select datas"));
    m->addAction(ui->actionToLinerData);
    m->addAction(ui->actionToPointFVectorData);

}
///
/// \brief 根据选中的表格单元，分析数据对应的有用表格
/// \param indexLists 表格选中的索引index
/// \param res 分析的结果
///
void SAValueTableWidget::analysisSelectModelIndex(const QModelIndexList &indexLists, QMap<SAAbstractDatas *, QVector<QPoint> > &res)
{
    SADataTableModel* model = getDataModel();
    const QList<SAAbstractDatas*>& datas = model->getSADataPtrs();
    struct _tmp_struct
    {
        _tmp_struct()
        {
            colStart = -1;
            colEnd = -1;
            dataPtr = nullptr;
        }
        int colStart;
        int colEnd;
        SAAbstractDatas* dataPtr;
    };
    QList<_tmp_struct> dataRanges;
    for(int i=0;i<datas.size();++i)
    {
        _tmp_struct st;
        st.dataPtr = datas[i];
        model->dataColumnRange(st.dataPtr,st.colStart,st.colEnd);
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
            if(col >= dataRanges[j].colStart && col <= dataRanges[j].colEnd && row < dataRanges[j].dataPtr->getSize(SA::Dim1))
            {
                res[dataRanges[j].dataPtr].push_back(QPoint(col,row));
            }
        }
    }
}

///
/// \brief 获取tableView
/// \return
///
QTableView *SAValueTableWidget::getTableView()
{
    return ui->tableView;
}

///
/// \brief 获取model
/// \return
///
SADataTableModel *SAValueTableWidget::getDataModel() const
{
    return static_cast<SADataTableModel*>(ui->tableView->model ());
}

///
/// \brief 设置数据到表格中显示
///
/// 此操作会覆盖掉原来已经设置的数据指针
///
/// \param data 数据指针
///
///
void SAValueTableWidget::setSADataPtr(SAAbstractDatas *data)
{
    m_undoStack->clear();
    getDataModel()->setSADataPtr(data);
}

///
/// \brief 设置多个数据指针到表格中显示
/// \param datas
///
void SAValueTableWidget::setSADataPtrs(const QList<SAAbstractDatas *> &datas)
{
    m_undoStack->clear();
    getDataModel()->setSADataPtrs(datas);
}

///
/// \brief 在已有数据指针后继续插入数据显示
/// \param data
///
void SAValueTableWidget::appendSADataPtr(SAAbstractDatas *data)
{
    m_undoStack->clear();
    getDataModel()->appendSADataPtr(data);
}

///
/// \brief 插入多个数据指针
/// \param datas
///
void SAValueTableWidget::appendSADataPtrs(QList<SAAbstractDatas *> datas)
{
    m_undoStack->clear();
    getDataModel()->appendSADataPtrs(datas);
}

///
/// \brief 移除模型中的数据指针
/// \param datas
///
void SAValueTableWidget::removeDatas(const QList<SAAbstractDatas *> &datas)
{
    m_undoStack->clear();
    getDataModel()->removeDatas(datas);
}

///
/// \brief 把整个表格内容保存为csv格式
/// \param fullFilePath csv文件的路径
///
void SAValueTableWidget::saveTableToCsv(const QString &fullFilePath)
{
    SADataTableModel* model = getDataModel();
    QFile file;
    file.setFileName (fullFilePath);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QMessageBox::critical(nullptr, tr("information")
                              , tr("can not create file"));
        return;
    }
    SAWaitCursor wait;

    SACsvStream csv(&file);
    const int rows = model->dataRowCount();
    const int columns = model->dataColumnCount();
    for(int r=0;r<rows;++r)
    {
        for(int c=0;c<columns;++c)
        {
            QVariant var = model->data(model->index(r,c));
            csv << var.toString();
        }
        csv<<endl;
    }
    file.close();
    wait.release();
    QMessageBox::critical(nullptr, tr("information")
                          , tr("can not create file"));
}

void SAValueTableWidget::redo()
{
    m_redo->activate(QAction::Trigger);
    getDataModel()->update();
    saUI->updateValueManagerTreeView();
}

void SAValueTableWidget::undo()
{
    m_undo->activate(QAction::Trigger);
    getDataModel()->update();
    saUI->updateValueManagerTreeView();
}

void SAValueTableWidget::enableColorCell(bool on)
{
    SADataTableModel * model = getDataModel();
    model->enableColorCell(on);
}

void SAValueTableWidget::onTableViewCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    if(nullptr == m_menu)
    {
        createMenu();
    }
    m_pasteAction->setEnabled(QApplication::clipboard()->ownsClipboard());
    m_menu->exec (QCursor::pos());
}

void SAValueTableWidget::onActionToLinerDataTriggered()
{
    ++m_countNewData;
    QHash<int,QVector<double> > rawData;
    getSelectLinerData(rawData);
    auto end = rawData.end ();
    for(auto i=rawData.begin ();i!=end;++i)
    {
        if(i.value ().size() <= 0)
            continue;
        QVariant var = getDataModel()->headerData (i.key (),Qt::Horizontal,Qt::DisplayRole);
        QString name = var.toString ();
        name += QStringLiteral(" - %1").arg(m_countNewData);
        auto data = SAValueManager::makeData<SAVectorDouble>(name,i.value());
        saValueManager->addData(data);
    }
}

void SAValueTableWidget::onActionToPointFVectorDataTriggered()
{
    auto points = SAValueManager::makeData<SAVectorPointF>();
    if(!getSelectVectorPointData(points.get ()))
        return;
    bool ok = false;
    QString name = QInputDialog::getText(this,QStringLiteral("变量名"),QStringLiteral("请输入变量名:")
                                         ,QLineEdit::Normal,QStringLiteral("点序列"),&ok);
    points->setName(name);
    if (ok)
    {
        saValueManager->addData(points);
    }
}

///
/// \brief 变量删除发出的信号
/// \param dataBeDeletedPtr
///
void SAValueTableWidget::onDataRemoved(const QList<SAAbstractDatas *> &dataBeDeletedPtr)
{
    SADataTableModel* model = getDataModel();
    model->removeDatas(dataBeDeletedPtr);
}
///
/// \brief 插入
///
void SAValueTableWidget::onActionInsertTriggered()
{
    SAWaitCursor waitCursor;
    QItemSelectionModel* selModel = ui->tableView->selectionModel();
    QMap<SAAbstractDatas *, QVector<QPoint> > res;
    QModelIndexList indexs = selModel->selectedIndexes ();
    analysisSelectModelIndex(indexs,res);
    QScopedPointer<QUndoCommand> topCmd(new QUndoCommand);
    for(auto i=res.begin();i!=res.end();++i)
    {
        //指定了父对象，不会内存泄漏
        new SAValueTableOptInsertCommand(i.key(),i.value(),topCmd.data());
    }
    if(topCmd->childCount() > 0)
    {
        topCmd->setText(tr("delete datas"));
        m_undoStack->push(topCmd.take());
        getDataModel()->update();
        saUI->updateValueManagerTreeView();
    }
}
///
/// \brief 设置数据,此函数将设置到model的setData函数里
/// \param r
/// \param c
/// \param v
/// \return
///
bool SAValueTableWidget::onTableViewSetData(int r, int c, const QVariant &v)
{
    SADataTableModel* model = getDataModel();
    SAAbstractDatas* d = model->columnToData(c);

    if(nullptr == model)
    {
        return false;
    }
    if(nullptr == d)
    {
        //判断是否是表格
        QList<SAAbstractDatas*> datas = model->getSADataPtrs();
        if(datas.isEmpty())
        {
            return false;
        }
        d = datas.back();
        switch(d->getType())
        {
        case SA::TableDouble:
        case SA::TableVariant:
            break;
        default:
            return false;
        }
    }
    int startCol,endCol;
    model->dataColumnRange(d,startCol,endCol);
    if(startCol < 0 || endCol < 0)
    {
        return false;
    }
    QScopedPointer<SAValueTableOptEditValueCommand> cmd;
    cmd.reset(new SAValueTableOptEditValueCommand(d,v,r,c-startCol));
    if(cmd->isValid())
    {
        cmd->setText(tr("[\"%1\"]set data").arg(d->getName()));
    }
    else
    {
        cmd.reset(nullptr);
    }


    if(cmd)
    {
        m_undoStack->push(cmd.take());
        model->update();
        saUI->updateValueManagerTreeView();
        return true;
    }
    return false;
}

///
/// \brief 表格控件处理按下ctrl + v
///
void SAValueTableWidget::onActionPasteTriggered()
{
    SAWaitCursor waitCursor;
    QList<QVariantList> variantClipboardTable;
    QSize tableSize = getClipboardTextTable(variantClipboardTable);
    if(!tableSize.isValid())
    {
        saPrint() << "Clipboard text size is valid";
        return;
    }
    SADataTableModel* model = getDataModel();
    QItemSelectionModel* selModel = ui->tableView->selectionModel();
    if(nullptr == selModel || nullptr == model)
        return;
    //获取当前选择的列
    QModelIndexList selColIndexs = selModel->selectedIndexes();
    if(1 == selColIndexs.size())
    {
        //说明只选择了一个单元格
        int col = selColIndexs[0].column();
        int row = selColIndexs[0].row();
        //获取对应的数据
        SAAbstractDatas* data = model->columnToData(col);
        if(nullptr == data)
        {
            //此时有一种情况，就是打开的是表格
            QList<SAAbstractDatas *> datas = model->getSADataPtrs();
            if(datas.isEmpty())
            {
                saPrint() << "no data in table";
                return;
            }
            const int type = datas.back()->getType();
            if(type != SA::TableVariant && type != SA::TableDouble)
            {
                saPrint() << "current is have data,but paste col is out of data range ,"
                             "and is not table data,data type is :" << type;
                return;
            }
            data = datas.back();
        }
        //获取这个数据在表格里的起始列和终止列
        int startCol,endCol;
        model->dataColumnRange(data,startCol,endCol);
        if(-1 == startCol || -1 == endCol)
        {
            saPrint() << "startCol and endCol invalid: startCol"<<startCol
                     <<" endCol:" << endCol;
            return;
        }
        //可以复制执行复制命令
        QScopedPointer<SAValueTableOptPasteCommand> cmd(new SAValueTableOptPasteCommand(
                                                            data
                                                            ,variantClipboardTable
                                                            ,row
                                                            ,col-startCol));
        if(cmd->isValid())
        {
            cmd->setText(tr("paste datas"));
            m_undoStack->push(cmd.take());
            model->update();
            saUI->updateValueManagerTreeView();
        }
        else
        {
            saPrint() << "can not paste value";
        }
    }
    else
    {
        waitCursor.release();
        saPrint() << "current no support selection region paste";
        return;
    }
}

void SAValueTableWidget::onActionCopyTriggered()
{
    ui->tableView->copySelectedCellInClipboard();
}
///
/// \brief 点击delete按钮的事件回调
///
void SAValueTableWidget::onActionDeleteTriggered()
{
    SAWaitCursor waitCursor;
    QItemSelectionModel* selModel = ui->tableView->selectionModel();
    QMap<SAAbstractDatas *, QVector<QPoint> > res;
    QModelIndexList indexs = selModel->selectedIndexes ();
    analysisSelectModelIndex(indexs,res);
    QScopedPointer<QUndoCommand> topCmd(new QUndoCommand);
    for(auto i=res.begin();i!=res.end();++i)
    {
        //指定了父对象，不会内存泄漏
        new SAValueTableOptDeleteCommand(i.key(),i.value(),topCmd.data());
    }
    if(topCmd->childCount() > 0)
    {
        topCmd->setText(tr("delete datas"));
        m_undoStack->push(topCmd.take());
        getDataModel()->update();
        saUI->updateValueManagerTreeView();
    }
}
///
/// \brief 获取选中的线性数据
/// \param rawData 数据的列存放在hash的key，数据值存放在hash的value
///
void SAValueTableWidget::getSelectLinerData(QHash<int, QVector<double> > &rawData) const
{
    QItemSelectionModel* sel = ui->tableView->selectionModel ();
    QModelIndexList indexs = sel->selectedIndexes ();
    const int size = indexs.size ();
    for(int i=0;i<size;++i)
    {
        int col = indexs[i].column ();

        if(rawData.contains (col))
        {
            doubleVectorAppendFromVariant(indexs[i].data (),rawData[col]);
        }
        else
        {
            QVector<double> temp;
            doubleVectorAppendFromVariant(indexs[i].data (),temp);
            rawData[col] = temp;
        }
    }
}


///
/// \brief 获取选中的两列数据作为SAVectorPointF的值
/// \param data 获取的结果
/// \return  成功获取返回true
///
bool SAValueTableWidget::getSelectVectorPointData(SAVectorPointF* data)
{
    SADataTableModel* model = getDataModel ();
    QItemSelectionModel* selModel = ui->tableView->selectionModel ();
    QModelIndexList indexList = selModel->selectedIndexes ();
    int xcolumn(0),ycolumn(0);
    xcolumn = ycolumn = indexList.first ().column ();
    for(int i=1;i<indexList.size();++i)
    {
        ycolumn = indexList[i].column ();
        if(xcolumn != ycolumn)//说明两个列不相等，可以提取
            break;
    }
    if(xcolumn == ycolumn)
        return false;
    QPointF p(0,0);
    bool xOK,yOK;
    for(int i=0;i<indexList.size();++i)
    {
        const QModelIndex& xindex = indexList[i];
        if(xcolumn == xindex.column())
        {
            p.rx() = xindex.data().toDouble(&xOK);
            if(!xOK)
            {
                continue;
            }
            QModelIndex yindex = model->index(xindex.row(),ycolumn);
            if(!yindex.isValid())
            {
                continue;
            }
            p.ry() = yindex.data().toDouble(&yOK);
            if(!yOK)
            {
                continue;
            }
            data->append(p);
        }
    }
    return (data->getSize ())>0;
}

void SAValueTableWidget::doubleVectorAppendFromVariant(const QVariant &var, QVector<double> &data)
{
    bool isOk = false;
    double d = var.toDouble (&isOk);
    if(isOk)
        data.push_back (d);
}

QPoint SAValueTableWidget::tableHeaderPositionOffset() const
{
    QHeaderView* verticalHeader = ui->tableView->verticalHeader();
    QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
    return QPoint(verticalHeader->width(),horizontalHeader->height());

}


///
/// \brief 工程函数，获取QItemSelectionModel里的所有列的值
///
/// 每个列放置在一个std::shared_ptr<QVector<QVariant> >中，
/// \param selModel QItemSelectionModel指针
/// \param res 获取的结果
///
void SAValueTableWidget::getItemSelectionColumns(QItemSelectionModel *selModel, QMap<int, std::shared_ptr<QVector<QVariant> > > &res)
{
    QModelIndexList indexs = selModel->selectedIndexes ();
    for(auto i = indexs.begin();i!=indexs.end();++i)
    {
        int c = i->column();
        if (res.end() == res.find(c))//并没有选中的列
        {
            std::shared_ptr<QVector<QVariant> > val;
            val.reset(new QVector<QVariant>);
            res[c] = val;
        }
        res[c]->append(i->data());
    }
}

void SAValueTableWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        qDebug()<<numSteps;
    }
    event->accept();
}


QSize SAValueTableWidget::getClipboardTextTable(QList<QVariantList> &res)
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
        QVariantList varList;
        std::for_each(listStr.begin(),listStr.end(),[&varList](const QString& str){
            varList.append(str);
        });
        res.append(varList);
        ++maxRow;
    }
    return QSize(maxColumn,maxRow);
}


