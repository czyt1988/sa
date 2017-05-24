#ifndef DATAFEATUREWIDGET_H
#define DATAFEATUREWIDGET_H

#include <QWidget>
class QMdiSubWindow;

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
    DataFeatureTreeModel *getTreeModel();
public slots:
    //子窗口激活槽
    void mdiSubWindowActived(QMdiSubWindow *arg1);
    //子窗口关闭槽
    void mdiSubWindowClosed(QMdiSubWindow *arg1);
signals:

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_toolButton_clearDataFeature_clicked();

private:
    SAFigureWindow* getChartWidgetFromSubWindow(QMdiSubWindow* sub);
private:
    Ui::DataFeatureWidget *ui;
    SAFigureWindow* m_plotWidget;
};

#endif // DATAFEATUREWIDGET_H
