#include "SAXMLWriteHelper.h"
#include "SAXMLTagDefined.h"
#include "SAVariantCaster.h"
#include <QDomDocument>
class SAXMLWriteHelperPrivate{
    SA_IMPL_PUBLIC(SAXMLWriteHelper)
public:
    SAXMLWriteHelperPrivate(SAXMLWriteHelper* p, const QString& type);
    //由于是随机读写，不能用xmlstream
    QDomDocument mDoc;
    QDomElement mRootEle;
    QDomElement mHeaderEle;
    QDomElement mContentEle;
    QDomElement mTmpHeadEle;
    QDomElement mTmpContentEle;
};


SAXMLWriteHelperPrivate::SAXMLWriteHelperPrivate(SAXMLWriteHelper *p, const QString& type):q_ptr(p)
  ,mDoc("SA_XML")
{
    mRootEle = mDoc.createElement(SA_XML_TAG_SA);
    mRootEle.setAttribute(SA_XML_ATT_TYPE,type);
    mDoc.appendChild(mRootEle);
    mHeaderEle = mDoc.createElement(SA_XML_TAG_HEADER_ROOT);
    mRootEle.appendChild(mHeaderEle);
    mContentEle = mDoc.createElement(SA_XML_TAG_CONTENT_ROOT);
    mRootEle.appendChild(mContentEle);
}


SAXMLWriteHelper::SAXMLWriteHelper(const QString& type)
    :d_ptr(new SAXMLWriteHelperPrivate(this,type))

{
}

SAXMLWriteHelper::~SAXMLWriteHelper()
{
}
/**
 * @brief 开始协议头编辑
 * @param name 协议头的组属性
 */
void SAXMLWriteHelper::startHeadWriteGroup(const QString &name)
{
    d_ptr->mTmpHeadEle = d_ptr->mDoc.createElement(name);
    d_ptr->mRootEle.appendChild(d_ptr->mTmpHeadEle);
}
/**
 * @brief 结束协议头的编辑
 * 必须和 @sa startHeadWriteGroup 配套
 */
void SAXMLWriteHelper::endHeadWriteGroup()
{
    d_ptr->mTmpHeadEle = QDomElement();
}

/**
 * @brief 写入协议头的数据
 * @param name 名字
 * @param d 数据
 */
void SAXMLWriteHelper::writeHeadValue( const QString &name, const QVariant &d)
{
    QDomElement ei;
    ei = d_ptr->mDoc.createElement(SA_XML_TAG_ITEM);
    ei.setAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VALUE);
    ei.setAttribute(SA_XML_ATT_NAME,name);
    ei.setAttribute(SA_XML_ATT_VALUE_TYPE,d.typeName());
    ei.setAttribute(SA_XML_ATT_VALUE,SAVariantCaster::variantToString(d));
    if(d_ptr->mTmpHeadEle.isNull())
    {
        d_ptr->mHeaderEle.appendChild(ei);
    }
    else
    {
        d_ptr->mTmpHeadEle.appendChild(ei);
    }
}

/**
 * @brief 开始内容区分组
 * @param name 组名
 */
void SAXMLWriteHelper::startContentWriteGroup(const QString &name)
{
    d_ptr->mTmpContentEle = d_ptr->mDoc.createElement(name);
    d_ptr->mRootEle.appendChild(d_ptr->mTmpContentEle);
}

/**
 * @brief 结束内容区分组
 *
 * 此函数必须和 @sa startContentWriteGroup 搭配使用
 */
void SAXMLWriteHelper::endContentWriteGroup()
{
    d_ptr->mTmpContentEle = QDomElement();
}

/**
 * @brief 在内容区写入数据
 * @param name 数据描述
 * @param d 数据值
 */
void SAXMLWriteHelper::writeContentValue(const QString &name, const QVariant &d)
{
    QDomElement ei;
    ei = d_ptr->mDoc.createElement(SA_XML_TAG_ITEM);
    ei.setAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VALUE);
    ei.setAttribute(SA_XML_ATT_NAME,name);
    ei.setAttribute(SA_XML_ATT_VALUE_TYPE,d.typeName());
    ei.setAttribute(SA_XML_ATT_VALUE,SAVariantCaster::variantToString(d));
    if(d_ptr->mTmpContentEle.isNull())
    {
        d_ptr->mContentEle.appendChild(ei);
    }
    else
    {
        d_ptr->mTmpContentEle.appendChild(ei);
    }
}

/**
 * @brief 转换为文本
 * @param indent 缩进大小 @default 1
 * @return 返回转换的xml
 */
QString SAXMLWriteHelper::toString(int indent) const
{
    return d_ptr->mDoc.toString(indent);
}

