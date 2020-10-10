#include "SAFigureSetWidget.h"
#include "SAFigureWindow.h"
#include <QVBoxLayout>
#include "SAColorSetPropertyItem.h"
#include <QApplication>

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
        verticalLayout->addWidget(backgroundColorItem);

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
        backgroundColorItem->setText(QApplication::translate("SAFigureSetWidget", "Background Color", 0));
    }


    SAFigureWindow *figure;
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
    if (nullptr == fig) {
        clear();
        return;
    }else {
        connect(fig, &QObject::destroyed, this, &SAFigureSetWidget::onFigureDestroy);
        qDebug() << "setCurrentColor " << fig->getBackgroundColor().color();
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
        ui->figure = nullptr;
        clear();
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
