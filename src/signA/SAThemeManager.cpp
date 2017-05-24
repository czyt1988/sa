#include "SAThemeManager.h"

SAThemeManager::SAThemeManager(QWidget* wind):m_mainWindow(wind)
{

}
///
/// \brief 设置图层表格的样式
/// \param wind 表格窗体指针
/// \param qss qss样式，如果没有定义，设置为默认
///
void SAThemeManager::setTableViewLayout(QWidget* wind, const QString& qss)
{
    if(qss.isNull () || qss.isEmpty ())
    {
        wind->setStyleSheet(getDefaultTableViewLayoutQss());
        return;
    }
    wind->setStyleSheet (qss);
}
///
/// \brief 获取默认的图层表格样式
/// \return
///
QString SAThemeManager::getDefaultTableViewLayoutQss()
{
    QString qss;
//    qss = QString(""
//                  "QTableView:{selection-background-color:transparent;selection-color:transparent;}"
//                  "");
//    qss = QString(""
//                  //无效"QTableView{show-decoration-selected: 1}"
//                  //设置隔行色"QTableView{alternate-background-color: blue;}"
//                  //"QTableView::item:selected{selection-background-color:transparent;}"
//                  "QTableView::item:selected{selection-background-color:rgba(15,152,95,50%);}"
//                  "QTableView::item:selected{selection-color:rgb(0,0,0);}"
//                  "");
    return qss;
}

