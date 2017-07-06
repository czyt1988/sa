#ifndef SAXMLREADHELPER_H
#define SAXMLREADHELPER_H
#include "SALibGlobal.h"
#include <QScopedPointer>
#include <memory>
#include <QXmlStreamReader>
//class QXmlStreamReader;
class SADataFeatureItem;

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

#endif // SAXMLPARSEHELPER_H
