#include "SAConfigXMLReadWriter.h"
#include "SAConfig.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define CONFIG_FILE_NAME "saconfig.cfg"
#define CONFIG_SECTION_NAME "config"
#define DEFAULT_PROP_TYPE_NAME "defaultPropertyType"
QHash<QString,SAConfigXMLReadWriter::FUN_PTR> init_element2funptr_hash();
//函数指针散列初始化
QHash<QString,SAConfigXMLReadWriter::FUN_PTR> SAConfigXMLReadWriter::s_element2funptr = init_element2funptr_hash();

//设置字段DEFAULT_PROP_TYPE_NAME
void set_default_property_type(SAConfig* cfg,const QString& val)
{
    SAConfig::PropertyBrowserType type = static_cast<SAConfig::PropertyBrowserType>(val.toInt());
    cfg->setDefaultPropertySetDialogType(type);
}
//函数指针散列初始化
QHash<QString,SAConfigXMLReadWriter::FUN_PTR> init_element2funptr_hash()
{
    QHash<QString,SAConfigXMLReadWriter::FUN_PTR> funPtrs;
    funPtrs[DEFAULT_PROP_TYPE_NAME] = set_default_property_type;
    return funPtrs;
}



SAConfigXMLReadWriter::SAConfigXMLReadWriter(SAConfig *config, QObject *par):QObject(par)
  ,m_config(config)
{

}

QString SAConfigXMLReadWriter::getConfigXMLFileFullPath()
{
    QString str = SAConfig::getConfigFolderPath();
    return (str + QDir::separator() + CONFIG_FILE_NAME);
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
    QXmlStreamWriter xml(&file);
    xml.setCodec("UTF-8");//设置编码
    xml.setAutoFormatting(true);//自动换行，否则会写成一坨不好看
    xml.setAutoFormattingIndent(2);//设置一个tab的空格数，也就是缩进量，一般是2或4
    xml.writeStartDocument();//写入xml文档头<?xml version="1.0" encoding="UTF-8"?>
    xml.writeStartElement(CONFIG_SECTION_NAME);//每个start ele都要有write end
    {
        xml.writeTextElement(DEFAULT_PROP_TYPE_NAME,QString::number((int)m_config->getDefaultPropertySetDialogType()));
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
