#include "SAConfigXMLReadWriter.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QDomNode>
#include <QDebug>
#include "SAVariantCaster.h"
#define CONFIG_FILE_NAME "saconfig.cfg"

#define CONFIG_ROOT_NAME "config"
#define CONFIG_GROUP_NAME "group"
#define CONFIG_CONTENT_PROP_NAME "name"
#define CONFIG_KEY_NAME "key"
#define CONFIG_KEY_PROP_NAME_NAME "name"
#define CONFIG_KEY_PROP_TYPE_NAME "type"
#define CONFIG_KEY_LIST_NAME "list"


class SAConfigXMLReadWriterPrivate
{
	SA_IMPL_PUBLIC(SAConfigXMLReadWriter)    
public:
    QString m_cfgPath;
    QDomDocument m_doc;
    bool m_isDirty;
    SAConfigXMLReadWriterPrivate(SAConfigXMLReadWriter* par)
        :q_ptr(par)
        ,m_doc("confog")
        ,m_isDirty(false)
    {
        m_doc.createElement("config");
    }
    
    SAConfigXMLReadWriterPrivate(SAConfigXMLReadWriter* par,const QString& cfgPath)
        :q_ptr(par)
        ,m_doc("confog")
        ,m_isDirty(false)
    {
        setCfgFile(cfgPath);
    }
    
    void createDefaultDoc()
    {
        m_doc = QDomDocument("config");
        QDomProcessingInstruction instruction = m_doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
        m_doc.appendChild(instruction);
        QDomElement base = m_doc.createElement(CONFIG_ROOT_NAME);
        m_doc.appendChild(base);
    }

    bool setCfgFile(const QString& cfgPath)
    {
        QFile file(cfgPath);
        if(!file.open(QIODevice::ReadWrite))
        {
            return false;
        }
        if(!m_doc.setContent(&file))
        {
            createDefaultDoc();
            return true;
        }
        if(m_doc.isNull())
    	{
            createDefaultDoc();
            return true;
    	}
        m_cfgPath = cfgPath;
        return true;
    }

    QDomNodeList getAllGroupNodes() const
    {
        QDomElement rootElement = m_doc.documentElement();//<config>
        return rootElement.childNodes();//<group>
    }

    bool getGroupNode(const QString& groupName,QDomElement* groupNode) const
    {
        QDomNodeList groupsNode = getAllGroupNodes();
        const int size = groupsNode.size();
        for(int i=0;i<size;++i)
        {
            QDomElement node = groupsNode.at(i).toElement();

            QDomNamedNodeMap attr = node.attributes();
            QDomNode attrName = attr.namedItem(CONFIG_CONTENT_PROP_NAME);
            if(attrName.isNull())
                continue;
            if(attrName.nodeValue() == groupName)
            {
                if(groupNode)
                {
                    *groupNode = groupsNode.at(i).toElement();
                }
                return true;
            }
        }
        return false;
    }

    bool getKeyNode(const QDomElement& groupNode,const QString& keyName,QDomElement* keyNode,QString* typeName,QString* valStr) const
    {
        QDomNodeList keyNodeLists = groupNode.elementsByTagName(CONFIG_KEY_NAME);
        const int count = keyNodeLists.size();
        for(int i=0;i<count;++i)
        {
            QDomElement node = keyNodeLists.at(i).toElement();
            if(node.isNull() || CONFIG_KEY_NAME!=node.tagName())
            {
                continue;
            }

            QString str = node.attribute(CONFIG_KEY_PROP_NAME_NAME,QString());
            if(str.isNull())
                continue;
            if(keyName == str)
            {
                if(keyNode)
                {
                    *keyNode = node;
                }
                if(typeName)
                {
                    *typeName = node.attribute(CONFIG_KEY_PROP_TYPE_NAME,QString());
                    if(typeName->isNull())
                    {
                        *typeName = "Invalid";
                    }
                }
                if(valStr)
                {
                    *valStr = node.text();
                }
                return true;
            }
        }
        return false;
    }



    QVariant getValue(const QString& groupName, const QString& keyName,const QVariant& defaultVal) const
    {
        QDomElement groupNode;
        if(!getGroupNode(groupName,&groupNode))
        {
            qDebug() << "!!!<" << groupName << ">" << " [" << keyName << "]" << "can not find group :" <<defaultVal;
            return defaultVal;
        }
        QString typeName;
        QString variantString;
        QDomElement keyNode;
        if(!getKeyNode(groupNode,keyName,&keyNode,&typeName,&variantString))
        {
            return defaultVal;
        }
        if(typeName == QVariant::typeToName(QVariant::StringList))
        {
            //字符串列表特殊处理
            QStringList strList;
            QDomNodeList listNodes = keyNode.elementsByTagName(CONFIG_KEY_LIST_NAME);
            for(int i=0;i<listNodes.size();++i)
            {
                strList << listNodes.at(i).toElement().text();
            }
            return strList;
        }
        return SAVariantCaster::stringToVariant(variantString,typeName);
    }

    void setValue(const QString &groupName, const QString &keyName, const QVariant &var)
    {
        QDomElement groupNode;
        if(!getGroupNode(groupName,&groupNode))
        {
            groupNode = m_doc.createElement(CONFIG_GROUP_NAME);
            groupNode.setAttribute(CONFIG_CONTENT_PROP_NAME,groupName);
            m_doc.documentElement().appendChild(groupNode);
        }
        QDomElement keyNode;
        if(!getKeyNode(groupNode,keyName,&keyNode,nullptr,nullptr))
        {
            //没有key
            keyNode = m_doc.createElement(CONFIG_KEY_NAME);
            keyNode.setAttribute(CONFIG_KEY_PROP_NAME_NAME,keyName);

        }
        else
        {
            //先删除所有的子节点
            while(keyNode.hasChildNodes())
            {
                keyNode.removeChild(keyNode.firstChild());
            }
        }
        keyNode.setAttribute(CONFIG_KEY_PROP_TYPE_NAME,QString(var.typeName()));
        if(QVariant::StringList == var.type())
        {
            //对于string list类型进行特殊处理，而不是保存为base64
            QStringList d = var.value<QStringList>();
            for(int i=0;i<d.size();++i)
            {
                QDomElement listNode = m_doc.createElement(CONFIG_KEY_LIST_NAME);
                listNode.setAttribute(CONFIG_KEY_PROP_TYPE_NAME,QVariant::typeToName(QVariant::String));
                listNode.appendChild(m_doc.createTextNode(d[i]));
                keyNode.appendChild(listNode);
            }
        }
        else
        {
            keyNode.appendChild(m_doc.createTextNode(SAVariantCaster::variantToString(var)));
        }
        groupNode.appendChild(keyNode);
        m_isDirty = true;
    }

    QStringList getGroupList() const
    {
        QStringList res;
        QDomNodeList groupsNodeLists = getAllGroupNodes();
        for(int i=0;i<groupsNodeLists.size();++i)
        {
            res.append(groupsNodeLists.at(i).nodeName());
        }
        return res;
    }

    QStringList getKeyList(const QString& groupName) const
    {
        QStringList res;
        QDomElement groupEle;
        if(!getGroupNode(groupName,&groupEle))
        {
            return res;
        }
        QDomNodeList keysNodeList = groupEle.childNodes();
        for(int i=0;i<keysNodeList.size();++i)
        {
            QDomElement ele = keysNodeList.at(i).toElement();
            if(ele.isNull() || CONFIG_KEY_NAME!=ele.tagName())
                continue;
            res.append(ele.attribute(CONFIG_KEY_PROP_NAME_NAME));
        }
        return res;
    }

    bool save(const QString& saveFilePath)
    {
        QFile file(saveFilePath);
        if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            return false;
        }
        QTextStream txt(&file);
        txt.setCodec("UTF-8");
        m_doc.save(txt, 4, QDomNode::EncodingFromTextStream);
        txt.flush();
        file.close();
        m_isDirty = false;
        return true;
    }
};





SAConfigXMLReadWriter::SAConfigXMLReadWriter(QObject *par):QObject(par)
	,d_ptr(new SAConfigXMLReadWriterPrivate(this))
{
    
}

SAConfigXMLReadWriter::SAConfigXMLReadWriter(const QString &filepath, QObject *par):QObject(par)
    ,d_ptr(new SAConfigXMLReadWriterPrivate(this,filepath))
{
    
}

SAConfigXMLReadWriter::~SAConfigXMLReadWriter()
{

}
///
/// \brief 设置配置文件
/// \param filePath
/// \return
///
bool SAConfigXMLReadWriter::setFilePath(const QString &filePath)
{
    return d_ptr->setCfgFile(filePath);
}
///
/// \brief 是否存在组判断
/// \param groupName 组名
/// \return 存在返回true
///
bool SAConfigXMLReadWriter::isHasGroup(const QString &groupName) const
{
    return d_ptr->getGroupNode(groupName,nullptr);
}
///
/// \brief 是否存在key
/// \param groupName 组名
/// \param keyName key名
/// \return 存在返回true
///
bool SAConfigXMLReadWriter::isHasKey(const QString &groupName, const QString &keyName) const
{
    QDomElement groupNode;
    if(!d_ptr->getGroupNode(groupName,&groupNode))
    {
        return false;
    }
    return d_ptr->getKeyNode(groupNode,keyName,nullptr,nullptr,nullptr);
}
///
/// \brief 获取值
/// \param groupName 组名
/// \param keyName key名
/// \param defaultVal 默认值
/// \return 返回对应的值，若没有，返回默认值
///
QVariant SAConfigXMLReadWriter::getValue(const QString &groupName, const QString &keyName, const QVariant &defaultVal) const
{
    return d_ptr->getValue(groupName,keyName,defaultVal);
}
///
/// \brief 获取值
/// \param namePath 通过/作为分割的路径，如“xxgroup/xxkey”.
/// 也就是
/// \code
/// getValue("xxgroup/xxkey",defaultVar);
/// \endcode
/// 等效于
/// \code
/// getValue（"xxgroup","xxkey",defaultVar));
/// \endcode
/// \param defaultVal 默认值，如果没有获取到，会返回此默认值
/// \return
///
QVariant SAConfigXMLReadWriter::getValue(const QString &namePath, const QVariant &defaultVal) const
{
    QString group = "";
    QString key;
    splitNamePath(namePath,group,key);
    return getValue(group,key,defaultVal);
}
///
/// \brief 设置值
/// \param groupName 组名
/// \param keyName key名
/// \param var 值
///
void SAConfigXMLReadWriter::setValue(const QString &groupName, const QString &keyName, const QVariant &var)
{
    d_ptr->setValue(groupName,keyName,var);
}
///
/// \brief 设置值
/// \param namePath 通过/作为分割的路径，如“xxgroup/xxkey”.
/// 也就是
/// \code
/// setValue（"xxgroup/xxkey",var);
/// \endcode
/// 等效于
/// \code
/// setValue（"xxgroup","xxkey",var));
/// \endcode
/// \param var
///
void SAConfigXMLReadWriter::setValue(const QString &namePath, const QVariant &var)
{
    QString group = "";
    QString key;
    splitNamePath(namePath,group,key);
    setValue(group,key,var);
}
///
/// \brief 获取所有组名
/// \return
///
QStringList SAConfigXMLReadWriter::getGroupNames() const
{
    return d_ptr->getGroupList();
}
///
/// \brief 获得keylist
/// \param groupName
/// \return
///
QStringList SAConfigXMLReadWriter::getKeyNames(const QString &groupName) const
{
    return d_ptr->getKeyList(groupName);
}
///
/// \brief 判断是否有改变
/// \return
///
bool SAConfigXMLReadWriter::isDirty() const
{
    return d_ptr->m_isDirty;
}

///
/// \brief 保存修改
///
/// 如果有打开文件，会保存到已有文件路径，如果没有打开文件，此函数不做任何动作
/// \return
///
bool SAConfigXMLReadWriter::save()
{
    if(d_ptr->m_cfgPath.isEmpty())
    {
        return false;
    }
    return saveAs(d_ptr->m_cfgPath);
}
///
/// \brief 另存为
/// \param filePath 文件路径
/// \return 成功返回true
///
bool SAConfigXMLReadWriter::saveAs(const QString &filePath)
{
    return d_ptr->save(filePath);
}

void SAConfigXMLReadWriter::splitNamePath(const QString &namePath, QString &groupName, QString &keyName)
{
    QStringList pl = namePath.split("/");
    if(1 == pl.size())
    {
        groupName = "";
        keyName = pl[0];
    }
    else if(2 == pl.size())
    {
        groupName = pl[0];
        keyName = pl[1];
    }
    else
    {
        groupName = "";
        keyName = namePath;
    }
}


