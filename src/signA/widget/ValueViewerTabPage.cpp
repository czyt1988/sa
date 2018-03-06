#include "ValueViewerTabPage.h"
#include "ui_ValueViewerTabPage.h"
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
class SAValueTableWidgetBaseCommand : public QUndoCommand
{
public:
    SAValueTableWidgetBaseCommand(SAAbstractDatas* data,SADataTableModel* model,QUndoCommand* par = Q_NULLPTR)
        :m_data(data)
        ,m_model(model)
    {
    }
    SAAbstractDatas* data() const
    {
        return m_data;
    }
    SADataTableModel* model() const
    {
        return m_model;
    }
private:
    SAAbstractDatas* m_data;
    SADataTableModel* m_model;
};

///
/// \brief 编辑命令
///
class SAValueTableWidgetEditValueCommand : public SAValueTableWidgetBaseCommand
{
public:
    SAValueTableWidgetEditValueCommand(SAAbstractDatas* data
                                       ,SADataTableModel* model
                                       ,const QVariantList& oldDatas
                                       ,const QVariantList& newDatas
                                       ,int row
                                       ,int col
                                       ,QUndoCommand* par = Q_NULLPTR);
    void redo();
    void undo();
private:
    QVariantList m_oldDatas;
    QVariantList m_newDatas;
    int m_row;
    int m_col;
};
SAValueTableWidgetEditValueCommand::SAValueTableWidgetEditValueCommand(SAAbstractDatas *data
                                                                       ,SADataTableModel* model
                                                                       , const QVariantList &oldDatas
                                                                       , const QVariantList &newDatas
                                                                       ,int row
                                                                       ,int col
                                                                       , QUndoCommand *par
                                                                       )
    :SAValueTableWidgetBaseCommand(data,model,par)
    ,m_oldDatas(oldDatas)
    ,m_newDatas(newDatas)
    ,m_row(row)
    ,m_col(col)
{

}

void SAValueTableWidgetEditValueCommand::redo()
{
    SAAbstractDatas* d = data();
    SADataTableModel* m = model();
    if(SA::TableVariant == d->getType() || SA::TableDouble == d->getType())
    {
        bool isOK = d->setAt(m_newDatas[0],{m_row,m_col});
        if(isOK && (m_row>=m->dataRowCount() || m_col>=m->dataColumnCount()) )
        {
            m->update();
        }
    }
    else if(d->getDim() < SA::Dim2)
    {
        d->setAt(m_newDatas[0],{m_row});
        if(m_row==m->dataRowCount()-1)
        {
            m->update();
        }
    }
}

void SAValueTableWidgetEditValueCommand::undo()
{
    SAAbstractDatas* d = data();
    SADataTableModel* m = model();
    if(SA::TableVariant == d->getType() || SA::TableDouble == d->getType())
    {
        bool isOK = d->setAt(m_oldDatas[0],{m_row,m_col});
        if(isOK && (m_row>=m->dataRowCount() || m_col>=m->dataColumnCount()) )
        {
            m->update();
        }
    }
    if(d->getDim() < SA::Dim2)
    {
        d->setAt(m_oldDatas[0],{m_row});
        if(m_row==m->dataRowCount()-1)
        {
            m->update();
        }
    }
}






/////////////////////////////////////////////////

SAValueTableWidget::SAValueTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ValueViewerTabPage)
  ,m_countNewData(0)
  ,m_menu(nullptr)
{
    ui->setupUi(this);
    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(20);
    SADataTableModel* model = new SADataTableModel(ui->tableView);
    ui->tableView->setModel (model);
    QHeaderView* plotLayerVerticalHeader = ui->tableView->verticalHeader();
    if(plotLayerVerticalHeader)
    {
        plotLayerVerticalHeader->setDefaultSectionSize(19);
    }
    ui->tableView->setCtrlVFunPtr([&](){
        this->onTableViewCtrlV();
    });
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

void SAValueTableWidget::onTableViewCustomContextMenuRequested(const QPoint &pos)
{
    if(nullptr == m_menu)
    {
        m_menu = new QMenu(this);
        QAction* act = nullptr;
        act = m_undoStack->createUndoAction(m_menu,tr("undo"));
        m_menu->addAction(act);
        act = m_undoStack->createRedoAction(m_menu,tr("redo"));
        m_menu->addAction(act);
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
    int r = index.row();
    int c = index.column();
    SADataTableModel* model = getDataModel();
    SAAbstractDatas* data = model->columnToData(c);
    if(nullptr == data)
        return;
    QHeaderView* verticalHeader = ui->tableView->verticalHeader();
    QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
    QPoint offset(verticalHeader->width(),horizontalHeader->height());
    int colStart=-1,colEnd=-1;
    model->dataColumnRange(data,colStart,colEnd);
    if(-1 == colStart || -1 == colEnd)
    {
        return;
    }



    //对于2维数据要特殊处理
    if(SA::TableVariant == data->getType() || SA::TableDouble == data->getType())
    {
        //表格
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

        SACellInputWidget cellInput;
        int buttonAreaWidth = cellInput.getButtonAreaWidth();
        rightBottom.rx() += buttonAreaWidth;
        cellInput.setGeometry(QRect(leftTop,rightBottom));
        cellInput.resizeCells(1);

        QVariant var = model->data(index);
        cellInput.setCellTitleText(0,tr("value:"));
        cellInput.setCellEditText(0,var.toString());
        cellInput.exec();
        if(!cellInput.isAcceptInput())
        {
            return;
        }
        QString str = cellInput.getCellEditText(0);
        SAValueTableWidgetEditValueCommand* cmd = new SAValueTableWidgetEditValueCommand(data
                                                                                         ,model
                                                                                         ,{var}
                                                                                         ,{str}
                                                                                         ,r,c);
        cmd->setText(tr("edit table:%1").arg(data->getName()));
        m_undoStack->push(cmd);
//        if(isOK && (r>=model->dataRowCount() || c>=model->dataColumnCount()) )
//        {
//            model->update();
//        }
        return;
    }
    else
    {
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

        SACellInputWidget cellInput;
        int buttonAreaWidth = cellInput.getButtonAreaWidth();
        rightBottom.rx() += buttonAreaWidth;
        cellInput.setGeometry(QRect(leftTop,rightBottom));

        int dataDim2Size = data->getSize(SA::Dim2);
        cellInput.resizeCells(dataDim2Size);
        if(data->getDim() < SA::Dim2)
        {
            //对于小于2维的直接设置
            QVariant var = model->data(index);
            cellInput.resizeCells(1);
            cellInput.setCellTitleText(0,tr("value"));
            cellInput.setCellEditText(0,var.toString());
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

            SAValueTableWidgetEditValueCommand* cmd = new SAValueTableWidgetEditValueCommand(data
                                                                                             ,model
                                                                                             ,{var}
                                                                                             ,{d}
                                                                                             ,r
                                                                                             ,c);
            cmd->setText(tr("edit value:%1").arg(data->getName()));
            m_undoStack->push(cmd);

//            if(r==model->dataRowCount()-1)
//            {
//                model->update();
//            }
            return;
        }
        else
        {
            //根据类型设定文字显示
            switch(data->getType())
            {
            case SA::VectorPoint:
            {
                if(2 != dataDim2Size)
                    return;
                cellInput.setCellTitleText(0,tr("x"));
                cellInput.setCellTitleText(1,tr("y"));
                cellInput.setCellEditText(0,model->data(model->index(r,colStart)).toString());
                cellInput.setCellEditText(1,model->data(model->index(r,colStart+1)).toString());
                cellInput.setCellWidth(0,ui->tableView->columnWidth(colStart));
                cellInput.setCellWidth(1,ui->tableView->columnWidth(colStart+1));
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
                SAVectorPointF* vd = static_cast<SAVectorPointF*>(data);
                if(r < vd->getSize())
                {
                    vd->set(r,QPointF(x,y));
                }
                else if(r == vd->getSize())
                {
                    vd->append(QPointF(x,y));
                }

                break;
            }
            default:
                return;
            }
        }
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
/// \brief 设置数据
/// \param r
/// \param c
/// \param v
/// \return
///
bool SAValueTableWidget::setData(int r, int c, const QVariant &v)
{
    qDebug() << "setData";
    SADataTableModel* model = getDataModel();
    SAAbstractDatas* d = model->columnToData(c);
    if(nullptr == d)
    {
        return false;
    }
    if(d->getDim() < SA::Dim2)
    {
        //对于小于2维的直接设置
        bool isOK = d->setAt(v,{r,c});
        if(isOK && r==model->rowCount()-1)
        {
            model->update();
        }
        return isOK;
    }
    else
    {
        //对于2维数据要特殊处理
        if(d->getDim() == SA::Dim2)
        {
            if(SA::TableVariant == d->getType() || SA::TableDouble == d->getType())
            {
                //表格
            }
            else
            {
                qDebug() << "============";
                int colStart=-1,colEnd=-1;
                model->dataColumnRange(d,colStart,colEnd);
                if(-1 == colStart || -1 == colEnd)
                {
                    return false;
                }
                QPoint leftTop,rightBottom;
                leftTop.rx() = ui->tableView->columnViewportPosition(colStart);
                rightBottom.rx() = ui->tableView->columnViewportPosition(colEnd);
                int tmp = ui->tableView->columnWidth(colEnd);
                rightBottom.rx() += tmp;
                leftTop.ry() = ui->tableView->rowViewportPosition(r);
                tmp = ui->tableView->rowHeight(r);
                rightBottom.ry() = leftTop.y() + tmp;
                if(tmp < 16 || tmp > 30)
                {
                    tmp = 16;
                }
                leftTop.ry() -= tmp;
                leftTop = ui->tableView->mapToGlobal(leftTop);
                rightBottom = ui->tableView->mapToGlobal(rightBottom);
                int dataDim2Size = d->getSize(SA::Dim2);
                SACellInputWidget cellInput;
                cellInput.resizeCells(dataDim2Size);
                cellInput.setGeometry(QRect(leftTop,rightBottom));

                qDebug() << "SACellInputWidget Geometry"<<cellInput.geometry();
                cellInput.exec();
                return false;
            }

        }
    }
    return false;
}
///
/// \brief 表格控件处理按下ctrl + v
///
void SAValueTableWidget::onTableViewCtrlV()
{
    SAWaitCursor waitCursor;
    QVector<QStringList> paseredStringTable;
    QSize tableSize = getClipboardTextTable(paseredStringTable);
    if(!tableSize.isValid())
        return;
    QModelIndex curIndex = ui->tableView->currentIndex();
    if(!curIndex.isValid())
        return;
    SADataTableModel* model = getDataModel();

    //情况1 复制的内长度小于数据的第二维度，且小于第一维度
    //考虑到使用ctrl+z 用命令模式
//    SAAbstractDatas* data = model->columnToData(curIndex.column());
//    if(nullptr == data)
//        return;
//    int colStart=-1,colEnd=-1;
//    model->dataColumnRange(data,colStart,colEnd);

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
            appendDataFromVariant(indexs[i].data (),rawData[col]);
        }
        else
        {
            QVector<double> temp;
            appendDataFromVariant(indexs[i].data (),temp);
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
    SAAbstractDatas::converToDoubleVector(xPtr,x);
    SAAbstractDatas::converToDoubleVector(yPtr,y);

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

void SAValueTableWidget::appendDataFromVariant(const QVariant &var, QVector<double> &data) const
{
    bool isOk = false;
    double d = var.toDouble (&isOk);
    if(isOk)
        data.push_back (d);
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

QSize SAValueTableWidget::getClipboardTextTable(QVector<QStringList > &res)
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


