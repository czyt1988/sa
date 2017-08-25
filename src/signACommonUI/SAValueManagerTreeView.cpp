#include "SAValueManagerTreeView.h"
#include "SAValueManagerModel.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDebug>
#include "SAValueManagerMimeData.h"
#include <QItemSelectionModel>
#include "SAAbstractDatas.h"
#include <QDrag>
SAValueManagerTreeView::SAValueManagerTreeView(QWidget *parent)
    :QTreeView(parent)
{
    m_modelValueMgr = new SAValueManagerModel(this);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setModel(m_modelValueMgr);
}

SAValueManagerModel *SAValueManagerTreeView::getValueManagerModel() const
{
    return m_modelValueMgr;
}

QList<SAAbstractDatas *> SAValueManagerTreeView::getSeletedDatas() const
{
    QItemSelectionModel *sel = selectionModel();
    QModelIndexList indexList = sel->selectedRows();
    QList<SAAbstractDatas*> datas;
    for(int i=0;i<indexList.size ();++i)
    {
        SAAbstractDatas* data = m_modelValueMgr->castToDataPtr(indexList[i]);
        if(nullptr != data)
        {
            datas.append(data);
        }
    }
    return datas;
}

SAAbstractDatas *SAValueManagerTreeView::getSeletedData() const
{
    QModelIndex index = currentIndex();
    if(!index.isValid())
    {
        return nullptr;
    }
    return m_modelValueMgr->castToDataPtr(index);
}

void SAValueManagerTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    QTreeView::dragEnterEvent(event);
    qDebug() <<" SAValueManagerTreeView::dragEnterEvent";
}

void SAValueManagerTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeView::dragMoveEvent(event);
    qDebug() <<" SAValueManagerTreeView::dragMoveEvent";
}

void SAValueManagerTreeView::dropEvent(QDropEvent *event)
{
    QTreeView::dropEvent(event);
    qDebug() <<" SAValueManagerTreeView::dropEvent";
}

void SAValueManagerTreeView::startDrag(Qt::DropActions supportedActions)
{
    QTreeView::startDrag(supportedActions);
    qDebug() <<" SAValueManagerTreeView::startDrag";

    QList<SAAbstractDatas *> datas = getSeletedDatas();
    SAValueManagerMimeData *mimeData = new SAValueManagerMimeData;
    QList<int> ids;
    std::for_each(datas.begin(),datas.end(),[&ids](SAAbstractDatas* d){
       ids.append(d->getID());
    });
    mimeData->setValueIDs(ids);

     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
//     drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
//     drag->setPixmap(pixmap);
     Qt::DropAction dropAction = drag->exec();
}
