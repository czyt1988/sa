#ifndef MdiSubWindow_H
#define MdiSubWindow_H

#include <QMdiSubWindow>
#include "SACommonUIGlobal.h"
class QEvent;
class QCloseEvent;
class QString;



///
/// \brief QMdiSubWindow扩展，增加生成时间和关闭的信号函数
///
class SA_COMMON_UI_EXPORT SAMdiSubWindow: public QMdiSubWindow
{
	Q_OBJECT
public:


    SAMdiSubWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);

	//! Return the creation date
    QString birthDate(){return m_birthdate;}
	//! Set the creation date
    void setBirthDate(const QString& s){m_birthdate = s;}

	//!Notifies that a window was hidden by a direct user action
	void setHidden();
	
    int getID() const {return m_id;}

    SA::SubWndType getType() const {return m_type;}
    void setType(SA::SubWndType type) {m_type = type;}
	//event handlers
	//! Close event handler
	/**
	 * Ask the user "delete, hide, or cancel?" if the
	 * "ask on close" flag is set.
	 */
	void closeEvent( QCloseEvent *);


    //! 设置当用户关闭窗口时是否进行关闭询问
    void askOnCloseEvent(bool ask){m_confirm_close = ask;}
	//! Filters other object's events (customizes title bar's context menu)
	bool eventFilter(QObject *object, QEvent *e);

signals:
    //! 子窗口关闭信号
    void closedWindow(SAMdiSubWindow *);
    //! 子窗口隐藏信号
    void hiddenWindow(SAMdiSubWindow *);
	//! Show the context menu
	void showContextMenu();

protected:
	bool m_confirm_close;
	//! The creation date
	QString m_birthdate;
    //! id
    int m_id;
    //! type
    SA::SubWndType m_type;

};

#endif
