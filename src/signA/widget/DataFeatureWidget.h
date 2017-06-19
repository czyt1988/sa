#ifndef DATAFEATUREWIDGET_H
#define DATAFEATUREWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QMap>
#include "SAGlobals.h"
class QMdiSubWindow;
class QLocalServer;
class QLocalSocket;
class QAbstractItemModel;
class SAFigureWindow;
class DataFeatureTreeModel;
namespace Ui {
class DataFeatureWidget;
}

class DataFeatureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataFeatureWidget(QWidget *parent = 0);
    ~DataFeatureWidget();
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

private:
    //初始化本地服务
    void initLocalServer();
    //
    void startDataProc();
    //本地服务连接的槽
    Q_SLOT void onLocalServeNewConnection();
    //数据处理进程异常停止
    Q_SLOT void onProcessDataProcFinish(int exitCode, QProcess::ExitStatus exitStatus);
    //接收到数据处理进程的内容
    Q_SLOT void onProcessDataReadyRead();
    SAFigureWindow* getChartWidgetFromSubWindow(QMdiSubWindow* sub);
    //计算绘图窗口的dataFeature
    void callCalcFigureWindowFeature(SAFigureWindow* figure);
private:
    Ui::DataFeatureWidget *ui;
    QMdiSubWindow* m_lastActiveSubWindow;///< 记录最后激活的子窗口


    QScopedPointer<QLocalServer> m_localServer;///< 本地服务器
    QLocalSocket* m_dataProcessSocket;
    QProcess* m_dataProcPro;///< 数据处理进程
    QLocalSocket* m_dataProcSocket;///< 数据处理对应的socket
    QMap<QMdiSubWindow*,QAbstractItemModel*> m_subWindowToDataInfo;///< 记录子窗口对应的数据属性表上显示的model
};

#endif // DATAFEATUREWIDGET_H
