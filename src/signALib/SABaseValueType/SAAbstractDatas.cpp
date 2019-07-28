#include "SAAbstractDatas.h"
#include "SADataReference.h"
#include "SAVectorDouble.h"
#include "SAVectorInt.h"
#include "SAVectorPointF.h"
#include "SAVariantDatas.h"
#include <QDebug>

SAAbstractDatas::SAAbstractDatas():SAItem()
{

}

SAAbstractDatas::SAAbstractDatas(const QString &text):SAItem(text)
{

}

SAAbstractDatas::~SAAbstractDatas()
{
    qDebug() << "destroy:" << getName();
}

///
/// \brief 相当于getAt({});适用0维单一数据
/// \return
///
QVariant SAAbstractDatas::getAt() const
{
    return getAt({});
}

///
/// \brief 相当于getAt({dim1});适用1维向量数据
/// \param dim1
/// \return
///
QVariant SAAbstractDatas::getAt(size_t dim1) const
{
    return getAt({dim1});
}

///
/// \brief 相当于getAt({dim1,dim2});适用2维表数据
/// \param dim1
/// \param dim2
/// \return
///
QVariant SAAbstractDatas::getAt(size_t dim1, size_t dim2) const
{
    return getAt({dim1,dim2});
}


QString SAAbstractDatas::displayAt() const
{
    return displayAt({});
}

QString SAAbstractDatas::displayAt(size_t dim1) const
{
    return displayAt({dim1});
}

QString SAAbstractDatas::displayAt(size_t dim1, size_t dim2) const
{
    return displayAt({dim1,dim2});
}
///
/// \brief 用于编辑，默认SAAbstractDatas返回false不接受编辑
/// \param val 需要设置的值
/// \param index 索引序列
/// \return 返回true设置成功，返回false设置失败
///
bool SAAbstractDatas::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    Q_UNUSED(val);
    Q_UNUSED(index);
    return false;
}



void SAAbstractDatas::read(QDataStream &in)
{
    QIcon icon;
    QString name;
    int propCounts;
    in >> icon >> name >> propCounts;
    setIcon(icon);
    setName(name);
    for(int i=0;i<propCounts;++i)
    {
        int id;
        QVariant var;
        in >> id >> var;
        setProperty(id,var);
    }
}

void SAAbstractDatas::write(QDataStream &out) const
{
    //QStandardItem::write(out);
    int dc = getPropertyCount();
    out << getIcon() << getName() << dc;
    for(int i=0;i<dc;++i)
    {
        int id;
        QVariant var;
        getProperty(i,id,var);
        out << id << var;
    }
}




