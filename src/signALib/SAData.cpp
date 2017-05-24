#include "SAData.h"
#include <algorithm>
#include <QDebug>
//#include <SADataTypeInfo.h>

///
/// \fn bool SAAbstractDatas::toDoubleVector(QVector<double>& data,const QVector<int>* index=nullptr) const
/// \brief
///



///



//SATable::SATable()
//{
//    this->id = int(this);
//}

//SATable::SATable(const QString& name)
//{
//    this->id = int(this);
//    setName(name);
//}

//void SATable::push_back(SAAbstractDatas* d)
//{
//    COLUMNPtr ptr(d);
//    this->columnData.push_back(ptr);
//}

//void SATable::append(SATable::COLUMNPtr d)
//{
//    this->columnData.push_back(d);
//}

//void SATable::addByTable(const SATable* table)
//{
//    auto size = table->columnCount();
//    for(auto i(0);i<size;++i)
//    {
//        append(table->at(i));
//    }
//}

//inline SATable::COLUMNPtr SATable::lastColumn()
//{
//    return columnData.back();
//}

//inline SATable::COLUMNPtr SATable::firstColumn()
//{
//    return columnData.front();
//}



//QList<QString> SATable::columnsNameList() const
//{
//    QStringList columnNames;
//    std::for_each(this->columnData.begin(),this->columnData.end()
//                  ,[&](COLUMNPtr d){columnNames<< (d->getName());});
//    return std::move(columnNames);
//}

/////
///// \brief 判断现有的列里面是否存在同名的列
///// \param name 用于判断的列名
///// \return
/////
//bool SATable::isHaveColumnName(const QString& name) const
//{
//    auto nameList = columnsNameList();
//    return (std::find(nameList.begin(),nameList.end(),name) != nameList.end());
//}

//SAAbstractDatas* SATable::get(int index)
//{
//    return columnData[index].get();
//}
//SATable::COLUMNPtr& SATable::at(int index)
//{
//    return columnData[index];
//}
//const SATable::COLUMNPtr& SATable::at(int index) const
//{
//    return columnData[index];
//}
/////
///// \brief 创建序列数据
///// \return
/////
//SATable::SeriesPtr SATable::makeCloumnSeries()
//{
//    SeriesPtr series = std::make_shared<SACloumnSeries>();
//    this->columnData.push_back(std::static_pointer_cast<SAAbstractDatas>(series));
//    return series;
//}
/////
///// \brief 创建点群数据
///// \return
/////
//SATable::PointsPtr SATable::makeCloumnPoints()
//{
//    PointsPtr points = std::make_shared<SACloumnPoints>();
//    this->columnData.push_back(std::static_pointer_cast<SACloumnPoints>(points));
//    return points;
//}

////SATable::COLUMNPtr& SATable::makeCloumnPoints(SACloumnPoints** ptr)
////{
////    SACloumnPoints* points = new SACloumnPoints();
////    COLUMNPtr sptr(points);
////    this->columnData.push_back(sptr);
////    if(ptr)
////        *ptr = points;
////    return this->columnData.back();
////}

//bool SATable::findById(int id, COLUMNPtr& res) const
//{
//    auto i = std::find_if(this->begin(),this->end()
//                          ,[&](const COLUMNPtr& d)->bool{return d->getID() == id;});
//    if(i == this->end())
//        return false;
//    res = *i;
//    return true;
//}
/////
///// \brief 清除会把数据内存清除，如果多个表关联了一个数据注意使用这个函数
///// \return
/////
//void SATable::clearAll()
//{
//    std::for_each(this->columnData.begin(),this->columnData.end()
//                  ,[&](COLUMNPtr d){d.reset();});
//    this->columnData.clear();
//}
/////
///// \brief 清除某列，此函数直接清除某列的数据，如果其他表有引用此列数据，注意，否则会出问题
///// \param index
/////
//void SATable::clearAt(size_t index)
//{
//    COLUMNPtr col = this->at(index);
//    this->columnData.removeAt(index);
//    col.reset();
//}
/////
///// \brief 移除所有数据
///// \return
/////
//void SATable::removeAll()
//{
//    this->columnData.clear();
//}
/////
///// \brief 移除一列数据
///// \param index
///// \return
/////
//SATable::COLUMNPtr SATable::removeAt(size_t index)
//{
//    COLUMNPtr col = this->at(index);
//    this->columnData.removeAt(index);
//    return col;
//}














