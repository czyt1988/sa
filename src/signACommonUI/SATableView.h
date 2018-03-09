#ifndef SATABLEVIEW_H
#define SATABLEVIEW_H
#include "SACommonUIGlobal.h"
#include <QTableView>
#include <functional>
class QKeyEvent;
///
/// \brief SA 数据表格的封装
/// - 添加全局剪切板复制功能
/// - 添加全局剪切板粘贴功能
/// - 添加ctrl+滚轮缩放功能
///
///
class SA_COMMON_UI_EXPORT SATableView : public QTableView
{
    Q_OBJECT
public:
    SATableView(QWidget* par = Q_NULLPTR);
    typedef std::function<void()> FunPtr;
    FunPtr onCtrlVFun;///< ctrl + v的函数指针
    FunPtr onCtrlCFun;///< ctrl + c的函数指针
protected:
    void keyPressEvent(QKeyEvent *e);
    //处理ctrl+c事件
    virtual void onCtrlCPressed();
    //处理ctrl+v事件
    virtual void onCtrlVPressed();
private:
    //获取选中的表格范围和四个端点的行列号
    static QSize getSelectSize(const QModelIndexList& indexs, int* minRow, int* maxRow, int *minCol, int *maxCol);
    //把二维字符串表转换为tab分隔的单一字符串
    static QString toTabString(QString **tableString, const int r, const int c);
};

#endif // SADATATABLEVIEW_H
