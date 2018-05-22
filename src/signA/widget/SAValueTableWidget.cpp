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
#include "SACellInputWidget.h"
#include <QClipboard>
#include "SAWaitCursor.h"
#include "SACsvStream.h"
#include "SAValueTableOptCommands.h"
#include "SAUIHelper.h"
#include "SALog.h"
#include "SADataConver.h"
#include <QShortcut>
/////////////////////////////////////////////////

SAValueTableWidget::SAValueTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAValueTableWidget)
  ,m_countNewData(0)
  ,m_menu(nullptr)
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
    ui->tableView->setModel (model);
    //设置setdata的操作
    model->onSetDataFun = [&](int r,int c,const QVariant& v)->bool{
        return this->setData(r,c,v);
    };

    QHeaderView* plotLayerVerticalHeader = ui->tableView->verticalHeader();
    if(plotLayerVerticalHeader)
    {
        plotLayerVerticalHeader->setDefaultSectionSize(19);
    }
    //设置Ctrl + v的操作
    ui->tableView->onCtrlVFun = [&]()
    {
        this->onTableViewCtrlV();
    };
    //设置delete和backspace的操作
    auto deleteFunPtr = [&]()
    {
        this->onTableViewDeletePressed();
    };
    ui->tableView->onDeleteFun = deleteFunPtr;
    ui->tableView->onBackspaceFun = deleteFunPtr;

    connect(ui->tableView,&QTableView::customContextMenuRequested
            ,this,&SAValueTableWidget::onTableViewCustomContextMenuRequested);
    connect(ui->actionToLinerData,&QAction::triggered
            ,this,&SAValueTableWidget::onActionToLinerDataTriggered);
    connect(ui->actionToPointFVectorData,&QAction::triggered
            ,this,&SAValueTableWidget::onActionToPointFVectorDataTriggered);
    connect(ui->tableView,&QTableView::doubleClicked,this,&SAValueTableWidget::onTableViewDoubleClicked);
    connect(saValueManager,&SAValueManager::dataRemoved
            ,this,&SAValueTableWidget::onDataRemoved);
}

SAValueTableWidget::~SAValueTableWidget()
{
    delete ui;
}

QTableView *SAValueTableWidget::getTableView()
{
    return ui->tableView;
}

SADataTableModel *SAValueTableWidget::getDataModel() const
{
    return static_cast<SADataTableModel*>(ui->tableView->model ());
}

void SAValueTableWidget::setSADataPtr(SAAbstractDatas *data)
{
    m_undoStack->clear();
    getDataModel()->setSADataPtr(data);
}

void SAValueTableWidget::setSADataPtrs(const QList<SAAbstractDatas *> &datas)
{
    m_undoStack->clear();
    getDataModel()->setSADataPtrs(datas);
}

void SAValueTableWidget::appendSADataPtr(SAAbstractDatas *data)
{
    m_undoStack->clear();
    getDataModel()->appendSADataPtr(data);
}

void SAValueTableWidget::appendSADataPtrs(QList<SAAbstractDatas *> datas)
{
    m_undoStack->clear();
    getDataModel()->appendSADataPtrs(datas);
}

void SAValueTableWidget::removeDatas(const QList<SAAbstractDatas *> &datas)
{
    m_undoStack->clear();
    getDataModel()->removeDatas(datas);
}

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
}

void SAValueTableWidget::redo()
{
    m_redo->activate(QAction::Trigger);
}

void SAValueTableWidget::undo()
{
    m_undo->activate(QAction::Trigger);
}

void SAValueTableWidget::onTableViewCustomContextMenuRequested(const QPoint &pos)
{
    if(nullptr == m_menu)
    {
        m_menu = new QMenu(this);
        m_menu->addAction(m_undo);
        m_menu->addAction(m_redo);
        QMenu* m = m_menu->addMenu (tr("export select datas"));
        m->addAction(ui->actionToLinerData);
        m->addAction(ui->actionToPointFVectorData);
    }
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

void SAValueTableWidget::onTableViewDoubleClicked(const QModelIndex &index)
{
    SADataTableModel* model = getDataModel();
//    if(model->isInDataRange(index.row(),index.column()))
//    {
//        return;
//    }
    SAAbstractDatas* data = model->columnToData(index.column());
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


    switch(data->getType())
    {
    case SA::VectorPoint:
    {
        appendVectorPointFData(data,index);
        break;
    }
    case SA::VectorDouble:
    {
        appendVectorDoubleData(data,index);
        break;
    }
    case SA::VectorInt:
    {
        appendVectorIntData(data,index);
        break;
    }
    case SA::VectorVariant:
    {
        appendVectorVariantData(data,index);
        break;
    }
    case SA::TableVariant:
    {
        appendTableVariantData(data,index);
        break;
    }
    case SA::TableDouble:
    {
        appendTableDoubleData(data,index);
        break;
    }
    default:
        return;
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
/// \brief 设置数据,此函数将设置到model的setData函数里
/// \param r
/// \param c
/// \param v
/// \return
///
bool SAValueTableWidget::setData(int r, int c, const QVariant &v)
{
    SADataTableModel* model = getDataModel();
    SAAbstractDatas* d = model->columnToData(c);

    if(nullptr == d || nullptr == model)
    {
        return false;
    }
    QUndoCommand* cmd = nullptr;
    if(r < d->getSize(SA::Dim1) && c < d->getSize(SA::Dim2))
    {
        qDebug() << "EditValueCommand";
        cmd = new SAValueTableOptEditValueCommand(d,model,v,r,c);
        cmd->setText(tr("[\"%1\"]set data").arg(d->getName()));
    }

    if(cmd)
    {
        m_undoStack->push(cmd);
        return true;
    }
    return false;
}
///
/// \brief 在VectorPointF插入新数据
/// \param data
/// \param index
///
void SAValueTableWidget::appendVectorPointFData(SAAbstractDatas* data,const QModelIndex &index)
{
    int r = index.row();
    int c = index.column();
    if(r != data->getSize(SA::Dim1))
    {
        return;
    }
    SADataTableModel* model = getDataModel();
    SACellInputWidget cellInput;
    initCellInputWidget(&cellInput,data,index);
    cellInput.exec();
    if(!cellInput.isAcceptInput())
    {
        return;
    }
    bool isOK = false;
    double x = cellInput.getCellEditText(0).toDouble(&isOK);
    if(!isOK)
    {
        return;
    }
    double y = cellInput.getCellEditText(1).toDouble(&isOK);
    if(!isOK)
    {
        return;
    }
    SAValueTableOptAppendValueCommand* cmd = new SAValueTableOptAppendValueCommand(data
                ,model
                ,{x,y}
                ,r
                ,c
                );
    cmd->setText(tr("[\"%1\"] append").arg(data->getName()));
    m_undoStack->push(cmd);
}
///
/// \brief 在VectorDouble插入新数据
/// \param data
/// \param index
///
void SAValueTableWidget::appendVectorDoubleData(SAAbstractDatas *data, const QModelIndex &index)
{
    int r = index.row();
    int c = index.column();
    if(r != data->getSize(SA::Dim1))
    {
        return;
    }
    SADataTableModel* model = getDataModel();
    SACellInputWidget cellInput;
    initCellInputWidget(&cellInput,data,index);
    cellInput.exec();
    if(!cellInput.isAcceptInput())
    {
        return;
    }
    bool isOK = false;
    double d = cellInput.getCellEditText(0).toDouble(&isOK);
    if(!isOK)
    {
        return;
    }
    SAValueTableOptAppendValueCommand* cmd = new SAValueTableOptAppendValueCommand(data
                ,model
                ,{d}
                ,r
                ,c
                );
    cmd->setText(tr("[\"%1\"] append").arg(data->getName()));
    m_undoStack->push(cmd);
}
///
/// \brief 在VectorInt插入新数据
/// \param data
/// \param index
///
void SAValueTableWidget::appendVectorIntData(SAAbstractDatas *data, const QModelIndex &index)
{
    int r = index.row();
    int c = index.column();
    if(r != data->getSize(SA::Dim1))
    {
        return;
    }
    SADataTableModel* model = getDataModel();
    SACellInputWidget cellInput;
    initCellInputWidget(&cellInput,data,index);
    cellInput.exec();
    if(!cellInput.isAcceptInput())
    {
        return;
    }
    bool isOK = false;
    int d = cellInput.getCellEditText(0).toInt(&isOK);
    if(!isOK)
    {
        return;
    }
    SAValueTableOptAppendValueCommand* cmd = new SAValueTableOptAppendValueCommand(data
                ,model
                ,{d}
                ,r
                ,c
                );
    cmd->setText(tr("[\"%1\"] append").arg(data->getName()));
    m_undoStack->push(cmd);
}
///
/// \brief 在VectorVariant插入新数据
/// \param data
/// \param index
///
void SAValueTableWidget::appendVectorVariantData(SAAbstractDatas *data, const QModelIndex &index)
{
    int r = index.row();
    int c = index.column();
    if(r != data->getSize(SA::Dim1))
    {
        return;
    }
    SADataTableModel* model = getDataModel();
    SACellInputWidget cellInput;
    initCellInputWidget(&cellInput,data,index);
    cellInput.exec();
    if(!cellInput.isAcceptInput())
    {
        return;
    }
    QString str = cellInput.getCellEditText(0);
    SAValueTableOptAppendValueCommand* cmd = new SAValueTableOptAppendValueCommand(data
                ,model
                ,{str}
                ,r
                ,c
                );
    cmd->setText(tr("[\"%1\"] append").arg(data->getName()));
    m_undoStack->push(cmd);
}

void SAValueTableWidget::appendTableVariantData(SAAbstractDatas *data, const QModelIndex &index)
{
    int r = index.row();
    int c = index.column();
    if(r != data->getSize(SA::Dim1))
    {
        return;
    }
    SADataTableModel* model = getDataModel();
    SACellInputWidget cellInput;
    initCellInputWidget(&cellInput,data,index);
    cellInput.exec();
    if(!cellInput.isAcceptInput())
    {
        return;
    }
    QString str = cellInput.getCellEditText(0);
    SAValueTableOptAppendValueCommand* cmd = new SAValueTableOptAppendValueCommand(data
                ,model
                ,{str}
                ,r
                ,c
                );
    cmd->setText(tr("[\"%1\"] append").arg(data->getName()));
    m_undoStack->push(cmd);
}

void SAValueTableWidget::appendTableDoubleData(SAAbstractDatas *data, const QModelIndex &index)
{
    int r = index.row();
    int c = index.column();
    if(r != data->getSize(SA::Dim1))
    {
        return;
    }
    SADataTableModel* model = getDataModel();
    SACellInputWidget cellInput;
    initCellInputWidget(&cellInput,data,index);
    cellInput.exec();
    if(!cellInput.isAcceptInput())
    {
        return;
    }
    bool isOK = false;
    int d = cellInput.getCellEditText(0).toInt(&isOK);
    if(!isOK)
    {
        return;
    }

    SAValueTableOptAppendValueCommand* cmd = new SAValueTableOptAppendValueCommand(data
                ,model
                ,{d}
                ,r
                ,c
                );
    cmd->setText(tr("[\"%1\"] append").arg(data->getName()));
    m_undoStack->push(cmd);
}

void SAValueTableWidget::initCellInputWidget(SACellInputWidget *w,SAAbstractDatas* data,const QModelIndex &index)
{
    int r = index.row();
    int c = index.column();
    SADataTableModel* model = getDataModel();
    QPoint offset = tableHeaderPositionOffset();
    int colStart=-1,colEnd=-1;
    model->dataColumnRange(data,colStart,colEnd);
    if(-1 == colStart || -1 == colEnd)
    {
        return;
    }
    //特殊的处理
    if(SA::TableVariant == data->getType() || SA::TableDouble == data->getType())
    {
        QPoint leftTop,rightBottom;
        leftTop.setX(ui->tableView->columnViewportPosition(c));
        leftTop.setY(ui->tableView->rowViewportPosition(r));
        rightBottom.setX(leftTop.x()+ui->tableView->columnWidth(c));
        rightBottom.setY(leftTop.y()+ui->tableView->rowHeight(r));
        int tmp = ui->tableView->rowHeight(r);
        if(tmp < 20 || tmp > 30)
        {
            tmp = 20;
        }
        leftTop.ry() -= tmp;
        leftTop = ui->tableView->mapToGlobal(leftTop);
        rightBottom = ui->tableView->mapToGlobal(rightBottom);
        leftTop += offset;
        rightBottom += offset;
        int buttonAreaWidth = w->getButtonAreaWidth();
        rightBottom.rx() += buttonAreaWidth;
        w->setGeometry(QRect(leftTop,rightBottom));
        w->resizeCells(1);
        return;
    }
    QPoint leftTop,rightBottom;
    leftTop.setX(ui->tableView->columnViewportPosition(colStart));
    leftTop.setY(ui->tableView->rowViewportPosition(r));
    rightBottom.setX(ui->tableView->columnViewportPosition(colEnd)+ui->tableView->columnWidth(colEnd));
    rightBottom.setY(leftTop.y()+ui->tableView->rowHeight(r));
    int tmp = ui->tableView->rowHeight(r);
    if(tmp < 20 || tmp > 30)
    {
        tmp = 20;
    }
    leftTop.ry() -= tmp;
    leftTop = ui->tableView->mapToGlobal(leftTop);
    rightBottom = ui->tableView->mapToGlobal(rightBottom);
    leftTop += offset;
    rightBottom += offset;
    int buttonAreaWidth = w->getButtonAreaWidth();
    rightBottom.rx() += buttonAreaWidth;
    w->setGeometry(QRect(leftTop,rightBottom));
    const int dim2 = colEnd - colStart + 1;
    w->resizeCells(dim2);
    for(int i=0;i<dim2;++i)
    {
        w->setCellWidth(i,ui->tableView->columnWidth(colStart+i));
    }

    switch(data->getType())
    {
        case SA::VectorPoint:
        {
            w->setCellTitleText(0,tr("x"));
            w->setCellTitleText(1,tr("y"));
            break;
        }
        default:
        {
            for(int i=0;i<dim2;++i)
            {
                if(i+colStart<model->columnCount())
                {
                    w->setCellTitleText(i,QString::number(i+1));
                }
            }
        }
    }

}
///
/// \brief 表格控件处理按下ctrl + v
///
void SAValueTableWidget::onTableViewCtrlV()
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
    saPrint() << "selColIndexs size:" <<selColIndexs.size();
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
        QScopedPointer<SAValueTableOptPasteCommand> cmd(new SAValueTableOptPasteCommand(data,model,variantClipboardTable,row,col));
        if(cmd->isValid())
        {
            cmd->setText(tr("paste datas"));
            m_undoStack->push(cmd.take());
            model->update();
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
///
/// \brief 点击delete按钮的事件回调
///
void SAValueTableWidget::onTableViewDeletePressed()
{

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
/// \brief 把选择的数据转为点序列
/// \param rawData 输出rawData
/// \param dim 选择数据的方向，0为向下，即为选择列，1为横向选择
///
void SAValueTableWidget::getSelectVectorPointData(QVector<std::shared_ptr<QVector<QPointF> > > &rawData, int dim)
{
    QItemSelectionModel* selModel = ui->tableView->selectionModel ();
    if(0 == dim)
    {
        QMap<int, std::shared_ptr<QVector<QVariant> > > colVals;
        getItemSelectionColumns(selModel,colVals);
        if(colVals.size() == 0)
        {
            QMessageBox::warning(this,QStringLiteral("通知"),QStringLiteral("未获得有效的列"));
            return;
        }
        else if(colVals.size()<2)
        {
            QMessageBox::information(this,QStringLiteral("通知"),QStringLiteral("至少选择2列以上"));
            return;
        }
        bool isConver = false;
        for(auto i = colVals.begin();i!=colVals.end();i+=2)
        {
            auto col1 = i.value();
            if (i+1 == colVals.end())
                break;
            auto col2 = (i+1).value();
            auto minSize = std::min(col1->size(),col2->size());
            if (0 == minSize)
                continue;
            std::shared_ptr<QVector<QPointF> > onePointColumn;
            onePointColumn.reset(new QVector<QPointF>());
            for(auto j=0;j<minSize;++j)
            {
                double x = col1->at(j).toDouble(&isConver);
                if(!isConver)
                    continue;
                double y = col2->at(j).toDouble(&isConver);
                if(!isConver)
                    continue;
                onePointColumn->append(QPointF(x,y));
            }
            rawData.append(onePointColumn);
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
    QItemSelectionModel* selModel = ui->tableView->selectionModel ();
    int xcolumn(0),ycolumn(0);
    QVector<int> xrows,yrows;
    //提取选中的x，y列
    if(1){
        QModelIndexList indexList = selModel->selectedIndexes ();

        int rowStart(-1),rowEnd(-1);
        const int size = indexList.size ();
        xcolumn = ycolumn = indexList.first ().column ();
        rowStart = rowEnd = indexList.first ().row ();
        for(int i=1;i<size;++i)
        {
            ycolumn = indexList[i].column ();
            if(xcolumn != ycolumn)//说明两个列不相等，可以提取
                break;
        }
        if(xcolumn == ycolumn)
            return false;
        for (int i=0;i<size;++i)
        {
            if(xcolumn == indexList[i].column ())
               xrows.push_back (indexList[i].row ());
            else if(ycolumn == indexList[i].column ())
               yrows.push_back (indexList[i].row ());
        }
    }
    if(xrows.size ()>yrows.size ())
        xrows.resize (yrows.size ());
    else if(xrows.size ()<yrows.size ())
        yrows.resize (xrows.size ());

    SADataTableModel* model = getDataModel ();

    QList<SAAbstractDatas*> datasPtr;
    datasPtr = model->getSADataPtrs ();
    if(datasPtr.size ()<std::max(xcolumn,ycolumn))
        return false;
    SAAbstractDatas* xPtr = datasPtr[xcolumn];
    SAAbstractDatas* yPtr = datasPtr[ycolumn];
    QVector<double> x,y;
    SADataConver::converToDoubleVector(xPtr,x);
    SADataConver::converToDoubleVector(yPtr,y);

    QVector<double> tmp;
    tmp.reserve(xrows.size());
    std::for_each(xrows.begin(),xrows.end(),[&](int i){
        if(i < x.size())
            tmp.push_back(i);
    });
    x.swap(tmp);

    tmp.clear();
    tmp.reserve(yrows.size());
    std::for_each(yrows.begin(),yrows.end(),[&](int i){
        if(i < y.size())
            tmp.push_back(i);
    });
    y.swap(tmp);
    tmp.clear();


    int minSize = std::min(x.size (),y.size ());
    for(int i=0;i<minSize;++i)
    {
        data->append (QPointF(x[i],y[i]));
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

//void SAValueTableWidget::keyPressEvent(QKeyEvent *e)
//{
//    qDebug() << "keyPressEvent:"<<e->modifiers()<<"key:"<<e->key();
//    qDebug() << "match:" <<e->matches(QKeySequence::Undo);
//    if((Qt::ControlModifier &e->modifiers()) && Qt::Key_Z == e->key())
//    {
//        qDebug() << "ctrl + z";
//        m_undo->activate(QAction::Trigger);
//        e->accept();
//    }
//    else if((Qt::ControlModifier|Qt::ShiftModifier) ==e->modifiers() && Qt::Key_Z == e->key())
//    {
//        qDebug() << "shift + ctrl + z";
//        m_redo->activate(QAction::Trigger);
//        e->accept();
//    }

//    QWidget::keyPressEvent(e);
//}

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


