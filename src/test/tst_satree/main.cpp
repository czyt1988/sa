#include <QApplication>
#include <QDebug>
#include <QWidget>
#include "SAItem.h"
#include "SATree.h"
void test_saitem_copy();
void test_saitem_delete();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test_saitem_copy();
    test_saitem_delete();
    qDebug() <<"done";
    QWidget w;
    w.show();
    return a.exec();
}

void test_saitem_copy()
{
    SAItem* par = new SAItem("1");
    SAItem* c11 = new SAItem("1.1");
    SAItem* c12 = new SAItem("1.2");
    SAItem* c13 = new SAItem("1.3");
    SAItem* c14 = new SAItem("1.4");
    par->setProperty(1,"property test 1");
    par->setProperty(2,2.33);
    par->appendChild(c11);
    par->appendChild(c12);
    par->appendChild(c13);
    par->appendChild(c14);
    SAItem* c121 = new SAItem("1.2.1");
    SAItem* c122 = new SAItem("1.2.2");
    c12->appendChild(c121);
    c12->appendChild(c122);
    SAItem* c141 = new SAItem("1.4.1");
    SAItem* c142 = new SAItem("1.4.2");
    SAItem* c143 = new SAItem("1.4.3");
    SAItem* c144 = new SAItem("1.4.4");
    c14->appendChild(c141);
    c14->appendChild(c144);
    c14->insertChild(c142,1);
    c14->insertChild(c143,2);
    qDebug() << "origin:";
    qDebug() << *par;
    SAItem* par2 = new SAItem();
    *par2 = *par;

    qDebug() << "copy test";
    qDebug() << *par2;

}

void test_saitem_delete()
{
    SAItem* par = new SAItem("1");
    SAItem* c11 = new SAItem("1.1");
    SAItem* c12 = new SAItem("1.2");
    SAItem* c13 = new SAItem("1.3");
    SAItem* c14 = new SAItem("1.4");
    par->setProperty(1,"property test 1");
    par->setProperty(2,2.33);
    par->appendChild(c11);
    par->appendChild(c12);
    par->appendChild(c13);
    par->appendChild(c14);
    SAItem* c121 = new SAItem("1.2.1");
    SAItem* c122 = new SAItem("1.2.2");
    c12->appendChild(c121);
    c12->appendChild(c122);
    SAItem* c141 = new SAItem("1.4.1");
    SAItem* c142 = new SAItem("1.4.2");
    SAItem* c143 = new SAItem("1.4.3");
    SAItem* c144 = new SAItem("1.4.4");
    c14->appendChild(c141);
    c14->appendChild(c144);
    c14->insertChild(c142,1);
    c14->insertChild(c143,2);
    qDebug() << "origin:";
    qDebug() << *par;

    delete c142;
    qDebug() << "delete 1.4.2:";
    qDebug() << *par;

    delete c12;
    qDebug() << "delete 1.2:";
    qDebug() << *par;

    SATree tree;
    tree.appendItem(par);
    SAItem* par2 = new SAItem("2");
    *par2 = *par;
    tree.appendItem(par2);

    QString jsonstr = toJson(&tree);
    qDebug() << tree;
    qDebug().resetFormat().noquote().nospace() << jsonstr;
    SATree tree2;
    fromJson(jsonstr,&tree2);
    qDebug() << tree2;
}
