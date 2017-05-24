#include "SAMessageWidget.h"
#include "ui_SAMessageWidget.h"
#include <QDateTime>
SAMessageWidget::SAMessageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAMessageWidget)
{
    ui->setupUi(this);
}

SAMessageWidget::~SAMessageWidget()
{
    delete ui;
}

void SAMessageWidget::addString(const QString &str, QColor clr)
{  
    ui->textBrowser->append (QStringLiteral("<span style=\"color:%1;\">%2</span>")
                             .arg(clr.name())
                             .arg(str));
}

void SAMessageWidget::insertTimeLine(QColor clr)
{
    QString line = QStringLiteral("<span style=\"color:%1;\">[").arg(clr.name())
                   + QDateTime::currentDateTime ().toString ("yyyy-MM-dd HH:mm:ss")
            +"]</span>";
    ui->textBrowser->append (line);
}

void SAMessageWidget::addStringWithTime(const QString &str, QColor clr)
{
    insertTimeLine();
    addString(str,clr);
}
