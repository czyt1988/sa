#include "Widget.h"
#include "ui_Widget.h"
#include "SAChart2D.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
  ,m_chart2D(nullptr)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_SAChart2DWidget_clicked()
{
    if(!m_chart2D)
        m_chart2D= new SAChart2D;
    m_chart2D->show();
}
