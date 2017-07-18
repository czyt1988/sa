#ifndef SADATAFEATUREWIDGET_H
#define SADATAFEATUREWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QMap>
#include "SAGlobals.h"
#include <memory>
class QwtPlotItem;
class QMdiSubWindow;
class QAbstractItemModel;
class SAFigureWindow;
class DataFeatureTreeModel;
class SAChart2D;
class SAFiugreSetWidget;
/// \def 使用多线程来进行数据特征的计算，否则使用多进程，
/// 如果程序不希望使用多线程进行计算，把这个宏注释掉，会使用多进程进行计算
//#define USE_THREAD_CALC_FEATURE

//数据接收相关
#ifdef USE_THREAD_CALC_FEATURE

#else
    #define USE_IPC_CALC_FEATURE //使用多进程进行计算
    class QLocalServer;
    class QLocalSocket;
    class SALocalServeReader;
    class SALocalServeWriter;
    #include "SALocalServeBaseHeader.h"
    #include "SALocalServeFigureItemProcessHeader.h"
#endif

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
    void showMessageInfo(const QString& info,SA::MeaasgeType messageType);
private slots:

    void on_treeView_clicked(const QModelIndex &index);

    void on_toolButton_clearDataFeature_clicked();

    void onShowErrorMessage(const QString& info);

private:
    SAFigureWindow* getChartWidgetFromSubWindow(QMdiSubWindow* sub);
    //计算绘图窗口的dataFeature
    void callCalcFigureWindowFeature(SAFigureWindow* figure);
    //检测model是否需要重新计算datafeature
    void checkModelItem(QAbstractItemModel* baseModel,QMdiSubWindow *subWndPtr);

#ifdef USE_THREAD_CALC_FEATURE

#else
private:
    //计算一个plot item
    void calcPlotItemFeature(const QwtPlotItem* plotitem,const QMdiSubWindow *arg1,const SAChart2D* arg2);
    //初始化本地服务
    void initLocalServer();
    //连接服务器
    void connectToServer();
    //尝试连接服务器
    Q_SLOT void tryToConnectServer();
    //启动数据处理进程
    void startDataProc();
    //进程状态改变
    Q_SLOT void onProcessStateChanged(QProcess::ProcessState newState);
    //数据处理进程异常停止
    Q_SLOT void onProcessDataProcFinish(int exitCode, QProcess::ExitStatus exitStatus);
    //接收主包头完毕
    Q_SLOT void onReceivedShakeHand(const SALocalServeBaseHeader& mainHeader);
    //接收到xml字符
    Q_SLOT void onReceivedString(const QString& xmlString);
    //接收到到点数组
    Q_SLOT void onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader& header,QVector<QPointF>& ys);
#endif
private:
    Ui::SADataFeatureWidget *ui;
    QMdiSubWindow* m_lastActiveSubWindow;///< 记录最后激活的子窗口
    QMap<QMdiSubWindow*,QAbstractItemModel*> m_subWindowToDataInfo;///< 记录子窗口对应的数据属性表上显示的model
#ifdef USE_THREAD_CALC_FEATURE

#else
private://数据接收相关的类型
    QProcess* m_dataProcPro;///< 数据处理进程
    QLocalSocket* m_dataProcessSocket;///< 数据处理对应的socket
    SALocalServeReader* m_dataReader;
    SALocalServeWriter* m_dataWriter;
    short m_connectRetryCount;
#endif
};

#endif // DATAFEATUREWIDGET_H
