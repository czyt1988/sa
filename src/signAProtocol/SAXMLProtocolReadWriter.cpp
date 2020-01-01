#include "SAXMLProtocolReadWriter.h"
#include "SAXMLTagDefined.h"
#include "SAVariantCaster.h"
#include <QDomDocument>
#include <QHash>
class SAXMLProtocolReadWriterPrivate{
    SA_IMPL_PUBLIC(SAXMLWriteHelper)
public:
    SAXMLProtocolReadWriterPrivate(SAXMLProtocolReadWriter* p, const QString& type);
    //由于是随机读写，不能用xmlstream
    QDomDocument mDoc;
    QDomElement mRootEle; //根节点
    QDomElement mHeaderEle; //头区节点
    QDomElement mContentEle; //内容区的节点
    QHash<QString,QDomElement> mUserGroup; //保存用户自定义的分组
    QString mCurrentGroupName;///< 当前分组的名字
    QDomElement mCurrentEle;
    QHash<QString,QHash<QString,QString> > mGroupValue2Type;///< Content的所有变量和类型
    //测试当前节点，此函数会保证当前节点有效，如果无效会指到Content Ele
    void testEle();
    void recordGroupValueType(const QString& valuename,const QString& valuetype);
};


SAXMLProtocolReadWriterPrivate::SAXMLProtocolReadWriterPrivate(SAXMLProtocolReadWriter *p, const QString& type):q_ptr(p)
  ,mDoc("SA_XML_PROTOCOL")
{
    mRootEle = mDoc.createElement(SA_XML_TAG_SA);
    mRootEle.setAttribute(SA_XML_ATT_TYPE,type);
    mDoc.appendChild(mRootEle);
    mHeaderEle = mDoc.createElement(SA_XML_TAG_HEADER_ROOT);
    mRootEle.appendChild(mHeaderEle);
    mContentEle = mDoc.createElement(SA_XML_TAG_CONTENT_ROOT);
    mRootEle.appendChild(mContentEle);
    mCurrentEle = mContentEle;
    mCurrentGroupName = SA_XML_TAG_CONTENT_ROOT;
}

void SAXMLProtocolReadWriterPrivate::testEle()
{
    if(mCurrentEle.isNull())
    {
        mCurrentEle = mContentEle;
        mCurrentGroupName = SA_XML_TAG_CONTENT_ROOT;
    }
}

void SAXMLProtocolReadWriterPrivate::recordGroupValueType(const QString &valuename, const QString &valuetype)
{
    if (!mGroupValue2Type.contains(mCurrentGroupName))
    {
        mGroupValue2Type[mCurrentGroupName] = QHash<QString,QString>();
    }
    mGroupValue2Type[mCurrentGroupName].insert(valuename,valuetype);
}


SAXMLProtocolReadWriter::SAXMLProtocolReadWriter(const QString& type)
    :d_ptr(new SAXMLProtocolReadWriterPrivate(this,type))

{
}

SAXMLProtocolReadWriter::~SAXMLProtocolReadWriter()
{
}

/**
 * @brief 切换分组
 * @param name 分组名称
 * @param createGopIfNone 如果不存在分组，将创建这个分组并把当前分组指到创建的这个分组中
 */
void SAXMLProtocolReadWriter::changeGroup(const QString &name, bool createGopIfNone)
{
    else if(SA_XML_TAG_CONTENT_ROOT == name)
    {
        d_ptr->mCurrentEle = d_ptr->mContentEle;
        d_ptr->mCurrentGroupName = SA_XML_TAG_CONTENT_ROOT;
        return;
    }
    else if(d_ptr->mUserGroup.contains(name))
    {
        d_ptr->mCurrentEle = d_ptr->mUserGroup[name];
        d_ptr->mCurrentGroupName = name;
        return;
    }
    if(createGopIfNone)
    {
        QDomElement ele = d_ptr->mDoc.createElement(name);
        d_ptr->mRootEle.appendChild(ele);
        d_ptr->mUserGroup[name] = ele;
        d_ptr->mCurrentEle = ele;
        d_ptr->mCurrentGroupName = name;
    }
}
/**
 * @brief 切换到content分组
 */
void SAXMLProtocolReadWriter::changeToDefaultGroup()
{
    changeGroup(SA_XML_TAG_CONTENT_ROOT);
}

/**
 * @brief 判断当前是否在默认分组
 * @return 如果是返回true
 */
bool SAXMLProtocolReadWriter::isInDefaultGroup() const
{
    return d_ptr->mCurrentEle == d_ptr->mContentEle;
}


/**
 * @brief 把QVariant内容区写入数据
 *
 * QVariant的类型描述为type="var-value"，且会多一个varType类型描述
 * @param name 数据描述
 * @param d 数据值
 * @example
 * @code
 * <sa>
 *  <root>
 *   <header></header>
 *   <content>
 *     <item type="var-value" name="point-size" varType="uint">5</item>
 *     <item type="var-value" name="sequenceID" varType="uint">123</item>
 *   </content>
 *  </root>
 * </sa>
 * @endcode
 */
void SAXMLProtocolReadWriter::writeValue(const QString &name, const QVariant &d)
{
    QDomElement ei;
    ei = d_ptr->mDoc.createElement(SA_XML_TAG_ITEM);
    ei.setAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VALUE);
    ei.setAttribute(SA_XML_ATT_NAME,name);
    ei.setAttribute(SA_XML_ATT_VALUE_TYPE,d.typeName());
    ei.setNodeValue(SAVariantCaster::variantToString(d));
    d_ptr->testEle();
    d_ptr->mCurrentEle.appendChild(ei);
    d_ptr->recordGroupValueType(name,ATT_TYPE_VALUE);
}

/**
 * @brief 把QVector<QPoint>内容区写入数据
 *
 * @code
 * <sa>
 *  <root>
 *   <header></header>
 *   <content>
 *     <item type="vectorpoint-value" name="points">1,2|3,5|4,6</item>
 *   </content>
 *  </root>
 * </sa>
 * @endcode
 *
 * @param name 数据描述
 * @param d 数据值
 * @see SAXMLProtocolReadWriter::writeValue(const QString &name, const QVector<QPointF> &d)

 */
void SAXMLProtocolReadWriter::writeValue(const QString &name, const QVector<QPoint> &d)
{
    QDomElement ei;
    ei = d_ptr->mDoc.createElement(SA_XML_TAG_ITEM);
    ei.setAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VECTORPOINT);
    ei.setAttribute(SA_XML_ATT_NAME,name);
    ei.setNodeValue(castToString(d));
    d_ptr->testEle();
    d_ptr->mCurrentEle.appendChild(ei);
    d_ptr->recordGroupValueType(name,ATT_TYPE_VECTORPOINT);
}



/**
 * @brief 针对点序列的xml协议写入
 * @param name 数据描述
 * @param d 数据值
 */
void SAXMLProtocolReadWriter::writeValue(const QString &name, const QVector<QPointF> &d)
{
    QDomElement ei;
    ei = d_ptr->mDoc.createElement(SA_XML_TAG_ITEM);
    ei.setAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VECTORPOINTF);
    ei.setAttribute(SA_XML_ATT_NAME,name);
    ei.setNodeValue(castToString(d));
    d_ptr->testEle();
    d_ptr->mCurrentEle.appendChild(ei);
    d_ptr->recordGroupValueType(name,ATT_TYPE_VECTORPOINTF);
}

/**
 * @brief 转换为文本
 * @param indent 缩进大小 @default 1
 * @return 返回转换的xml
 */
QString SAXMLProtocolReadWriter::toString(int indent) const
{
    return d_ptr->mDoc.toString(indent);
}


/**
 * @brief 获取用户分组内容
 * @return
 */
QList<QString> SAXMLProtocolReadWriter::getUserGroup() const
{
    return d_ptr->mUserGroup.keys();
}

/**
 * @brief 获取所有Content区的变量名
 * @return
 */
QList<QString> SAXMLProtocolReadWriter::getContentGroupValueNames() const
{

}

/**
 * @brief 把QPointF类型转换为字符，数组的间隔默认使用|
 * @param d 点
 * @return x,y字符
 */
QString SAXMLProtocolReadWriter::castToString(const QPointF &d)
{
    return QString("%1,%2").arg(d.x()).arg(d.y());
}
/**
 * @brief 把QPoint类型转换为字符，数组的间隔默认使用|
 * @param d 点
 * @return x,y字符
 */
QString SAXMLProtocolReadWriter::castToString(const QPoint &d)
{
    return QString("%1,%2").arg(d.x()).arg(d.y());
}

