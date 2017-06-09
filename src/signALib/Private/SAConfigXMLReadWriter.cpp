#include "SAConfigXMLReadWriter.h"
#include "SAGlobalConfig.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define CONFIG_FILE_NAME "saconfig.cfg"
#define CONFIG_SECTION_NAME "config"
#define CONFIG_CONTENT_NAME "content"
#define CONFIG_KEY_NAME "key"
#define CONFIG_KEY_PROP_NAME_NAME "name"
#define CONFIG_KEY_PROP_TYPE_NAME "type"
#include <QByteArray>
#include <QDataStream>
#include <QBitArray>
#include <QBitmap>
SAConfigXMLReadWriter::SAConfigXMLReadWriter(SAGlobalConfig *config, QObject *par):QObject(par)
  ,m_config(config)
{

}

QString SAConfigXMLReadWriter::getConfigXMLFileFullPath()
{
    QString str = saConfig->getConfigPath();
    return (str + QDir::separator() + CONFIG_FILE_NAME);
}

QString SAConfigXMLReadWriter::variantToString(const QVariant &var)
{
    switch(var.type())
    {
    case QVariant::Invalid:
        return QString();
    case QVariant::BitArray:
    {
        QByteArray byte;
        QDataStream st(&byte,QIODevice::ReadWrite);
        QBitArray ba = var.toBitArray();
        st << ba;
        return QString(byte.toBase64());
    }
    case QVariant::Bitmap:
    {
        if(var.canConvert<QBitmap>())
        {
            QByteArray byte;
            QDataStream st(&byte,QIODevice::ReadWrite);
            QBitmap ba = var.value<QBitmap>();
            st << ba;
            return QString(byte.toBase64());
        }
        return QString();
    }
        ==
    }
    return QString();
}

QVariant SAConfigXMLReadWriter::stringTovariant(const QString &str, const QString &typeName)
{

}

void SAConfigXMLReadWriter::startRead()
{
    QFile file(getConfigXMLFileFullPath());
    if(!file.open(QIODevice::ReadOnly))
    {
        emit message(tr("can not open config file:\"%1\",because:%2").arg(file.fileName()).arg(file.errorString())
                     ,SA::ErrorMessage);
        emit readComplete(false);
        return;
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();
        if(xml.isStartElement())
        {
            if(xml.name() == CONFIG_SECTION_NAME)
            {
                readConfigSection(&xml);
            }
        }
    }
    emit readComplete(true);
}

void SAConfigXMLReadWriter::startWrite()
{
    QFile file(getConfigXMLFileFullPath());
    if(!file.open(QIODevice::WriteOnly))
    {
        emit message(tr("can not open config file:\"%1\",because:%2").arg(file.fileName()).arg(file.errorString())
                     ,SA::ErrorMessage);
        emit writeComplete(false);
        return;
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
    emit writeComplete(true);
}

void SAConfigXMLReadWriter::readConfigSection(QXmlStreamReader* xml)
{
    while(!xml->atEnd() && !xml->hasError())
    {
        xml->readNext();
        if(xml->isStartElement())
        {
            FUN_PTR fun = s_element2funptr.value(xml->name().toString(),nullptr);
            QString val = xml->readElementText();
            if(fun)
            {
                fun(m_config,val);
            }
        }
    }
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
    for(int i=0;i<keySize;++i)
    {
        writeKey(&xml,keys[i],m_config->getKey(content,keys[i]));
    }
    xml->writeEndElement();
}

void SAConfigXMLReadWriter::writeKey(QXmlStreamWriter *xml, const QString &key, const QVariant &var)
{
    xml->writeStartElement(CONFIG_KEY_NAME);//每个start ele都要有write end
    xml->writeAttribute(CONFIG_KEY_PROP_NAME_NAME,key);
    xml->writeAttribute(CONFIG_KEY_PROP_TYPE_NAME,var.typeName());

    xml->writeEndElement();
}