#ifndef SAVALUEMANAGER_H
#define SAVALUEMANAGER_H
#include "SALibGlobal.h"
#include "SAData.h"
//--stl
#include <memory>
//--qt
#include <QObject>
#include <QList>
#include <QSet>
#include <QtWidgets/QUndoStack>
class QFile;
class PointerContainerPrivate;
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
    typedef std::shared_ptr<SAAbstractDatas> IDATA_PTR;

    ~SAValueManager();
    //获取对象
    static SAValueManager* getInstance();
    //用于生成数据的工厂函数
    template<typename DataType, typename... _Args>
    static std::shared_ptr<DataType> makeData(_Args&&... __args);
    // 根据类型，创建数据
    static IDATA_PTR makeDataByType(SA::DataType type);
    //创建一个引用数据,引用和原始数据有一样的功能，但引用删除原始不删除，原始删除引用都会删除
    static IDATA_PTR makeRefData(SAAbstractDatas* data);
    //把智能指针列表转换为普通指针列表
    static QList<SAAbstractDatas*> toNormalPtrList(const QList<IDATA_PTR>& ptrs);
    ///
    /// \brief 把智能指针转换为基类智能指针
    /// \param r 数据的智能指针
    /// \return 基类智能指针
    ///
    template<typename DATA_TYPE>
    static IDATA_PTR castPointToBase(const std::shared_ptr<DATA_TYPE>& r)
    {
        return std::static_pointer_cast<SAAbstractDatas>(r);
    }

    //根据数据类型获取对应的图标
    static QIcon getDataIcon(int dtype);
    //注册类型对应的图标
    static void registerDataTypeIcon(int type,const QIcon& ico);
    //判断是否是有效名字
    static bool isValidName(const QString& name);



    //清空所有数据
    void clear();
    //根据id查找
    SAAbstractDatas* findData(int id) const;
    QList<SAAbstractDatas*> findDatas(const QList<int>& ids) const;
    //获取所有数据
    QList<SAAbstractDatas*> allDatas() const;
    //根据名字查找
    SAAbstractDatas* findData(const QString& name) const;
    //根据添加顺序获取变量的内存地址
    SAAbstractDatas* at(int index) const;
    QString nameAt(int index) const;
    //
    int indexOf(SAAbstractDatas* dataPtr) const;
    //变量数量
    int count() const;
    //判断变量的名字是否有冲突，无冲突返回true
    bool isNameNotConflict(const QString& name,const SAAbstractDatas* ignoreDataPtr = nullptr) const;
    //删除数据
    void removeDatas(QList<SAAbstractDatas*> datas);
    //删除数据
    void removeData(SAAbstractDatas* datas);
    //更改变量名字，变量如果不通过SAValueManager更改名字，将无法获得管理权
    bool renameData(SAAbstractDatas* data, const QString& name);

    //判断数据是否处于管理状态
    bool isDataInManager(const SAAbstractDatas* data) const;


    // 判断变量是否有更改而没保存
    bool isDirty() const;
    //把普通指针转换为智能指针
    std::shared_ptr<SAAbstractDatas> fromNormalPtr(SAAbstractDatas* norPtr);
    QList<std::shared_ptr<SAAbstractDatas> > fromNormalPtr(QList<SAAbstractDatas*> norPtrs);
    //把智能指针列表转换为普通指针列表
    static QList<SAAbstractDatas*> fromSmartPtr(const QList<std::shared_ptr<SAAbstractDatas> >& smrPtrs);
    //清空undo stack
    void clearUndoStack();
    //
public slots:
    void redo();
    void undo();
public:

    /// \name 变量添加操作
    /// \{
    ///
    //添加变量，返回变量的id
    void addData(std::shared_ptr<SAAbstractDatas> data);
     //添加变量，返回变量的id
    void addDatas(QList<std::shared_ptr<SAAbstractDatas> > datas);
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
    void dataRemoved(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    ///
    /// \brief 数据被清除时触发的信号
    ///
    void dataClear();
private:
    //自动转换为正确的变量名，若变量名重复或冲突，会自动把输入的名字后加上其他内容改为正确的变量名
    bool toCorrectName(SAAbstractDatas* data);
    //删除数据
    void __removeData(SAAbstractDatas* datas);
    void __removeDatas(QList<SAAbstractDatas*> datas);
    //
    void __addData(std::shared_ptr<SAAbstractDatas> data);
    void __addDatas(QList<std::shared_ptr<SAAbstractDatas> > datas);
    //
    bool __renameData(SAAbstractDatas* data, const QString& name);
    //友元类
    friend class SAValueAddCommand;
    friend class SAValuesAddCommand;
    friend class SAValueRemoveCommand;
    friend class SAValuesRemoveCommand;
    friend class SAValueRenameCommand;
private:
    static SAValueManager* s_instance;
private:
    std::unique_ptr<PointerContainerPrivate> m_ptrContainer;///< 此数据结构的指针管理容器
    QString m_lastSaveDataFolder;///< 记录最后保存的目录
    QUndoStack m_undoStack;
    static QHash<int,QIcon> s_dataType2Icon; ///<保存数据类型对应的图标
};

///
/// \brief 用于生成数据的工厂
/// 生成的数据并不会加入管理系统中需要调用addData
/// \return 数据指针
///
template<typename DataType, typename... _Args>
std::shared_ptr<DataType> SAValueManager::makeData(_Args &&... __args)
{
    return  std::make_shared<DataType>(std::forward<_Args>(__args)...);
}
#ifndef saValueManager
#define saValueManager SAValueManager::getInstance()
#endif












#endif // SAVALUEMANAGER_H
