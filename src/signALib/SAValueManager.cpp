#include "SAValueManager.h"
#include <QMutexLocker>
#include <QRegExp>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
#include <QRegExp>
#include "SAValueOptCommand.h"
#include "SALibResourDefine.h"
QHash<int,QIcon> SAValueManager::s_dataType2Icon = QHash<int,QIcon>();

QMutex g_mutexSAValueManager;
SAValueManager* SAValueManager::s_instance = nullptr;


///
/// \brief 保存指针和一些额外信息的容器
///
class PointerContainerPrivate
{
public:
    PointerContainerPrivate();
    ~PointerContainerPrivate();
    //从普通指针转换为智能指针
    std::shared_ptr<SAAbstractDatas> fromNormalPtr(SAAbstractDatas* norPtr) const;
    QList<std::shared_ptr<SAAbstractDatas> > fromNormalPtr(QList<SAAbstractDatas *> norPtrs);
    //添加管理的变量
    void append(std::shared_ptr<SAAbstractDatas> data);
    //设置数据文件的文件映射路径
    void setDataFilePath(SAAbstractDatas* data);
    //根据索引查找
    SAAbstractDatas* at(int index) const;
    //根据指针找到索引
    int indexOf(SAAbstractDatas* dataPtr) const;
    //根据id查找
    SAAbstractDatas* findData(int id) const;
    //根据名字查找
    SAAbstractDatas* findData(const QString& name) const;
    //清空所有数据
    void clear();
    //获取变量的数量
    int count() const;
    //删除数据指针内存同时在容器移除
    void removeData(SAAbstractDatas* data);
    //变量改名
    bool changDataName(SAAbstractDatas* data,const QString& name);
    //判断是否已存在名字
    bool isHaveDataName(const QString& name) const;
    //获取所有的一维向量数据
    QList<SAAbstractDatas*> getVectorDataPtrs() const;
    //判断数据是否处于管理状态
    bool isDataInManager(const SAAbstractDatas* data) const;
    //
    QList<SAAbstractDatas*> allDatas() const;
private:
//    QList<SAAbstractDatas*> m_ptrList;///< 总指针队列
//    QMap <SAAbstractDatas*,int> m_ptrListSet;///< 用于快速判断是否存在

    QList<std::shared_ptr<SAAbstractDatas> > m_smrPtrList;///< 指针队列,此队列通过智能指针添加，可以记录添加的顺序
    QMap <SAAbstractDatas*,std::shared_ptr<SAAbstractDatas> > m_norPtr2smrPtr;///< 普通指针和智能指针的映射
    QMap<int,SAAbstractDatas*> m_id2data;///<记录变量的id和内存地址，通过此可以快速查找
    QMap<QString,SAAbstractDatas*> m_dataName2DataPtr;///< 记录变量名对应的数据类型
};




SAValueManager::SAValueManager():QObject(nullptr)
  ,m_ptrContainer(new PointerContainerPrivate)
  ,m_lastSaveDataFolder("")
{
    m_ptrContainer->clear();
}

SAValueManager::~SAValueManager()
{
    if(nullptr != s_instance)
    {
        m_undoStack.clear();
        QMutexLocker locker(&g_mutexSAValueManager);
        delete s_instance;
        s_instance = nullptr;
    }
}
///
/// \brief 获取对象，返回全局位于对象实例
/// \return 对象指针
///
SAValueManager *SAValueManager::getInstance()
{
    if(nullptr == s_instance)
    {
        QMutexLocker locker(&g_mutexSAValueManager);
        if(nullptr == s_instance)
        {
            s_instance = new SAValueManager();
        }
    }
    return s_instance;
}
///
/// \brief 创建一个引用数据
/// 引用和原始数据有一样的功能，但引用删除原始不删除，原始删除引用都会删除
/// \param data 需要引用的数据
/// \return 引用的数据指针
///
SAValueManager::IDATA_PTR SAValueManager::makeRefData(SAAbstractDatas *data)
{
    return  std::make_shared<SADataReference>(data);
}
///
/// \brief 把智能指针列表转换为普通指针列表
/// \param ptrs 智能指针列表
/// \return 普通指针列表
///
QList<SAAbstractDatas *> SAValueManager::toNormalPtrList(const QList<SAValueManager::IDATA_PTR> &ptrs)
{
    QList<SAAbstractDatas *> res;
    const int size = ptrs.size();
    for(int i=0;i<size;++i)
    {
        res.append(ptrs[i].get());
    }
    return res;
}
///
/// \brief 获取类型对应图标
/// \param dtype 数据类型
/// \return 图标
///
QIcon SAValueManager::getDataIcon(int dtype)
{
    QIcon ico = s_dataType2Icon.value(dtype,QIcon());
    if(ico.isNull())
    {
        return ICON_DEFAULT_DATA_TYPE;
    }
    return ico;
}
///
/// \brief 注册类型对应的图标
/// \param type
/// \param ico
///
void SAValueManager::registerDataTypeIcon(int type, const QIcon &ico)
{
    s_dataType2Icon[type] = ico;
}

///
/// \brief 清空所有数据,同时也会清除内存
///
void SAValueManager::clear()
{
    m_ptrContainer->clear();
    m_undoStack.clear();
    emit dataClear();
}
///
/// \brief 根据id查找变量，所有变量都只有唯一id
/// \param id id
/// \return 未能获取放回nullptr
///
SAAbstractDatas *SAValueManager::findData(int id) const
{
    return m_ptrContainer->findData(id);
}
///
/// \brief 根据id查找变量，所有变量都只有唯一id
/// \param ids id数组
/// \return
///
QList<SAAbstractDatas*> SAValueManager::findDatas(const QList<int> &ids) const
{
    QList<SAAbstractDatas*> res;
    std::for_each(ids.begin(),ids.end(),[&res,this](int id){
        SAAbstractDatas* d = this->findData(id);
        if(d)
        {
            res.append(d);
        }
    });
    return res;
}
///
/// \brief 获取所有数据
/// \return
///
QList<SAAbstractDatas *> SAValueManager::allDatas() const
{
    return m_ptrContainer->allDatas();
}
///
/// \brief 根据名字查找变量，所有变量管理器管理的变量都只有唯一名字
/// \param name 变量名
/// \return 未能获取放回nullptr
///
SAAbstractDatas *SAValueManager::findData(const QString &name) const
{
    return m_ptrContainer->findData(name);
}
///
/// \brief 根据QModelIndex查找变量
/// \param index
/// \return 未能获取放回nullptr
///
//SAAbstractDatas *SAValueManager::findData(const QModelIndex index) const
//{
//    //todo
//    //index.

//}
///
/// \brief 根据添加顺序获取变量的内存地址
/// \param index 索引
/// \return 未能获取放回nullptr
///
SAAbstractDatas *SAValueManager::at(int index) const
{
    return m_ptrContainer->at(index);
}
///
/// \brief 根据索引获取变量名字
/// \param index 索引
/// \return
///
QString SAValueManager::nameAt(int index) const
{
    SAAbstractDatas* p = at(index);
    return (p ? p->getName() : QString());
}
///
/// \brief SAValueManager::indexOf
/// \param dataPtr
/// \return
///
int SAValueManager::indexOf(SAAbstractDatas *dataPtr) const
{
    return m_ptrContainer->indexOf(dataPtr);
}
///
/// \brief 获取变量的数量
/// \return
///
int SAValueManager::count() const
{
    return m_ptrContainer->count();
}
///
/// \brief 判断是否是有效名字
/// \param name 变量名
/// \return 对于不合法的如:;等字眼会返回false
/// \see isNameConflict
///
bool SAValueManager::isValidName(const QString &name)
{
    if(name.isEmpty())
    {
        return false;
    }
    QRegExp rp("[\\\\/:*?\"<>|]`");
    if(rp.indexIn(name)>0)
    {
        qDebug("bool SAValueManager::isValidName(const QString &name) const ,name unaccept!");
        return false;
    }
    return true;
}
///
/// \brief 命名和原来的变量有冲突
/// \param name 命名
/// \param ignoreDataPtr 忽略的变量指针
/// \return 无冲突返回true
/// \see isValidName
///
bool SAValueManager::isNameNotConflict(const QString &name, const SAAbstractDatas *ignoreDataPtr) const
{
    SAAbstractDatas* data = m_ptrContainer->findData(name);
    if(nullptr != data)
    {//说明有重命名
        if(nullptr != ignoreDataPtr)
        {//说明有指定忽略
            if(ignoreDataPtr != data)
            {//于指定忽略不同，说明命名有冲突
                return false;
            }
        }
        else
        {//没有指定忽略，还出现重命名，说明命名不合法
            return false;
        }
    }
    return true;
}




///
/// \brief 销毁数据内存
/// \param datas
///
void SAValueManager::removeData(SAAbstractDatas *datas)
{
    SAValueRemoveCommand* removeCmd = new SAValueRemoveCommand(tr("value remove"),fromNormalPtr(datas));
    m_undoStack.push(removeCmd);
}

void SAValueManager::__removeData(SAAbstractDatas *datas)
{
    m_ptrContainer->removeData(datas);
    emit dataRemoved(QList<SAAbstractDatas*>()<<datas);
}

///
/// \brief 删除数据
/// \param datas 数据指针
///
void SAValueManager::removeDatas(QList<SAAbstractDatas *> datas)
{
    SAValuesRemoveCommand* cmd = new SAValuesRemoveCommand(tr("values remove"),fromNormalPtr(datas));
    m_undoStack.push(cmd);
}

void SAValueManager::__removeDatas(QList<SAAbstractDatas *> datas)
{
    for (auto i = datas.begin();i!=datas.end();++i)
    {
        m_ptrContainer->removeData(*i);
    }
    emit dataRemoved(datas);
}



///
/// \brief 更改变量名字
/// 变量如果不通过SAValueManager更改名字，将无法获得管理权
/// \param datas 变量的指针
/// \param name 变量的新名字
/// \return 成功返回true
/// \note 此操作会触发 dataNameChanged信号
///
bool SAValueManager::renameData(SAAbstractDatas *data, const QString &name)
{
    SAValueRenameCommand* renameCMD = new SAValueRenameCommand(tr("rename value"),name,fromNormalPtr(data));
    m_undoStack.push(renameCMD);
}

bool SAValueManager::__renameData(SAAbstractDatas *data, const QString &name)
{
    QString oldName = data->getName();
    if(!isNameNotConflict(name))
    {
        return false;
    }
    if(!isValidName(name))
    {
        return false;
    }
    if(m_ptrContainer->changDataName(data,name))
    {
        emit dataNameChanged(data,oldName);
        return true;
    }
    return false;
}

///
/// \brief 判断数据是否处于管理状态
/// \param data
/// \return
/// \note not O(1), is O(n)
///
bool SAValueManager::isDataInManager(const SAAbstractDatas *data) const
{
    return m_ptrContainer->isDataInManager(data);
}
///
/// \brief 获取所有的一维数据
/// \return
///
QList<SAAbstractDatas *> SAValueManager::getVectorDataPtrs() const
{
    QList<SAAbstractDatas *> res = m_ptrContainer->getVectorDataPtrs();
    return res;
}
///
/// \brief 获取线性向量型数据
/// \param id 数据id
/// \param datas 数据的容器
/// \return 成功获取返回true
///
bool SAValueManager::getVectorData(int id, QVector<double> &datas)
{
    SAAbstractDatas* dataPtr = findData(id);
    return getVectorData(dataPtr,datas);
}
///
/// \brief 获取线性向量型数据
/// \param dataPtr 数据指针
/// \param datas 数据的容器
/// \return 成功获取返回true
///
bool SAValueManager::getVectorData(SAAbstractDatas *dataPtr, QVector<double> &datas)
{
    return SAAbstractDatas::converToDoubleVector(dataPtr,datas);
}
///
/// \brief 添加变量
/// \param data 变量指针
///
void SAValueManager::addData(std::shared_ptr<SAAbstractDatas> data)
{
    SAValueAddCommand* addCMD = new SAValueAddCommand(tr("value add"),data);
    m_undoStack.push(addCMD);
}

void SAValueManager::__addData(std::shared_ptr<SAAbstractDatas> data)
{
    if(!toCorrectName(data.get()))
    {
        //变量名不合法
        emit messageInformation(tr("value name:%1 is not correct!")
                                ,SA::WarningMessage);
        return;
    }
    m_ptrContainer->append(data);
    emit dataAdded({data.get()});
}


///
/// \brief 添加变量到变量管理器中
/// \param datas 变量指针列表
///
void SAValueManager::addDatas(QList<std::shared_ptr<SAAbstractDatas> > datas)
{
    SAValuesAddCommand* cmd = new SAValuesAddCommand(tr("values add"),datas);
    m_undoStack.push(cmd);
}

void SAValueManager::__addDatas(QList<std::shared_ptr<SAAbstractDatas> > datas)
{
    QList<SAAbstractDatas*> r;
    auto end = datas.end();
    for(auto i=datas.begin();i!=end;++i)
    {
        toCorrectName(i->get());
        this->m_ptrContainer->append(*i);
        r << i->get();
    }
    emit dataAdded(r);
}
///
/// \brief 自动转换为正确的变量名，若变量名重复或冲突，会自动把输入的名字后加上其他内容改为正确的变量名
/// \return 可以正确更改名字返回true
///
bool SAValueManager::toCorrectName(SAAbstractDatas *data)
{
    QString name = data->getName();
    if(name.isEmpty())
    {
        name = "data";
    }
    if(!isValidName(name))
    {
        QRegExp rp("[\\\\/:*?\"<>|]`");
        name.replace(rp,"_");
    }
    //名字有冲突
    while(!isNameNotConflict(name))
    {
        QRegExp rp("_\\d+$");
        const int index = name.indexOf(QRegExp("_\\d+$"));
        if(index > 0)
        {
            QString numStr = name.mid(index+1,name.size()-index-1);
            int num = numStr.toInt();
            ++num;
            QString tmp = name;
            name.replace(QRegExp("_\\d+$"),QString("_%1").arg(num));
            if(name == tmp)
            {
                name = name+"_1";
            }
        }
        else
        {
            name += "_1";
        }
    }
    if(isValidName(name))
    {
        data->setName(name);
        return true;
    }
    else
    {
        return false;
    }

//    if (-1 == index)
//    {//名字里没有_说明没有被重新命名，自动添加_1,再查找有没有被添加过
//        name += QString("_1");
//        data->setName(name);
//        return toCorrectName(data);
//    }
//    //说明有_，检查后面是否是数字
//    if(index == (name.size()-1))
//    {//说明最后一个是_
//        name += QString("1");
//        data->setName(name);
//        return toCorrectName(data);
//    }
//    bool isSuccess = false;
//    QString numStr = name.mid(index+1);
//    int num = numStr.toInt(&isSuccess);
//    if(isSuccess)
//    {//成功转换为数字
//        name = name.mid(0,index+1) + QString::number(++num);
//        data->setName(name);
//        return toCorrectName(data);
//    }
//    else
//    {//不是数字
//        name += QString("_1");
//        data->setName(name);
//        return toCorrectName(data);
//    }
}


///
/// \brief 根据类型，创建数据
/// \param type 数据类型
/// \return 如果没有对应，返回nullptr
/// \see SA::DataType
///
SAValueManager::IDATA_PTR SAValueManager::makeDataByType(SA::DataType type)
{
    if(SA::VectorInt == type)
    {
        return castPointToBase(SAValueManager::makeData<SAVectorInt>());
    }
    else if(SA::VectorDouble == type)
    {
        return castPointToBase(SAValueManager::makeData<SAVectorDouble>());
    }
    else if(SA::VectorVariant == type)
    {
        return castPointToBase(SAValueManager::makeData<SAVectorVariant>());
    }
    else if(SA::VectorInterval == type)
    {
        return castPointToBase(SAValueManager::makeData<SAVectorInterval>());
    }
    else if(SA::VectorPoint == type)
    {
        return castPointToBase(SAValueManager::makeData<SAVectorPointF>());
    }
    else if(SA::TableVariant == type)
    {
        return castPointToBase(SAValueManager::makeData<SATableVariant>());
    }
    else if(SA::TableDouble == type)
    {
        return castPointToBase(SAValueManager::makeData<SATableDouble>());
    }
    return nullptr;
}
///
/// \brief 判断变量是否有更改而没保存
/// \return 如果有变量没有保存，将返回true
///
bool SAValueManager::isDirty() const
{
    int count = m_ptrContainer->count();
    for(int i=0;i<count;++i)
    {
        if(m_ptrContainer->at(i)->isDirty())
            return true;
    }
    return false;
}
///
/// \brief 把普通指针转换为智能指针
/// \param norPtr
/// \return
///
std::shared_ptr<SAAbstractDatas> SAValueManager::fromNormalPtr(SAAbstractDatas *norPtr)
{
    return m_ptrContainer->fromNormalPtr(norPtr);
}

QList<std::shared_ptr<SAAbstractDatas> > SAValueManager::fromNormalPtr(QList<SAAbstractDatas *> norPtrs)
{
    return m_ptrContainer->fromNormalPtr(norPtrs);
}
///
/// \brief 把智能指针列表转换为普通指针列表
/// \param smrPtrs 智能指针列表
/// \return
///
QList<SAAbstractDatas *> SAValueManager::fromSmartPtr(const QList<std::shared_ptr<SAAbstractDatas> > &smrPtrs)
{
    QList<SAAbstractDatas*> datas;
    auto fun = [&datas](std::shared_ptr<SAAbstractDatas> p){
        datas.append(p.get());
    };
    std::for_each(smrPtrs.begin(),smrPtrs.end(),fun);
    return datas;
}
///
/// \brief 清空undo stack
///
void SAValueManager::clearUndoStack()
{
    m_undoStack.clear();
}

void SAValueManager::redo()
{
    m_undoStack.redo();
}

void SAValueManager::undo()
{
    m_undoStack.undo();
}







//============================================================
//PointerContainerPrivate
//============================================================


PointerContainerPrivate::PointerContainerPrivate()
{
    m_smrPtrList.clear();
    m_norPtr2smrPtr.clear();
    m_id2data.clear();
    m_dataName2DataPtr.clear();
}

PointerContainerPrivate::~PointerContainerPrivate()
{

}

std::shared_ptr<SAAbstractDatas> PointerContainerPrivate::fromNormalPtr(SAAbstractDatas *norPtr) const
{
    return m_norPtr2smrPtr.value(norPtr,nullptr);
}

QList<std::shared_ptr<SAAbstractDatas> > PointerContainerPrivate::fromNormalPtr(QList<SAAbstractDatas *> norPtrs)
{
    QList<std::shared_ptr<SAAbstractDatas> > res;
    auto lamfun = [this,&res](SAAbstractDatas* p){
        auto smtp = this->fromNormalPtr(p);
        if(smtp)
        {
            res.append(smtp);
        }
    };
    std::for_each(norPtrs.begin(),norPtrs.end(),lamfun);
    return res;
}

void PointerContainerPrivate::append(std::shared_ptr<SAAbstractDatas> data)
{
    m_smrPtrList.append(data);
    m_norPtr2smrPtr[data.get()] = data;
    m_id2data[data->getID()] = data.get();
    m_dataName2DataPtr[data->getName()] = data.get();
}


SAAbstractDatas *PointerContainerPrivate::at(int index) const
{
    return m_smrPtrList[index].get();
}
///
/// \brief 返回索引，如果没有返回-1
/// \param dataPtr
/// \return
///
int PointerContainerPrivate::indexOf(SAAbstractDatas *dataPtr) const
{
    auto smptr = fromNormalPtr(dataPtr);
    return m_smrPtrList.indexOf(smptr);
}

SAAbstractDatas *PointerContainerPrivate::findData(int id) const
{
    return m_id2data.value(id,nullptr);
}

SAAbstractDatas *PointerContainerPrivate::findData(const QString &name) const
{
    return m_dataName2DataPtr.value(name,nullptr);
}

void PointerContainerPrivate::clear()
{
    m_smrPtrList.clear();
    m_norPtr2smrPtr.clear();
    m_id2data.clear();
    m_dataName2DataPtr.clear();

}

int PointerContainerPrivate::count() const
{
    return m_smrPtrList.size();
}

void PointerContainerPrivate::removeData(SAAbstractDatas *data)
{
    if(nullptr == data)
    {
        return;
    }
    //总队列里查找指针
    auto smpt = fromNormalPtr(data);
    m_smrPtrList.removeOne(smpt);
    m_id2data.remove(data->getID());
    m_dataName2DataPtr.remove(data->getName());
    m_norPtr2smrPtr.remove(data);
}

bool PointerContainerPrivate::isHaveDataName(const QString &name) const
{
    return (m_dataName2DataPtr.find(name) != m_dataName2DataPtr.end());
}

QList<SAAbstractDatas *> PointerContainerPrivate::getVectorDataPtrs() const
{
    QList<SAAbstractDatas*> res;
    auto fun = [&res](const std::shared_ptr<SAAbstractDatas>& p){
        if(SA::Dim1 == p->getDim())
        {
            res.append(p.get());
        }
    };
    std::for_each(m_smrPtrList.begin(),m_smrPtrList.end(),fun);
    return res;
}

bool PointerContainerPrivate::isDataInManager(const SAAbstractDatas *data) const
{
    return m_norPtr2smrPtr.contains(const_cast<SAAbstractDatas *>(data));
}

QList<SAAbstractDatas *> PointerContainerPrivate::allDatas() const
{
    QList<SAAbstractDatas*> res;
    auto fun = [&res](const std::shared_ptr<SAAbstractDatas>& p){
        res.append(p.get());
    };
    std::for_each(m_smrPtrList.begin(),m_smrPtrList.end(),fun);
    return res;
}


bool PointerContainerPrivate::changDataName(SAAbstractDatas *data, const QString &name)
{
    if(isHaveDataName(name))
    {
        return false;
    }
    m_dataName2DataPtr.remove(data->getName());
    m_dataName2DataPtr[name] = data;
    data->setText(name);
    return true;
}


