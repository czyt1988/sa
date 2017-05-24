#ifndef QTPROPERTYIDSTORAGE_H
#define QTPROPERTYIDSTORAGE_H

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include <QtTreePropertyBrowser>

template<typename ID_TYPE>
class QtPropertyIdStorage
{
public:
	QtPropertyIdStorage()
	{

	}
	~QtPropertyIdStorage()
	{

	}

public:
	void rememberTheProperty(const ID_TYPE& propertyIdName,QtProperty* propertyPtr)
	{
		m_id2Property[propertyIdName] = propertyPtr;
		m_Property2id[propertyPtr] = propertyIdName;
	}
	///
	/// \brief 删除所有property指针
	///
	void deleteAllProperty()
	{
		//删除所有记录的内容
		QMapIterator<QtProperty*,ID_TYPE> i(m_Property2id);
		while(i.hasNext())
		{
			i.next();
			delete i.key();
		}
		m_id2Property.clear();
		m_Property2id.clear();
	}
    void deleteProperty(ID_TYPE id)
    {
        QtProperty* pro = m_id2Property[id];
        m_id2Property.remove(id);
        m_Property2id.remove(pro);
    }

	QtProperty* getProperty(const ID_TYPE& propertyIdName) const
	{
		return m_id2Property[propertyIdName];
	}
	//    QString getPropertyID(QtProperty* property) const
	//    {
	//        return m_Property2id[property];
	//    }
	ID_TYPE getPropertyID(QtProperty* property) const
	{
		return m_Property2id[property];
	}
	///
	/// \brief addDoublePropertyInGroup 添加子对象进属性组里
	/// \param groupItem 属性组的指针
	/// \param propertyName 属性名，前端显示的名字
	/// \param propertyIdName 属性标识名，不允许重复
	/// \param propertyData 属性的值
	///
	QtProperty* addDoublePropertyInGroup(QtDoublePropertyManager* doubleManage
		,QtProperty* groupItem
		,const QString& propertyName
		,const ID_TYPE& propertyIdName
		,double propertyData)
	{
		QtProperty* subitem = doubleManage->addProperty(propertyName);
		doubleManage->setValue(subitem,propertyData);
		groupItem->addSubProperty(subitem);
		rememberTheProperty(propertyIdName,subitem);
		return subitem;
	}
	QtProperty* addDoublePropertyInGroup(QtVariantPropertyManager* varManage
		,QtProperty* groupItem
		,const QString& propertyName
		,const ID_TYPE& propertyIdName
		,double propertyData)
	{
		QtProperty* subitem = varManage->addProperty(QVariant::Double,propertyName);
		varManage->setValue(subitem,propertyData);
		groupItem->addSubProperty(subitem);
		rememberTheProperty(propertyIdName,subitem);
		return subitem;
	}
	///
	/// \brief property_addVariantPropertyInGroup 添加子对象进属性组里
	/// \param groupItem 属性组的指针
	/// \param propertyName 属性名，前端显示的名字
	/// \param propertyIdName 属性标识名，不允许重复
	/// \param propertyData 属性的值
	///
	QtVariantProperty* addVariantPropertyInGroup(QtVariantPropertyManager* VariantManage
		,int nType
		,QtProperty* groupItem
		,const QString& propertyName
		,const ID_TYPE& propertyIdName
		,QVariant propertyData)
	{
		QtVariantProperty* subitem = VariantManage->addProperty(nType,propertyName);
		VariantManage->setValue(subitem,propertyData);
		groupItem->addSubProperty(subitem);
		rememberTheProperty(propertyIdName,subitem);
		return subitem;
	}
	///
	/// \brief 在一个组里面再加入一个组
	/// \param pG 组管理指针
	/// \param parentGroupItem 父级组
	/// \param groupName 组名
	/// \param groupIdName 组名id标志
	/// \return
	///
	QtProperty* addGroupInGroup(QtGroupPropertyManager * pG
		,QtProperty* parentGroupItem
		,const QString& groupName
		,const ID_TYPE& groupIdName)
	{
		QtProperty* groupItem = pG->addProperty(groupName);
		parentGroupItem->addSubProperty(groupItem);
		rememberTheProperty(groupIdName,groupItem);
		return groupItem;
	}
	QtProperty* addGroupInGroup(QtVariantPropertyManager * pVar
		,QtProperty* parentGroupItem
		,const QString& groupName
		,const ID_TYPE& groupIdName)
	{
		QtProperty* groupItem = pVar->addProperty(QtVariantPropertyManager::groupTypeId()
			,groupName);
		parentGroupItem->addSubProperty(groupItem);
		rememberTheProperty(groupIdName,groupItem);
		return groupItem;
	}
	///
	/// \brief 设置属性表已有的double属性项目的值
	/// \param doubleManage
	/// \param propertyIdName 属性表项目的字符串id
	/// \param propertyData 值
	/// \return 成功修改返回true
	///
	bool setDoublePropertyData(QtDoublePropertyManager* doubleManage
		,const ID_TYPE& propertyIdName
		,double propertyData)
	{
		QtProperty* prop = m_id2Property[propertyIdName];
		if(nullptr == prop){
			return false;
		}
		doubleManage->setValue(prop,propertyData);
		return true;
	}
	//    bool setVarPropertyData(QtVariantPropertyManager* varManage
	//                                        , const QString& propertyIdName
	//                                        , QVariant varData)
	//    {
	//        QtProperty* prop = m_id2Property[propertyIdName];
	//        if(nullptr == prop){
	//            return false;
	//        }
	//        varManage->setValue(prop,varData);
	//        return true;
	//    }
	bool setVarPropertyData(const ID_TYPE& propertyIdName, QVariant varData)
	{
		QtVariantProperty* prop = static_cast<QtVariantProperty*>(m_id2Property[propertyIdName]);
		if(nullptr == prop){
			return false;
		}
		prop->setValue(varData);
		return true;
	}

	//     bool addGroupInTreeProperty(QtTreePropertyBrowser* treePro
	//                                 ,QtVariantPropertyManager * pVar
	//                                 ,const QString& groupName
	//                                 ,const QString& groupIdName)
	//     {
	//         pVar->addProperty(QtVariantPropertyManager::groupTypeId(),groupName);
	//     }

private:
	QMap<ID_TYPE,QtProperty*> m_id2Property;///< id对应的属性指针
	QMap<QtProperty*,ID_TYPE> m_Property2id;///<属性指针对应的id
};

#endif // QTPROPERTYIDSTORAGE_H
