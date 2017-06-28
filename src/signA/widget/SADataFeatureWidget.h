#ifndef SADATAFEATUREWIDGET_H
#define SADATAFEATUREWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QMap>
#include <QBuffer>
#include "SAGlobals.h"
#include <memory>
class QMdiSubWindow;
class QLocalServer;
class QLocalSocket;
class QAbstractItemModel;
class SAFigureWindow;
class DataFeatureTreeModel;
//数据接收相关
class SALocalServeReader;
class SALocalServeWriter;
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
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
    //初始化本地服务
    void initLocalServer();
    //
    void startDataProc();
    //本地服务连接的槽
    Q_SLOT void onLocalServeNewConnection();
    //数据处理进程异常停止
    Q_SLOT void onProcessDataProcFinish(int exitCode, QProcess::ExitStatus exitStatus);

    SAFigureWindow* getChartWidgetFromSubWindow(QMdiSubWindow* sub);
    //计算绘图窗口的dataFeature
    void callCalcFigureWindowFeature(SAFigureWindow* figure);
    //接收主包头完毕
    Q_SLOT void onReceivedShakeHand(const SALocalServeBaseHeader& mainHeader);
    //接收到xml字符
    Q_SLOT void onReceivedString(const QString& xmlString);
    //接收到xml字符
    Q_SLOT void onReceivedVectorDoubleData(const SALocalServeFigureItemProcessHeader& header,QVector<QPointF>& ys);
private:
    Ui::SADataFeatureWidget *ui;
    QMdiSubWindow* m_lastActiveSubWindow;///< 记录最后激活的子窗口
    QScopedPointer<QLocalServer> m_localServer;///< 本地服务器
    QProcess* m_dataProcPro;///< 数据处理进程
    QLocalSocket* m_dataProcessSocket;///< 数据处理对应的socket
    QMap<QMdiSubWindow*,QAbstractItemModel*> m_subWindowToDataInfo;///< 记录子窗口对应的数据属性表上显示的model
private://数据接收相关的类型
    SALocalServeReader* m_dataReader;
    SALocalServeWriter* m_dataWriter;
};

#endif // DATAFEATUREWIDGET_H
