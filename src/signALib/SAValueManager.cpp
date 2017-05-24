#include "SAValueManager.h"
#include <QMutexLocker>
#include <QRegExp>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
#include <QRegExp>

QMutex g_mutexSAValueManager;
SAValueManager* SAValueManager::s_instance = nullptr;

SAValueManager::SAValueManager():QObject(nullptr)
  ,m_lastSaveDataFolder("")
{
    m_ptrContainer.clear();
}

SAValueManager::~SAValueManager()
{
    if(nullptr != s_instance)
    {
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
std::shared_ptr<SADataReference> SAValueManager::makeRefData(SAAbstractDatas *data)
{
    return  std::make_shared<SADataReference>(data);
}

///
/// \brief 清空所有数据,同时也会清除内存
///
void SAValueManager::clear()
{
    m_ptrContainer.clear(true);
    emit dataClear();
}
///
/// \brief 根据id查找变量，所有变量都只有唯一id
/// \param id id
/// \return 未能获取放回nullptr
///
SAAbstractDatas *SAValueManager::findData(int id) const
{
    return m_ptrContainer.findData(id);
}
///
/// \brief 根据名字查找变量，所有变量管理器管理的变量都只有唯一名字
/// \param name 变量名
/// \return 未能获取放回nullptr
///
SAAbstractDatas *SAValueManager::findData(const QString &name) const
{
    return m_ptrContainer.findData(name);
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
    return m_ptrContainer.at(index);
}
///
/// \brief SAValueManager::indexOf
/// \param dataPtr
/// \return
///
int SAValueManager::indexOf(SAAbstractDatas *dataPtr) const
{
    return m_ptrContainer.indexOf(dataPtr);
}
///
/// \brief 获取变量的数量
/// \return
///
int SAValueManager::count() const
{
    return m_ptrContainer.count();
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
    SAAbstractDatas* data = m_ptrContainer.findData(name);
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
/// \brief 删除数据
/// \param datas 数据指针
///
void SAValueManager::deleteDatas(QList<SAAbstractDatas *> datas)
{
    for (auto i = datas.begin();i!=datas.end();++i)
    {
        m_ptrContainer.deleteData(*i);
    }
    emit dataDeleted(datas);
}
///
/// \brief 销毁数据内存
/// \param datas
///
void SAValueManager::deleteData(SAAbstractDatas *datas)
{
    m_ptrContainer.deleteData(datas);
    emit dataDeleted(QList<SAAbstractDatas*>()<<datas);
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
    QString oldName = data->getName();
    if(!isNameNotConflict(name))
    {
        return false;
    }
    if(!isValidName(name))
    {
        return false;
    }
    if(m_ptrContainer.changDataName(data,name))
    {
        emit dataNameChanged(data,oldName);
        return true;
    }
    return false;
}
///
/// \brief 保存变量到文件
/// \param data 数据的指针
/// \param path 要保存的目录
/// \param file QFile文件指针
/// \return 返回QFileDevice::FileError,其中0代表成功
/// QFileDevice::NoError	0	No error occurred.
/// QFileDevice::ReadError	1	An error occurred when reading from the file.
/// QFileDevice::WriteError	2	An error occurred when writing to the file.
/// QFileDevice::FatalError	3	A fatal error occurred.
/// QFileDevice::ResourceError	4	Out of resources (e.g., too many open files, out of memory, etc.)
/// QFileDevice::OpenError	5	The file could not be opened.
/// QFileDevice::AbortError	6	The operation was aborted.
/// QFileDevice::TimeOutError	7	A timeout occurred.
/// QFileDevice::UnspecifiedError	8	An unspecified error occurred.
/// QFileDevice::RemoveError	9	The file could not be removed.
/// QFileDevice::RenameError	10	The file could not be renamed.
/// QFileDevice::PositionError	11	The position in the file could not be changed.
/// QFileDevice::ResizeError	12	The file could not be resized.
/// QFileDevice::PermissionsError	13	The file could not be accessed.
/// QFileDevice::CopyError	14	The file could not be copied.
///
int SAValueManager::saveAs(const SAAbstractDatas *data, const QString &path, QFile *file)
{
    if(nullptr == file)
    {
        return -1;
    }

    if(data->getType() == SA::DataLink)
    {//引用数据变量，后缀为sadref
        file->setFileName(QStringLiteral("%1%2%3.sadref")
                         .arg(path)
                         .arg(QDir::separator())
                         .arg(data->getName()));
    }
    else
    {
        file->setFileName(QStringLiteral("%1%2%3.sad")
                         .arg(path)
                         .arg(QDir::separator())
                         .arg(data->getName()));
    }

    if(file->open(QIODevice::WriteOnly))
    {
        QDataStream out(file);
        data->write(out);
    }
    else
    {
        emit messageInformation(tr("write data file error:%1")
                                .arg(file->errorString())
                                ,SA::ErrorMessage
                                );
    }
    return static_cast<int>(file->error());
}
///
/// \brief 保存数据到文件系统
/// \param path 指定的路径
/// \return 成功返回0,否则返回失败的个数，如果一个变量失败返回1，以此类推
///
int SAValueManager::saveAs(const QString &path)
{
    m_lastSaveDataFolder = path;
    //-----创建数据文件夹
    if(!QFileInfo::exists(m_lastSaveDataFolder))
    {
        emit messageInformation(tr("can not find dir:%1").arg(m_lastSaveDataFolder)
                                ,SA::ErrorMessage
                                );
        return -1;
    }
    //-----保存数据文件，同时更新数据对应文件索引
    const int size = count();
    int errCount = 0;
    QFile file;
    for(int i=0;i<size;++i)
    {
        SAAbstractDatas* data = m_ptrContainer.at(i);
        if(saveAs(data,path,&file) != static_cast<int>(QFileDevice::NoError))
        {
            ++errCount;
            continue;
        }
        //更新文件映射
        m_ptrContainer.setDataFilePath(data,file.fileName());
        qDebug() << file.fileName();
    }
    return errCount;
}
///
/// \brief 从文件系统中加载数据
/// \param path 指定的路径
/// \return 成功返回0,否则返回失败的个数，如果一个变量失败返回1，以此类推,-1代表路径不正确
///
int SAValueManager::load(const QString &path)
{
    QDir dir(path);
    int errCount = 0;
    if(!dir.exists())
    {
        emit messageInformation(tr("path :\"%1\" may not exist").arg(path),SA::ErrorMessage);
        return -1;
    }
    QStringList dataFileList = dir.entryList({"*.sad"},QDir::Files|QDir::NoSymLinks);
    const int size = dataFileList.size();
    if(0 == size)
    {
        return 0;
    }
    QList<SAAbstractDatas*> datasBeLoad;
    for(int i=0;i<size;++i)
    {
        QString fileName = dataFileList[i];
        int index = fileName.lastIndexOf(".");
        if(index < 0)
        {
            emit messageInformation(tr("file:\"%1\" may not incorrect").arg(fileName),SA::WarningMessage);
            continue;
        }
        QString suffix = fileName.mid(index+1);
        QString fullFilePath = path + QDir::separator() + fileName;
        //处理sad文件
        if("sad" == suffix)
        {
            //说明是sad数据格式

            std::shared_ptr<SAAbstractDatas> data = loadSad(fullFilePath);
            if(nullptr == data)
            {//说明没有读取成功
                ++errCount;
                continue;
            }
            //说明读取成功
            //记录id映射
            if(toCorrectName(data.get()))
            {
                m_ptrContainer.append(data,fullFilePath);
                datasBeLoad.append(data.get());
            }
        }
        else if("sadref" == suffix)
        {

        }
    }
    emit dataAdded(datasBeLoad);
    return errCount;
}
///
/// \brief 判断数据是否处于管理状态
/// \param data
/// \return
/// \note not O(1), is O(n)
///
bool SAValueManager::isDataInManager(const SAAbstractDatas *data) const
{
    return m_ptrContainer.isDataInManager(data);
}
///
/// \brief 获取所有的一维数据
/// \return
///
QList<SAAbstractDatas *> SAValueManager::getVectorDataPtrs() const
{
    QList<SAAbstractDatas *> res = m_ptrContainer.getVectorDataPtrs();
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
void SAValueManager::addData(SAAbstractDatas *data)
{
    if(!toCorrectName(data))
    {
        //变量名不合法
        emit messageInformation(tr("value name:%1 is not correct!")
                                ,SA::WarningMessage);
        return;
    }

    m_ptrContainer.append(data);
    emit dataAdded({data});
}
///
/// \brief 添加变量
/// \param data 变量指针
///
void SAValueManager::addData(std::shared_ptr<SAAbstractDatas> data)
{
    if(!toCorrectName(data.get()))
    {
        //变量名不合法
        emit messageInformation(tr("value name:%1 is not correct!")
                                ,SA::WarningMessage);
        return;
    }
    m_ptrContainer.append(data);
    emit dataAdded({data.get()});
}
///
/// \brief 添加多个变量
/// \param data 变量指针列表
///
void SAValueManager::addDatas(QList<SAAbstractDatas *> datas)
{
    QList<SAAbstractDatas*> r;
    auto end = datas.end();
    for(auto i=datas.begin();i!=end;++i)
    {
        toCorrectName(*i);
        this->m_ptrContainer.append(*i);
        r << *i;
    }
    emit dataAdded(r);
}

void SAValueManager::addDatas(QList<std::shared_ptr<SAAbstractDatas> > datas)
{
    QList<SAAbstractDatas*> r;
    auto end = datas.end();
    for(auto i=datas.begin();i!=end;++i)
    {
        toCorrectName(i->get());
        this->m_ptrContainer.append(*i);
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
std::shared_ptr<SAAbstractDatas> SAValueManager::createDataByType(SA::DataType type)
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
/// \brief 加载一个sad文件
/// \param filePath 文件路径
/// \param typeInfo 文件的信息
/// \param err 错误信息
/// \return 返回SAAbstractDatas 指针，此函数建立，若不使用，需要手动delete
///
std::shared_ptr<SAAbstractDatas> SAValueManager::loadSad(const QString &filePath)
{
    SADataHeader typeInfo;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit messageInformation(tr("can not open file:\"%1\"").arg(filePath),SA::WarningMessage);
        return nullptr;
    }
    QDataStream in(&file);
    try{
        in >> typeInfo;
        if(!typeInfo.isValid())
        {
            emit messageInformation(tr("file:\"%1\" may be incorrect").arg(filePath),SA::WarningMessage);
            return nullptr;
        }
        std::shared_ptr<SAAbstractDatas> data = createDataByType(static_cast<SA::DataType>(typeInfo.getDataType()));
        if(nullptr == data)
        {
            emit messageInformation(tr("file:\"%1\" failed to reflect").arg(filePath),SA::WarningMessage);
            return nullptr;
        }
        data->read(in);
        return data;
    }
    catch(...)
    {
        emit messageInformation(tr("file:\"%1\" failed to read").arg(filePath),SA::ErrorMessage);
        return nullptr;
    }
    return nullptr;
}

/////
///// \brief SAValueManager::findDataByType
///// \param type
///// \return
/////
//QList<SAAbstractDatas *> SAValueManager::findDataByType(SA::DataType type) const
//{

//}

//============================================================


SAValueManager::PointerContainer::PointerContainer()
{
    m_ptrList.clear();
    m_ptrListSet.clear();
    m_ptrNormalList.clear();
    m_ptrNormalListSet.clear();
    m_smrPtrList.clear();
    m_smrPtrListSet.clear();
    m_id2data.clear();
    m_dataName2DataPtr.clear();
    m_dataFileName2DataPtr.clear();
    m_dataPtr2DataFileName.clear();
}

SAValueManager::PointerContainer::~PointerContainer()
{

}

void SAValueManager::PointerContainer::append(std::shared_ptr<SAAbstractDatas> data,const QString& dataFile)
{
    m_ptrList.append(data.get());
    m_ptrListSet.insert(data.get(),m_ptrList.size()-1);

    m_smrPtrList.append(data);
    m_smrPtrListSet.insert(data,m_smrPtrList.size()-1);

    m_id2data[data->getID()] = data.get();
    m_dataName2DataPtr[data->getName()] = data.get();

    if(!dataFile.isEmpty())
    {
        m_dataFileName2DataPtr[dataFile] = data.get();
        m_dataPtr2DataFileName[data.get()] = dataFile;
    }
}

void SAValueManager::PointerContainer::append(SAAbstractDatas *data, const QString &dataFile)
{
    m_ptrList.append(data);
    m_ptrListSet.insert(data,m_ptrList.size()-1);

    m_ptrNormalList.append(data);
    m_ptrNormalListSet.insert(data,m_ptrNormalList.size()-1);

    m_id2data[data->getID()] = data;
    m_dataName2DataPtr[data->getName()] = data;

    if(!dataFile.isEmpty())
    {
        m_dataFileName2DataPtr[dataFile] = data;
        m_dataPtr2DataFileName[data] = dataFile;
    }
}



void SAValueManager::PointerContainer::setDataFilePath(SAAbstractDatas *data, const QString &dataFile)
{
    m_dataFileName2DataPtr[dataFile] = data;
    m_dataPtr2DataFileName[data] = dataFile;
}

SAAbstractDatas *SAValueManager::PointerContainer::at(int index) const
{
    return m_ptrList[index];
}
///
/// \brief 返回索引，如果没有返回-1
/// \param dataPtr
/// \return
///
int SAValueManager::PointerContainer::indexOf(SAAbstractDatas *dataPtr) const
{
    return m_ptrListSet.value(dataPtr,-1);
}

SAAbstractDatas *SAValueManager::PointerContainer::findData(int id) const
{
    return m_id2data.value(id,nullptr);
}

SAAbstractDatas *SAValueManager::PointerContainer::findData(const QString &name) const
{
    return m_dataName2DataPtr.value(name,nullptr);
}

QString SAValueManager::PointerContainer::getDataFilePath(const SAAbstractDatas *dataPtr) const
{
    return m_dataPtr2DataFileName.value(const_cast<SAAbstractDatas*>(dataPtr),QString());
}

void SAValueManager::PointerContainer::clear(bool deletePtr)
{
    if(deletePtr)
    {
        //删除智能指针保存的
        std::for_each(m_smrPtrList.begin(),m_smrPtrList.end(),[this](std::shared_ptr<SAAbstractDatas>& p){
            SAAbstractDatas* tmp = m_id2data.value(p->getID(),nullptr);
            if(tmp)
            {
                this->m_id2data[p->getID()] = nullptr;
                p = nullptr;
            }
        });

        //删除普通指针保存的
        std::for_each(m_ptrList.begin(),m_ptrList.end()
                      ,[this](SAAbstractDatas* p){
            if(p)
            {
                delete p;
            }
        });

    }
    m_ptrList.clear();
    m_ptrListSet.clear();

    m_ptrNormalList.clear();
    m_ptrNormalListSet.clear();

    m_smrPtrList.clear();
    m_smrPtrListSet.clear();

    m_id2data.clear();
    m_dataName2DataPtr.clear();
    m_dataFileName2DataPtr.clear();
    m_dataPtr2DataFileName.clear();
}

int SAValueManager::PointerContainer::count() const
{
    return m_ptrList.size();
}

void SAValueManager::PointerContainer::deleteData(SAAbstractDatas *data)
{
    if(nullptr == data)
    {
        return;
    }
    //总队列里查找指针
    auto ite = m_ptrListSet.find(data);
    if(ite != m_ptrListSet.end())
    {
        int index = ite.value();
        m_ptrList.removeAt(index);


        m_id2data.remove(data->getID());
        m_dataName2DataPtr.remove(data->getName());
        QString fileName = m_dataPtr2DataFileName.value(data,QString());
        if(!fileName.isEmpty())
        {
            m_dataPtr2DataFileName.remove(data);
            m_dataFileName2DataPtr.remove(fileName);
        }
        m_ptrListSet.erase(ite);
    }
    auto iteNor = m_ptrNormalListSet.find(data);
    if(iteNor != m_ptrNormalListSet.end())
    {
        int index = iteNor.value();
        m_ptrNormalList.removeAt(index);
        m_ptrNormalListSet.erase(iteNor);
        delete data;
    }
    else
    {
        std::shared_ptr<SAAbstractDatas> smr(data,[](SAAbstractDatas* p){Q_UNUSED(p);});
        auto iteSmr = m_smrPtrListSet.find(smr);
        if(iteSmr != m_smrPtrListSet.end())
        {
            m_smrPtrListSet.erase(iteSmr);
        }
    }
}

bool SAValueManager::PointerContainer::isHaveDataName(const QString &name) const
{
    return (m_dataName2DataPtr.find(name) != m_dataName2DataPtr.end());
}

QList<SAAbstractDatas *> SAValueManager::PointerContainer::getVectorDataPtrs() const
{
    QList<SAAbstractDatas*> res;
    auto fun = [&res](SAAbstractDatas* p){
        if(SAAbstractDatas::isVector(p))
        {
            res.append(p);
        }
    };
    std::for_each(m_ptrList.begin(),m_ptrList.end(),fun);
    return res;
}

bool SAValueManager::PointerContainer::isDataInManager(const SAAbstractDatas *data) const
{
    return m_ptrListSet.contains(const_cast<SAAbstractDatas *>(data));
}


bool SAValueManager::PointerContainer::changDataName(SAAbstractDatas *data, const QString &name)
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



///
/// \brief 转换为double vector
/// \param data 输入的数据
/// \return 如果转换失败，返回nullptr，如果成功，返回一个指针，此指针需要无拥有者，需要交给saValueManager或者其他进行管理
///
std::shared_ptr<SAVectorDouble> SAValueManager::createVectorDoubleFromData(const SAAbstractDatas *data)
{
    if(data->getDim() != 1)
    {
        return nullptr;
    }
    std::shared_ptr<SAVectorDouble> res = SAValueManager::makeData<SAVectorDouble>();
    if(data->getType() == SA::VectorDouble)
    {
        *res = *(static_cast<const SAVectorDouble*>(data));
        return res;
    }
    else
    {
        const int size = data->getSize(0);
        res->reserve(size);
        double d;
        bool isOK = false;
        QVariant var;
        for(int i=0;i<size;++i)
        {
            var = data->getAt(i,0);
            d = var.toDouble(&isOK);
            if(isOK)
            {
                res->append(d);
            }
        }
        if(res->getSize(0) > 0)
        {
            return res;
        }
    }
    return nullptr;
}
