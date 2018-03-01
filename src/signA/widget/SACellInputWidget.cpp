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

QString SACellInputWidget::cellTitleText(int index) const
{
    QPair<QLabel*,QLineEdit*> p = m_cellInfos[index];
    return p.first->text();
}

void SACellInputWidget::setCellEditText(int index, const QString &text)
{
    QPair<QLabel*,QLineEdit*> p = m_cellInfos[index];
    p.second->setText(text);
}

QString SACellInputWidget::cellEditText(int index) const
{
    QPair<QLabel*,QLineEdit*> p = m_cellInfos[index];
    return p.second->text();
}

bool SACellInputWidget::isAcceptInput() const
{
    return m_isAccept;
}

int SACellInputWidget::buttonAreaWidth() const
{
    return ui->toolButtonOk->width()*2+1;
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
