#ifndef SADATAHEADER_H
#define SADATAHEADER_H
#include <QString>
#include <QDataStream>
#include "SALibGlobal.h"
#ifndef SA_DATA_VERSION
#define SA_DATA_VERSION "v0.1"
#endif


class SAAbstractDatas;
///
/// \brief 用于记录数据的文件头
///
class SALIB_EXPORT SADataHeader
{
public:
    typedef unsigned int MAGIC_TYPE;
    SADataHeader();
    SADataHeader(const SAAbstractDatas* data);

    friend QDataStream &operator<<(QDataStream & out, const SADataHeader & item);
    friend QDataStream &operator>>(QDataStream & in, SADataHeader & item);
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
    void setVersionFlag(int flag){m_versionFlag = flag;}
    int getVersionFlag() const{return m_versionFlag;}
    ///
    /// \brief 扩展标记整形字段
    /// \param flag
    ///
    void setExternFlag(int flag){m_externFlag = flag;}
    int getExternFlag() const{return m_externFlag;}
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
    void setDataType(int type){m_dataType = type;}
    int getDataType() const{return m_dataType;}
    ///
    /// \brief 判断是否有效的文件头
    /// \return
    ///
    bool isValid() const;
    void setInValid();

    int getDataDim() const;
    void setDataDim(int dataDim);

private:
    static bool parserXML(SADataHeader* d, const QString& xmlString, QString &errString);
    static bool makeXML(const SADataHeader *d, QString& xmlString);
    static QString getXMLRootStartElementString(){return "sa";}
    static QString getXMLPropertyStartElementString(){return "prop";}
private:

    unsigned int m_magic1;///< 魔数1
    int m_dataType;///< 记录变量类型
    int m_versionFlag;///< 版本标量
    int m_externFlag;///< 扩展标记，用于保存旧的id
    int m_dataDim;///<数据的维度
    QString m_version;///< 记录版本
    QString m_dataName;///< 记录变量名
    QString m_externInfo;///< 扩展标记信息
    unsigned int m_magic2;///< 魔数2
};


SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const SADataHeader & item);
SALIB_EXPORT QDataStream &operator>>(QDataStream & in, SADataHeader & item);

#endif // SADATATYPEINFO_H
