#include "SAMainWindow.h"
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>

class SAMainWindowPrivate
{
public:
    SA_IMPL_PUBLIC(SAMainWindow)
    SAMainWindowPrivate(SAMainWindow *p);
    bool m_confirm_close;
    //! The creation date
    QDateTime m_birthdate;
};

SAMainWindowPrivate::SAMainWindowPrivate(SAMainWindow *p) : q_ptr(p)
    , m_confirm_close(false)
    , m_birthdate(QDateTime::currentDateTime())
{
}


SAMainWindow::SAMainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
    , d_ptr(new SAMainWindowPrivate(this))
{
}


SAMainWindow::~SAMainWindow()
{
}


QDateTime SAMainWindow::getBirthDate()
{
    return (d_ptr->m_birthdate);
}


QString SAMainWindow::getBirthDateString(Qt::DateFormat f)
{
    return (d_ptr->m_birthdate.toString(f));
}


void SAMainWindow::setBirthDate(const QDateTime& s)
{
    d_ptr->m_birthdate = s;
}


void SAMainWindow::setHidden()
{
    hide();
    emit hiddenWindow(this);
}


int SAMainWindow::getID() const
{
    return (reinterpret_cast<int>(this));
}


void SAMainWindow::closeEvent(QCloseEvent *e)
{
    if (d_ptr->m_confirm_close) {
        switch (QMessageBox::question(this, windowTitle(), tr("Are You Sure Close Window?")))
        {
        case QMessageBox::Yes:
            emit closedWindow(this);
            e->accept();
            break;

        default:
            e->ignore();
            break;
        }
    }else {
        emit closedWindow(this);
        e->accept();
    }
}


void SAMainWindow::askOnCloseEvent(bool ask)
{
    d_ptr->m_confirm_close = ask;
}


bool SAMainWindow::eventFilter(QObject *object, QEvent *e)
{
    if ((e->type() == QEvent::ContextMenu) && (object == centralWidget())) {
        emit showContextMenu();
        return (true);
    }
    return (QMainWindow::eventFilter(object, e));
}
