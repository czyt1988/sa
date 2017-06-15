#include "SAConfigXMLReadWriter.h"
#include "SAGlobalConfig.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "SAVariantCaster.h"
#include "SAVariantCaster.h"
#define CONFIG_FILE_NAME "saconfig.cfg"
#define CONFIG_SECTION_NAME "config"

#define CONFIG_CONTENT_NAME "content"
#define CONFIG_CONTENT_PROP_NAME "name"

#define CONFIG_KEY_NAME "key"
#define CONFIG_KEY_PROP_NAME_NAME "name"
#define CONFIG_KEY_PROP_TYPE_NAME "type"

SAConfigXMLReadWriter::SAConfigXMLReadWriter(SAGlobalConfig *config, QObject *par):QObject(par)
  ,m_config(config)
{

}

QString SAConfigXMLReadWriter::getConfigXMLFileFullPath()
{
    QString str = m_config->getConfigPath();
    return (str + QDir::separator() + CONFIG_FILE_NAME);
}


bool SAConfigXMLReadWriter::startWrite()
{
    QFile file(getConfigXMLFileFullPath());
    if(!file.open(QIODevice::WriteOnly))
    {
        emit readWriteComplete(FileOpenError);
        return false;
    }
    QList<QString> contents = m_config->getContentList();
    const int contentsSize = contents.size();
    QXmlStreamWriter xml(&file);
    xml.setCodec("UTF-8");//设置编码
    xml.setAutoFormatting(true);//自动换行，否则会写成一坨不好看
    xml.setAutoFormattingIndent(2);//设置一个tab的空格数，也就是缩进量，一般是2或4
    xml.writeStartDocument();//写入xml文档头<?xml version="1.0" encoding="UTF-8"?>
    xml.writeStartElement(CONFIG_SECTION_NAME);//每个start ele都要有write en
    for(int i=0;i<contentsSize;++i)
    {
        writeContent(&xml,contents[i]);
    }
    xml.writeEndElement();
    if(xml.hasError())
    {
        emit readWriteComplete(WriteError);
        return false;
    }
    else
    {
        emit readWriteComplete(WriteComplete);
    }
    return true;
}


void SAConfigXMLReadWriter::writeContent(QXmlStreamWriter *xml, const QString &content)
{
    QList<QString> keys = m_config->getKeyList(content);
    const int keySize = keys.size();
    if(keySize <= 0)
    {
        return;
    }
    xml->writeStartElement(CONFIG_CONTENT_NAME);//每个start ele都要有write end
    xml->writeAttribute(CONFIG_CONTENT_PROP_NAME,content);
    for(int i=0;i<keySize;++i)
    {
        writeKey(xml,keys[i],m_config->getValue(content,keys[i]));
    }
    xml->writeEndElement();
}

void SAConfigXMLReadWriter::writeKey(QXmlStreamWriter *xml, const QString &key, const QVariant &var)
{
    xml->writeStartElement(CONFIG_KEY_NAME);//每个start ele都要有write end
    xml->writeAttribute(CONFIG_KEY_PROP_NAME_NAME,key);
    xml->writeAttribute(CONFIG_KEY_PROP_TYPE_NAME,var.typeName());
    xml->writeCharacters(SAVariantCaster::variantToString(var));
    xml->writeEndElement();
}

bool SAConfigXMLReadWriter::startRead()
{
    QFile file(getConfigXMLFileFullPath());
    if(!file.open(QIODevice::ReadOnly))
    {
        emit readWriteComplete(FileOpenError);
        return false;
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();
        if(xml.isStartElement())
        {
            if(xml.name() == CONFIG_SECTION_NAME)
            {
                readContent(&xml);
            }
        }
    }
    if(xml.hasError())
    {
        emit readWriteComplete(ReadError);
        return false;
    }
    else
    {
        emit readWriteComplete(ReadComplete);
    }
    return true;
}

void SAConfigXMLReadWriter::readContent(QXmlStreamReader* xml)
{
    while(!xml->atEnd() && !xml->hasError())
    {
        xml->readNext();
        if(xml->isStartElement())
        {
            if(xml->name() == CONFIG_CONTENT_NAME)//<content>
            {
                QStringRef att = xml->attributes().value(CONFIG_CONTENT_PROP_NAME);
                if(!att.isEmpty())
                {
                    readKey(xml,att.toString());
                }
            }
        }
    }
}

void SAConfigXMLReadWriter::readKey(QXmlStreamReader *xml, const QString &contentName)
{
    while(!xml->atEnd() && !xml->hasError())
    {
        xml->readNext();
        if(xml->isStartElement())
        {
            if(xml->name() == CONFIG_KEY_NAME)//<key>
            {
                QXmlStreamAttributes att = xml->attributes();
                QStringRef ref = att.value(CONFIG_KEY_PROP_NAME_NAME);
                if(ref.isEmpty())
                    continue;
                QString name = ref.toString();

                ref = att.value(CONFIG_KEY_PROP_TYPE_NAME);
                if(ref.isEmpty())
                    continue;
                QString type = ref.toString();
                QString data = xml->readElementText();
                m_config->setValue(contentName,name,SAVariantCaster::stringToVariant(data,type));
            }
        }
    }
}
