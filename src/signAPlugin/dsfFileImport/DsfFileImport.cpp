#include "DsfFileImport.h"
#include "DsfFileImportFactory.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include "SAValueManager.h"
#include "SAVectorPointF.h"
#include "SAUIReflection.h"

#define DSF_WAVE_TYPE (100)
struct DSF_Header{
    int KindOfData;
    float RMSValue;
    float PpValue;
    float KurValue;
    int SampleLen;
    int SampleFre;
    short Gain;
    short Filter;
    short OutTrigger;
    short Integral;
};



#define TR(str)\
    QApplication::translate("DsfFileImport",str,0)
static const QString s_pluginName = TR("Dsf File Import");
static const QString s_pluginDescription =  TR("Import BUCT DSE106 dsf file");
static const QString s_pluginVersion = TR("1.0");
static const SAAbstractPlugin::PluginType s_pluginType = SAAbstractPlugin::DataImport;

SA_PLUGIN_EXPORT SAAbstractPlugin *createPlugin()
{
    return new DsfFileImport();
}
SA_PLUGIN_EXPORT void deletePlugin(SAAbstractPlugin *obj)
{
    if(obj != nullptr)
    {
        delete obj;
    }
}
SA_PLUGIN_EXPORT QString pluginName()
{
    return s_pluginName;
}
SA_PLUGIN_EXPORT QString pluginVersion()
{
    return s_pluginVersion;
}
SA_PLUGIN_EXPORT QString pluginDescription()
{
    return s_pluginDescription;
}
SA_PLUGIN_EXPORT SAAbstractPlugin::PluginType pluginType()
{
    return s_pluginType;
}

DsfFileImport::DsfFileImport()
{
    m_isOpenFile = false;
}

DsfFileImport::~DsfFileImport()
{

}

SAAbstractPlugin::PluginType DsfFileImport::getType() const
{
    return s_pluginType;
}

QString DsfFileImport::getPluginName() const
{
    return s_pluginName;
}

QString DsfFileImport::getVersion() const
{
    return s_pluginVersion;
}

QString DsfFileImport::getDescription() const
{
    return s_pluginDescription;
}

void DsfFileImport::setupUI(SAUIInterface *ui)
{
    m_ui = ui;
}
///
/// \brief isUseSAOpenFile返回false调用的打开文件接口函数
/// \return 成功返回true,将调用createResultPtr
///
bool DsfFileImport::openFile()
{
    QFileDialog dlg(m_ui->getMainWindowPtr());
    QStringList strNFilter;
    strNFilter.push_back(tr("dsf file (*.dsf)"));
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilters(strNFilter);
    if (QDialog::Accepted == dlg.exec())
    {
        QStringList strfileNames = dlg.selectedFiles();
        return openFile(strfileNames);
    }
    return false;
}
///
/// \brief DsfFileImport::openFile
/// \param filePaths
/// \return
///
bool DsfFileImport::openFile(const QStringList &filePaths)
{
    m_resPtr.clear();
    const int size = filePaths.size();
    int readCount = 0;
    QString strRes;
    for(int i=0;i<size;++i)
    {
        QFile file(filePaths[i]);
        if(!file.open(QIODevice::ReadOnly))
        {
            m_ui->showWarningMessageInfo(tr("can not open file:\"%1\" ,because:%2").arg(filePaths[i]).arg(file.errorString()));
            continue;
        }
        file.seek(0);
        QDataStream st(&file);
        int type;
        if(4 != st.readRawData((char*)&type,4))
        {
            m_ui->showWarningMessageInfo(tr("file data invalid:\"%1\"").arg(filePaths[i]));
            continue;
        }
        if(DSF_WAVE_TYPE != type || file.size() < 640)
        {
            m_ui->showWarningMessageInfo(tr("dsf file is not wave type:\"%1\"").arg(filePaths[i]));
            continue;
        }
        file.seek(608);
        //读文件头
        DSF_Header header;
        if(sizeof(DSF_Header) != st.readRawData((char*)(&header),sizeof(DSF_Header)))
        {
            m_ui->showWarningMessageInfo(tr("dsf file invalid:\"%1\",can not read wave header").arg(filePaths[i]));
            continue;
        }
        if(header.SampleLen<=0 || header.SampleLen > 1e8)
        {
            m_ui->showWarningMessageInfo(tr("dsf file invalid:\"%1\",SampleLen:%2").arg(filePaths[i]).arg(header.SampleLen));
            continue;
        }
        file.seek(640);
        QVector<float> y;
        y.resize(header.SampleLen);
        const int totalReadWaveSize = sizeof(float)*header.SampleLen;
        int readedSize = 0;
        while(!st.atEnd())
        {
            int r = st.readRawData((char*)(y.data()+readedSize),totalReadWaveSize-readedSize);
            readedSize += r;
            if(readedSize == totalReadWaveSize)
            {
                break;
            }
        }
        if(readedSize != totalReadWaveSize)
        {
            m_ui->showWarningMessageInfo(tr("dsf file invalid:\"%1\",data length error").arg(filePaths[i]));
            continue;
        }
        //生成x值
        QVector<QPointF> wave;
        wave.resize(header.SampleLen);
        float dt = 1.0/header.SampleFre;
        for(int i=0;i<header.SampleLen;++i)
        {
            float x = dt*i;
            wave[i] = QPointF(x,y[i]);
        }
        if(wave.size() <= 0)
        {
            m_ui->showWarningMessageInfo(tr("invalid wave data:\"%1\"").arg(filePaths[i]));
            continue;
        }
        QFileInfo fi(filePaths[i]);
        auto p = SAValueManager::makeData<SAVectorPointF>(fi.baseName(),wave);
        saValueManager->addData(SAValueManager::castPointToBase(p));
        m_resPtr.append(SAValueManager::castPointToBase(p));
        ++readCount;
        strRes += "[" + fi.baseName() + "]";
    }
    if(readCount > 0)
    {
        m_ui->showNormalMessageInfo(tr("success import dsf files:%1").arg(strRes));
        return true;
    }
    return false;
}
///
/// \brief 是否打开了文件
/// \return
///
bool DsfFileImport::isOpenFile()
{
    return m_isOpenFile;
}

bool DsfFileImport::isUseSAOpenFile() const
{
    return true;
}

void DsfFileImport::getSAOpenFileInfo(QStringList &nameFileter, QStringList &suffixKey) const
{
    nameFileter.push_back(tr("dsf file (*.dsf)"));
    suffixKey.push_back("dsf");
}

QList<SAAbstractDatas *> DsfFileImport::getResultPtr()
{
    QList<SAAbstractDatas *> res;
    std::for_each(m_resPtr.cbegin(),m_resPtr.cend(),[&res](const std::shared_ptr<SAAbstractDatas>& d){
        res << d.get();
    });
    return res;
}


