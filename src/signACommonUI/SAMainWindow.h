#ifndef SAMAINWINDOW_H
#define SAMAINWINDOW_H
#include <QMainWindow>
#include <QDateTime>
#include "SACommonUIGlobal.h"
class QEvent;
class QCloseEvent;
class QString;

SA_IMPL_FORWARD_DECL(SAMainWindow)

class SA_COMMON_UI_EXPORT SAMainWindow : public QMainWindow
{
    Q_OBJECT
    SA_IMPL(SAMainWindow)
public:
    explicit SAMainWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
    ~SAMainWindow();
    //返回创建时间
    QDateTime getBirthDate();
    //获取创建日期的文本
    QString getBirthDateString(Qt::DateFormat f = Qt::SystemLocaleShortDate);
    //设置创建日期，如果不设置，默认创建日期就是new出来的日期
    void setBirthDate(const QDateTime& s);
    //!Notifies that a window was hidden by a direct user action
    void setHidden();

    //获取唯一id
    int getID() const;
    //event handlers
    //! Close event handler
    /**
     * 过滤 "delete, hide, or cancel?" if the
     * "ask on close" flag is set.
     */
    void closeEvent( QCloseEvent *e);


    //! 设置当用户关闭窗口时是否进行关闭询问
    void askOnCloseEvent(bool ask);
    //! Filters other object's events (customizes title bar's context menu)
    bool eventFilter(QObject *object, QEvent *e);
signals:
    //! 子窗口关闭信号
    void closedWindow(SAMainWindow *);
    //! 子窗口隐藏信号
    void hiddenWindow(SAMainWindow *);
    //! Show the context menu
    void showContextMenu();
};




#endif // SAMAINWINDOW_H
