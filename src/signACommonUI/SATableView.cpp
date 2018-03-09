#include "SATableView.h"
#include <QKeyEvent>
#include <QClipboard>
#include <QItemSelectionModel>
#include <QApplication>
#include <QTextStream>

SATableView::SATableView(QWidget *par):QTableView(par)
    ,onCtrlVFun(nullptr)
    ,onCtrlCFun(nullptr)
{

}

void SATableView::keyPressEvent(QKeyEvent *e)
{
    if(Qt::ControlModifier == e->modifiers() && Qt::Key_C == e->key())
    {
        onCtrlCPressed();
        return;
    }
    else if(Qt::ControlModifier == e->modifiers() && Qt::Key_V == e->key())
    {
        onCtrlVPressed();
        return;
    }
    QTableView::keyPressEvent(e);
}
///
/// \brief 处理ctrl+c事件
///
void SATableView::onCtrlCPressed()
{
    if(onCtrlCFun)
    {
        onCtrlCFun();
        return;
    }
    QItemSelectionModel *selModel = selectionModel();
    if(!selModel || !(selModel->hasSelection()))
    {
        return;
    }
    QModelIndexList sels = selModel->selectedIndexes();
    const int selectionCount = sels.size();
    int minR,minC,maxR,maxC;
    const QSize selectSize = getSelectSize(sels,&minR,&maxR,&minC,&maxC);

    if(!selectSize.isValid())
    {
        return;
    }

    QString **tableString;//二维数组
    tableString = new QString*[selectSize.height()];
    for (int i=0;i<selectSize.height();++i)
    {
        tableString[i] = new QString[selectSize.width()];
    }

    for(int i=0;i<selectionCount;++i)
    {
        int r = sels[i].row()-minR;
        int c = sels[i].column()-minC;
        tableString[r][c] = sels[i].data().toString();
    }
    QString res = toTabString(tableString,selectSize.height(),selectSize.width());
    QClipboard *clipboard = QApplication::clipboard();
    if(clipboard)
    {
        clipboard->setText(res);
    }
    //释放
    for (int i=0;i<selectSize.height();++i)
    {
        delete [] tableString[i];
    }
    delete tableString;
}
///
/// \brief 处理ctrl+v事件
///
/// 从函数将会调用model的setData进行赋值
///
void SATableView::onCtrlVPressed()
{
    if(onCtrlVFun)
    {
        onCtrlVFun();
        return;
    }
    QClipboard *clipboard = QApplication::clipboard();
    if(nullptr == clipboard)
    {
        return;
    }
    QString str = clipboard->text();
    if(str.isEmpty())
    {
        return;
    }
    QAbstractItemModel* itemModel = model();
    if(nullptr == itemModel)
    {
        return;
    }
    //获取当前选中的单元格的行、列
    int rowStart=0,colStart=0;
    QModelIndex curIndex = currentIndex();
    if(curIndex.isValid())
    {
        rowStart = curIndex.row();
        colStart = curIndex.column();
    }
    QTextStream st(&str);
    int r = 0;
    while(!st.atEnd())
    {
        QString lineStr = st.readLine();
        QStringList sp = lineStr.split('\t');
        for(int c=0;c<sp.size();++c)
        {
            itemModel->setData(itemModel->index(r+rowStart,c+colStart),sp[c]);
        }
        ++r;
    }
}
///
/// \brief 获取一系列选择的范围，就是行数和列数
/// \param indexs 输入的QModelIndexList
/// \param minRow 所选的idnexs在表格中最小行数
/// \param maxRow 所选的idnexs在表格中最大行数
/// \param minCol 所选的idnexs在表格中最小列数
/// \param maxCol 所选的idnexs在表格中最大列数
/// \return size 的width对应的是列数，height对应的是行数
///
QSize SATableView::getSelectSize(const QModelIndexList &indexs, int *minRow, int *maxRow, int *minCol, int *maxCol)
{
    if(indexs.isEmpty())
    {
        return QSize();
    }
    const int size = indexs.size();
    int minR,minC,maxR,maxC;
    minR = maxR = indexs[0].row();
    minC = maxC = indexs[0].column();
    for(int i=1;i<size;++i)
    {
        int t = indexs[i].row();
        minR = qMin(minR,t);
        maxR = qMax(maxR,t);
        t = indexs[i].column();
        minC = qMin(minC,t);
        maxC = qMax(maxC,t);
    }
    if(minRow)
    {
        *minRow = minR;
    }
    if(maxRow)
    {
        *maxRow = maxR;
    }
    if(minCol)
    {
        *minCol = minC;
    }
    if(maxCol)
    {
        *maxCol = maxC;
    }
    return QSize(maxC-minC+1,maxR-minR+1);//宽对应的是列数
}

///
/// \brief 把二维字符串表转换为tab分隔的单一字符串
/// \param tableString 字符串表格
/// \param r 字符串二维数组的行数 table[r][c]
/// \param c 字符串数组的列数 table[r][c]
/// \return
///
QString SATableView::toTabString(QString **tableString, const int r, const int c)
{
    QString res;
    for(int i=0;i<r;++i)
    {
        if(0 != i)
        {
            res += '\n';
        }
        for(int j=0;j<c;++j)
        {
            if(0 != j)
            {
                res += '\t';
            }
            if(!(tableString[i][j].isEmpty()))
            {
                res += tableString[i][j];
            }
        }
    }
    return res;
}
