#include "QTDMFile.h"
#include <vector>
#include <memory>
#include <QFileInfo>
#include <QDir>
QTDMFile::QTDMFile():m_file(nullptr)
  ,m_isAutoCloseGroup(false)
  ,m_lastErrCode(DDC_NoError)
{
}

QTDMFile::QTDMFile(DDCFileHandle file):m_isAutoCloseGroup(false)
,m_lastErrCode(DDC_NoError)
{
    setHandle (file);
}

QTDMFile::QTDMFile(QString strFile,bool isRedOnly):m_isAutoCloseGroup(false)
,m_lastErrCode(DDC_NoError)
{
    openFile (strFile,isRedOnly);
}

QTDMFile::~QTDMFile()
{
    if(m_isAutoCloseGroup)
        closeGroup();
}

void QTDMFile::setHandle(DDCFileHandle file)
{
    m_file = file;
    getGroupHandlesInFile();
}

DDCFileHandle QTDMFile::getHandle() const
{
    return m_file;
}

unsigned int QTDMFile::getGroupNums() const
{
    unsigned int groupNum = 0;
    DDC_GetNumChannelGroups(m_file, &groupNum);
    return groupNum;
}

QList<DDCChannelGroupHandle> QTDMFile::getGroupHandles() const
{
    return m_groupHandles;
}

QList<QTDMGroup*> QTDMFile::getGroups() const
{
    QList<QTDMGroup*> groups;
    for(int i=0;i<m_groups.size ();++i)
    {
        groups.push_back (m_groups[i].get () );
    }
    return groups;
}

QString QTDMFile::getName() const
{
    return getStringProperty(DDC_FILE_NAME);
}

QString QTDMFile::getDescription() const
{
    return getStringProperty(DDC_FILE_DESCRIPTION);
}

QString QTDMFile::getTitle() const
{
    return getStringProperty(DDC_FILE_TITLE);
}

QString QTDMFile::getAuthor() const
{
    return getStringProperty(DDC_FILE_AUTHOR);
}

bool QTDMFile::openFile(QString strFile, bool readOnly)
{
    QFileInfo temDir(strFile);
    QString strSuffix = temDir.suffix();
    strSuffix = strSuffix.toLower();
    if(QStringLiteral("tdms") == strSuffix)
    {
        return openTdms(strFile,readOnly);
    }
    else if(QStringLiteral("tdm") == strSuffix)
    {
        return openTdm(strFile,readOnly);
    }
    return false;
}

void QTDMFile::closeFile()
{
    DDC_CloseFile(m_file);
}

bool QTDMFile::isFileOpen() const
{
    return m_file != nullptr;
}

bool QTDMFile::isAutoCloseGroup() const
{
    return m_isAutoCloseGroup;
}

void QTDMFile::setAutoCloseGroup(bool enable)
{
    m_isAutoCloseGroup = enable;
}

void QTDMFile::getGroupHandlesInFile()
{
    m_groupHandles.clear ();
    m_groups.clear ();
    unsigned int groupNums = getGroupNums();
    if(groupNums <= 0)
        return;
    std::vector<DDCChannelGroupHandle> pGroupHandles(groupNums,nullptr);
    DDC_GetChannelGroups (m_file, pGroupHandles.data(), groupNums);
    //转移
    for(unsigned int i=0;i<pGroupHandles.size ();++i){
        m_groupHandles.push_back (pGroupHandles[i]);
        m_groups.push_back (makeGroupPtr (pGroupHandles[i]));
    }
}

QString QTDMFile::getStringProperty(const char* propertyName) const
{
    unsigned int length(0);
    //获取文件属性 - DDC_FILE_NAME
    int errCode = DDC_GetFileStringPropertyLength (m_file, propertyName, &length);
    if(errCode != DDC_NoError)
    {
        return QString();
    }
    std::unique_ptr<char> property;
    length = (8)*(length + 1);//让它长度有足够
    property.reset (new char[length]);
    memset(property.get (),0,sizeof(char)*length);
    errCode = DDC_GetFileProperty (m_file, propertyName, property.get (), length);
    if(errCode != DDC_NoError)
    {
        return QString();
    }
    QString str = QString::fromLocal8Bit(property.get ());
    return str;
}

QTDMFile::QTDMGroupPtr QTDMFile::makeGroupPtr(DDCChannelGroupHandle handle) const
{
    return std::make_shared<QTDMGroup>(handle);
}

bool QTDMFile::openTdms(QString strFile, bool readOnly)
{
    if(m_file != nullptr)
        DDC_CloseFile(m_file);
    m_file = nullptr;
    QString path = QDir::toNativeSeparators(strFile);
    m_lastErrCode = DDC_OpenFileEx (path.toLocal8Bit().constData(), "TDMS",readOnly, &m_file);
    if(m_lastErrCode < 0)
        return false;
    getGroupHandlesInFile();
    return true;
}

bool QTDMFile::openTdm(QString strFile, bool readOnly)
{
    if(m_file != nullptr)
        DDC_CloseFile(m_file);
    m_file = nullptr;
    QString path = QDir::toNativeSeparators(strFile);
    m_lastErrCode = DDC_OpenFileEx (path.toLocal8Bit().constData(), "TDM",readOnly, &m_file);
    if(m_lastErrCode < 0)
        return false;
    getGroupHandlesInFile();
    return true;
}

void QTDMFile::closeGroup()
{
    int size = m_groupHandles.size ();
    for(int i=0;i<size;++i)
    {
        m_groups[i]->closeChannel ();
        DDC_CloseChannelGroup (m_groupHandles[i]);
    }
    m_groupHandles.clear ();
    m_groups.clear ();
}
