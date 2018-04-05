#ifndef SADATAHEADER_H
#define SADATAHEADER_H
#ifndef SA_DATA_VERSION
#define SA_DATA_VERSION "v0.1"
#endif
#include <QString>
#include <QDataStream>
#include "SALibGlobal.h"

class SADataHeaderPrivate;
class SAAbstractDatas;
///
/// \brief 用于记录数据的文件头
///
class SALIB_EXPORT SADataHeader
{
    SA_IMPL(SADataHeader)
    Q_DISABLE_COPY(SADataHeader)
public:
    typedef unsigned int MAGIC_TYPE;
    SADataHeader();
    SADataHeader(const SAAbstractDatas* data);
    ~SADataHeader();
    friend SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const SADataHeader & item);
    friend SALIB_EXPORT QDataStream &operator>>(QDataStream & in, SADataHeader & item);
    ///
    /// \brief 设置版本字符
    /// \param version
    ///
    void setVersionString(const QString& version);
    QString getVersionString() const;
    ///
    /// \brief 版本号信息
    /// \param flag
    ///
    void setVersionFlag(int flag);
    int getVersionFlag() const;
    ///
    /// \brief 扩展标记整形字段
    /// \param flag
    ///
    void setExternFlag(int flag);
    int getExternFlag() const;
    ///
    /// \brief 扩展标记信息
    /// \param info
    ///
    void setExternInfo(const QString& info);
    QString getExternInfo() const;
    ///
    /// \brief 设置变量名
    /// \param name
    ///
    void setDataName(const QString& name);
    QString getDataName() const;
    ///
    /// \brief 设置变量类型
    /// \param type
    ///
    void setDataType(int type);
    int getDataType() const;
    ///
    /// \brief 判断是否有效的文件头
    /// \return
    ///
    bool isValid() const;
    void setInValid();
    ///
    /// \brief 数据的维度
    /// \return
    ///
    int getDataDim() const;
    void setDataDim(int dataDim);

private:
    static bool parserXML(SADataHeader* d, const QString& xmlString, QString &errString);
    static bool makeXML(const SADataHeader *d, QString& xmlString);
    static QString getXMLRootStartElementString();
    static QString getXMLPropertyStartElementString();
};


SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const SADataHeader & item);
SALIB_EXPORT QDataStream &operator>>(QDataStream & in, SADataHeader & item);


#endif // SADATATYPEINFO_H
