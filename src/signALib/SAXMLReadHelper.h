#ifndef SAXMLREADHELPER_H
#define SAXMLREADHELPER_H
#include "SALibGlobal.h"
#include <QScopedPointer>
#include <memory>
#include <QXmlStreamReader>
#include <QDomDocument>
//class QXmlStreamReader;
class SADataFeatureItem;
class SADataProcessVectorPointFXMLHelperPrivate;

///
/// \brief 此类用于方便解析xml文件
///
class SALIB_EXPORT SAXMLReadHelper
{
public:
    ///
    /// \brief 协议类型
    ///
    enum ProtocolType
    {
        UnknowType
        ,TypeVectorPointFProcessResult ///< 线性数组处理结果返回
    };

    SAXMLReadHelper(const QString &str);
    //是否有效
    bool isValid() const;
    //获取协议类型
    SAXMLReadHelper::ProtocolType getProtocolType() const;
    //获取线性数组的数据
    bool getVectorPointFProcessResult(quintptr &wnd,quintptr &fig,quintptr& itp,SADataFeatureItem* item);
protected:
    void init();
private:
    QScopedPointer<QXmlStreamReader> m_xml;
    bool m_isvalid;///< 是否有效
    ProtocolType m_protocolType;
    const QString* m_strPtr;
};


///
/// \brief 处理结果的xml读写帮助类
/// <dfi>
///     <group name=''>
///     <group>
/// <\dfi>
///
class SALIB_EXPORT SADataProcessVectorPointFXMLHelper
{
    SA_IMPL(SADataProcessVectorPointFXMLHelper)
public:
    SADataProcessVectorPointFXMLHelper();
    ~SADataProcessVectorPointFXMLHelper();
    void startWrite(QString* string);
    void endWrite();
    void startWriteGroup(const QString& name);
    void endWriteGroup();
    void writeValue(const QVariant &d, const QString& name);
    static bool read(const QString& xmlStr,SADataFeatureItem *item);
private:
    static bool readChildItem(const QDomNode& groupItem,SADataFeatureItem *parentItem);
    static bool getItemInfoFromElement(const QDomNode &xmlItem, SADataFeatureItem *item);
    static bool getGroupInfo(const QDomNode &xmlItem, QString& name);
};
#endif // SAXMLPARSEHELPER_H
