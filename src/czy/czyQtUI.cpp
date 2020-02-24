#include "czyQtUI.h"
///
/// \brief 迭代QStandardItemModel下的所有条目
/// \param model
/// \param pFun 为回调函数，输入为当前迭代的QStandardItem指针，回调函数返回true代表继续迭代，回调函数返回false代表结束迭代
///
void SA::QtUI::SAStandardItemModel::ergodicAllItem(const QStandardItemModel* model, SA::QtUI::SAStandardItemModel::callback_ergodicFun_ptr pFun)
{
    int rows = model->rowCount();
    int column = model->columnCount();
    for (int i=0;i<rows;++i)
    {
        for(int j=0;j<column;++j)
        {
            QStandardItem* item = model->item(i,j);
            if (item)
            {
                if(!ergodicItem(item,pFun))
                    return;
            }
        }
    }
}
///
/// \brief 从某一个QStandardItem开始向下迭代，如果迭代结束，返回false
/// \param item 开始迭代的QStandardItem
/// \param pFun 为回调函数，输入为当前迭代的QStandardItem指针，回调函数返回true代表继续迭代，回调函数返回false代表结束迭代
/// \return false 代表迭代结束
///
bool SA::QtUI::SAStandardItemModel::ergodicItem(QStandardItem* item, SA::QtUI::SAStandardItemModel::callback_ergodicFun_ptr pFun)
{
    int rows = item->rowCount();
    int column = item->columnCount();
    for (int i=0;i<rows;++i)
    {
        for(int j=0;j<column;++j)
        {
            QStandardItem* childItem = item->child(i,j);
            if (childItem)
            {
                if(!ergodicItem(childItem,pFun))
                    return false;
            }
        }
    }
    return pFun(item);
}


#include <QEvent>
#include <QtWidgets/QWidget>
void SA::QtUI::MainWindowEx::setHidden()
{
    hide();
    emit hiddenWindow(this);
}

bool SA::QtUI::MainWindowEx::eventFilter(QObject *object, QEvent *e)
{
    if (e->type() == QEvent::ContextMenu && object == this->window()){
        emit showContextMenu();
        return true;
    }
    return QMainWindow::eventFilter(object, e);
}

void SA::QtUI::MainWindowEx::closeEvent(QCloseEvent *e)
{
    QMainWindow::closeEvent(e);
    emit closedWindow(this);
}


void SA::QtUI::QWidgetEx::setHidden()
{
    hide();
    emit hiddenWindow(this);
}

bool SA::QtUI::QWidgetEx::eventFilter(QObject* object, QEvent* e)
{
    if (e->type() == QEvent::ContextMenu && object == this->window()){
        emit showContextMenu();
        return true;
    }
    return QWidget::eventFilter(object, e);
}

void SA::QtUI::QWidgetEx::closeEvent(QCloseEvent* e)
{
    QWidget::closeEvent(e);
    emit closedWindow(this);
}
