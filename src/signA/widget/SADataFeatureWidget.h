#ifndef SADATAFEATUREWIDGET_H
#define SADATAFEATUREWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QMap>
#include "../global/SAGlobals.h"
#include <memory>
#include <QTimer>
#include <QDateTime>
#include <QPair>
#include "SADataClient.h"
#include "SADataFeatureTreeModel.h"

class QwtPlotItem;
class QMdiSubWindow;
class QAbstractItemModel;
class SAFigureWindow;
class SAChart2D;
class SAFigureSetWidget;


namespace Ui {
class SADataFeatureWidget;
}

/**
 * @brief 数据属性显示窗口
 */
class SADataFeatureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SADataFeatureWidget(QWidget *parent = 0);
    ~SADataFeatureWidget();
public://数据接收相关的类型
    class DataInfo {
public:
        DataInfo();
        DataInfo(QwtPlotItem *plotitem, QMdiSubWindow *midwidget, SAChart2D *chartptr);
        QwtPlotItem *item;
        QMdiSubWindow *midwidget;
        SAChart2D *chart;
        bool operator <(const DataInfo& other);
    };
public slots:
    //子窗口激活槽
    void mdiSubWindowActived(QMdiSubWindow *subwnd);

    //子窗口关闭槽
    void mdiSubWindowClosed(QMdiSubWindow *arg1);

signals:
    //显示消息
    void showMessageInfo(const QString& info, SA::MeaasgeType messageType = SA::NormalMessage);

private slots:
    //重新请求服务器计算
    void onToolButtonRequestCalc();

    //树形控件点击
    void onTreeViewClicked(const QModelIndex& index);

    //点击清除属性按钮
    void onToolButtonClearDataFeatureClicked();
    void onToolButtonExpandAllClicked();

    //心跳超时
    void onHeartbeatCheckerTimerout();

    //获取服务器的反馈
    void onReceive2DPointsDescribe(const SAPropertiesGroup& propgroups,int sequenceID,unsigned int extendValue);


private:
    //计算绘图窗口的dataFeature
    void calcFigureFeature();
    void calcFigureFeature(QMdiSubWindow *subwnd);
    void calcFigureFeature(QMdiSubWindow *subwnd, SAFigureWindow *figure, SADataFeatureTreeModel *model);

    //获取mdisubwindow的FigureWindow
    SAFigureWindow *getFigureFromSubWindow(QMdiSubWindow *sub);


    //通过流水号找到对应的model和datainfo，如果没有找到返回nullptr
    QPair<SADataFeatureTreeModel *, DataInfo> findModelBySsequenceID(int sequenceID);

    //设置点击反馈
    void setupClickedYValueAction(SADataFeatureTreeModel::ItemPtr it, const double& v);

    //设置点击反馈
    void setupClickedPointFValueAction(SADataFeatureTreeModel::ItemPtr it, const QPointF& v);

private:
    //计算一个plot item
    void calcPlotItemFeature(QMdiSubWindow *subwnd, SAChart2D *chart, SADataFeatureTreeModel *model, QwtPlotItem *plotitem);



private:
    Ui::SADataFeatureWidget *ui;
    QMdiSubWindow *m_lastActiveSubWindow;                           ///< 记录最后激活的子窗口
    QMap<QMdiSubWindow *, SADataFeatureTreeModel *> m_mdiToModel;   ///< 子窗口对应的数据模型
    QMap<int, DataInfo> m_keyToDatainfo;
    SADataClient m_client;
};



#endif // DATAFEATUREWIDGET_H
