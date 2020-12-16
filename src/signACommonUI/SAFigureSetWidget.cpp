#include "SAFigureSetWidget.h"
#include "SAFigureWindow.h"
#include <QVBoxLayout>
#include "SAColorSetPropertyItem.h"
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QLabel>
#include "SAFigureWindowChartTableModel.h"



class SAFigureSetWidget::UI {
public:
    void setupUI(SAFigureSetWidget *par)
    {
        this->figure = nullptr;
        par->setObjectName(QStringLiteral("SAFigureSetWidget"));
        par->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(3, 5, 3, 5);
        par->setLayout(verticalLayout);

        backgroundColorItem = new SAColorSetPropertyItem(par);
        backgroundColorItem->setText(par->tr("background color:"));

        //添加说明
        label_chartpos = new QLabel();

        //添加图表的列表

        chartposTableview = new QTableView();
        chartposTableview->clearSpans();
        chartposTableview->verticalHeader()->setVisible(false);// 水平不可见
//        chartposTableview->horizontalHeader()->setStretchLastSection(true);
        chartposTableview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        chartposListModel = new SAFigureWindowChartTableModel(chartposTableview);
        chartposTableview->setModel(chartposListModel);

        //设置布局
        verticalLayout->addWidget(backgroundColorItem);
        verticalLayout->addWidget(label_chartpos);
        verticalLayout->addWidget(chartposTableview);
        verticalLayout->addStretch();
        //设置文本
        retranslateUi(par);
        //信号槽
        par->connect(backgroundColorItem, &SAColorSetPropertyItem::colorChanged,
            par, &SAFigureSetWidget::onFigureBackgroundColorChanged);
    }


    void retranslateUi(SAFigureSetWidget *w)
    {
        w->setWindowTitle(QApplication::translate("SAFigureSetWidget", "Figure Set", 0));
        label_chartpos->setText(QApplication::translate("SAFigureSetWidget", "charts postion:", 0));
        backgroundColorItem->setText(QApplication::translate("SAFigureSetWidget", "Background Color", 0));
    }


    QLabel *label_chartpos;
    SAFigureWindow *figure;
    QTableView *chartposTableview;
    SAFigureWindowChartTableModel *chartposListModel;
    QVBoxLayout *verticalLayout;
    SAColorSetPropertyItem *backgroundColorItem;
};

SAFigureSetWidget::SAFigureSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new SAFigureSetWidget::UI)
{
    ui->setupUI(this);
}


SAFigureSetWidget::~SAFigureSetWidget()
{
    delete ui;
}


/**
 * @brief 设置figure
 * @param fig 可以为nullptr，此时会清空
 */
void SAFigureSetWidget::setFigureWidget(SAFigureWindow *fig)
{
    if (fig == ui->figure) {
        return;
    }
    if (ui->figure) {
        disconnect(ui->figure, &QObject::destroyed, this, &SAFigureSetWidget::onFigureDestroy);
    }
    ui->figure = fig;
    ui->chartposListModel->setFigure(fig);
    if (nullptr == fig) {
        clear();
        return;
    }else {
        connect(fig, &QObject::destroyed, this, &SAFigureSetWidget::onFigureDestroy);
        ui->backgroundColorItem->setCurrentColor(fig->getBackgroundColor().color());
    }
}


SAFigureWindow *SAFigureSetWidget::figure() const
{
    return (ui->figure);
}


/**
 * @brief SAFigureSetWidget::clear
 */
void SAFigureSetWidget::clear()
{
    ui->backgroundColorItem->setCurrentColor(QColor());
    ui->chartposListModel->setFigure(nullptr);
}


/**
 * @brief 刷新界面
 */
void SAFigureSetWidget::refresh()
{
    if (ui->chartposListModel) {
        ui->chartposListModel->refresh();
    }
    if (ui->figure) {
        ui->backgroundColorItem->setCurrentColor(ui->figure->getBackgroundColor().color());
    }
}


void SAFigureSetWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }else{
        QWidget::changeEvent(e);
    }
}


/**
 * @brief figure销毁时触发
 */
void SAFigureSetWidget::onFigureDestroy(QObject *obj)
{
    if (ui->figure == obj) {
        clear();
        ui->figure = nullptr;
    }
}


/**
 * @brief 设置背景颜色
 * @param clr
 */
void SAFigureSetWidget::onFigureBackgroundColorChanged(const QColor& clr)
{
    if (nullptr == ui->figure) {
        return;
    }
    ui->figure->setBackgroundColor(clr);
    ui->figure->repaint();
}
