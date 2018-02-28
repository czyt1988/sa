#include "SALog.h"
#include <QMutexLocker>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include "SACsvStream.h"
#include "SATextReadWriter.h"
#include "QThread"

///< 线程安全的读写锁
SALog* SALog::s_instance = NULL;
static QMutex s_logMutex;

#define LOCK QMutexLocker locker(&s_logMutex);
#define UNLOCK locker.unlock();
#define RELOCK locker.relock();


class SALogPrivate
{
public:
    SALogPrivate();
    class TypeProperty
    {
    public:
        QString name;///< 类型名称
        bool isCanBeWrite;///< 是否可以写入文件
        bool isCanBeShow;///< 是否可以显示
    };
    typedef QMap<int,TypeProperty> RegisterTypeMap;
    typedef QMap<QDateTime,SALogInfo> LogInfoList;
    typedef QMap<QDateTime,SALogInfo>::iterator iterator;
    typedef QMap<QDateTime,SALogInfo>::const_iterator const_iterator;


    QMap<QDateTime,SALogInfo> logInfos;
    RegisterTypeMap registerType;///< 可写的类别

    int maxLog;///< 定义每种log最大的数量，默认为10000
    QThread* textWriteThread;
    SATextReadWriter* readwrite;

    QString logFilePath;///< log文件的保存路径，可以通过setLogFilePath进行设置，默认为当前路径下的log文件夹
    QFile file;///< 日志文件io

    void appendLogInfo(const SALogInfo &info);
    int getLogInfoSize() const;
    QList<SALogInfo> getLogInfo() const;
    QList<SALogInfo> getLogInfo(QList<int> types) const;
    //注册类型,类型为类别的下级分类
    void registeType(int type
                      ,const QString& typeName
                      ,bool isCanBeWrite = true
                      ,bool isCanBeShow = true);
    QList<int> getRegisterType() const;
    QString getRegisterTypeName(int id) const;
    void clear();
    int clear(int type);
    // 获取开始迭代器
    iterator begin();
    const_iterator begin() const;
    // 获取结束迭代器
    iterator end();
    const_iterator end() const;
};

SALogPrivate::SALogPrivate()
    :maxLog(10000)
    ,textWriteThread(nullptr)
    ,readwrite(nullptr)
{

}

void SALogPrivate::appendLogInfo(const SALogInfo& info)
{
    logInfos.insert(info.getDateTime(),info);
    //判断总长是否超过设定
    while(getLogInfoSize() > maxLog)
    {
        LogInfoList::iterator beginIte = logInfos.begin();
        if(beginIte != logInfos.end())
        {
            logInfos.erase(beginIte);//总长度超过，删除第一个内容
        }
    }
}

int SALogPrivate::getLogInfoSize() const
{
    return logInfos.size();
}

QList<SALogInfo> SALogPrivate::getLogInfo() const
{
    QList<SALogInfo> res;
    const_iterator i = end();
    const_iterator ibegin = begin();
    if(i == ibegin)
    {
        //防止空
        return res;
    }
    for(--i;i != ibegin;--i)
    {
        res.append(i.value());
    }
    res.append(ibegin.value());
    return res;
}

QList<SALogInfo> SALogPrivate::getLogInfo(QList<int> types) const
{
    QList<SALogInfo> res;
    const_iterator i = end();
    const_iterator ibegin = begin();
    if(i == ibegin)
    {
        return res;
    }
    for(--i;i != ibegin;--i)
    {
        if(types.contains( i.value().getType()))
        {
            res.append(i.value());
        }
    }
    if(types.contains( ibegin.value().getType()))
    {
        res.append(ibegin.value());
    }
    return res;
}


void SALogPrivate::registeType(int type, const QString &typeName, bool isCanBeWrite, bool isCanBeShow)
{
    if(this->registerType.contains(type))
    {
        qDebug()<<"warning!! DGlobalLog::registerType have register same type:"<<type;
    }
    TypeProperty ty;
    ty.name = typeName;
    ty.isCanBeShow = isCanBeShow;
    ty.isCanBeWrite = isCanBeWrite;
    this->registerType.insert(type,ty);
}

QList<int> SALogPrivate::getRegisterType() const
{
    RegisterTypeMap::const_iterator ite = registerType.begin();
    RegisterTypeMap::const_iterator iteEnd =  registerType.end();
    QList<int> res;
    for(;ite != iteEnd;++ite)
    {
        res.append(ite.key());
    }
    return res;
}

QString SALogPrivate::getRegisterTypeName(int id) const
{
    RegisterTypeMap::const_iterator ite = registerType.find(id);
    if(ite != registerType.end())
    {
        return ite.value().name;
    }
    return QString();
}

void SALogPrivate::clear()
{
    logInfos.clear();
}

int SALogPrivate::clear(int type)
{
    iterator i = begin();
    int size(0);
    for(;i!=end();)
    {
        if(i.value().getType() == type)
        {
            i = logInfos.erase(i);//注意map的删除陷阱
            ++size;
        }
        else
        {
            ++i;
        }
    }
    return size;
}

SALogPrivate::iterator SALogPrivate::begin()
{
    return logInfos.begin();
}

SALogPrivate::const_iterator SALogPrivate::begin() const
{
    return logInfos.begin();
}

SALogPrivate::iterator SALogPrivate::end()
{
    return logInfos.end();
}

SALogPrivate::const_iterator SALogPrivate::end() const
{
    return logInfos.end();
}


SALogInfo::SALogInfo():m_dateTime(QDateTime::currentDateTime())
  ,m_info("")
  ,m_functionName("")
  ,m_fileName("")
  ,m_line(-1)
  ,m_type(-1)
{

}
///
/// \brief 获取日志对应的日期
/// \return
///
QDateTime SALogInfo::getDateTime() const
{
    return m_dateTime;
}
///
/// \brief 获取日志信息
/// \return
///
QString SALogInfo::getInfo() const
{
    return m_info;
}
///
/// \brief 获取函数名
/// \return
///
QString SALogInfo::getFunctionName() const
{
    return m_functionName;
}
///
/// \brief 获取文件名
/// \return
///
QString SALogInfo::getFileName() const
{
    return m_fileName;
}
///
/// \brief 获取行号
/// \return
///
int SALogInfo::getLineNum() const
{
    return m_line;
}
///
/// \brief 分类类型
/// \return
///
int SALogInfo::getType() const
{
    return m_type;
}





///
/// \def saLog
/// \brief saLog宏，模仿qApp的全局变量管理方式
///


/*!
 \def saAddLog(type,info,...)
 写入日志信息的宏函数

 此宏用于快速写入日志信息，在包含头文件#include "DGlobalLog.h"后即可使用

 此函数即可类似C的可变参数风格字符串输入，也支持Qt的字符串风格方式输入，建议使用Qt风格的字符串格式化方法

 使用例子如下：
 在需要记录日志的地方引入头文件#include "DGlobalLog.h"
 在要记录日志的位置：
 \code
 saAddLog(LOG_TYPE_XX,"这是一个日志记录，参数：%d,%f,%s",1,2.3,"字符串");
 \endcode
 或者使用Qt风格的格式化：
 \code
 saAddLog(LOG_TYPE_XX,QString("这是一个日志记录，参数：%1,%2,%3").arg(1).arg(2.3).arg("字符串"));
 \endcode
 这里还是建议使用Qt风格的字符串格式化，它会更加安全！

 上面代码即可向已经注册的类型写入日志。

 但是，需要注意的是，此类型必须已经注册，否则无法写入文件
 注册方式：
 \code
 #define LOG_TYPE_XX (10000)

 ......

 saLog->registerType(LOG_TYPE_XX,"日志类别名称");

 \endcode
 registerType函数还有两个默认变量，用于标定是否可写入文件，和是否可显示

 \param type 日志分类
 \param info 日志信息，日志信息自动包含__FILE__,__FUNCTION__,__LINE__三个宏，不需要再内容中体现
 \param ...  类似sprintf的字符串，支持C++标准支持的%d,%f,%u,%s,%g等转义
 \see saAddLogWithTime saDebugPrint DGlobalLog::registerType
*/

/*!
 \def saAddLogWithTime(type,info,time)
 添加日志文件

 和saAddLog类似，但可以指定时间，saAddLog默认会记录函数调用的时间
 调用示例：
 \code
 saAddLogWithTime(19956,QTime::currentTime(),"线程的log信息")
 \endcode
 或者在线程的某个重要位置声明时间变量：
 \code
 .........
 QTime time;
 do something
 time = QTime::currentTime();
 .....
 saAddLogWithTime(19956,time,"线程的log信息,%d,%f,%s",1,2.3,"字符串");
 \endcode

 上面代码即可向已经注册的类型写入日志。

 但是，需要注意的是，此类型必须已经注册，否则无法写入文件
 注册方式：
 \code
 #define LOG_TYPE_XX (10000)

 ......

 saLog->registerType(LOG_TYPE_XX,"日志类别名称");

 \endcode
 registerType函数还有两个默认变量，用于标定是否可写入文件，和是否可显示。

 \param type 日志分类
 \param info 日志信息，日志信息自动包含__FILE__,__FUNCTION__,__LINE__三个宏，不需要再内容中体现
 \param time 调用时间，建议传入QDateTime::currentDateTime() 或者 QTime::currentTime()
 \see saAddLogWithTime saDebugPrint DGlobalLog::registerType
*/

/*!
 \def saDebugPrint(type,info,...)
 添加日志文件，同时把日志信息打印到屏幕

 此函数相当于调用qDebug和saAddLog

 调用示例，如在DSystemSetDialog类中调用：
 \code
 saDebugPrint(1001,"测试:%d,%f,%s",123,123.222,"textString 测试字符");
 \endcode

 此时程序输出：
 \par
 ../CustomButton/SystemSet/DSystemSetDialog.cpp : DSystemSetDialog [ 158 ]-> "测试:123,123.222000,textString 测试字符"

 需要注意的是，此类型必须已经注册，否则无法写入文件
 注册方式：

 \code
 #define LOG_TYPE_XX (10000)

 ......

 saLog->registerType(LOG_TYPE_XX,"日志类别名称");

 \endcode

 registerType函数还有两个默认变量，用于标定是否可写入文件，和是否可显示，

 \param type 日志分类
 \param info 日志信息，类似sprintf的字符串，支持C++标准支持的%d,%f,%u,%s,%g等转义
 \see saAddLogWithTime saDebugPrint DGlobalLog::registerType
*/




SALog::SALog() : QObject()
  ,d_p(new SALogPrivate)
{
    d_p->textWriteThread = new QThread();
    d_p->readwrite = new SATextReadWriter();
    setDefaultSavePath();
    QString logPath = makeFullPath();
    d_p->readwrite->moveToThread(d_p->textWriteThread);
    d_p->readwrite->setFileName(logPath);

    connect(d_p->textWriteThread,SIGNAL(finished()),d_p->textWriteThread,SLOT(deleteLater()));
    connect(d_p->textWriteThread,SIGNAL(finished()),d_p->readwrite,SLOT(deleteLater()));
    connect(this,&SALog::appendLogInfoToFile,d_p->readwrite,&SATextReadWriter::appendLine);
    connect(d_p->readwrite,&SATextReadWriter::occurError,this,&SALog::onWtiterErrorHappen);
    registerType(TypeError,tr("error"),true,true);
    registerType(TypeWarning,tr("warning"),true,true);
    registerType(TypeNormal,tr("normal"),true,true);
    registerType(TypeCritical,tr("critical"),true,true);
    registerType(TypeDebug,tr("debug"),true,false);

    setCodec("GB18030");
    d_p->textWriteThread->start();
}

SALog::~SALog()
{

    clear();
}
///
/// \brief 获取全局唯一单例
/// \return 全局唯一单例指针
///
SALog *SALog::getInstance()
{
    static SALog s_log;
    return &s_log;
}
///
/// \brief 添加log信息
/// \param type 分类
/// \param info 内容
/// \param file 文件名，建议调用时传入__FILE__
/// \param fun 函数名，建议调用时传入__FUNCTION__
/// \param line 行号，建议调用时传入__LINE__
/// \note 此函数会自动记录调用的时间，
/// 另外此DGlobalLog可以显示的传入记录时间的函数
/// 可以看此函数的其它重载类型
///
/// 此函数线程安全
/// \see saAddLog
///
void SALog::addLog(int type, const QString &info, const QString &file, const QString &fun, int line)
{
    LOCK;
    SALogInfo content;
    content.m_info = info;
    content.m_fileName = file;
    content.m_functionName = fun;
    content.m_line = line;
    content.m_type = type;
    d_p->appendLogInfo(content);
    UNLOCK;
    appendInFile(content);
    emit addedLog(content);
}


///
/// \brief 注册类型，否则无法获知类型id对应的名称
/// \param type 类型id
/// \param typeName 类型名称
/// \param isCanBeWrite 是否可写入文件
/// \param isCanBeShow 是否可显示
///
SALog* SALog::registerType(int type, const QString &typeName, bool isCanBeWrite, bool isCanBeShow)
{
    d_p->registeType(type,typeName,isCanBeWrite,isCanBeShow);
    return this;
}
///
/// \brief 获取注册的类型
/// \return
///
QList<int> SALog::getRegisterType() const
{
    return d_p->getRegisterType();
}
///
/// \brief 根据id获取注册的类型的名字
/// \param id 类型id
/// \return 类型名字
///
QString SALog::getRegisterTypeName(int id) const
{
    return d_p->getRegisterTypeName(id);
}
///
/// \brief 获取内存的所有日志
/// \return
///
QList<SALogInfo> SALog::getLogInfo() const
{
    return d_p->getLogInfo();
}

///
/// \brief 获取多个分类目录下的所有日志信息
/// \param type 目录 id
/// \return 日志列表,日志列表是按照时间排序
///
QList<SALogInfo> SALog::getLogInfo(QList<int> types)
{
    return d_p->getLogInfo(types);
}
///
/// \brief 开始迭代器
///
/// \note 注意此操作无锁
/// \return
///
SALog::iterator SALog::begin()
{
    return d_p->begin();
}
///
/// \brief 开始迭代器
///
/// \note 注意此操作无锁
/// \return
///
SALog::const_iterator SALog::begin() const
{
    return d_p->begin();
}
///
/// \brief 获取结束迭代器
///
/// \note 注意此操作无锁
/// \return
///
SALog::iterator SALog::end()
{
    return d_p->end();
}
///
/// \brief 获取结束迭代器
///
/// \note 注意此操作无锁
/// \return
///
SALog::const_iterator SALog::end() const
{
    return d_p->end();
}

///
/// \brief 清除某个分类的日志信息的内容
/// \param type 日志类型
/// \return 返回清除的数量
/// \note 日志后，此分类的日志内容将为空
///
int SALog::clear(int type)
{
    LOCK;
    return d_p->clear(type);
}
///
/// \brief 获取日志的总数
/// \return
///
int SALog::getLogSize() const
{
    return d_p->getLogInfoSize();
}


///
/// \brief 设置log文件保存路径
///
/// \param path 保存路径
/// \return 如果路径不存在或者异常，将会返回false
///
void SALog::setLogFilePath(const QString &path)
{
    d_p->logFilePath = path;
    d_p->readwrite->setFileName(makefileName());
}
///
/// \brief 设置log文件保存路径为默认保存路径
///
void SALog::setDefaultSavePath()
{
    QString path = QDir::currentPath();
    path += "/log";
    if(!QFile::exists(path))
    {
        //路径不存在先创建
        QDir dir(path);
        if(!dir.mkpath(path))
        {
            return;
        }
    }
    d_p->logFilePath = path;
}
///
/// \brief 设置文件编码，默认为GB18030
/// \param codec 编码字符串，如UTF-8
///
void SALog::setCodec(const QString &codec)
{
    d_p->readwrite->setCodec(codec);
}



///
/// \brief 在文件中插入日志信息
/// \param type 日志类型
/// \param info 日志内容
/// \param isClose 是否写入完后关闭文件，默认不关闭
/// \return 成功写入返回true
///
bool SALog::appendInFile(const SALogInfo &info)
{
    QStringList csvSection;
    QString str = getRegisterTypeName(info.getType());
    if(str.isEmpty())
    {
        str = QString::number(info.getType());
    }
    csvSection << info.getDateTime().toString("yyyy-MM-dd HH:mm:ss")
               << str
               << info.getInfo()
               << info.getFileName()
               << info.getFunctionName()
               << QString::number(info.getLineNum())
               << QString::number(info.getType());
    str = SACsvStream::toCsvStringLine(csvSection);
    emit appendLogInfoToFile(str);
    return true;
}


///
/// \brief 删除所有分类信息，并释放内存
///
void SALog::clear()
{
    LOCK;
    d_p->clear();
}
///
/// \brief 生成log文件的名字
/// \return log文件名字，前面带/
///
QString SALog::makefileName() const
{
    QDateTime datetime = QDateTime::currentDateTime();
    return ("/LOG_" + datetime.toString("yyyyMMdd") + ".log");
}

QString SALog::makeFullPath() const
{
   return d_p->logFilePath + makefileName();
}


///
/// \brief 开始全局计时
///
void SALog::startElapsedTimer()
{
    if(!m_elapsedTimer.isValid())
    {
        m_elapsedTimer.start();
    }
    else
    {
        m_elapsedTimer.restart();
    }
}
///
/// \brief 返回耗时
/// \return ms
///
qint64 SALog::elapsed()
{
    return m_elapsedTimer.elapsed();
}

void SALog::onWtiterErrorHappen(int code, const QString &errString)
{
    Q_UNUSED(code);
    emit occurError(errString);
    qDebug() << "SALog:"<<errString;
}

///
/// \brief 获取允许最大的日志数目
///
/// 最大日志数是每个种类的最大日志数，若有10个种类，那么内存会保存10*maxLogNum的记录
/// \return 日志数
/// \see setMaxLogNums
///
int SALog::getMaxLogNums() const
{
    return d_p->maxLog;
}
///
/// \brief 设置最大日志数，最大日志数是软件运行时内存保存的每个种类的最大的日志信息
///
/// 若有10个种类，内存保存的最大日志内容将是10*maxLogNum
/// \param maxLog 最大日志数
///	\see getMaxLogNums
///
void SALog::setMaxLogNums(int maxLog)
{
    d_p->maxLog = maxLog;
}
QString SALog::printf()
{
    return "";
}
QString SALog::printf(const char * cformat,...)
{
    QString str;
    va_list ap;
    va_start(ap, cformat); // use variable arg list
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    int strlenth = strlen(cformat);
    size_t size = strlenth+100;
    char p[size];
    vsnprintf(p,size,cformat,ap);
    str = QString(p);
#else
    str = QString::asprintf(cformat,ap);
#endif
    va_end(ap);
    return str;
}

QString SALog::printf(const QString &strformat)
{
    return strformat;
}







