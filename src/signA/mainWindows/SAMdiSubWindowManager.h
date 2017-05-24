#ifndef MDISUBWINDOWMANAGER_H
#define MDISUBWINDOWMANAGER_H
#include <QMdiArea>
#include <QMdiSubWindow>
#include <memory>
///
/// \brief 用于方便生成mdi子窗体的管理类
///
class SAMdiSubWindowManager : public QObject
{
    Q_OBJECT
public:
    SAMdiSubWindowManager(){
        m_mdi = nullptr;
    }
    SAMdiSubWindowManager(QMdiArea* mdi){
        m_mdi = mdi;
    }
    QMdiArea* getMdi() const{
        return m_mdi;
    }
    void setMdi(QMdiArea* mdi){
        m_mdi = mdi;
    }
signals:
    ///
    /// \brief 子窗体创建发射的信号
    /// \param window
    ///
    void subWindowCreated(QMdiSubWindow * window);
public:


    ///
    /// \brief 显示mdi子窗体通过模板类名
    ///
    /// 模板第一个参数用来指定子窗体类名，如果不做更改，传入QMdiSubWindow即可(由于许多版本C++11支撑不够完善，模板函数默认参数未能完全支持)
    /// 因此这里没赋予默认参数，第二个模板参数T用于指定子窗口内的widget类型。
    ///
    /// 如有一个widget，类名叫exampleWidget，需要作为mdi子窗体，那么可以如下方式调用：
    ///
    /// h:
    ///
    /// MdiSubWindowManager manager;
    ///
    /// cpp:
    ///
    /// manager.setMdi(ui->mdiArea);
    /// manager.showMdiSubWindow<QMdiSubWindow,exampleWidget>(true);
    ///
    /// 即可添加exampleWidget作为mdi的子窗口
    ///
    /// \param isCreateIfNotExist 如果目前没有这个窗体，isCreateIfNotExist为真时则调用newMdiSubWndByClassName创建一个
    /// \return SUBWND_CLASS指针
    ///
    template <typename SUBWND_CLASS/* = QMdiSubWindow*/,typename T>
    SUBWND_CLASS* showMdiSubWindow(bool isCreateIfNotExist = true)
    {
        SUBWND_CLASS* subWnd = nullptr;
        QList<QMdiSubWindow*> subMdiWnds = m_mdi->subWindowList();
        //遍历看看有没有激活这个窗口
        for(auto ite = subMdiWnds.begin();ite != subMdiWnds.end();++ite){

            if(T::staticMetaObject.className() == (*ite)->widget()->metaObject()->className())
            {//说明已经激活过这个窗口了
                m_mdi->setActiveSubWindow(*ite);
                subWnd = qobject_cast<SUBWND_CLASS*>(*ite);
                return subWnd;
            }
        }
        if(!isCreateIfNotExist)
            return nullptr;
        //如果函数进入这里说明没有打开过这个窗口，就新建一个
        SUBWND_CLASS* newSubMdiWnd  = newMdiSubWnd<SUBWND_CLASS,T>();
        if(nullptr == newSubMdiWnd)
            return nullptr;
        //添加进mdi中
        newSubMdiWnd->show();
        m_mdi->setActiveSubWindow(newSubMdiWnd);
        return newSubMdiWnd;
    }

    template <typename SUBWND_CLASS /*= QMdiSubWindow*/,typename T>
    ///
    /// \brief 创建一个subwindow
    ///
    /// 模板第一个参数用来指定子窗体类名，如果不做更改，传入QMdiSubWindow即可(由于许多版本C++11支撑不够完善，模板函数默认参数未能完全支持)
    /// 因此这里没赋予默认参数，第二个模板参数T用于指定子窗口内的widget类型。
    ///
    /// \return
    ///
    SUBWND_CLASS* newMdiSubWnd()
    {
        std::unique_ptr<SUBWND_CLASS> pSubw(new SUBWND_CLASS);
        pSubw->setAttribute(Qt::WA_DeleteOnClose);
        QWidget* w = qobject_cast<QWidget*>(new T);
        if(!w)
            return nullptr;
        pSubw->setWidget(w);
        //遍历树形控件的item，找出对应的item,并把窗口指针设置进去
        m_mdi->addSubWindow(pSubw.get());
        emit subWindowCreated(pSubw.get());
        return pSubw.release();
    }
    template <typename SUBWND_CLASS /*= QMdiSubWindow*/,typename T,typename Arg1>
    ///
    /// \brief 创建一个subwindow
    ///
    /// 模板第一个参数用来指定子窗体类名，如果不做更改，传入QMdiSubWindow即可(由于许多版本C++11支撑不够完善，模板函数默认参数未能完全支持)
    /// 因此这里没赋予默认参数，第二个模板参数T用于指定子窗口内的widget类型。
    ///
    /// \return
    ///
    SUBWND_CLASS* newMdiSubWnd(Arg1 a1)
    {
        std::unique_ptr<SUBWND_CLASS> pSubw(new SUBWND_CLASS);
        pSubw->setAttribute(Qt::WA_DeleteOnClose);
        QWidget* w = qobject_cast<QWidget*>(new T(a1));
        if(!w)
            return nullptr;
        pSubw->setWidget(w);
        //遍历树形控件的item，找出对应的item,并把窗口指针设置进去
        m_mdi->addSubWindow(pSubw.get());
        emit subWindowCreated(pSubw.get());
        return pSubw.release();
    }
private:
    QMdiArea* m_mdi;
};

#endif // MDISUBWINDOWMANAGER_H
