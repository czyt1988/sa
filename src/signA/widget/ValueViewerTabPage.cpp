#include "ValueViewerTabPage.h"
#include "ui_ValueViewerTabPage.h"
#include <SADataTableModel.h>
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
ValueViewerTabPage::ValueViewerTabPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ValueViewerTabPage)
  ,m_countNewData(0)
{
    ui->setupUi(this);
    SADataTableModel* model = new SADataTableModel(ui->tableView);
    ui->tableView->setModel (model);
    QHeaderView* plotLayerVerticalHeader = ui->tableView->verticalHeader();
    if(plotLayerVerticalHeader)
    {
        plotLayerVerticalHeader->setDefaultSectionSize(19);
    }
    m_menu = new QMenu(this);
    QMenu* m = m_menu->addMenu (QStringLiteral("导出选中数据"));
    m->addAction(ui->actionToLinerData);
    m->addAction(ui->actionToPointFVectorData);
   // ui->tableView->verticalHeader()->setDefaultSectionSize(12);
}

ValueViewerTabPage::~ValueViewerTabPage()
{
    delete ui;
}

QTableView *ValueViewerTabPage::getTableView()
{
    return ui->tableView;
}

SADataTableModel *ValueViewerTabPage::getModel() const
{
    return static_cast<SADataTableModel*>(ui->tableView->model ());
}

void ValueViewerTabPage::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if(m_menu)
        m_menu->exec (QCursor::pos());
}

void ValueViewerTabPage::on_actionToLinerData_triggered()
{
    ++m_countNewData;
    QHash<int,QVector<double> > rawData;
    getSelectLinerData(rawData);
    auto end = rawData.end ();
    for(auto i=rawData.begin ();i!=end;++i)
    {
        if(i.value ().size() <= 0)
            continue;
        QVariant var = getModel()->headerData (i.key (),Qt::Horizontal,Qt::DisplayRole);
        QString name = var.toString ();
        name += QStringLiteral(" - %1").arg(m_countNewData);
        auto data = SAValueManager::makeData<SAVectorDouble>(name,i.value());
        saValueManager->addData(data);
    }
}

void ValueViewerTabPage::on_actionToPointFVectorData_triggered()
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
//    QVector< std::shared_ptr<QVector<QPointF> > > rawData;
//    getSelectVectorPointData(rawData);
//    for (auto i = rawData.begin();i != rawData.end();++i)
//    {
//        if ((*i)->size() <= 0)
//            continue;
//        bool ok = false;
//        QString name = QInputDialog::getText(this,QStringLiteral("变量名"),QStringLiteral("请输入变量名:")
//                                             ,QLineEdit::Normal,QStringLiteral("点序列"),&ok);
//        if (ok)
//            m_values->addPointDatas(name,*(*i));
//    }
}
///
/// \brief 获取选中的线性数据
/// \param rawData 数据的列存放在hash的key，数据值存放在hash的value
///
void ValueViewerTabPage::getSelectLinerData(QHash<int, QVector<double> > &rawData) const
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
void ValueViewerTabPage::getSelectVectorPointData(QVector<std::shared_ptr<QVector<QPointF> > > &rawData, int dim)
{
    QItemSelectionModel* selModel = ui->tableView->selectionModel ();
    if(0 == dim)
    {
        QMap<int, std::shared_ptr<QVector<QVariant> > > colVals;
        getQItemSelectionColumns(selModel,colVals);
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
bool ValueViewerTabPage::getSelectVectorPointData(SAVectorPointF* data)
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

    SADataTableModel* model = getModel ();

    QList<SAAbstractDatas*> datasPtr;
    model->getSADataPtrs (datasPtr);
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

void ValueViewerTabPage::appendDataFromVariant(const QVariant &var, QVector<double> &data) const
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
void ValueViewerTabPage::getQItemSelectionColumns(QItemSelectionModel *selModel, QMap<int, std::shared_ptr<QVector<QVariant> > > &res)
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
#include <QDebug>
void ValueViewerTabPage::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        qDebug()<<numSteps;
    }
    event->accept();
}
