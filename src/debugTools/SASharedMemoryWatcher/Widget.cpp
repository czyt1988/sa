#include "Widget.h"
#include "ui_Widget.h"
#include "SAServeShareMemory.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButtonWatch,&QPushButton::clicked,this,&Widget::onPushButtonWatchClicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onPushButtonWatchClicked()
{
    SAServeShareMemory mem;
    mem.updateFromMem();
    ui->textBrowser->setPlainText(mem.describe());
}
