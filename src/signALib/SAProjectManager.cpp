#include "SAProjectManager.h"
#include "SAValueManager.h"
#include <QDir>
#include <QTextStream>
#include <QDomDocument>
#define VERSION_STRING "pro.0.0.1"
#define PROJECT_DES_XML_FILE_NAME "saProject.prodes"
#define DATA_FOLDER_NAME "DATA"
SAProjectManager* SAProjectManager::s_instance = nullptr;


class SAProjectManagerPrivate
{
    SA_IMPL_PUBLIC(SAProjectManager)
public:
    QString m_projectFullPath;///< 项目对应路径
    QString m_projectName;///< 项目名
    QString m_projectDescribe;///< 项目描述
    bool m_isdirty;///< 工程变更标记
    //--记录文件和数据相互的映射
    QMap<QString,SAAbstractDatas*> m_dataFileName2DataPtr;///< 记录数据文件路径
    QMap<SAAbstractDatas*,QString> m_dataPtr2DataFileName;///< 记录数据指针对应的保存文件名
    //end
    QSet<QString> m_dataWillBeDeleted;///< 记录将要删除的数据文件
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
/// \brief 加载项目信息
/// \param projectFullPath 项目文件夹
/// \return 成功加载返回true
///
bool SAProjectManager::loadProjectInfo(const QString &projectFullPath)
{

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
    if(root.tagName() == "SaProject")
    {
        QString ver = root.attribute("version");

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
            node = node.nextSibling();//读取下一个兄弟节点
        }
    }
    return true;
}

void SAProjectManager::loadValues(const QString &projectFullPath)
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
    d->m_dataPtr2DataFileName.clear();
    d->m_dataFileName2DataPtr.clear();
    saValueManager->load(dataPath,d->m_dataPtr2DataFileName);
    for(auto i = d->m_dataPtr2DataFileName.begin();i != d->m_dataPtr2DataFileName.end();++i)
    {
        d->m_dataFileName2DataPtr[i.value()] = i.key();
    }
    return;
}
///
/// \brief 保存变量
/// \param projectFullPath
/// \return
///
bool SAProjectManager::saveValues(const QString &projectFullPath)
{
    QString dataPath = getProjectDataFolderPath(projectFullPath);
    if(dataPath.isEmpty())
    {
        emit messageInformation(tr("can not make dir:%1").arg(dataPath)
                                ,SA::ErrorMessage
                                );
        return false;
    }

    //保存之前，先对变量文件夹和变量进行比对，对已经变更名字的变量进行处理

    int r = saValueManager->saveAs(dataPath);
    return (0 == r);
}
///
/// \brief 移除记录的要删除的数据
///
void SAProjectManager::removeWillDeletedFiles()
{
    SA_D(SAProjectManager);
    for(auto i = d->m_dataWillBeDeleted.begin();i!=d->m_dataWillBeDeleted.end();++i)
    {
        if(QFile::exists(*i))
        {
            QFile::remove(*i);
        }
    }
    d->m_dataWillBeDeleted.clear();
}
///
/// \brief 数据管理器移除数据时
///
/// 记录移除数据对应的文件，保存时把这些文件删除
/// \param dataBeDeletedPtr 移除的数据文件
///
void SAProjectManager::onDataRemoved(const QList<SAAbstractDatas *> &dataBeDeletedPtr)
{
    SA_D(SAProjectManager);
    for(auto i = dataBeDeletedPtr.begin();i!=dataBeDeletedPtr.end();++i)
    {
        auto dataPtr2PathIte = d->m_dataPtr2DataFileName.find(*i);
        if(dataPtr2PathIte != d->m_dataPtr2DataFileName.end())
        {
            d->m_dataWillBeDeleted.insert(dataPtr2PathIte.value());
            auto dataPath2Ptr = d->m_dataFileName2DataPtr.find(dataPtr2PathIte.value());
            if(dataPath2Ptr != d->m_dataFileName2DataPtr.end())
            {
                d->m_dataFileName2DataPtr.erase(dataPath2Ptr);
            }
            d->m_dataPtr2DataFileName.erase(dataPtr2PathIte);
        }
    }
    setDirty(true);
}
///
/// \brief 数据管理器进行清除时触发的槽
///
void SAProjectManager::onDataClear()
{
    SA_D(SAProjectManager);
    //变量管理器的数据进行了清除
    //把所有已经记录的数据文件映射标记位删除状态
    for(auto i = d->m_dataFileName2DataPtr.begin();i != d->m_dataFileName2DataPtr.end();++i)
    {
        d->m_dataWillBeDeleted.insert(i.key());
    }
    setDirty(true);
}
///
/// \brief 变量名更改
/// \param data 变量数据指针
/// \param oldName 旧的名字
///
void SAProjectManager::onDataNameChanged(SAAbstractDatas *data, const QString &oldName)
{
    SA_D(SAProjectManager);
    setDirty(true);
    if(d->m_projectFullPath.isEmpty())
    {
        //说明没指定项目位置
        return;
    }
    QString dataPath = getProjectDataFolderPath(d->m_projectFullPath);
    if(dataPath.isEmpty())
    {
        emit messageInformation(tr("can not make dir:%1").arg(dataPath)
                                ,SA::ErrorMessage
                                );
        return;
    }
    QString oldFilePath = dataPath + QDir::separator() + oldName;
    QString newFilePath = dataPath + QDir::separator() + data->getName();
    if(!QFile::exists(oldFilePath))
    {
        return;
    }
    QFile::rename(oldFilePath,newFilePath);
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
/// \return
///
QString SAProjectManager::getProjectDataFolderPath(const QString &projectFolder,bool autoMakePath)
{
    QString dataPath = QDir::cleanPath (projectFolder) + QDir::separator () + DATA_FOLDER_NAME;
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
/// \brief 获取数据对应的文件路径,如果已经保存了的话，
/// \param dataPtr 数据指针
/// \return 返回的路径，如果没有，返回一个QString()
///
QString SAProjectManager::getDataFilePath(const SAAbstractDatas *dataPtr) const
{
    SA_DC(SAProjectManager);
    return d->m_dataPtr2DataFileName.value(const_cast<SAAbstractDatas *>(dataPtr));
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
    removeWillDeletedFiles();
    //保存数据
    saveValues(savePath);


    setProjectFullPath(savePath);
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
    //- 本地变量初始化
    d->m_dataWillBeDeleted.clear();

    //- start %加载项目描述
    loadProjectInfo(projectedPath);

    //加载变量
    loadValues(projectedPath);


    setProjectFullPath(projectedPath);
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
