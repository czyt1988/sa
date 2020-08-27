#ifndef SAICONHELPER_H
#define SAICONHELPER_H
#include <QIcon>
class QwtPlotItem;

/**
 * @brief 用于方便管理本项目的图标
 */
class SAIconHelper
{
public:
    SAIconHelper();

    // qwtplotitem 对应的icon
    static QIcon getIconByPlotItem(const QwtPlotItem *item);

    // figure 窗口的icon
    static QIcon getFigureWindowIcon();

    // 获取图层可见性按钮的图标
    static QIcon getLayoutVisibleIcon(bool visible);
};

#endif // SAICONHELPER_H
