#include "SAAddCurveTypeDialog.h"
#include "ui_SAAddCurveTypeDialog.h"
#include <QButtonGroup>
SAAddCurveTypeDialog::SAAddCurveTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAAddCurveTypeDialog)
{
    ui->setupUi(this);
    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(ui->radioButtonNewFig,AddInNewFig);
    m_btnGroup->addButton(ui->radioButtonCurFig,AddInCurrentFig);
    m_btnGroup->addButton(ui->radioButtonCurFigInSubplot,AddInCurrentFigWithSubplot);
    m_btnGroup->setExclusive(true);
    ui->radioButtonNewFig->setChecked(true);
}

SAAddCurveTypeDialog::~SAAddCurveTypeDialog()
{
    delete ui;
}

SAAddCurveTypeDialog::AddCurveType SAAddCurveTypeDialog::getSelectType() const
{
    return static_cast<AddCurveType>(m_btnGroup->checkedId());
}

void SAAddCurveTypeDialog::setSelectType(SAAddCurveTypeDialog::AddCurveType type)
{
    QAbstractButton* btn = m_btnGroup->button(static_cast<int>(type));
    if(btn)
    {
        btn->setChecked(true);
    }
}

SAAddCurveTypeDialog::AddCurveType SAAddCurveTypeDialog::getAddCurveType(QWidget *par, SAAddCurveTypeDialog::AddCurveType defaultType, const QString &title)
{
    QString wndTitle = title;
    if(wndTitle.isNull())
    {
        wndTitle = tr("select add curve type");
    }
    SAAddCurveTypeDialog dlg(par);
    dlg.setWindowTitle(wndTitle);
    dlg.setSelectType(defaultType);
    if(QDialog::Accepted == dlg.exec())
    {
        return static_cast<SAAddCurveTypeDialog::AddCurveType>(dlg.getSelectType());
    }
    return SAAddCurveTypeDialog::Unknow;
}
