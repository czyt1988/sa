#include "SAProjectManager.h"
#include "SAValueManager.h"
#include <QDir>
#include <QTextStream>
#include <QDomDocument>
#define VERSION_STRING "pro.0.0.1"
#define PROJECT_DES_XML_FILE_NAME "saProject.prodes"
#define DATA_FOLDER_NAME "DATA"
SAProjectManager* SAProjectManager::s_instance = nullptr;

SAProjectManager::SAProjectManager():QObject(nullptr)
{

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
    return m_projectFullPath;
}
///
/// \brief 保存工程
/// \return
///
bool SAProjectManager::save()
{
    if(m_projectFullPath.isNull())
    {
        return false;
    }
    saveAs(m_projectFullPath);
}
///
/// \brief 设置项目的路径
/// \param projectFullPath
///
void SAProjectManager::setProjectFullPath(const QString &projectFullPath)
{
    m_projectFullPath = projectFullPath;
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

}

QString SAProjectManager::getProjectDescribe() const
{
    return m_projectDescribe;
}

void SAProjectManager::setProjectDescribe(const QString &projectDescribe)
{
    m_projectDescribe = projectDescribe;
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
QString SAProjectManager::getProjectDataFolderPath(const QString &projectFolder)
{
    return QDir::cleanPath (projectFolder) + QDir::separator () + DATA_FOLDER_NAME;
}

QString SAProjectManager::getProjectName() const
{
    return m_projectName;
}

void SAProjectManager::setProjectName(const QString &projectName)
{
    m_projectName = projectName;
}
///
/// \brief 另存工程
/// 保存工程包括在目录下
/// \param savePath 另存的路径
/// \return 成功另存，返回true
///
bool SAProjectManager::saveAs(const QString &savePath)
{
    //---------------------------------------
    //- start %验证目录正确性
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
    // - end %验证目录正确性结束

    //- start %保存项目描述
    saveProjectInfo(savePath);



    //-----------------------------------------
    //- start %保存数据
    QString dataPath = getProjectDataFolderPath(savePath);
    if(!QFileInfo::exists(dataPath))
    {
        if(!dir.mkdir (dataPath))
        {
            emit messageInformation(tr("can not make dir:%1").arg(dataPath)
                                    ,SA::ErrorMessage
                                    );
            return false;
        }
    }
    saValueManager->saveAs(dataPath);
    // - end %保存数据结束

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
    //1.---------------------------------------
    //- start %验证目录正确性
    QDir dir(projectedPath);
    if(!dir.exists ())
    {
        emit messageInformation(tr("can not find dir:%1").arg(projectedPath)
                                ,SA::ErrorMessage
                                );
        return false;
    }
    // - end %验证目录正确性结束


    //- start %加载项目描述
    loadProjectInfo(projectedPath);


    //3.-----------------------------------------
    //- start %加载数据数据
    QString dataPath = getProjectDataFolderPath(projectedPath);
    if(QFileInfo::exists(dataPath))
    {
        saValueManager->clear();
        int r = saValueManager->load(dataPath);
        if(0 != r)
        {
            return false;
        }
    }
    // - end %加载数据结束

    setProjectFullPath(projectedPath);
    setDirty(false);
    return true;
}

bool SAProjectManager::isdirty() const
{
    return m_isdirty;
}

void SAProjectManager::setDirty(bool isdirty)
{
    m_isdirty = isdirty;
}
