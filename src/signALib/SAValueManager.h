#ifndef SAVALUEMANAGER_H
#define SAVALUEMANAGER_H
#include "SALibGlobal.h"
#include "SAGlobals.h"
#include "SAData.h"
//--stl
#include <memory>
//--qt
#include <QObject>
#include <QList>
#include <QSet>
class QFile;
///
/// \brief sa的变量管理类
/// 此类设计为单例模式，不允许拷贝，通过getInstance();获取实例
/// 或者使用宏saValueManager直接获取实例.
/// 只要在需要使用到变量的地方#Include "SAValueManager.h"即可使用sa变量管理的相关功能，
/// 可以向变量管理器中保存所有SAAbstractDatas系列的变量
/// 添加数据的示例：
/// \code
/// SAVectorDouble* d = new SAVectorDouble("valueTest");
/// for(int i=0;i<1000;++i)
/// {
/// 	d->append(sin((double)i));
/// }
///	saValueManager->addData(d);
///	\endcode
///
///	\see SAValueModel SAValueManager
/// \author czy
/// \date 2016-10-28
///
class SALIB_EXPORT SAValueManager : public QObject
{
    Q_OBJECT
private:
    explicit SAValueManager();
    Q_DISABLE_COPY(SAValueManager)
public:
    ~SAValueManager();
    //获取对象
    static SAValueManager* getInstance();
    ///
    /// \brief 用于生成数据的工厂
    /// 生成的数据并不会加入管理系统中需要调用addData
    /// \return 数据指针
    ///
    template<typename DataType>
    static DataType* makeData_s()
    {
        return new DataType;
    }
    template<typename DataType,typename ARG1>
    static DataType* makeData_s(ARG1 arg1)
    {
        return new DataType(arg1);
    }
    template<typename DataType,typename ARG1,typename ARG2>
    static DataType* makeData_s(ARG1 arg1,ARG2 arg2)
    {
        return new DataType(arg1,arg2);
    }
    template<typename DataType,typename ARG1,typename ARG2,typename ARG3>
    static DataType* makeData_s(ARG1 arg1,ARG2 arg2,ARG3 arg3)
    {
        return new DataType(arg1,arg2,arg3);
    }
    template<typename DataType,typename ARG1,typename ARG2,typename ARG3,typename ARG4>
    static DataType* makeData_s(ARG1 arg1,ARG2 arg2,ARG3 arg3,ARG4 arg4)
    {
        return new DataType(arg1,arg2,arg3,arg4);
    }
    template<typename DataType,typename ARG1,typename ARG2,typename ARG3,typename ARG4,typename ARG5>
    static DataType* makeData_s(ARG1 arg1,ARG2 arg2,ARG3 arg3,ARG4 arg4,ARG5 arg5)
    {
        return new DataType(arg1,arg2,arg3,arg4,arg5);
    }

#if 1
    template<typename DataType, typename... _Args>
    static std::shared_ptr<DataType> makeData(_Args&&... __args)
    {
        return  std::make_shared<DataType>(std::forward<_Args>(__args)...);
    }
    //创建一个引用数据,引用和原始数据有一样的功能，但引用删除原始不删除，原始删除引用都会删除
    static std::shared_ptr<SADataReference> makeRefData(SAAbstractDatas* data);

    template<typename DATA_TYPE>
    static std::shared_ptr<SAAbstractDatas> castPointToBase(const std::shared_ptr<DATA_TYPE>& r)
    {
        return std::static_pointer_cast<SAAbstractDatas>(r);
    }

#endif
    ///
    /// \brief 删除数据，注意此函数用于使用makeData生成，但没有添加到变量管理系统的指针删除
    /// 添加到变量管理系统的指针需要使用SAValueManager::deleteData(SAABstractData*)函数
    ///
    template<typename DataType>
    static void deleteData_s(DataType* p)
    {
        if(nullptr != p)
        {
            delete p;
        }
    }

    //清空所有数据
    void clear();
    //根据id查找
    SAAbstractDatas* findData(int id) const;
    //根据名字查找
    SAAbstractDatas* findData(const QString& name) const;
    //根据名字查找
    //SAAbstractDatas* findData(const QModelIndex index) const;
    //根据添加顺序获取变量的内存地址
    SAAbstractDatas* at(int index) const;
    //
    int indexOf(SAAbstractDatas* dataPtr) const;
    //根据类型查找
    //QList<SAAbstractDatas*> findDataByType(SA::DataType type) const;
    //变量数量
    int count() const;
    //判断是否是有效名字
    static bool isValidName(const QString& name);
    //判断变量的名字是否有冲突，无冲突返回true
    bool isNameNotConflict(const QString& name,const SAAbstractDatas* ignoreDataPtr = nullptr) const;
    //删除数据
    void deleteDatas(QList<SAAbstractDatas*> datas);
    //删除数据
    void deleteData(SAAbstractDatas* datas);
    //更改变量名字，变量如果不通过SAValueManager更改名字，将无法获得管理权
    bool renameData(SAAbstractDatas* data, const QString& name);
    //保存数据到文件系统
    int saveAs(const SAAbstractDatas* data, const QString& path,QFile* file);
    //保存数据到文件系统
    int saveAs(const QString& path);
    //从文件系统中加载数据
    int load(const QString& path);
    //判断数据是否处于管理状态
    bool isDataInManager(const SAAbstractDatas* data) const;
    //从任意数据转换为double vector，如果可以转换的话
    static std::shared_ptr<SAVectorDouble> createVectorDoubleFromData(const SAAbstractDatas *data);
    // 根据类型，创建数据
    std::shared_ptr<SAAbstractDatas> createDataByType(SA::DataType type);
public:
    ///
    /// \name 对于1维数据的操作
    /// \{
    ///
    QList<SAAbstractDatas*> getVectorDataPtrs() const;
    bool getVectorData(int id,QVector<double>& datas);
    bool getVectorData(SAAbstractDatas* dataPtr, QVector<double>& datas);
    /// \}
    ///

    /// \name 变量添加操作
    /// \{
    ///
    //添加变量，返回变量的id
    void addData(SAAbstractDatas* data);
    void addData(std::shared_ptr<SAAbstractDatas> data);
     //添加变量，返回变量的id
    void addDatas(QList<SAAbstractDatas*> datas);
    void addDatas(QList<std::shared_ptr<SAAbstractDatas>> datas);
    /// \}
    ///
signals:
    ///
    /// \brief 信息，对于一些操作的错误等内容，通过message信号发射，信息的类型通过type进行筛选
    /// \param des 信息的内容
    /// \param type 信息的类型
    ///
    void messageInformation(const QString& des,SA::MeaasgeType type);
    ///
    /// \brief 变量名字变更触发的信号
    /// \param data 变量指针
    /// \param oldName 旧的名字
    ///
    void dataNameChanged(SAAbstractDatas* data,const QString& oldName);
    ///
    /// \brief 数据被添加触发的信号
    /// \param data 数据指针
    /// \note load函数不会触发此信号
    /// \see dataDeleted
    ///
    void dataAdded(const QList<SAAbstractDatas*>& datas);
    ///
    /// \brief 数据被删除触发的指针
    /// \param 被删除数据对应的指针的值
    /// \note 此指针不能做任何操作，仅用于参考数值
    /// \note clear函数不会触发此信号，需要观察dataClear
    /// \see dataClear dataAdded
    ///
    //void dataDeleted(const QList<quintptr>& dataBeDeletedPtr);
    void dataDeleted(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    ///
    /// \brief 数据被清除时触发的信号
    ///
    void dataClear();
private:
    //自动转换为正确的变量名，若变量名重复或冲突，会自动把输入的名字后加上其他内容改为正确的变量名
    bool toCorrectName(SAAbstractDatas* data);
    //加载一个sad文件
    std::shared_ptr<SAAbstractDatas> loadSad(const QString &filePath);
private:

    ///
    /// \brief 保存指针和一些额外信息的容器
    ///
    class PointerContainer
    {
    public:
        PointerContainer();
        ~PointerContainer();
        void append(std::shared_ptr<SAAbstractDatas> data,const QString& dataFile = QString());
        void append(SAAbstractDatas* data,const QString& dataFile = QString());
        //设置数据文件的文件映射路径
        void setDataFilePath(SAAbstractDatas* data,const QString& dataFile);
        //根据索引查找
        SAAbstractDatas* at(int index) const;
        //根据指针找到索引
        int indexOf(SAAbstractDatas* dataPtr) const;
        //根据id查找
        SAAbstractDatas* findData(int id) const;
        //根据名字查找
        SAAbstractDatas* findData(const QString& name) const;
        //获取每个数据对应的文件路径
        QString getDataFilePath(const SAAbstractDatas* dataPtr) const;
        //清空所有数据
        void clear(bool deletePtr = true);
        //获取变量的数量
        int count() const;
        //删除数据指针内存同时在容器移除
        void deleteData(SAAbstractDatas* data);
        //变量改名
        bool changDataName(SAAbstractDatas* data,const QString& name);
        //判断是否已存在名字
        bool isHaveDataName(const QString& name) const;
        //获取所有的一维向量数据
        QList<SAAbstractDatas*> getVectorDataPtrs() const;
        //判断数据是否处于管理状态
        bool isDataInManager(const SAAbstractDatas* data) const;

    private:
        QList<SAAbstractDatas*> m_ptrList;///< 总指针队列
        QMap <SAAbstractDatas*,int> m_ptrListSet;///< 用于快速判断是否存在

        QList<SAAbstractDatas*> m_ptrNormalList;///< 指针队列,此队列通过普通指针添加
        QMap <SAAbstractDatas*,int> m_ptrNormalListSet;
        QList<std::shared_ptr<SAAbstractDatas> > m_smrPtrList;///< 指针队列,此队列通过智能指针添加
        QMap <std::shared_ptr<SAAbstractDatas>,int> m_smrPtrListSet;

        QMap<int,SAAbstractDatas*> m_id2data;///<记录变量的id和内存地址，通过此可以快速查找
        QMap<QString,SAAbstractDatas*> m_dataName2DataPtr;///< 记录变量名对应的数据类型

        QMap<QString,SAAbstractDatas*> m_dataFileName2DataPtr;///< 记录数据文件路径
        QMap<SAAbstractDatas*,QString> m_dataPtr2DataFileName;///< 记录数据指针对应的保存文件名

    };
    PointerContainer m_ptrContainer;///< 此数据结构的指针管理容器
    static SAValueManager* s_instance;
    QString m_lastSaveDataFolder;///< 记录最后保存的目录
};
#ifndef saValueManager
#define saValueManager SAValueManager::getInstance()
#endif
#endif // SAVALUEMANAGER_H
