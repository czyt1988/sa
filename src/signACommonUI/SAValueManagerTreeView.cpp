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
#include <QFontMetrics>
#include <QPainter>
#include "SAValueManager.h"

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
}

void SAValueManagerTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeView::dragMoveEvent(event);
}

void SAValueManagerTreeView::dropEvent(QDropEvent *event)
{
    QTreeView::dropEvent(event);
}

void SAValueManagerTreeView::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);
    QList<SAAbstractDatas *> datas = getSeletedDatas();
    SAValueManagerMimeData *mimeData = new SAValueManagerMimeData;
    QList<int> ids;
    std::for_each(datas.begin(),datas.end(),[&ids](SAAbstractDatas* d){
       ids.append(d->getID());
    });
    mimeData->setValueIDs(ids);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(makeDragPixmap(datas,font(),palette()));
    drag->exec();
}
///
/// \brief 生成拖曳的预览图
/// \param datas 拖曳的数据
/// \return
///
QPixmap SAValueManagerTreeView::makeDragPixmap(const QList<SAAbstractDatas *> datas, const QFont &f, const QPalette &pl, int span)
{
    //获取最长的名字
    const int size = datas.size();
    QString longestName;
    QFontMetrics fm(f);
    int pixmapH,pixmapW;
    QSize iconSize = QSize(fm.height(),fm.height());
    pixmapH = fm.height()*size+span*(size+1);

    for(int i=0;i<size;++i)
    {
        if(datas[i]->getName().size()>longestName.size())
        {
            longestName = datas[i]->getName();
        }
    }
    pixmapW = fm.width(longestName) + 3*span + iconSize.width();
    //绘制pixmap
    QPixmap pixmap = QPixmap(pixmapW,pixmapH);
    QPainter painter(&pixmap);
    painter.setBrush(pl.highlight());
    painter.drawRect(pixmap.rect());
    painter.setPen(pl.highlightedText().color());
    int x = span;
    int y = span;
    int textWidth = pixmapW - 3*span - iconSize.width();
    for(int i = 0;i < size;++i)
    {
        //绘制图标
        SAAbstractDatas *d = datas[i];
        QIcon icon = SAValueManager::getDataIcon(d->getType());
        icon.paint(&painter,x,y,iconSize.width(),iconSize.height());
        //绘制文字
        x += (span+iconSize.width());
        painter.drawText(x,y,textWidth,iconSize.height(),Qt::AlignVCenter|Qt::AlignLeft,d->getName());
        x = span;
        y += (span+iconSize.height());
    }
    return pixmap;
}
