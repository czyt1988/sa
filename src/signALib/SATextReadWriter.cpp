#include "SATextReadWriter.h"
#include <QMutexLocker>
#include <QTextStream>
#include <limits>
#include "SALog.h"
class SATextReadWriterPrivate
{
public:
    SATextReadWriterPrivate();
    QFile* file;
    int totalReadLineCount;///<设置总共读取的行数，当读取到达此行后会发射readAllComplete
    int readOnceLineCount;///< 设置一次读取的行数，当开始读取时达到这个行数后会发射readComplete，继续往下读取（没有超过m_totalReadLineCount的行数），再次达到m_readOnceLineCount设定值后发射readComplete，继续往下读取，直到到达m_totalReadLineCount的行数(发射readAllComplete)或文件的末尾(发射reachTextEnd和readAllComplete)
    int totalReadCharCount;///< 设置总共读取的字符数，当读取到达此字符数后会发射readAllComplete
    int readOnceCharCount;///< 设置一次读取的字符数，当开始读取时达到这个字符数后会发射readComplete，继续往下读取（没有超过m_totalReadCharCount的字符数），再次达到m_readOnceCharCount设定值后发射readComplete，继续往下读取，直到到达m_totalReadLineCount的字符数(发射readAllComplete)或文件的末尾(发射reachTextEnd和readAllComplete)
    QString codec;///<  编码
    SATextReadWriter::StringFun stringFun;///< 处理每次读取的函数指针，默认为nullptr
};

SATextReadWriterPrivate::SATextReadWriterPrivate()
    :file(new QFile)
    ,totalReadLineCount(-1)
    ,readOnceLineCount(500)
    ,totalReadCharCount(-1)
    ,readOnceCharCount(-1)
    ,codec("UTF-8")
    ,stringFun(nullptr)
{

}

SATextReadWriter::SATextReadWriter(QObject *parent):QObject(parent)
  ,d_p(new SATextReadWriterPrivate)
{
    connect(this,&SATextReadWriter::startedReadText
            ,this,&SATextReadWriter::onStartReadText);
}

SATextReadWriter::~SATextReadWriter()
{
    if(d_p)
    {
        delete d_p;
    }
}

void SATextReadWriter::setFileName(const QString &name)
{
    d_p->file->setFileName(name);
}
///
/// \brief 总共读取的行数，当读取到达此行后会发射readAllComplete
/// \return 返回-1代表没有设置，此时会一直度到文本末尾
///
int SATextReadWriter::getTotalReadLineCount() const
{
    return d_p->totalReadLineCount;
}
///
/// \brief 设置总共读取的行数，当读取到达此行后会发射readAllComplete
/// \param -1代表没有设置，此时会一直度到文本末尾
///
void SATextReadWriter::setTotalReadLineCount(int totalReadLineCount)
{
    d_p->totalReadLineCount = totalReadLineCount;
}
///
/// \brief 一次读取的行数，当开始读取时达到这个行数后会发射readComplete，
/// 继续往下读取（没有超过m_totalReadLineCount的行数），再次达到m_readOnceLineCount设定值后发射readComplete，
/// 继续往下读取，直到到达m_totalReadLineCount的行数(发射readAllComplete)或文件的末尾(发射reachTextEnd和readAllComplete)
/// \return 返回-1代表没有设置，此时会读取所有到文件末尾才发射一次readComplete
/// \note 默认为500
///
int SATextReadWriter::getReadOnceLineCount() const
{
    return d_p->readOnceLineCount;
}
///
/// \brief 设置一次读取的行数，但开始读取时达到这个行数后会发射readComplete，
/// 继续往下读取（没有超过m_totalReadLineCount的行数），再次达到m_readOnceLineCount设定值后发射readComplete，
/// 继续往下读取，直到到达m_totalReadLineCount的行数(发射readAllComplete)或文件的末尾(发射reachTextEnd和readAllComplete)
/// \param readOnceLineCount -1代表没有设置，此时会读取所有到文件末尾才发射一次readComplete
/// \note 默认为500
///
void SATextReadWriter::setReadOnceLineCount(int readOnceLineCount)
{
    d_p->readOnceLineCount = readOnceLineCount;
}
///
/// \brief 总共读取的字符数，当读取到达此字符数后会发射readAllComplete
/// \return 返回-1代表没有设置，此时会读取所有到文件末尾才发射一次readComplete
///
int SATextReadWriter::getReadOnceCharCount() const
{
    return d_p->readOnceCharCount;
}
///
/// \brief 设置总共读取的字符数，当读取到达此字符数后会发射readAllComplete
/// \param readOnceCharCount -1代表没有设置，此时会读取所有到文件末尾才发射一次readComplete
///
void SATextReadWriter::setReadOnceCharCount(int readOnceCharCount)
{
    d_p->readOnceCharCount = readOnceCharCount;
}


///
/// \brief 一次读取的字符数，当开始读取时达到这个字符数后会发射readComplete，
/// 继续往下读取（没有超过m_totalReadCharCount的字符数），再次达到m_readOnceCharCount设定值后发射readComplete，
/// 继续往下读取，直到到达m_totalReadLineCount的字符数(发射readAllComplete)或文件的末尾(发射reachTextEnd和readAllComplete)
/// \return 返回-1代表没有设置
///
int SATextReadWriter::getTotalReadCharCount() const
{
    return d_p->totalReadCharCount;
}
///
/// \brief 设置一次读取的字符数，当开始读取时达到这个字符数后会发射readComplete，
/// 继续往下读取（没有超过m_totalReadCharCount的字符数），再次达到m_readOnceCharCount设定值后发射readComplete，
/// 继续往下读取，直到到达m_totalReadLineCount的字符数(发射readAllComplete)或文件的末尾(发射reachTextEnd和readAllComplete)
/// \param -1代表没有设置
///
void SATextReadWriter::setTotalReadCharCount(int totalReadCharCount)
{
    d_p->totalReadCharCount = totalReadCharCount;
}
///
/// \brief 文件是否已经打开
/// \return
///
bool SATextReadWriter::isOpen() const
{
    return d_p->file->isOpen();
}
///
/// \brief 打开文件
/// \param mode QIODevice::OpenMode
/// \return
/// \see QFile::open
///
bool SATextReadWriter::open(QIODevice::OpenMode mode)
{
    if(d_p->file->open(mode))
        return true;
    int code = static_cast<SATextReadWriter::ErrorCode>(d_p->file->error());
    emit occurError(code,d_p->file->errorString());
    return false;
}
///
/// \brief 最后一次错误信息
/// \return
///
QString SATextReadWriter::getLastErrorString() const
{
    return d_p->file->errorString();
}
QString SATextReadWriter::getCodec() const
{
    return d_p->codec;
}

void SATextReadWriter::setCodec(const QString &codec)
{
    d_p->codec = codec;
}
///
/// \brief 处理每次读取的函数指针，默认为nullptr
/// \param fun
///
void SATextReadWriter::setStringFun(const SATextReadWriter::StringFun &fun)
{
    d_p->stringFun = fun;
}
///
/// \brief 开始读取文本
/// 可以在主线程直接调用而不阻塞
///
void SATextReadWriter::startReadText()
{
    m_stopRead = true;
    emit startedReadText();
}
///
/// \brief 插入文本
///
/// \param text
///
///
void SATextReadWriter::appendText(const QString &text)
{
    if(!isOpen())
    {
        if(!open(QIODevice::ReadWrite|QIODevice::Text))
        {
            return;
        }
    }
    QTextStream stream(d_p->file);
    stream<<text;
}
///
/// \brief 插入一行文本
/// \param text
///
void SATextReadWriter::appendLine(const QString &text)
{
    if(!isOpen())
    {
        if(!open(QIODevice::ReadWrite|QIODevice::Text))
        {
            return;
        }
    }
    QTextStream stream(d_p->file);
    stream<<text<<endl;
}
///
/// \brief Flushes any buffered data to the file. Returns true if successful; otherwise returns false.
/// \return
///
bool SATextReadWriter::flush()
{
    if(d_p->file->flush())
        return true;
    int code = static_cast<SATextReadWriter::ErrorCode>(d_p->file->error());
    emit occurError(code,d_p->file->errorString());
    return false;
}
///
/// \brief 传入一个操作QFile的函数指针，此函数指针用于操作内部保存的QFile
/// \param fun
///
void SATextReadWriter::operatorFile(std::function<void (QFile *)> fun)
{
    if(fun)
    {
        fun(d_p->file);
    }
}

void SATextReadWriter::onStartReadText()
{
    m_stopRead = false;
    if(!isOpen())
    {
        if(!open())
        {
            return;
        }
    }
    QTextStream stream(d_p->file);

    const int readOnceCharCount = d_p->readOnceCharCount > 0 ? d_p->readOnceCharCount : 0;
    const int totalReadCharCount = d_p->totalReadCharCount > 0 ? d_p->totalReadCharCount : std::numeric_limits<int>::max();
    const int readOnceLineCount = d_p->readOnceLineCount > 0 ? d_p->readOnceLineCount : std::numeric_limits<int>::max();
    const int totalReadLineCount = d_p->totalReadLineCount > 0 ? d_p->totalReadLineCount : std::numeric_limits<int>::max();
    //这里readOnceCharCount < totalReadCharCount,readOnceLineCount < totalReadLineCount,所有下面循环的判断中会先判断小的一方
    //也就是先判断readOnceCharCount，再判断totalReadCharCount，再判读readOnceLineCount，最后totalReadLineCount

    int currentReadLines=0;
    int totalReadLines = 0;
    StringFun& fun = d_p->stringFun;

    stream.setCodec(d_p->codec.toLocal8Bit().data());
    {
        QMutexLocker lock(&m_mutex);
        if(m_stopRead)
        {
            return;
        }
    }
    stream.seek(0);
    QString currentReadString;
    do{
        {
            QMutexLocker lock(&m_mutex);
            if(m_stopRead)
            {
                return;
            }
        }
        QString readString = stream.readLine(readOnceCharCount);
        currentReadString.append(readString);
        if(readString.size() != readOnceCharCount)
        {
            currentReadString.append("\n");
        }
        ++currentReadLines;
        ++totalReadLines;
        //1. 先判断字符，字符判断从总设定字符数判读开始，总的字符数没超越，判断单次字符数
        if(currentReadString.size() >= totalReadCharCount && totalReadCharCount>0)
        {
            //说明读取的总字节长度超过设定,截取正好的长度
            currentReadString = currentReadString.left(totalReadCharCount);
            if(fun)
            {
                currentReadString = fun(currentReadString);
            }
            emit readComplete(currentReadString,true);
            emit readAllComplete();
            return;
        }

        //说明读取的字符数超过了设定的单次读取的字符数限定，这时要把后段截取
        while(currentReadString.size() >= readOnceCharCount &&  readOnceCharCount>0)
        {
            QString tmp = currentReadString.left(readOnceCharCount);
            if(fun)
            {
                tmp = fun(tmp);
            }
            emit readComplete(tmp,true);
            currentReadString = currentReadString.right(currentReadString.size() - readOnceCharCount);
        }

        if(0 == currentReadString.size())
        {
            continue;
        }
        //2. 这时currentReadString的长度肯度小于readOnceCharCount和totalReadCharCount
        // 开始判读行数

        if(totalReadLines >= totalReadLineCount && totalReadLineCount>0)
        {
            //说明读取的行数超过设定
            if(fun)
            {
                currentReadString = fun(currentReadString);
            }
            emit readComplete(currentReadString,true);
            emit readAllComplete();
            return;
        }
        //达到单次设定的读取行数
        if(currentReadLines > readOnceLineCount && readOnceLineCount>0)
        {
            if(fun)
            {
                currentReadString = fun(currentReadString);
            }
            emit readComplete(currentReadString,true);
            currentReadString = "";
            currentReadLines = 0;
        }
    }while(!stream.atEnd());

    if(0 != currentReadString.size())
    {
        emit readComplete(currentReadString,true);
    }
    emit readAllComplete();
    emit reachTextEnd();
}


void SATextReadWriter::setStopRead(volatile bool stopRead)
{
    m_stopRead = stopRead;
}






