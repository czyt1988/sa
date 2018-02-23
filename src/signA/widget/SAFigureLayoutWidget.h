#ifndef SAFIGURELAYOUTWIDGET_H
#define SAFIGURELAYOUTWIDGET_H

#include <QWidget>
class SAPlotLayerModel;
class SAFigureWindow;
class SAChart2D;
namespace Ui {
class SAFigureLayoutWidget;
}
///
/// \brief 图层管理窗口
///
class SAFigureLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAFigureLayoutWidget(QWidget *parent = 0);
    ~SAFigureLayoutWidget();

    SAPlotLayerModel *getLayoutModel() const;
    void setFigure(SAFigureWindow* fig);
    SAFigureWindow* currentFigure() const;
    SAPlotLayerModel* getPlotLayerModel() const;
public slots:
    void updateLayout();
private slots:
    void onTableViewLayerPressed(const QModelIndex &index);

private:
    Ui::SAFigureLayoutWidget *ui;
    SAPlotLayerModel* m_layoutModel;
    SAFigureWindow* m_figure;

};

#endif // SAFIGURELAYOUTWIDGET_H
