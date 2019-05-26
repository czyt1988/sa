#ifndef SAXMLREADHELPER_H
#define SAXMLREADHELPER_H
#include "SALibGlobal.h"
#include <QScopedPointer>
#include <memory>
#include <QXmlStreamReader>
#include <QDomDocument>
#include "SAXMLTagDefined.h"
//class QXmlStreamReader;
class SADataFeatureItem;
class SADataProcessVectorPointFXMLHelperPrivate;

///
/// \brief 此类用于方便解析xml文件
///
class SALIB_EXPORT SAXMLReadHelper
{
public:

    SAXMLReadHelper(const QString &str);
    //是否有效
    bool isValid() const;
    //获取协议类型
    SA_XML::ProtocolType getProtocolType() const;
    //获取线性数组的数据
    bool getVectorPointFProcessResult(uint& key,SADataFeatureItem* item);
    //获取错误描述
    QString getErrorMsg() const;

protected:
    void init();
private:
    //读取dfi节点
    bool readDfi(SADataFeatureItem *item);
    //读取dfi节点下的子节点
    bool readDfiChildItem(const QDomNode &groupItem, SADataFeatureItem *parentItem);
    //获取属性的值
    bool getAttrValue(const QDomNode &xmlItem,const QString& name, QString &value);
    //从dfi下面获取值
    bool getDfiParamItem(const QDomNode &xmlItem, SADataFeatureItem *item);
private:
    QDomDocument m_doc;
    QDomElement m_rootEle;
    QDomElement m_headerEle;
    QDomElement m_contentEle;
    QString m_errorMsg;
    bool m_isvalid;///< 是否有效
    SA_XML::ProtocolType m_protocolType;
};


#endif // SAXMLPARSEHELPER_H
