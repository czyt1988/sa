#ifndef DGLOBALLOG_H
#define DGLOBALLOG_H

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QMutex>
#include <QDebug>
#include <QSet>
#include <QFile>
#include <QElapsedTimer>


#include "SALibGlobal.h"

class SALogPrivate;
class SATextReadWriter;
class QThread;

#include <stdlib.h>
#include <stdio.h>
/// \def 增强型打印
#ifndef saPrint
#define saPrint() qDebug() << "["<<__FUNCTION__<<"]("<<__LINE__<<") ["<<__FILE__<<"]:\n"
#endif
/// \def 打印字节十六进制
#ifndef printByteArray
#define printByteArray(dataPtr,dataLen) \
    do{\
        const char* __c__char__ptr__ = (const char*)(dataPtr);\
        fprintf(stderr,"[");\
        for(int M__i=0;M__i<(dataLen);++M__i)\
        {\
            if(M__i % 10 == 0)\
            {\
                fprintf(stderr,"\n");\
                fprintf(stderr,">> [%d]-[%d]: ",M__i,M__i+9);\
            }\
            fprintf(stderr,"%02X,",(int)__c__char__ptr__[M__i]);\
        }\
        fprintf(stderr,"\n]");\
    }while(0)
#endif
/// \def 打印QByteArray的十六进制
#ifndef printQByteArray
#define printQByteArray(qBytearr)\
    do{\
        QByteArray __tmp_byte_array__ = qBytearr;\
        fprintf(stderr,"[");\
        for(int M__i=0;M__i<__tmp_byte_array__.size();++M__i)\
        {\
            if(M__i % 10 == 0)\
            {\
                fprintf(stderr,"\n");\
                fprintf(stderr,">> [%d]-[%d]: ",M__i,M__i+9);\
            }\
            fprintf(stderr,"%02X,",(int)__tmp_byte_array__[M__i]);\
        }\
        fprintf(stderr,"\n]");\
    }while(0)
#endif



///
/// \brief The SALogInfo class
///
class SALIB_EXPORT SALogInfo
{
public:
    friend class SALog;//DGlobalLog为其友元，此类只提供读接口，不提供写接口
public:
    SALogInfo();
    //获取日期
    QDateTime getDateTime() const;
    //获取内容
    QString getInfo() const;
    //获取函数名
    QString getFunctionName() const;
    //获取文件名
    QString getFileName() const;
    //获取行号
    int getLineNum() const;
    //分类类型
    int getType() const;

private:
    QDateTime m_dateTime;///< 时间
    QString m_info;///<信息
    QString m_functionName;///< 函数名
    QString m_fileName;///< 文件名
    int m_line;///< 行号
    int m_type;///< 分类类型
};
Q_DECLARE_METATYPE(SALogInfo)
///
/// \brief 全局日志记录类
///
/// 此类设计为单例模式
///
/// <H1>具体用法介绍：</H1>
///
/// <H2>思路</H2>
///
/// 日志分为2大类别，第一级可以称之为目录（Category）。
///
/// 第二级可以称之为类型（Type）。
///
/// log信息都包含目录和类型两个参数，用于对此log信息进行详细划分
///
/// 每个log信息由DLogInfo类进行管理，可以使用QList<SALogInfo> getLogInfo(int category)函数获取对应目录的所有信息
///
/// \see SALogInfo getLogInfo
///
/// <H2>使用方法：</H2>
///
/// 由于需要全局管理，因此，此类设计为单例模式，若要调用可以使用DGlobalLog::getInstance()函数进行
/// 获取全局唯一单例，此函数返回一个指针，可以通过调用其共有成员函数来进行日志的写入。
///
/// 另外，使用宏saAddLog(category,type,info,...)可以简化上述方法的代码量，例如：
/// \code
/// saLog->addLog(1,1001,"测试日志信息");
/// \endcode
/// 上述方法等价于
/// \code
/// DGlobalLog::getInstance()->addLog(1,1001,"测试日志信息");
/// \endcode
///
/// 其中函数第一个参数1代表的是目录id，第二个参数1001代表的是类型信息，这些信息需要提前进行注册，否则会无法写入文件或显示
///
/// 特别的，此宏函数可以使用可变参数，类似于printf，可以使用%d,%s等转义,如:
/// \code
/// int a =1;
/// double b = 2.1;
/// const char* str = "this is string";
/// DGlobalLog::getInstance()->addLog(1,1001,"测试日志信息：int:%d,double:%f,string:%s",a,b,str);
/// \endcode
///
/// <H2>写入日志方法</H2>
///
/// 使用addLog函数可以写入日志信息，日志信息第一个参数为分类，每个开发者必先给自己的日志信息分好类，
/// 且不要混淆分类，另外，最好不要定义小于0的分类，为此类的内部错误(分类为-1)也会记录到日志中，例如日志文件无法打开等错误。
///
/// addLog函数有多个重载版本，具体可见其函数说明。
///
/// 开发者需要记录日志时，只需在调用的地方引入此类的头文件，即可使用，例如：
///
/// 某cpp文件：xxx.cpp
/// \code
/// #include "DGlobalLog.h"
/// ......
/// ......
/// saLog->addLog(1,1001,"测试日志信息");//添加日志信息，日志的时间会自动记录调用的时间，但函数名，调用位置等无法记录
/// saLog->addLog(1,1001,"测试日志信息",__FILE__,__FUNCTION__,__LINE__);//记录日志信息，日志的时间会自动记录调用的时间
/// //同时调用此函数的文件名，函数名，及行号会被记录
/// saLog->addLog(1,1001,"测试日志信息",QTime::currentTime(),__FILE__,__FUNCTION__,__LINE__);//同上，但时间由用户显示输入
/// \endcode
///
/// 宏函数saAddLog是用于简化代码量设计的，上述代码可以简化为:
/// \code
/// saAddLog(1,1001,"测试日志信息");//等价于saLog->addLog(1001,"测试日志信息",__FILE__,__FUNCTION__,__LINE__);
/// saAddLogWithTime(1,1001,"测试日志信息",QTime::currentTime());//等价于saLog->addLog(1001,"测试日志信息",QTime::currentTime(),__FILE__,__FUNCTION__,__LINE__);
/// \endcode
/// \see addLog
///
/// <H2>debug的扩展</H2>
/// 在有些时候，既想记录日志，又想把日志信息打印到屏幕，可以如下操作：
/// \code
/// saAddLog(1,1001,"测试日志信息");
/// qDebug()<<"测试日志信息";
/// \endcode
///
/// 为了简化上述操作，成员函数debugLog可以在添加日志同时，并把日志信息打印在控制台，相当于
/// addLog后再调用qDebug
/// \code
/// saLog->debugLog(1,1001,"测试日志信息");//等价于addLog,然后qDebug
/// \endcode
///
/// 宏saDebugPrintStr是对上述方法的简化：
/// \code
/// saDebugPrintStr(1,1001,"测试日志信息");//等价于saLog->debugLog(1001,"测试日志信息");
/// \endcode
///
/// 特别的，宏saDebugPrint可以使用可变参数，类似于printf，可以使用%d,%s等转义
/// \code
/// saDebugPrint(1,1001,"saDebugPrint测试%d,%g,%s",123,123.22,"textString 测试字符,含逗号,含引号\"测试");
/// \endcode
/// \see debugLog
///
/// \date 2016-08-09
/// \author czy
///
class SALIB_EXPORT SALog : public QObject
{
    Q_OBJECT
    Q_ENUMS(LogType)
    Q_ENUMS(LogCategory)
private:
    explicit SALog();
    ~SALog();
    Q_DISABLE_COPY(SALog)
public:
    ///
    /// \brief 默认类别
    ///
    enum LogType{
        TypeNormal = 1///< 正常
        ,TypeWarning = 2///< 警告
        ,TypeError = 3///< 错误
        ,TypeCritical = 4///< 关键错误
        ,TypeDebug = 5///<
        ,TypeUser = 1000
    };

    ///
    /// \brief 日志信息列表的迭代器
    ///
    typedef QMap<QDateTime,SALogInfo>::iterator iterator;
    ///
    /// \brief 日志信息列表的const迭代器
    ///
    typedef QMap<QDateTime,SALogInfo>::const_iterator const_iterator;

    //单例模式获取内容
    static SALog* getInstance();
    //添加log信息
    /// \{
    void addLog(int type, const QString& info
                , const QString& file = QString("")
                , const QString& fun = QString("")
                , int line = -1);

    /// \}


    //注册类型,类型为类别的下级分类
    SALog* registerType(int type
                      ,const QString& typeName
                      ,bool isCanBeWrite = true
                      ,bool isCanBeShow = true);
    //获取注册了的类别
    QList<int> getRegisterType() const;
    QString getRegisterTypeName(int id) const;
    //获取日志信息
    QList<SALogInfo> getLogInfo() const;
    QList<SALogInfo> getLogInfo(QList<int> types);
    // 获取开始迭代器
    iterator begin();
    const_iterator begin() const;
    // 获取结束迭代器
    iterator end();
    const_iterator end() const;
    //清除某个分类的日志--仅仅内存清除
    int clear(int type);
    //日志的总数
    int getLogSize() const;
    //设置log文件保存路径
    void setLogFilePath(const QString& path);
    //设置文件编码
    void setCodec(const QString& codec);

    static QString printf();
    static QString printf(const char * cformat,...);
    static QString printf(const QString& strformat);

    //开始全局计时
    void startElapsedTimer();
    //返回耗时
    qint64 elapsed();
signals:
    ///
    /// \brief 添加日志信息发射的信号
    /// \param info 日志信息
    ///
    void addedLog(const SALogInfo& info);
    ///
    /// \brief 发生错误
    ///
    void occurError(const QString& errString);
    ///
    /// \brief 在写入log文件之前发射的信号，触发写日志线程
    /// \param str 写入一行日志文件的内容
    ///
    void appendLogInfoToFile(const QString& str);
private slots:
    void onWtiterErrorHappen(int code,const QString& errString);
private:
    // 获取最大的日志数量
    int getMaxLogNums() const;
    // 设置允许的最大日志数量，超过这个数量，会从头部进行删除
    void setMaxLogNums(int maxLog);
    //设置为默认保存路径
    void setDefaultSavePath();
    //在文件插入日志信息
    bool appendInFile(const SALogInfo& info);
    //删除所有分类信息，并释放内存
    void clear();
    //生成log文件的名字
    QString makefileName() const;
    //生成一个完整的文件路径
    QString makeFullPath() const;
private:
#if 0
    class CategoryProperty
    {
    public:
        QString name;///< 类型名称
        bool isCanBeWrite;///< 是否可以写入文件
        bool isCanBeShow;///< 是否可以显示
    };
    typedef CategoryProperty TypeProperty;
    typedef QMap<int,CategoryProperty> RegisterCategoryMap;
    typedef QMap<int,TypeProperty> RegisterTypeMap;

    QMap<QDateTime,SALogInfo> m_info;


    RegisterCategoryMap m_registerCategory;///< 可写的目录
    RegisterTypeMap m_registerType;///< 可写的类别

    QThread* m_textWriteThread;
    SATextReadWriter* m_readwrite;

    QString m_logFilePath;///< log文件的保存路径，可以通过setLogFilePath进行设置，默认为当前路径下的log文件夹
    QString m_logFileName;///< log文件的名字，此名字会在程序初始化时自动根据当前日期生成，不可修改
    QFile m_file;///< 日志文件io
    QString m_enCodec;///< 文件编码，为了适应win下的excel，默认为gb2312
    int m_maxLog;///< 定义每种log最大的数量，默认为10000
    bool m_isWriteFileImmediately;///< 是否马上写入文件，此为真时，没添加一个log会马上写入指定的文件位置
    bool m_enableSaveDebugLog;///< 是否允许保存debug log信息，默认为false，为false时，DefaultCategory::Debug对应的信息是不会写入内存，直接忽略
#endif
private:
    static SALog* s_instance;
    SALogPrivate* d_p;
    QElapsedTimer m_elapsedTimer;
};



//saLog宏，模仿qApp的全局变量管理方式 详细说明见cpp文件头部
#ifndef saLog
#define saLog SALog::getInstance()
#endif

//写入日志信息的宏函数 详细说明见cpp文件头部
#ifndef saAddLog
#define saAddLog(...)\
    saLog->addLog(SALog::TypeNormal,SALog::printf(__VA_ARGS__),__FILE__,__FUNCTION__,__LINE__);
#endif

//添加日志文件，同时把日志信息打印到屏幕 详细说明见cpp文件头部
#ifndef saDebug
#define saDebug(...)\
    do{\
       QString __str_global_log_tmp__ = SALog::printf(__VA_ARGS__);\
       qDebug()<<"["<<__FUNCTION__<<"]["<<__LINE__<<"] at (" <<__FILE__ << ")->  "<< __str_global_log_tmp__;\
       saLog->addLog(SALog::TypeDebug,__str_global_log_tmp__,__FILE__,__FUNCTION__,__LINE__);\
    }while(0)
#endif
//添加日志文件，同时把日志信息打印到屏幕 详细说明见cpp文件头部
#ifndef saError
#define saError(...)\
    do{\
       QString __str_global_log_tmp__ = SALog::printf(__VA_ARGS__);\
       qDebug()<<"["<<__FUNCTION__<<"]["<<__LINE__<<"] at (" <<__FILE__ << ")->  "<< __str_global_log_tmp__;\
       saLog->addLog(SALog::TypeError,__str_global_log_tmp__,__FILE__,__FUNCTION__,__LINE__);\
    }while(0)
#endif
//添加日志文件，同时把日志信息打印到屏幕 详细说明见cpp文件头部
#ifndef saWarning
#define saWarning(...)\
    do{\
       QString __str_global_log_tmp__ = SALog::printf(__VA_ARGS__);\
       qWarning()<<"["<<__FUNCTION__<<"]["<<__LINE__<<"] at (" <<__FILE__ << ")->  "<< __str_global_log_tmp__;\
       saLog->addLog(SALog::TypeWarning,__str_global_log_tmp__,__FILE__,__FUNCTION__,__LINE__);\
    }while(0)
#endif
//添加日志文件，同时把日志信息打印到屏幕 详细说明见cpp文件头部
#ifndef saCritical
#define saCritical(...)\
    do{\
       QString __str_global_log_tmp__ = SALog::printf(__VA_ARGS__);\
       qCritical()<<"["<<__FUNCTION__<<"]["<<__LINE__<<"] at (" <<__FILE__ << ")->  "<< __str_global_log_tmp__;\
       saLog->addLog(SALog::TypeCritical,__str_global_log_tmp__,__FILE__,__FUNCTION__,__LINE__);\
    }while(0)
#endif
//开始进行计时
#ifndef saStartElapsed
#define saStartElapsed(...)\
    do{\
        saLog->startElapsedTimer();\
        saDebug(__VA_ARGS__);\
    }while(0)
#endif
//结束计时
#ifndef saElapsed
#define saElapsed(...)\
    do{\
        QString __str_global_saElapsed__ = SALog::printf(__VA_ARGS__);\
        __str_global_saElapsed__ = QString("%1 elapsed: %2 ms").arg(__str_global_saElapsed__).arg(saLog->elapsed());\
        qDebug()<<"["<<__FUNCTION__<<"]["<<__LINE__<<"] at (" <<__FILE__ << ")->  "<< __str_global_saElapsed__;\
        saLog->addLog(SALog::TypeDebug,__str_global_saElapsed__,__FILE__,__FUNCTION__,__LINE__);\
    }while(0)
#endif




#endif // DGLOBALLOG_H
