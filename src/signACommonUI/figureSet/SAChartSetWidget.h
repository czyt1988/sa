#ifndef SACHARTSETWIDGET_H
#define SACHARTSETWIDGET_H

#include <QWidget>
#include <QScopedPointer>
class SAChart2D;
///
/// \brief
///
class SAChartSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAChartSetWidget(QWidget* parent = nullptr);
    ~SAChartSetWidget();
    void setChart(SAChart2D *chart);
signals:
    void chartTitleChanged(const QString& text);
private slots:
    void onChartDelete(QObject *obj);
private:
    class UI;
    SAChartSetWidget::UI* ui;
};

#endif // SAFIGURECANVASSETWIDGET_H
