#ifndef SADATAFEATUREWIDGET_H
#define SADATAFEATUREWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QMap>
#include "../global/SAGlobals.h"
#include <memory>
#include <QTimer>
#include <QDateTime>
#include "SADataClient.h"


class QwtPlotItem;
class QMdiSubWindow;
class QAbstractItemModel;
class SAFigureWindow;
class DataFeatureTreeModel;
class SAChart2D;
class SAFigureSetWidget;


namespace Ui {
class SADataFeatureWidget;
}

class SADataFeatureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SADataFeatureWidget(QWidget *parent = 0);
    ~SADataFeatureWidget();
public slots:
    //子窗口激活槽
    void mdiSubWindowActived(QMdiSubWindow *arg1);
    //子窗口关闭槽
    void mdiSubWindowClosed(QMdiSubWindow *arg1);
signals:
    //显示消息
    void showMessageInfo(const QString& info,SA::MeaasgeType messageType = SA::NormalMessage);
private slots:
    //树形控件点击
    void onTreeViewClicked(const QModelIndex &index);
    //点击清除属性按钮
    void onToolButtonClearDataFeatureClicked();
    //图片隐藏触发的槽，隐藏绘图需要对显示的信息也隐藏
    void onChartHide();
    //绘图销毁触发的槽，绘图销毁，对数据进行销毁
    void onChartDestroy();
    //fig窗口销毁
    void onFigureDestroy();
private:
    //对MdiSubWindow进行绑定
    void bindMdiSubWindow(QMdiSubWindow *w);
    //对已经绑定的MdiSubWindow进行解绑
    void unbindMdiSubWindow(QMdiSubWindow *w);
    //获取mdisubwindow的FigureWindow
    SAFigureWindow* getChartWidgetFromSubWindow(QMdiSubWindow* sub);
    //计算绘图窗口的dataFeature
    void callCalcFigureWindowFeature(SAFigureWindow* figure);
    //检测model是否需要重新计算datafeature
    void checkModelItem(QAbstractItemModel* baseModel,QMdiSubWindow *subWndPtr);
private:
    //计算一个plot item
    void calcPlotItemFeature(const QwtPlotItem* plotitem,const QMdiSubWindow *midwidget,const SAChart2D* chartptr);
    //定时心跳检测时间到达触发槽
    Q_SLOT void onHeartbeatCheckerTimerout();
private:
    Ui::SADataFeatureWidget *ui;
    QMdiSubWindow* m_lastActiveSubWindow;///< 记录最后激活的子窗口
    QMap<QMdiSubWindow*,QAbstractItemModel*> m_subWindowToDataInfo;///< 记录子窗口对应的数据属性表上显示的model
private://数据接收相关的类型
    SADataClient m_client;
    class TmpStru{
    public:
        TmpStru(QwtPlotItem *p1,QMdiSubWindow *p2,SAChart2D *p3)
            :plotitem(p1),mdiSubWnd(p2),chart2d(p3)
        {

        }
        TmpStru()
            :plotitem(nullptr),mdiSubWnd(nullptr),chart2d(nullptr)
        {

        }
        QwtPlotItem *plotitem;
        QMdiSubWindow *mdiSubWnd;
        SAChart2D *chart2d;
    };
    uint m_wndPtrKey;
    QHash<uint,TmpStru> m_key2wndPtr;
};

#endif // DATAFEATUREWIDGET_H
