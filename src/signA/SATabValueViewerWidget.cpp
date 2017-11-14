#include "SATabValueViewerWidget.h"
#include <SADataTableModel.h>
//#include <ValueDataViewerTabWidgetPage.h>
#include <ValueViewerTabPage.h>
#include <QFileDialog>
#include <QTabBar>
#include <QTableView>
#include "SAResourDefine.h"

SATabValueViewerWidget::SATabValueViewerWidget(QWidget *parent):
    QTabWidget(parent)
  ,m_count(0)
{
    setTabsClosable(true);
    m_menuTab = new QMenu(tr("table operation"),this);//表格操作
    QMenu* save=new  QMenu(tr("save"),m_menuTab);
    save->setIcon (ICON_Save);
    QAction* actSave2csv = new QAction(this);
    actSave2csv->setText(tr("save as csv"));
    save->addAction(actSave2csv);
    m_menuTab->addMenu (save);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(actSave2csv,&QAction::triggered,this,&SATabValueViewerWidget::action_saveToCsv_triggered);
    connect(this,&QWidget::customContextMenuRequested,this,&SATabValueViewerWidget::tabBarCustomContextMenuRequested);
    connect(this,&QTabWidget::tabCloseRequested,this,&SATabValueViewerWidget::on_tab_closed);
}

SATabValueViewerWidget::~SATabValueViewerWidget()
{

}

///
/// \brief SATabValueViewerWidget::setDataInCurrentTab
/// \param datas
///
void SATabValueViewerWidget::setDataInCurrentTab(QList<SAAbstractDatas *> datas)
{
    QList<SAAbstractDatas*> tables;
    takeTableValue (datas,tables);
    if(datas.size () > 0)
    {
        SADataTableModel* model = getCurrentTabModel();
        if(!model)
            return;
        model->setSADataPtrs (datas);
    }
    if(tables.size ()>0)
    {//有表格数据
        setTableData (tables);
    }
}

void SATabValueViewerWidget::setDataInNewTab(QList<SAAbstractDatas *> datas)
{
    QList<SAAbstractDatas*> tables;
    takeTableValue (datas,tables);
    if(datas.size () > 0)
    {
        QString strName;
        if(datas.size () == 1)
            strName = datas[0]->getName ();
        SADataTableModel* model = createValueViewerTab(strName);
        if(!model)
            return;
        model->setSADataPtrs (datas);
    }
    if(tables.size ()>0)
    {//有表格数据
        setTableData (tables);
    }
}

void SATabValueViewerWidget::appendDataInCurrentTab(QList<SAAbstractDatas *> datas)
{
    QList<SAAbstractDatas*> tables;
    takeTableValue (datas,tables);
    if(datas.size () > 0)
    {
        SADataTableModel* model = getCurrentTabModel();
        if(!model)
            return;
        model->appendSADataPtrs (datas);
        model->update ();
    }
    if(tables.size ()>0)
    {//有表格数据
        setTableData (tables);
    }
}

SADataTableModel *SATabValueViewerWidget::createValueViewerTab(const QString &title)
{
    ++m_count;
    ValueViewerTabPage* w = new ValueViewerTabPage(this);
    QString t = title;
    if(t.isEmpty ())
        t = tr("  table-%1  ").arg(m_count);
    int index = addTab (w,t);
    setCurrentIndex(index);
    return w->getModel ();
}

SADataTableModel *SATabValueViewerWidget::getTabModel(int index)
{
    QWidget* w = widget (index);
    if(!w)
        return nullptr;
    ValueViewerTabPage* tw = qobject_cast<ValueViewerTabPage*>(w);
    if (!tw)
        return nullptr;
    return tw->getModel ();
}
///
/// \brief 数据删除
///
/// 数据删除时需要调用此函数，把保存的数据指针删除掉
/// \param datas要删除的数据指针
///
void SATabValueViewerWidget::removeDatas(const QList<SAAbstractDatas *> &datas)
{
    const int size = count();
    for(int i=0;i<size;++i)
    {
        SADataTableModel* tabModel = getTabModel(i);
        if(nullptr == tabModel)
        {
            continue;
        }
        tabModel->removeDatas(datas);
    }
}
///
/// \brief SATabValueViewerWidget::clearAndReleaseAll
///
void SATabValueViewerWidget::clearAndReleaseAll()
{
    while(count())
    {
        QWidget* w = widget(0);
        if(w)
        {
            delete w;
        }
        removeTab(0);
    }
}

void SATabValueViewerWidget::tabBarCustomContextMenuRequested(const QPoint &pos)
{
    if(tabBar ()->tabAt (pos) == currentIndex ())
    {
        if(m_menuTab)
            m_menuTab->exec (QCursor::pos());
    }
}

void SATabValueViewerWidget::action_saveToCsv_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,tr("save"),QString()
                                 ,tr("csv file(*.csv)"));
    SADataTableModel* model = getTabModel(currentIndex ());
    if(!model)
        return;
    model->saveToCsv (path);
}

void SATabValueViewerWidget::on_tab_closed(int index)
{
    QWidget* w = widget(index);
    removeTab (index);
    if(w)
    {
        ValueViewerTabPage* t = qobject_cast<ValueViewerTabPage*>(w);
        if(t)
            delete t;
    }
}

SADataTableModel *SATabValueViewerWidget::getCurrentTabModel()
{
    int index = currentIndex ();
    SADataTableModel* model = nullptr;
    if(-1 == index)
        model = createValueViewerTab(QString());
    else
        model = getTabModel(index);
    return model;
}

void SATabValueViewerWidget::takeTableValue(QList<SAAbstractDatas *> &originDatas, QList<SAAbstractDatas *> &tableValue)
{
    for(auto i=originDatas.begin ();i!= originDatas.end ();)
    {
        if((*i)->getDim() == 2)
        {
            tableValue.push_back (*i);
            i = originDatas.erase (i);
        }
        else
            ++i;
    }
}

void SATabValueViewerWidget::setTableData(QList<SAAbstractDatas *> tableDatas)
{
#if 0
    for(int i=0;i<tableDatas.size ();++i)
    {
        if((tableDatas[i])->getDim() != 2)
            continue;
        SADataTableModel* model = createValueViewerTab (tableDatas[i]->getName ());
        if(!model)
            continue;

        model->setSATableData(tableDatas[i]);
    }
#else
    if(tableDatas.size() <= 0)
    {
        return;
    }
    SADataTableModel* model = nullptr;
    if(1 == tableDatas.size())
    {
        model = createValueViewerTab (tableDatas[0]->getName ());
    }
    else
    {
        model = createValueViewerTab (tr("table value"));
    }
    model->setSADataPtrs(tableDatas);
#endif
}

