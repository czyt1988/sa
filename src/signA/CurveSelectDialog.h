#ifndef DIALOG_SELCURVE_H
#define DIALOG_SELCURVE_H
// === Qt
#include <QDialog>
#include <QList>
#include <QStandardItemModel>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QSet>
// == STL
#include <memory>

#include "SAMdiSubWindow.h"

#include "SASeries.h"


class SAChart2D;


class CurveSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CurveSelectDialog(SAChart2D* chart ,QWidget *parent = nullptr);
    ~CurveSelectDialog();
    //设置条目过滤
    void setItemFilter(const QSet<QwtPlotItem::RttiValues>& filters);
    QSet<QwtPlotItem::RttiValues> getItemFilter() const;
    ///
    /// \brief 获取选择的线条目，只会显示曲线相关条目
    /// \return
    ///
    QList<QwtPlotItem*> getSelItem()
	{
        return m_selItem;
	}
    //获取xy曲线
    static QList<QwtPlotCurve*> getSelCurve(SAChart2D* chart ,QWidget *par = 0);
private:
    void updateTable();

    QStandardItemModel* getTableModel();
    //根据过滤条件获取items
    QList<QwtPlotItem*> getFilterItems();

private slots:
	void on_pushButton_ok_clicked();
private:
    class UI
    {
    public:
        QVBoxLayout *verticalLayout;
        QTableView *tableView;
        QHBoxLayout *horizontalLayout;
        QSpacerItem *horizontalSpacer;
        QPushButton *pushButton_reject;
        QPushButton *pushButton_ok;
        void setupUi(CurveSelectDialog* d);
    };
    std::unique_ptr<CurveSelectDialog::UI> ui;
    SAChart2D* m_chart;
    QSet<QwtPlotItem::RttiValues> m_itemFilter;///< 记录需要列举的item
    QList<QwtPlotItem*> m_selItem;

};

#endif // DIALOG_SELCURVE_H
