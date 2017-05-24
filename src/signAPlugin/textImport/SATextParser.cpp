#include "SATextParser.h"

SATextParser::SATextParser(QObject *par):QObject(par)
    ,m_file(new QFile(this))
 //,m_prePos(0)
{
    
}

SATextParser::SATextParser(const QString &fileName)
    :QObject(nullptr)
 //   ,m_prePos(0)
{
    m_file.reset(new QFile(fileName));
    m_txt.setDevice(m_file.get());
}

SATextParser::SATextParser(QFile *device):QObject(nullptr)
{
    m_file.reset(device);
    m_txt.setDevice(m_file.get());
}
///
/// \brief 打开文件，需确保已经setDevice
/// \param openMode
/// \return
///
bool SATextParser::open(QIODevice::OpenMode openMode)
{
    if(nullptr == m_file)
    {
        return false;
    }
    bool isOpen = m_file->open(openMode);
    if(isOpen)
    {
        m_txt.setDevice(m_file.get());
    }
    return isOpen;
}
///
/// \brief 打开文件，自动设置Device
/// \param filePath 文件路径
/// \param openMode 打开方式
/// \return
///
bool SATextParser::open(const QString &filePath, QIODevice::OpenMode openMode)
{
    if(nullptr == m_file)
    {
        setDevice(new QFile(filePath));
    }
    else
    {
        m_file->setFileName(filePath);
    }
    return open(openMode);
}
///
/// \brief 设置device
///
/// 时将拥有device的所有权
///
/// 如果不想删除原来的device，可以先\a takeDevice
/// \param device
///
void SATextParser::setDevice(QFile *device)
{
    m_txt.setDevice(device);
    m_file.reset(device);
}
///
/// \brief 放弃QFile的所有权
/// \return
///
QFile *SATextParser::takeDevice()
{
    m_txt.setDevice(nullptr);
    return m_file.release();
}

QString SATextParser::getFileName() const
{
    return (nullptr != m_file ? m_file->fileName() : QString());
}

void SATextParser::setFileName(const QString &fileName)
{
    if(m_file)
    {
        m_file->setFileName(fileName);
    }
    else
    {
        m_file.reset(new QFile(fileName));
    }
}

QString SATextParser::readLine(qint64 maxlen)
{
    //m_txt.pos();
    return m_txt.readLine(maxlen);
}

QStringList SATextParser::readAndSplitLine(const QString &split)
{
    return readLine().split (split);
}

bool SATextParser::isOpen() const
{
    if(nullptr == m_file)
    {
        return false;
    }
    return m_file->isOpen();
}

QFile *SATextParser::getFilePtr()
{
    return m_file.get();
}
///
/// \brief 读取所有行，按行把字符串用qstringlist保存
/// \return
///
QStringList SATextParser::readAllLine()
{
   m_txt.seek(0);
   QStringList res;
   while(!m_txt.atEnd())
   {
       res.append(m_txt.readLine());
   }
   return res;
}

void SATextParser::setCodec(QTextCodec *codec)
{
    m_txt.setCodec(codec);
}

QTextCodec *SATextParser::getCodec() const
{
    return m_txt.codec();
}

bool SATextParser::seek(qint64 pos)
{
    return m_txt.seek(pos);
}

qint64 SATextParser::pos() const
{
    return m_txt.pos();
}

bool SATextParser::isValid() const
{
    if(nullptr == m_file)
    {
        return false;
    }
    return m_txt.device() == static_cast<QIODevice*>(m_file.get());
}

QTextStream &SATextParser::getStream()
{
    return m_txt;
}

const QTextStream &SATextParser::getStream() const
{
    return m_txt;
}

