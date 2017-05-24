#ifndef MDIWINDOWMODEL_H
#define MDIWINDOWMODEL_H
#include <QObject>
#include <QAbstractTableModel>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDebug>
#include <QColor>
///
/// \brief 用于显示mdi窗口个数的model
///
/// 尘中远，于2014-04-18
///
/// 例子：
///
/// MdiWindowModel* mdiListModel = new MdiWindowModel(ui->mdiArea,ui->listView_window);
///
/// ui->listView_window->setModel(mdiListModel);
///
/// connect(ui->listView_window,&QAbstractItemView::clicked,mdiListModel,&MdiWindowModel::onItemClick);//用于点击时弹出窗口
///
class MdiWindowModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MdiWindowModel(QMdiArea* mdi,QObject *parent = 0):QAbstractTableModel(parent)
      ,m_isHightLightSelItem(true)
      ,m_isDoubleClickActiveWindow(true)
      ,m_isSingleClickActiveWindow(false)
    {
        connectMDIArea(mdi);
        m_HightLightItemClr = QColor(212,232,255);
        m_header.append(QStringLiteral("子窗口名称"));
    }
public:
    int rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return this->m_mdiArea->subWindowList().size();
    }
    int columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return m_header.size ();
    }
    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::TextAlignmentRole){ //返回的是对其方式
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else if (role == Qt::DisplayRole){ //返回的是现实内容
            if(index.row() >= this->m_mdiArea->subWindowList().size())
                return QVariant();
            if(0 == index.column())
                return this->m_mdiArea->subWindowList().at(index.row())->windowTitle();
        }
        else if(Qt::BackgroundRole == role)
        {
            if(index.row() >= this->m_mdiArea->subWindowList().size())
                return QVariant();
            if(this->m_isHightLightSelItem)
            {
                if(this->m_mdiArea->subWindowList().at(index.row()) == m_activeWnd)
                    return this->m_HightLightItemClr;
            }
            return QVariant();
        }
        else if(Qt::DecorationRole == role)
        {
            if(index.row() >= this->m_mdiArea->subWindowList().size())
                return QVariant();
            if(0 == index.column())
                return this->m_mdiArea->subWindowList().at(index.row())->windowIcon ();
        }
        return QVariant();
    }
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();
        if(Qt::Horizontal == orientation){//说明是水平表头
            if(section<m_header.size())
                return m_header.at(section);
            return QVariant();
        }
        else if(Qt::Vertical == orientation){//垂直表头
            return QVariant();
        }

    }
    void connectMDIArea( QMdiArea* mdi){
        this->m_mdiArea = mdi;
        QObject::connect(m_mdiArea,&QMdiArea::subWindowActivated,this,&MdiWindowModel::MdiAreaSubWindowActivated);
    }

    void refresh()
    {
        //重置模型至原始状态，告诉所有视图，他们数据都无效，强制刷新数据
        beginResetModel();

        endResetModel();
    }
    ///
    /// \brief 设置选中高亮
    /// \param enableHight
    ///
    void setHightLightSelectEnable(bool enable) {this->m_isHightLightSelItem = enable;}
    bool isHightLightSelectEnable() const {return m_isHightLightSelItem;}
    ///
    /// \brief 设置双击激活窗口
    /// \param enableHight
    ///
    void setDoubleClickActiveWindowEnable(bool enable) {this->m_isDoubleClickActiveWindow = enable;}
    bool isDoubleClickActiveWindowEnable() const {return m_isDoubleClickActiveWindow;}
    ///
    /// \brief 设置单击激活窗口
    /// \param enableHight
    ///
    void setSingleClickActiveWindowEnable(bool enable) {this->m_isSingleClickActiveWindow = enable;}
    bool isSingleClickActiveWindowEnable() const {return m_isSingleClickActiveWindow;}
    ///
    /// \brief 设置激活窗口的条目显示的背景颜色
    /// \param clr
    ///
    void setHightLightSelectColor(const QColor& clr){this->m_HightLightItemClr = clr;}
    QColor getHightLightSelectColor() const{return m_HightLightItemClr;}
public slots:
    void onItemClick(const QModelIndex & index)
    {
        if(m_isSingleClickActiveWindow)
        {
            if(index.row() < m_mdiArea->subWindowList().size())
            {
                m_mdiArea->setActiveSubWindow(m_mdiArea->subWindowList()[index.row()]);
            }
        }
    }
    void onItemDoubleClick(const QModelIndex & index)
    {
        if(m_isDoubleClickActiveWindow)
        {
            if(index.row() < m_mdiArea->subWindowList().size())
            {
                m_mdiArea->setActiveSubWindow(m_mdiArea->subWindowList()[index.row()]);
            }
        }
    }
private slots:
    void MdiAreaSubWindowActivated(QMdiSubWindow * window)
    {
        m_activeWnd = window;
        refresh();
    }

private:
    QMdiArea* m_mdiArea;
    QStringList m_header;///< 理论一个QString就可以，但是为了防止以后扩展，使用QStringList
    QMdiSubWindow* m_activeWnd;///< 标定哪个窗口选定
    bool m_isHightLightSelItem;///< 当窗口激活时，会对对应的条目高亮；
    bool m_isDoubleClickActiveWindow;///< 双击条目激活窗体，默认true；
    bool m_isSingleClickActiveWindow;///< 单击条目激活窗体，默认false；
    QColor m_HightLightItemClr;///< 当窗口激活时，会对对应的条目高亮；此为高亮的颜色
};

#endif // MDIWINDOWMODEL_H
