#include "SATabValueViewerWidget.h"
#include <SADataTableModel.h>
//#include <ValueDataViewerTabWidgetPage.h>
#include <ValueViewerTabPage.h>
#include <QFileDialog>
#include <QTabBar>
#include <QTableView>
#include "SAResourDefine.h"
#include <QMessageBox>
#include "SACsvStream.h"
SATabValueViewerWidget::SATabValueViewerWidget(QWidget *parent):
    QTabWidget(parent)
  ,m_count(0)
  ,m_menuTab(nullptr)
{
    setTabsClosable(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
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
void SATabValueViewerWidget::setDataInCurrentTab(const QList<SAAbstractDatas *> &datas)
{
    SAValueTableWidget* w = getCurrentTablePage();
    if(!w)
        return;
    w->setSADataPtrs (datas);
}

void SATabValueViewerWidget::setDataInNewTab(QList<SAAbstractDatas *> datas)
{
    QString strName;
    if(datas.size () == 1)
        strName = datas[0]->getName ();
    SAValueTableWidget* w = createValueViewerTab(strName);
    if(!w)
        return;
    w->setSADataPtrs (datas);
}

void SATabValueViewerWidget::appendDataInCurrentTab(QList<SAAbstractDatas *> datas)
{
    SAValueTableWidget* w = getCurrentTablePage();
    if(!w)
        return;
    w->appendSADataPtrs (datas);
}

SAValueTableWidget *SATabValueViewerWidget::createValueViewerTab(const QString &title)
{
    ++m_count;
    SAValueTableWidget* w = new SAValueTableWidget(this);
    QString t = title;
    if(t.isEmpty ())
        t = tr("  table-%1  ").arg(m_count);
    int index = addTab (w,t);
    setCurrentIndex(index);
    return w;
}

SAValueTableWidget *SATabValueViewerWidget::getTablePage(int index)
{
    QWidget* w = widget (index);
    if(!w)
        return nullptr;
    SAValueTableWidget* tw = qobject_cast<SAValueTableWidget*>(w);
    if (!tw)
        return nullptr;
    return tw;
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
        SAValueTableWidget* w = getTablePage(i);
        if(nullptr == w)
        {
            continue;
        }
        w->removeDatas(datas);
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
        if(nullptr == m_menuTab)
        {
            m_menuTab = new QMenu(tr("table operation"),this);//表格操作
            QMenu* save=new  QMenu(tr("save"),m_menuTab);
            save->setIcon (ICON_Save);
            QAction* actSave2csv = new QAction(this);
            actSave2csv->setText(tr("save as csv"));
            save->addAction(actSave2csv);
            m_menuTab->addMenu (save);
            connect(actSave2csv,&QAction::triggered,this,&SATabValueViewerWidget::action_saveToCsv_triggered);
        }
        m_menuTab->exec (QCursor::pos());
    }
}

void SATabValueViewerWidget::action_saveToCsv_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,tr("save"),QString()
                                 ,tr("csv file(*.csv)"));
    if(path.isEmpty())
        return;
    SAValueTableWidget* w = getTablePage(currentIndex ());
    if(!w)
        return;
    w->saveTableToCsv(path);
}

void SATabValueViewerWidget::on_tab_closed(int index)
{
    QWidget* w = widget(index);
    removeTab (index);
    if(w)
    {
        SAValueTableWidget* t = qobject_cast<SAValueTableWidget*>(w);
        if(t)
            delete t;
    }
}

SAValueTableWidget *SATabValueViewerWidget::getCurrentTablePage()
{
    int index = currentIndex ();
    SAValueTableWidget* w = nullptr;
    if(-1 == index)
        w = createValueViewerTab(QString());
    else
        w = getTablePage(index);
    return w;
}





