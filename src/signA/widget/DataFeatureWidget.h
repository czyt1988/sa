#ifndef DATAFEATUREWIDGET_H
#define DATAFEATUREWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QMap>
#include <QBuffer>
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
    //处理读取的数据
    void dealRecDatas();
    //处理读取的数据
    void dealMainHeaderData();
    //接收主包头完毕
    void receivedMainHeader();
private:
    Ui::DataFeatureWidget *ui;
    QMdiSubWindow* m_lastActiveSubWindow;///< 记录最后激活的子窗口
    QByteArray m_recData;///< 接收的数据
    bool m_isReadedHeader;///< 标记是否读取了包头
    bool m_isStartRecData;///< 接收数据标记
    size_t m_dataLen;///< 数据长度
    QScopedPointer<QLocalServer> m_localServer;///< 本地服务器
    QProcess* m_dataProcPro;///< 数据处理进程
    QLocalSocket* m_dataProcessSocket;///< 数据处理对应的socket
    QMap<QMdiSubWindow*,QAbstractItemModel*> m_subWindowToDataInfo;///< 记录子窗口对应的数据属性表上显示的model
};

#endif // DATAFEATUREWIDGET_H
