#include "CfxCsvDataImportConfig.h"
#include "SACfxCsv2DDataParser.h"
#include "SALog.h"
#include "SAValueManager.h"
#include <QTextCodec>
CfxCsvDataImportConfig::CfxCsvDataImportConfig(QObject *par):QObject(par)
    ,m_isOpenFile(false)
  ,m_exportWay(ToVectorDouble_1XnY)
{
    m_codex = QString(QTextCodec::codecForLocale()->name());
}

CfxCsvDataImportConfig::~CfxCsvDataImportConfig()
{

}

///
/// \brief 打开cfx导出的csv文件，可打开一个也可打开多个
/// \param path 文件路径
/// \return 若没有成功打开一个，返回false
///
bool CfxCsvDataImportConfig::openFiles(const QList<QString> &path)
{
    bool suc = false;

    for(int i=0;i<path.size();++i)
    {
        PARSER parser = std::make_shared<SACfxCsv2DDataParser>();
        if(!parser->open(path[i]))
            continue;
        m_pasers.push_back(parser);
        suc = true;
    }
    m_isOpenFile = suc;
    return suc;
}

bool CfxCsvDataImportConfig::isOpenFile()
{
    return m_isOpenFile;
}
///
/// \brief 获取数据，此操作会把所有csv的数据导出为点数组
/// \return
///
QList<std::shared_ptr<SAAbstractDatas> > CfxCsvDataImportConfig::createResultPtr()
{
    QList<std::shared_ptr<SAAbstractDatas> > res;
    switch(m_exportWay)
    {
    case ToVectorDouble_1XnY:
        createBy1XnYWay(res);
        break;
    case ToVectorDouble_nX1Y:
        createBynX1YWay(res);
        break;
    case ToVectorDouble_nXnY:
        createBynXnYWay(res);
        break;
    case toVectorPointF:
        createByPointFWay(res);
        break;
    }
    return res;
}
///
/// \brief 解析所有
/// \return 若有一个成功返回true
///
bool CfxCsvDataImportConfig::parser()
{
    bool suc;
    m_successFile.clear();
    for(int i=0;i<m_pasers.size();++i)
    {
        bool r = m_pasers[i]->parser();
        m_successFile.push_back(r);
        suc |= r;
    }
    return suc;
}

SACfxCsv2DDataParser *CfxCsvDataImportConfig::getParser(int index) const
{
    return m_pasers[index].get();
}
///
/// \brief 获取设定的文件数
/// \return
///
int CfxCsvDataImportConfig::getFileCount() const
{
    return m_pasers.size();
}
///
/// \brief 鉴定是否解析成功 O(1)
/// \param index 索引
/// \return 成功返回true
///
bool CfxCsvDataImportConfig::isSuccessParser(int index) const
{
    return m_successFile[index];
}
///
/// \brief 设置导出方式，默认为ToVectorDouble_1XnY
/// \param way 导出方式
/// \see ExportWay
///
void CfxCsvDataImportConfig::setExportWay(CfxCsvDataImportConfig::ExportWay way)
{
    m_exportWay = way;
}

void CfxCsvDataImportConfig::clear()
{
    m_pasers.clear ();
    m_successFile.clear ();
    m_isOpenFile = false;
}

void CfxCsvDataImportConfig::setCodec(const QString &codex)
{
    m_codex = codex;
    for(int i=0;i<m_pasers.size();++i)
    {
        getParser(i)->setCodec(QTextCodec::codecForName( codex.toLocal8Bit()));
    }
}

QString CfxCsvDataImportConfig::getCodec() const
{
    return m_codex;
}
///
/// \brief 把解析的数据转换为1行x，n行y的形式，此时默认所有的数据x值都一样，将只获取第一列的x值
/// \param res
///
void CfxCsvDataImportConfig::createBy1XnYWay(QList<std::shared_ptr<SAAbstractDatas> > &res)
{
    const int count = getFileCount();
    for(int i=0;i<count;++i)
    {
        SACfxCsv2DDataParser* parser = getParser(i);
        if(nullptr == parser)
            continue;
        int colCount = parser->getColumnCount();//获取列数，代表解析的数据系列数目
        for(int seriesCount = 0;seriesCount<colCount;++seriesCount)
        {
            if(0 == seriesCount && 0 == i)//第一个文件的第一列，读取x
            {
                std::shared_ptr<SAVectorDouble> x = SAValueManager::makeData<SAVectorDouble>();
                if(parser->getXSeries(0,x.get()))
                {
                    QString xName = parser->getSeriesXName(0);
                    int atIndex = xName.lastIndexOf('@');
                    if(atIndex > 0 && atIndex != xName.size()-1)
                        xName = xName.mid(atIndex+1,xName.size()-1);
                    xName += QString("-X");
                    x->setName(xName);
                    res.append(x);
                }
            }
            //处理y值
            std::shared_ptr<SAVectorDouble> y = SAValueManager::makeData<SAVectorDouble>();
            if(parser->getYSeries(seriesCount,y.get()))
            {
                QString yName = parser->getSeriesYName(seriesCount);
                int atIndex = yName.lastIndexOf('@');
                if(atIndex > 0 && atIndex != yName.size()-1)
                    yName = yName.mid(atIndex+1,yName.size()-1);
                yName += QString("-Y");
                y->setName(yName);
                res.append(y);
            }
        }
    }
}
///
/// \brief 把解析的数据转换为1行y，n行x的形式，此时默认所有的数据y值都一样，将只获取第一列的y值
/// \param res
///
void CfxCsvDataImportConfig::createBynX1YWay(QList<std::shared_ptr<SAAbstractDatas> > &res)
{
    const int count = getFileCount();
    for(int i=0;i<count;++i)
    {
        SACfxCsv2DDataParser* parser = getParser(i);
        if(nullptr == parser)
            continue;
        int colCount = parser->getColumnCount();//获取列数，代表解析的数据系列数目
        for(int seriesCount = 0;seriesCount<colCount;++seriesCount)
        {
            if(0 == seriesCount && 0 == i)//第一个文件的第一列，读取y
            {
                std::shared_ptr<SAVectorDouble> y = SAValueManager::makeData<SAVectorDouble>();
                if(parser->getYSeries(0,y.get()))
                {
                    QString yName = parser->getSeriesYName(0);
                    int atIndex = yName.lastIndexOf('@');
                    if(atIndex > 0 && atIndex != yName.size()-1)
                        yName = yName.mid(atIndex+1,yName.size()-1);
                    yName += QString("-Y");
                    y->setName(yName);
                    res.append(y);
                }
            }
            //处理y值
            std::shared_ptr<SAVectorDouble> x = SAValueManager::makeData<SAVectorDouble>();
            if(parser->getXSeries(seriesCount,x.get()))
            {
                QString xName = parser->getSeriesXName(seriesCount);
                int atIndex = xName.lastIndexOf('@');
                if(atIndex > 0 && atIndex != xName.size()-1)
                    xName = xName.mid(atIndex+1,xName.size()-1);
                xName += QString("-X");
                x->setName(xName);
                res.append(x);
            }
        }
    }
}

void CfxCsvDataImportConfig::createBynXnYWay(QList<std::shared_ptr<SAAbstractDatas> > &res)
{
    const int count = getFileCount();
    for(int i=0;i<count;++i)
    {
        SACfxCsv2DDataParser* parser = getParser(i);
        if(nullptr == parser)
            continue;
        int colCount = parser->getColumnCount();//获取列数，代表解析的数据系列数目
        for(int seriesCount = 0;seriesCount<colCount;++seriesCount)
        {
            std::shared_ptr<SAVectorDouble> x = SAValueManager::makeData<SAVectorDouble>();
            std::shared_ptr<SAVectorDouble> y = SAValueManager::makeData<SAVectorDouble>();
            if(parser->getXYSeries(seriesCount,x.get(),y.get()))
            {
                QString xName = parser->getSeriesXName(seriesCount);
                int atIndex = xName.lastIndexOf('@');
                if(atIndex > 0 && atIndex != xName.size()-1)
                    xName = xName.mid(atIndex+1,xName.size()-1);
                xName += QString("-X");
                QString yName = parser->getSeriesYName(seriesCount);
                atIndex = yName.lastIndexOf('@');
                if(atIndex > 0 && atIndex != yName.size()-1)
                    yName = yName.mid(atIndex+1,yName.size()-1);
                yName += QString("-Y");
                x->setName(xName);
                y->setName(yName);
                res.push_back(x);
                res.push_back(y);
            }
        }
    }

}

void CfxCsvDataImportConfig::createByPointFWay(QList<std::shared_ptr<SAAbstractDatas> > &res)
{
    const int count = getFileCount();
    for(int i=0;i<count;++i)
    {
        SACfxCsv2DDataParser* parser = getParser(i);
        if(nullptr == parser)
            continue;
        int colCount = parser->getColumnCount();//获取列数，代表解析的数据系列数目
        for(int seriesCount = 0;seriesCount<colCount;++seriesCount)
        {
            std::shared_ptr<SAVectorPointF> series = SAValueManager::makeData<SAVectorPointF>();
            if(parser->getSeries(seriesCount,series.get()))
            {
                QString name = parser->getSeriesName(seriesCount);
                int atIndex = name.lastIndexOf('@');
                if(atIndex > 0 && atIndex != name.size()-1)
                    name = name.mid(atIndex+1,name.size()-1);
                series->setName(name);
                res.push_back(series);
            }
        }
    }
}
