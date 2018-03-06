#ifndef SATABVALUEVIEWERWIDGET_H
#define SATABVALUEVIEWERWIDGET_H
#include <QTabWidget>
#include <QMenu>
#include <QAction>
#include <SAData.h>

class SAValueTableWidget;
class SATabValueViewerWidget : public QTabWidget
{
    Q_OBJECT
public:
    SATabValueViewerWidget(QWidget *parent = 0);
    ~SATabValueViewerWidget();
    //显示数据在当前tab
    void setDataInCurrentTab(const QList<SAAbstractDatas*>& datas);
    //显示数据在新的标签页
    void setDataInNewTab(QList<SAAbstractDatas*> datas);
    //在当前标签中添加新的数据
    void appendDataInCurrentTab(QList<SAAbstractDatas*> datas);
    //新建一个数据表
    SAValueTableWidget *createValueViewerTab(const QString& title);
    //获取对应数据表的model
    SAValueTableWidget *getTablePage(int index);
    //数据要删除，先把显示的内容擦除
    void removeDatas(const QList<SAAbstractDatas*>& datas);
public slots:
    //添加且释放内存
    void clearAndReleaseAll();
private slots:
    void tabBarCustomContextMenuRequested(const QPoint &pos);
    void action_saveToCsv_triggered();
    void on_tab_closed(int index);
private:
    SAValueTableWidget* getCurrentTablePage();
private:
    //OpenFileManager* m_values;
    QMenu* m_menuTab;
    unsigned int m_count;
};

#endif // SATABVALUEVIEWERWIDGET_H
