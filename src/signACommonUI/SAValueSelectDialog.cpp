#include "SAValueSelectDialog.h"
#include "ui_SAValueSelectDialog.h"
//SALib
#include "SAValueManagerModel.h"
#include "SADataTableModel.h"
#include "SAAbstractDatas.h"
SAValueSelectDialog::SAValueSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAValueSelectDialog)
  ,m_selectMode(SAValueSelectDialog::MultiSelection)
  ,m_hightLightColor(QColor(255,160,206))
{
    ui->setupUi(this);
    //
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&SAValueSelectDialog::onAccept);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&QDialog::reject);
    //
    connect(ui->treeView,&QTreeView::clicked,this,&SAValueSelectDialog::onTreeClicked);
    initUI();
}

SAValueSelectDialog::~SAValueSelectDialog()
{
    delete ui;
}
///
/// \brief 获取选择的数据
/// \return 如果没有，返回的QList 的size为0
///
QList<SAAbstractDatas *> SAValueSelectDialog::getSelectDatas()
{
    QList<SAAbstractDatas *> resData;
    QItemSelectionModel *sel = ui->treeView->selectionModel();
    QModelIndexList indexList = sel->selectedRows();
    if (indexList.size()<=0)
    {
        return resData;
    }
    //不能绘图的选择项目被移除
    QString info;
    for(int i=0;i<indexList.size();++i)
    {
        QModelIndex index = indexList[i];
        SAAbstractDatas* data = m_valueModel->castToDataPtr(index);

        if(nullptr == data)
        {
            continue;
        }
        if(data->isEmpty())
        {
            continue;
        }
        resData.append(data);
    }
    return resData;
}
///
/// \brief 获取选择的数据静态函数
/// \param parent 父窗口
/// \param selectMode 选择数据模式，单选或多选
/// \return 如果没有，返回的QList 的size为0
/// \see SAValueSelectDialog::SelectionMode
///
QList<SAAbstractDatas *> SAValueSelectDialog::getSelectDatas(QWidget *parent,const SelectionMode selectMode)
{
    SAValueSelectDialog dlg(parent);
    dlg.setSelectMode(selectMode);
    if(QDialog::Accepted == dlg.exec())
    {
        return dlg.getSelectDatas();
    }
    return QList<SAAbstractDatas *>();
}

void SAValueSelectDialog::initUI()
{
    m_valueModel = new SAValueManagerModel(this);
    m_valueModel->setFunBackgroundData([this](const QModelIndex &index,SAAbstractDatas* d)->QVariant{
        if(this->m_hightLightData.contains(d))
        {
            return this->getHightLightColor();
        }
        return QVariant();
    });
    ui->treeView->setModel(m_valueModel);


    m_dataModel = new SADataTableModel(this);
    ui->tableView->setModel(m_dataModel);
}


void SAValueSelectDialog::onTreeClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QList<SAAbstractDatas *> datas = getSelectDatas();
    m_dataModel->setSADataPtrs(datas);
}


void SAValueSelectDialog::onAccept()
{
    QDialog::accept();
}

QColor SAValueSelectDialog::getHightLightColor() const
{
    return m_hightLightColor;
}

void SAValueSelectDialog::setHightLightColor(const QColor &hightLightColor)
{
    m_hightLightColor = hightLightColor;
}
///
/// \brief 变量选择的树形控件的选择模式
/// \return
///
SAValueSelectDialog::SelectionMode SAValueSelectDialog::getSelectMode() const
{
    return m_selectMode;
}
///
/// \brief 变量选择的树形控件的选择模式设置，可设置为单选或多选
/// \param selectMode 单选或多选
///
void SAValueSelectDialog::setSelectMode(const SelectionMode selectMode)
{
    m_selectMode = selectMode;
    QAbstractItemView::SelectionMode mode;
    switch(selectMode)
    {
    case SAValueSelectDialog::SingleSelection:
        mode = QAbstractItemView::SingleSelection;
        break;
    case SAValueSelectDialog::MultiSelection:
        mode = QAbstractItemView::MultiSelection;
        break;
    default:
        mode = QAbstractItemView::MultiSelection;
    }
    ui->treeView->setSelectionMode(mode);
}

void SAValueSelectDialog::setDefaultSelectDatas(const QList<SAAbstractDatas *> &defaultSelDatas)
{
    m_hightLightData = defaultSelDatas.toSet();
    QItemSelectionModel *sel = ui->treeView->selectionModel();
    if(!sel)
    {
        return;
    }
    QModelIndexList indexList = m_valueModel->datasToIndexs(defaultSelDatas.toSet());
    std::for_each(indexList.begin(),indexList.end(),[&sel](const QModelIndex& index){
        sel->select(index,QItemSelectionModel::Select);
    });

}

