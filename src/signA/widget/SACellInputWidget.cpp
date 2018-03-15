#include "SACellInputWidget.h"
#include "ui_SACellInputWidget.h"
#include <QLabel>
#include <QLineEdit>
SACellInputWidget::SACellInputWidget(QWidget *parent) :
    SAPopupWidget(parent),
    ui(new Ui::SACellInputWidget)
  ,m_isAccept(false)
{
    ui->setupUi(this);

    setFrameStyle(QFrame::Box);
    connect(ui->toolButtonCancel,&QToolButton::clicked,this,&SACellInputWidget::onButtonCancelClicked);
    connect(ui->toolButtonOk,&QToolButton::clicked,this,&SACellInputWidget::onButtonOkPressed);
}

SACellInputWidget::~SACellInputWidget()
{
    delete ui;
}

int SACellInputWidget::cellsCount() const
{
    m_cellInfos.size();
}

void SACellInputWidget::clearCells()
{
    while(ui->horizontalLayoutEdit->count())
    {
        QLayoutItem* i = ui->horizontalLayoutEdit->takeAt(0);
        QWidget* w = i->widget();
        w->hide();
        w->deleteLater();
    }
    while(ui->horizontalLayoutLabel->count())
    {
        QLayoutItem* i = ui->horizontalLayoutLabel->takeAt(0);
        QWidget* w = i->widget();
        w->hide();
        w->deleteLater();
    }
    m_cellInfos.clear();
}

void SACellInputWidget::resizeCells(int size)
{
    int curSize = cellsCount();
    if(size > curSize)
    {
        while(cellsCount() != size)
        {
            appendCell();
        }
    }
    else if(size < curSize)
    {
        while(cellsCount() != size)
        {
            removeBackCell();
        }
    }
}

void SACellInputWidget::appendCell()
{
    int c = cellsCount();
    QLabel* label = new QLabel(this);
    label->setObjectName(QStringLiteral("Label%1").arg(c+1));
    QLineEdit* edit = new QLineEdit(this);
    edit->setObjectName(QStringLiteral("lineEdit%1").arg(c+1));
    ui->horizontalLayoutLabel->addWidget(label);
    ui->horizontalLayoutEdit->addWidget(edit);
    m_cellInfos.append(qMakePair(label,edit));
}

void SACellInputWidget::removeBackCell()
{
    QLayoutItem* i = ui->horizontalLayoutEdit->takeAt(ui->horizontalLayoutEdit->count()-1);
    QWidget* w = i->widget();
    w->hide();
    w->deleteLater();

    i = ui->horizontalLayoutLabel->takeAt(ui->horizontalLayoutLabel->count()-1);
    w = i->widget();
    w->hide();
    w->deleteLater();
    m_cellInfos.takeLast();
}

void SACellInputWidget::setCellTitleText(int index, const QString &text)
{
    QPair<QLabel*,QLineEdit*> p = m_cellInfos[index];
    p.first->setText(text);
}

QString SACellInputWidget::getCellTitleText(int index) const
{
    QPair<QLabel*,QLineEdit*> p = m_cellInfos[index];
    return p.first->text();
}

void SACellInputWidget::setCellEditText(int index, const QString &text)
{
    QPair<QLabel*,QLineEdit*> p = m_cellInfos[index];
    p.second->setText(text);
}

QString SACellInputWidget::getCellEditText(int index) const
{
    QPair<QLabel*,QLineEdit*> p = m_cellInfos[index];
    return p.second->text();
}
///
/// \brief 获取所有的数据
/// \return
///
QStringList SACellInputWidget::getCellEditTexts() const
{
    QStringList strList;
    const int size = cellsCount();
    for(int i=0;i<size;++i)
    {
        strList.append(getCellEditText(i));
    }
    return strList;
}
///
/// \brief 把每个单元格转换为double输出，若有一个不能转为double，返回false
/// \param res
/// \return
///
bool SACellInputWidget::getDoubleList(QList<double> &res) const
{
    QList<double> temp;
    const int cellCount = cellsCount();
    for(int i=0;i<cellCount;++i)
    {
        QString str = getCellEditText(i);
        bool isOK = false;
        double d = str.toDouble(&isOK);
        if(!isOK)
            return false;
        temp.append(d);
    }
    res = temp;
    return true;
}

bool SACellInputWidget::isAcceptInput() const
{
    return m_isAccept;
}

int SACellInputWidget::getButtonAreaWidth() const
{
    return ui->toolButtonOk->width()*2+1;
}

void SACellInputWidget::setCellWidth(int index, int w)
{
    QLayoutItem* i = ui->horizontalLayoutEdit->itemAt(index);
    if(nullptr == i)
        return;
    QWidget* widget = i->widget();
    if(nullptr == widget)
        return;
    widget->setFixedWidth(w);

    i = ui->horizontalLayoutLabel->itemAt(index);
    if(nullptr == i)
        return;
    widget = i->widget();
    if(nullptr == widget)
        return;
    widget->setFixedWidth(w);
}

int SACellInputWidget::getCellWidth(int index) const
{
    QLayoutItem* i = ui->horizontalLayoutEdit->itemAt(index);
    if(nullptr == i)
        return 0;
    QWidget* widget = i->widget();
    if(nullptr == widget)
        return 0;
    return widget->width();
}



void SACellInputWidget::onButtonOkPressed()
{
    m_isAccept = true;
    hide();
}

void SACellInputWidget::onButtonCancelClicked()
{
    m_isAccept = false;
    hide();
}
