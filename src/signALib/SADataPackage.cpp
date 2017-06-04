#include "SADataPackage.h"
#include <QIcon>
#define ICON_PackageNormal QIcon(":/treeItemIcon/res_treeItemIcon/package_normal.png")

SAABstractDataPackage::SAABstractDataPackage():SAAbstractDatas()
{

}

SAABstractDataPackage::SAABstractDataPackage(const QString &text):SAAbstractDatas(text)
{

}

///
/// \brief 插入数据
/// \param datas 数据的指针
///
void SAABstractDataPackage::appendData(SAAbstractDatas *datas)
{
    appendRow(datas);
}

///
/// \brief 查询是否存在名字的子数据
/// \param fieldName
/// \return
///
bool SAABstractDataPackage::isfield(const QString &fieldName)
{
    const int r = rowCount();
    for(int i=0;i<r;++i)
    {
        auto p = child(i);
        if(nullptr == p)
        {
            continue;
        }
        if(p->text() == fieldName)
        {
            return true;
        }
    }
    return false;
}


SADataPackage::SADataPackage():SAABstractDataPackage()
{
    setIcon(ICON_PackageNormal);
}

SADataPackage::SADataPackage(const QString &text):SAABstractDataPackage(text)
{
    setIcon(ICON_PackageNormal);
}

int SADataPackage::getSize(int sizeType) const
{
    Q_UNUSED(sizeType);
    return rowCount();
}

QVariant SADataPackage::getAt(size_t index, size_t index2) const
{
    Q_UNUSED(index2);
    if((int)index >= rowCount())
    {
        return QVariant();
    }
    return child(index)->text();
}

bool SADataPackage::toDoubleVector(QVector<double> &data, const QVector<int> *index) const
{
    Q_UNUSED(data);
    Q_UNUSED(index);
    return false;
}

bool SADataPackage::isCanCast2DoubleVector() const
{
    return false;
}

bool SADataPackage::toPointFVector(QVector<QPointF> &data, const QVector<int> *index) const
{
    Q_UNUSED(data);
    Q_UNUSED(index);
    return false;
}

bool SADataPackage::isCanCast2PointFVector() const
{
    return false;
}
