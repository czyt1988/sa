#include "SAProjectManager.h"
#include "SAValueManager.h"
#include <QDir>
#include <QTextStream>
#include <QDomDocument>
#include <QFileInfo>
#include "SALog.h"
#define VERSION_STRING "pro.0.0.1"
#define PROJECT_DES_XML_FILE_NAME "saProject.prodes"
#define DATA_FOLDER_NAME "DATA"
#define SUB_WINDOW_FOLDER_NAME "Wind"
#define TEMP_FOLDER_NAME "Temp"
SAProjectManager* SAProjectManager::s_instance = nullptr;





class SAProjectManagerPrivate
{
    SA_IMPL_PUBLIC(SAProjectManager)
public:
    SAUIInterface* m_ui;
    QString m_projectFullPath;///< 项目对应路径
    QString m_projectName;///< 项目名
    QString m_projectDescribe;///< 项目描述
    bool m_isdirty;///< 工程变更标记
    //--记录文件和数据相互的映射
//    QMap<QString,SAAbstractDatas*> m_dataFileName2DataPtr;///< 记录数据文件路径
//    QMap<SAAbstractDatas*,QString> m_dataPtr2DataFileName;///< 记录数据指针对应的保存文件名
    //保存其他操作的函数指针
    QList<SAProjectManager::FunAction> m_funcSaveActionList;///< 保存时的额外动作列表
    QList<SAProjectManager::FunAction> m_funcLoadActionList;///< 加载是的额外动作列表
    SAProjectManagerPrivate(SAProjectManager* p):q_ptr(p)
    {

    }
};


SAProjectManager::SAProjectManager():QObject(nullptr)
  ,d_ptr(new SAProjectManagerPrivate(this))
{
    connect(saValueManager,&SAValueManager::dataRemoved,this,&SAProjectManager::onDataRemoved);
    connect(saValueManager,&SAValueManager::dataNameChanged,this,&SAProjectManager::onDataNameChanged);
    connect(saValueManager,&SAValueManager::dataClear,this,&SAProjectManager::onDataClear);
}

SAProjectManager::~SAProjectManager()
{

}
///
/// \brief 是否有效
/// \return
///
bool SAProjectManager::isValid() const
{
    return (!d_ptr->m_projectFullPath.isEmpty());
}
///
/// \brief 获取SAProjectManager对象
///
/// 可以使用宏saProjectManager代替此函数
/// \return
///
SAProjectManager *SAProjectManager::getInstance()
{
    if(nullptr == s_instance)
    {
        s_instance = new SAProjectManager();
    }
    return s_instance;
}
///
/// \brief 获取当前项目的全称路径
/// \return
///
QString SAProjectManager::getProjectFullPath() const
{
    SA_DC(SAProjectManager);
    return d->m_projectFullPath;
}
///
/// \brief 保存工程
/// \return
///
bool SAProjectManager::save()
{
    SA_D(SAProjectManager);
    if(d->m_projectFullPath.isNull())
    {
        return false;
    }
    return saveAs(d->m_projectFullPath);
}
///
/// \brief 设置项目的路径
/// \param projectFullPath
///
void SAProjectManager::setProjectFullPath(const QString &projectFullPath)
{
    SA_D(SAProjectManager);
    d->m_projectFullPath = projectFullPath;
}
///
/// \brief 保存项目信息
/// \param projectFullPath
///
void SAProjectManager::saveProjectInfo(const QString &projectFullPath)
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("SaProject");//创建根节点
    root.setAttribute("version",VERSION_STRING);
    doc.appendChild(root);//添加根节点

    QDomElement ele = doc.createElement("name");//创建元素节点
    root.appendChild(ele);//添加元素节点到根节点
    QDomText eleText = doc.createTextNode(getProjectName());//创建元素文本
    ele.appendChild(eleText);//添加元素文本到元素节点

    ele = doc.createElement("des");//创建元素节点
    root.appendChild(ele);//添加元素节点到根节点
    eleText = doc.createTextNode(getProjectDescribe());//创建元素文本
    ele.appendChild(eleText);//添加元素文本到元素节点

    writeValuesXmlInfos(&doc,&root);

    QString projectDesXml = getProjectDescribeFilePath(projectFullPath);
    QFile file(projectDesXml);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        doc.save(out, 4);
        file.close();
    }
}

///
/// \brief 插入变量信息
/// \param doc DomDoc
///
void SAProjectManager::writeValuesXmlInfos(QDomDocument *doc,QDomNode* root)
{
    QDomElement eleValues = doc->createElement("values");//创建values节点
    root->appendChild(eleValues);
    QList<SAAbstractDatas*> datas = saValueManager->allDatas();
    std::for_each(datas.begin(),datas.end(),[&](SAAbstractDatas* d){
        QDomElement eleVar = doc->createElement("value");//创建一个value，保存一个变量的信息
        eleValues.appendChild(eleVar);
        QDomElement eleName = doc->createElement("name");
        eleVar.appendChild(eleName);
        eleName.appendChild(doc->createTextNode(d->getName()));
    });
}

///
/// \brief 加载项目信息
/// \param projectFullPath 项目文件夹
/// \param valuesNameList xml中记录的变量名顺序
/// \return 成功加载返回true
///
bool SAProjectManager::loadProjectInfo(const QString &projectFullPath, QStringList &valuesNameList)
{
    saDebug("load project");
    QString projectDesXml = getProjectDescribeFilePath(projectFullPath);
    QFile file(projectDesXml);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        return false;
    }
    QDomElement root = doc.documentElement();//读取根节点
    if(root.tagName() != "SaProject")
    {
        return false;
    }
    QString ver = root.attribute("version");
    saDebug("version:%s",ver);
    QDomNode node = root.firstChild();//读取第一个子节点
    while (!node.isNull())
    {
        QString tagName = node.toElement().tagName();
        if (tagName.compare("name") == 0) //节点标记查找
        {
            setProjectName( node.toElement().text());//读取节点文本
        }
        else if (tagName.compare("des") == 0)
        {
            setProjectDescribe(node.toElement().text());
        }
        else if("values" == tagName)
        {
            loadValuesProjectInfo(&node,valuesNameList);
        }
        node = node.nextSibling();//读取下一个兄弟节点
    }

    return true;
}

void SAProjectManager::loadValuesProjectInfo(QDomNode *nodeValues, QStringList &valueNames)
{
    QDomNode valNode = nodeValues->firstChild();
    while(!valNode.isNull())
    {
        QDomElement el = valNode.firstChildElement("name");
        if(!el.isNull())
        {
            QString name = el.text();
            if(!name.isEmpty())
            {
                valueNames.append(name);
            }
        }
        valNode = valNode.nextSibling();
    }
}
///
/// \brief 加载变量
/// \param projectFullPath
///
void SAProjectManager::loadValues(const QString &projectFullPath,const QStringList & suffixs)
{
    SA_D(SAProjectManager);
    QString dataPath = getProjectDataFolderPath(projectFullPath);
    QDir dir(dataPath);
    if(!dir.exists())
    {
        emit messageInformation(tr("project may have not datas path :\"%1\"").arg(dataPath),SA::ErrorMessage);
        return;
    }
    saValueManager->clear();

//    QStringList dataFileList = dir.entryList({"*.sad"},QDir::Files|QDir::NoSymLinks);
    QFileInfoList dataFileInfoList = dir.entryInfoList({"*.sad"},QDir::Files|QDir::NoSymLinks);
    const int size = dataFileInfoList.size();
    if(0 == size)
    {
        return;
    }
    QList<std::shared_ptr<SAAbstractDatas> > datasBeLoad;
    for(int i=0;i<size;++i)
    {
//        QString fileName = dataFileList[i];
        QFileInfo fi = dataFileInfoList[i];
        QString suffix = fi.suffix();
        if("sad" != suffix)
        {
            continue;
        }
        QString fullFilePath = fi.absoluteFilePath();
        //处理sad文件
        //说明是sad数据格式
        std::shared_ptr<SAAbstractDatas> data = loadSad(fullFilePath);
        if(nullptr == data)
        {//说明没有读取成功
            continue;
        }
        //说明读取成功
        datasBeLoad.append(data);

    }
    //根据记录的顺序调整list的位置
    for(int i=0;i<suffixs.size();++i)
    {
        int index = 0;
        while(index < datasBeLoad.size())
        {
            if(datasBeLoad[index]->getName() == suffixs[i])
            {
                datasBeLoad.swap(i,index);
                break;
            }
            ++index;
        }
    }
    if(!datasBeLoad.isEmpty())
    {
        saValueManager->addDatas(datasBeLoad);
    }
    saValueManager->clearUndoStack();
    return;
}
///
/// \brief 保存变量
/// \param projectFullPath
/// \return
///
void SAProjectManager::saveValues(const QString &projectFullPath)
{
    QString dataPath = getProjectDataFolderPath(projectFullPath);
    if(dataPath.isEmpty())
    {
        emit messageInformation(tr("can not make dir:%1").arg(dataPath)
                                ,SA::ErrorMessage
                                );
        return;
    }

    //保存之前，先对变量文件夹和变量进行比对，对已经变更名字的变量进行处理

    //int r = saValueManager->saveAs(dataPath);
    const int size = saValueManager->count();
    QString errstr;
    for(int i=0;i<size;++i)
    {
        SAAbstractDatas* data = saValueManager->at(i);
        QString str;
        if(!saveOneValue(data,dataPath,&str))
        {
            errstr += tr("save [%1] occur error:%2").arg(data->getName()).arg(str);
            continue;
        }
    }
    if(!errstr.isEmpty())
    {
        emit messageInformation(errstr,SA::ErrorMessage);
    }
    return;
}
///
/// \brief 保存一个数据
/// \param data 数据指针
/// \param path 指定保存的目录
/// \param errString 错误信息
/// \return
///
bool SAProjectManager::saveOneValue(const SAAbstractDatas *data,const QString &path,QString *errString)
{
    QFile file;
    if(data->getType() == SA::DataLink)
    {//引用数据变量，后缀为sadref
        file.setFileName(QStringLiteral("%1%2%3.sadref")
                         .arg(path)
                         .arg(QDir::separator())
                         .arg(data->getName()));
    }
    else
    {
        file.setFileName(QStringLiteral("%1%2%3.sad")
                         .arg(path)
                         .arg(QDir::separator())
                         .arg(data->getName()));
    }

    if(!file.open(QIODevice::WriteOnly))
    {
        if(errString)
        {
            *errString = file.errorString();
        }
        return false;

    }
    QDataStream out(&file);
    data->write(out);
    return true;
}
///
/// \brief 移除记录的要删除的数据
///
void SAProjectManager::removeNonExistDatas(const QString &projectFullPath)
{
    SA_D(SAProjectManager);
    QString dataPath = getProjectDataFolderPath(projectFullPath);
    QDir dir(dataPath);
    if(!dir.exists())
    {
        emit messageInformation(tr("project may have not datas path :\"%1\"").arg(dataPath),SA::ErrorMessage);
        return;
    }
    QStringList dataFileList = dir.entryList({"*.sad"},QDir::Files|QDir::NoSymLinks);
    QList<SAAbstractDatas*> datas = saValueManager->allDatas();
    QSet<QString> dataNameSet;
    std::for_each(datas.begin(),datas.end(),[&dataNameSet](const SAAbstractDatas* d){
        dataNameSet.insert(d->getName());
    });
    std::for_each(dataFileList.begin(),dataFileList.end(),[&](const QString& fileName){
        QString basefileName = fileName.left(fileName.size()-4);
        if(!dataNameSet.contains(basefileName))
        {
            //文件和数据不对应
            //把文件移动到Temp文件夹下
            QString tmpPath = getProjectTempFolderPath(projectFullPath);
            tmpPath = tmpPath + QDir::separator() + fileName;
            QString originPath = dataPath + QDir::separator() + fileName;
            QFile::copy(originPath,tmpPath);
            QFile::remove(originPath);
        }
    });
}
///
/// \brief 加载一个sad文件
/// \param filePath
/// \return
///
SAValueManager::IDATA_PTR SAProjectManager::loadSad(const QString &filePath)
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
        SAValueManager::IDATA_PTR data
                = SAValueManager::makeDataByType(static_cast<SA::DataType>(typeInfo.getDataType()));
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


///
/// \brief 数据管理器移除数据时
///
/// 记录移除数据对应的文件，保存时把这些文件删除
/// \param dataBeDeletedPtr 移除的数据文件
///
void SAProjectManager::onDataRemoved(const QList<SAAbstractDatas *> &dataBeDeletedPtr)
{
     Q_UNUSED(dataBeDeletedPtr);
    setDirty(true);
}
///
/// \brief 数据管理器进行清除时触发的槽
///
void SAProjectManager::onDataClear()
{
    setDirty(true);
}
///
/// \brief 变量名更改
/// \param data 变量数据指针
/// \param oldName 旧的名字
///
void SAProjectManager::onDataNameChanged(SAAbstractDatas *data, const QString &oldName)
{
    Q_UNUSED(data);
    Q_UNUSED(oldName);
    setDirty(true);
}


QString SAProjectManager::getProjectDescribe() const
{
    SA_DC(SAProjectManager);
    return d->m_projectDescribe;
}

void SAProjectManager::setProjectDescribe(const QString &projectDescribe)
{
    SA_D(SAProjectManager);
    d->m_projectDescribe = projectDescribe;
}
///
/// \brief 根据工程的文件夹获取工程的xml描述文件
/// \param projectFolder 当前工程的顶层目录
/// \return
///
QString SAProjectManager::getProjectDescribeFilePath(const QString &projectFolder)
{
    return QDir::cleanPath (projectFolder) + QDir::separator () + PROJECT_DES_XML_FILE_NAME;
}
///
/// \brief 获取工程的数据文件目录
/// \param projectFolder 当前工程的顶层目录
/// \param autoMakePath 此项为true，如果目录不存在会自动创建目录
/// \return
///
QString SAProjectManager::getProjectDataFolderPath(const QString &projectFolder,bool autoMakePath)
{
    return getProjectSubFolderPath(projectFolder,DATA_FOLDER_NAME,autoMakePath);
}
///
/// \brief  获取工程的数据文件目录
/// \param autoMakePath 此项为true，如果目录不存在会自动创建目录
/// \return
///
QString SAProjectManager::getProjectDataFolderPath(bool autoMakePath)
{
    return getProjectDataFolderPath(getProjectFullPath(),autoMakePath);
}
///
/// \brief 获取工程的图表文件目录
/// \param projectFolder 当前工程的顶层目录
/// \param autoMakePath 此项为true，如果目录不存在会自动创建目录
/// \return
///
QString SAProjectManager::getProjectSubWindowFolderPath(const QString &projectFolder, bool autoMakePath)
{
    return getProjectSubFolderPath(projectFolder,SUB_WINDOW_FOLDER_NAME,autoMakePath);
}
///
/// \brief 获取工程的图表文件目录
/// \param autoMakePath 此项为true，如果目录不存在会自动创建目录
/// \return
///
QString SAProjectManager::getProjectSubWindowFolderPath(bool autoMakePath)
{
    return getProjectSubWindowFolderPath(getProjectFullPath(),autoMakePath);
}
///
/// \brief 获取工程的临时目录
/// \param projectFolder 当前工程的顶层目录
/// \param autoMakePath 此项为true，如果目录不存在会自动创建目录
/// \return
///
QString SAProjectManager::getProjectTempFolderPath(const QString &projectFolder, bool autoMakePath)
{
    return getProjectSubFolderPath(projectFolder,TEMP_FOLDER_NAME,autoMakePath);
}
///
/// \brief 获取工程的临时目录
/// \param autoMakePath 此项为true，如果目录不存在会自动创建目录
/// \return
///
QString SAProjectManager::getProjectTempFolderPath(bool autoMakePath)
{
    return getProjectTempFolderPath(getProjectFullPath(),autoMakePath);
}
///
/// \brief 获取数据对应的文件路径,如果已经保存了的话，
/// \param dataPtr 数据指针
/// \return 返回的路径，如果没有，返回一个QString()
///
//QString SAProjectManager::getDataFilePath(const SAAbstractDatas *dataPtr) const
//{
//    SA_DC(SAProjectManager);
//    return d->m_dataPtr2DataFileName.value(const_cast<SAAbstractDatas *>(dataPtr));
//}
///
/// \brief 添加保存时的额外动作
/// \param fun 函数指针
///
void SAProjectManager::addSaveFunctionAction(SAProjectManager::FunAction fun)
{
    d_ptr->m_funcSaveActionList.append(fun);
}
///
/// \brief 获取保存额外动作的函数指针列表
/// \return
///
const QList<SAProjectManager::FunAction>& SAProjectManager::getSaveFunctionList() const
{
    return d_ptr->m_funcSaveActionList;
}
///
/// \brief 移除保存的额外动作函数指针
/// \param fun
///
void SAProjectManager::removeSaveFunctionAction(SAProjectManager::FunAction fun)
{
    for(int i=0;i<d_ptr->m_funcSaveActionList.size();++i)
    {
        if(d_ptr->m_funcSaveActionList[i].target<void(SAProjectManager*)>() == fun.target<void(SAProjectManager*)>())
        {
            d_ptr->m_funcSaveActionList.removeAt(i);
            return;
        }
    }
}
///
/// \brief 添加加载时的额外动作
/// \param fun
///
void SAProjectManager::addLoadFunctionAction(SAProjectManager::FunAction fun)
{
    d_ptr->m_funcLoadActionList.append(fun);
}
///
/// \brief 获取加载额外动作的函数指针列表
/// \return
///
const QList<SAProjectManager::FunAction> &SAProjectManager::getLoadFunctionList() const
{
    return d_ptr->m_funcLoadActionList;
}
///
/// \brief  移除加载的额外动作函数指针
/// \param fun
///
void SAProjectManager::removeLoadFunctionAction(SAProjectManager::FunAction fun)
{
    for(int i=0;i<d_ptr->m_funcLoadActionList.size();++i)
    {
        if(d_ptr->m_funcLoadActionList[i].target<void(SAProjectManager*)>() == fun.target<void(SAProjectManager*)>())
        {
            d_ptr->m_funcLoadActionList.removeAt(i);
            return;
        }
    }
}

void SAProjectManager::setupUI(SAUIInterface *ui)
{
    d_ptr->m_ui = ui;
}

SAUIInterface *SAProjectManager::ui()
{
    return d_ptr->m_ui;
}
///
/// \brief 获取projectFolder目录下的sub的完整路径，就是projectFolder/sub 如果没有回创建这个目录
/// \param projectFolder
/// \param sub
/// \return
///
QString SAProjectManager::getProjectSubFolderPath(const QString &projectFolder, const QString &sub, bool autoMakePath)
{
    if(projectFolder.isEmpty())
    {
        return QString();
    }
    QString dataPath = QDir::cleanPath (projectFolder) + QDir::separator () + sub;
    if(autoMakePath)
    {
        QDir dir(dataPath);
        if(!dir.exists())
        {
            if(!dir.mkdir (dataPath))
            {
                return QString();
            }
        }
    }
    return dataPath;
}


///
/// \brief 获取工程的名称
/// \return
///
QString SAProjectManager::getProjectName() const
{
    SA_DC(SAProjectManager);
    return d->m_projectName;
}
///
/// \brief 设置工程的名称
/// \param projectName
/// \see getProjectName
///
void SAProjectManager::setProjectName(const QString &projectName)
{
    SA_D(SAProjectManager);
    d->m_projectName = projectName;
}
///
/// \brief 另存工程
/// 保存工程包括在目录下
/// \param savePath 另存的路径
/// \return 成功另存，返回true
///
bool SAProjectManager::saveAs(const QString &savePath)
{
    //验证目录正确性
    QDir dir(savePath);
    if(!dir.exists ())
    {
        if(!dir.mkdir (savePath))
        {
            emit messageInformation(tr("can not make dir:%1").arg(savePath)
                                    ,SA::ErrorMessage
                                    );
            return false;
        }
    }

    //保存项目描述
    saveProjectInfo(savePath);
    //删除要移除的数据
    removeNonExistDatas(savePath);
    //保存数据
    saveValues(savePath);


    setProjectFullPath(savePath);
    std::for_each(d_ptr->m_funcSaveActionList.begin(),d_ptr->m_funcSaveActionList.end()
                  ,[this](FunAction fun){
        fun(this);
    });
    setDirty(false);
    emit messageInformation(tr("success save project:\"%1\" ").arg(savePath)
                            ,SA::NormalMessage
                            );
    return true;
}
///
/// \brief 加载工程
/// \param projectedPath 工程路径
/// \return 成功加载返回true
///
bool SAProjectManager::load(const QString &projectedPath)
{
    SA_D(SAProjectManager);
    //验证目录正确性
    QDir dir(projectedPath);
    if(!dir.exists ())
    {
        emit messageInformation(tr("can not find dir:%1").arg(projectedPath)
                                ,SA::ErrorMessage
                                );
        return false;
    }
    //- start %加载项目描述
    QStringList valNameList;
    loadProjectInfo(projectedPath,valNameList);

    //加载变量
    loadValues(projectedPath,valNameList);


    setProjectFullPath(projectedPath);
    std::for_each(d_ptr->m_funcLoadActionList.begin(),d_ptr->m_funcLoadActionList.end()
                  ,[this](FunAction fun){
       fun(this);
    });
    setDirty(false);
    return true;
}

bool SAProjectManager::isDirty() const
{
    SA_DC(SAProjectManager);
    return d->m_isdirty;
}

void SAProjectManager::setDirty(bool isdirty)
{
    SA_D(SAProjectManager);
    d->m_isdirty = isdirty;
}
