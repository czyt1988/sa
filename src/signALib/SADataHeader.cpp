#include "SADataHeader.h"
#include <SAData.h>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#define MAGIC_1 0x12340987
#define MAGIC_2 0xABCDEF56

#include <QApplication>
#define TR(str)\
    QApplication::translate("SADataHeader", str, 0)


class SADataHeaderPrivate
{
    SA_IMPL_PUBLIC(SADataHeader)
public:
    unsigned int m_magic1;///< 魔数1
    int m_dataType;///< 记录变量类型
    int m_versionFlag;///< 版本标量
    int m_externFlag;///< 扩展标记，用于保存旧的id
    int m_dataDim;///<数据的维度
    QString m_version;///< 记录版本
    QString m_dataName;///< 记录变量名
    QString m_externInfo;///< 扩展标记信息
    unsigned int m_magic2;///< 魔数2
    SADataHeaderPrivate(SADataHeader* p):q_ptr(p)
    {

    }
    void init()
    {
        m_versionFlag = 1;//第一版
        m_externFlag = 0;
        m_dataType = SA::UnknowType;
        m_externFlag = 0;
        m_dataDim = 0;
        m_magic1 = MAGIC_1;
        m_magic2 = MAGIC_2;
    }
};


SADataHeader::SADataHeader():d_ptr(new SADataHeaderPrivate(this))
{
    SA_D(SADataHeader);
    d->init();
    setVersionString(SA_DATA_VERSION);
}

SADataHeader::SADataHeader(const SAAbstractDatas *data):d_ptr(new SADataHeaderPrivate(this))
{
    SA_D(SADataHeader);
    d->init();
    d->m_versionFlag = 1;//第一版
    if(data)
    {
        d->m_externFlag = data->getID();
        d->m_dataType = data->getType();
        d->m_dataName = data->getName();
        d->m_dataDim = data->getDim();
    }
}

SADataHeader::~SADataHeader()
{

}

void SADataHeader::setVersionString(const QString &version)
{
    SA_D(SADataHeader);
    d->m_version = version;
}

QString SADataHeader::getVersionString() const
{
    SA_DC(SADataHeader);
    return d->m_version;
}

void SADataHeader::setVersionFlag(int flag)
{
    SA_D(SADataHeader);
    d->m_versionFlag = flag;
}

int SADataHeader::getVersionFlag() const
{
    SA_DC(SADataHeader);
    return d->m_versionFlag;
}

void SADataHeader::setExternFlag(int flag)
{
    SA_D(SADataHeader);
    d->m_externFlag = flag;
}

int SADataHeader::getExternFlag() const
{
    SA_DC(SADataHeader);
    return d->m_externFlag;
}

void SADataHeader::setExternInfo(const QString &info)
{
    SA_D(SADataHeader);
    d->m_externInfo = info;
}

QString SADataHeader::getExternInfo() const
{
    SA_DC(SADataHeader);
    return d->m_externInfo;
}

void SADataHeader::setDataName(const QString &name)
{
    SA_D(SADataHeader);
    d->m_dataName = name;
}

QString SADataHeader::getDataName() const
{
    SA_DC(SADataHeader);
    return d->m_dataName;
}

void SADataHeader::setDataType(int type)
{
    SA_D(SADataHeader);
    d->m_dataType = type;
}

int SADataHeader::getDataType() const
{
    SA_DC(SADataHeader);
    return d->m_dataType;
}


bool SADataHeader::isValid() const
{
    SA_DC(SADataHeader);
    return ((d->m_magic1 == MAGIC_1) && (d->m_magic2 == MAGIC_2));
}

void SADataHeader::setInValid()
{
    SA_D(SADataHeader);
    d->m_magic1 = 0;
    d->m_magic2 = 0;
}

bool SADataHeader::parserXML(SADataHeader *d, const QString &xmlString,QString& errString)
{
    QXmlStreamReader xml(xmlString);
    //ROOT节点解析
    xml.readNextStartElement();
    if(xml.name() != getXMLRootStartElementString())
    {//xml文件第一个节点root丢失，定义必须包含root节点以便扩展
        errString = TR("miss root element:%1").arg(getXMLRootStartElementString());
        return false;
    }
    QStringRef refStr = xml.attributes().value("version");
    if(refStr.isEmpty())
    {
        //说明version att丢失
        errString = TR("miss version attribute");
        return false;
    }
    d->setVersionString(refStr.toString());//获取版本
    QString tag;
    while(QXmlStreamReader::Invalid != xml.readNext())
    {
        if(xml.isStartElement())
        {
            if(xml.name() == getXMLPropertyStartElementString())
            {
                while(QXmlStreamReader::Invalid != xml.readNext())
                {
                    if(xml.isStartElement())
                    {
                        tag = xml.name().toString();
                        if("dataType" == tag)
                        {
                            d->setDataType(xml.readElementText().toInt());
                        }
                        else if("versionFlag" == tag)
                        {
                            d->setVersionFlag(xml.readElementText().toInt());
                        }
                        else if("externFlag" == tag)
                        {
                            d->setExternFlag(xml.readElementText().toInt());
                        }
                        else if("dataDim" == tag)
                        {
                            d->setDataDim(xml.readElementText().toInt());
                        }
                        else if("dataName" == tag)
                        {
                            d->setDataName(xml.readElementText());
                        }
                        else if("externInfo" == tag)
                        {
                            d->setExternInfo(xml.readElementText());
                        }
                    }
                }
            }
            else if(xml.isEndElement())
            {
                if(xml.name() == getXMLRootStartElementString())
                {
                    break;
                }
            }
        }
    }
    return true;
}

bool SADataHeader::makeXML(const SADataHeader* d, QString &xmlString)
{
    xmlString.clear();
    QXmlStreamWriter xml(&xmlString);
    xml.setCodec("UTF-8");
    xml.writeStartDocument();
    xml.writeStartElement(getXMLRootStartElementString());//<sa version="v0.1">
    xml.writeAttribute("version",SA_DATA_VERSION);
    xml.writeStartElement(getXMLPropertyStartElementString());//<prop>
    {
        xml.writeTextElement("dataType",QString::number(d->getDataType()));
        xml.writeTextElement("versionFlag",QString::number(d->getVersionFlag()));
        xml.writeTextElement("externFlag",QString::number(d->getExternFlag()));
        xml.writeTextElement("dataDim",QString::number(d->getDataDim()));
        xml.writeTextElement("dataName",d->getDataName());
        xml.writeTextElement("externInfo",d->getExternInfo());
    }
    xml.writeEndElement();//</prop>
    xml.writeEndElement();//</sa>
    xml.writeEndDocument();
    return (!xml.hasError());
}

QString SADataHeader::getXMLRootStartElementString()
{
    return "sa";
}

QString SADataHeader::getXMLPropertyStartElementString()
{
    return "prop";
}

int SADataHeader::getDataDim() const
{
    SA_DC(SADataHeader);
    return d->m_dataDim;
}

void SADataHeader::setDataDim(int dataDim)
{
    SA_D(SADataHeader);
    d->m_dataDim = dataDim;
}


///
/// \brief SADataTypeInfo文件头序列化
///
/// 文件第一个为魔数，第二个为xml文本的长度，第三个流为xml文本，第四个流为结尾魔数
///
/// 用xml文本的原因是为了以后可能性能无限兼容低版本，同时能进行扩展
/// \param out
/// \param item
/// \return
///
QDataStream &operator<<(QDataStream &out, const SADataHeader &item)
{
    SADataHeader::MAGIC_TYPE magic = MAGIC_1;
    out<<magic;
    QString xmlString;
    if(!SADataHeader::makeXML(&item,xmlString))
    {
        return out;
    }
    int sizeString = xmlString.size();
    out<<sizeString;
    out<<xmlString;
    magic = MAGIC_2;
    out<<magic;
    return out;
}
///
/// \brief SADataTypeInfo文件头序列化
/// \param in
/// \param item
/// \return
///
QDataStream &operator>>(QDataStream &in, SADataHeader &item)
{
    item.setInValid();//先设置为非有效
    SADataHeader::MAGIC_TYPE magic;
    in>>magic;//先读取第一个魔数
    if(magic != MAGIC_1)
        return in;
    int xmlStringSize;
    in>>xmlStringSize;//xml数据的长度
    QString strXML;
    QString strErr;
    in>>strXML;//xml文本
    SADataHeader::parserXML(&item,strXML,strErr);
    in>>magic;
    if(magic != MAGIC_2)
    {
        item.setInValid();//先设置为非有效
        return in;
    }
    item.d_ptr->m_magic1 = MAGIC_1;
    item.d_ptr->m_magic2 = MAGIC_2;
    return in;
}


