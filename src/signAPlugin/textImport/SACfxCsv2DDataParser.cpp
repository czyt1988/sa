#include "SACfxCsv2DDataParser.h"
#include <QStringList>
#include "SACsvParser.h"
#include <algorithm>
#include <memory>
#include <QVector>
#include <QStringList>
#include <QDebug>
#include "SAVectorPointF.h"
#include "SAVectorDouble.h"
SACfxCsv2DDataParser::SACfxCsv2DDataParser(QObject *par):SACsvParser(par)
  ,m_rowCount(0)
{
}

bool SACfxCsv2DDataParser::parser()
{
    QFile* file = getFilePtr ();
    if(nullptr == file)
        return false;
    m_dataSeries.clear ();
    file->seek (0);
    QTextStream inStream(file);
    QString strLine;
    QString strTemp;
    double x,y;
    bool isOK;
    QStringList splitLine;
    do{
        strLine = inStream.readLine();
        splitLine = SACsvParser::fromCsvLine(strLine);
        if(splitLine.size () <= 0)
            continue;
        if("[Name]" == splitLine[0])
            break;//说明遇到"[Name]"
    }while(!inStream.atEnd());
    do
    {
        QString name = inStream.readLine();//读取[Name]的下一行
        std::shared_ptr<dataContainer> seriesData = std::make_shared<dataContainer>();
        seriesData->m_name = name;
        inStream.readLine();//读取下一行
        strTemp = inStream.readLine();//读取下一行 - [Data]
        qDebug()<<strTemp;
        if("[Data]" != strTemp)
            continue;
        strLine = inStream.readLine();//标题
        qDebug()<<strLine;
        splitLine = SACsvParser::fromCsvLine(strLine);
        qDebug()<<splitLine;
        if(splitLine.size ()>=2)
        {
            seriesData->m_xName = splitLine[0];
            seriesData->m_yName = splitLine[1];
        }
        //开始读取数据
        while (!inStream.atEnd())
        {
            strLine = inStream.readLine();
            splitLine = SACsvParser::fromCsvLine(strLine);
            if(splitLine.size ()>=2)
            {
                x = splitLine[0].toDouble (&isOK);
                if(!isOK)
                    continue;
                y = splitLine[1].toDouble (&isOK);
                if(!isOK)
                    continue;
                seriesData->m_series.push_back (QPointF(x,y));
            }
            else if(1 == splitLine.size ())
            {
                if("[Name]" == splitLine[0])
                    break;
            }
        }
        if(m_rowCount < (seriesData->m_series.size ()) )
            m_rowCount = seriesData->m_series.size ();//记录行数
        m_dataSeries.push_back (seriesData);
    }while(!inStream.atEnd());
    return true;
}

QString SACfxCsv2DDataParser::getAt(int row, int column) const
{
    if(column > m_dataSeries.size ())
        return QString();
    if(row > m_dataSeries[column]->m_series.size ())
        return QString();
    const QPointF& f = m_dataSeries[column]->m_series[row];
    return QString("%1,%2").arg(f.x ()).arg(f.y ());
}

int SACfxCsv2DDataParser::getSeriesSize(int col) const
{
    if(col > m_dataSeries.size ())
        return -1;
    return m_dataSeries[col]->m_series.size();
}

QVector<QPointF> &SACfxCsv2DDataParser::getSeries(int col)
{
    return m_dataSeries[col]->m_series;
}

const QVector<QPointF> &SACfxCsv2DDataParser::getSeries(int col) const
{
    return m_dataSeries[col]->m_series;
}


bool SACfxCsv2DDataParser::getSeries(int col, SAVectorPointF *series)
{
    if(col > m_dataSeries.size ())
        return false;
    series->setValueDatas(m_dataSeries[col]->m_series);
    series->setName(m_dataSeries[col]->m_name);
    return true;
}

QString SACfxCsv2DDataParser::getSeriesName(int col)
{
    return m_dataSeries[col]->m_name;
}

QString SACfxCsv2DDataParser::getSeriesXName(int col)
{
    return m_dataSeries[col]->m_xName;
}

QString SACfxCsv2DDataParser::getSeriesYName(int col)
{
    return m_dataSeries[col]->m_yName;
}

bool SACfxCsv2DDataParser::getSeries(int col, QVector<QPointF>& series)
{
    if(col > m_dataSeries.size ())
        return false;
    series = m_dataSeries[col]->m_series;
    return true;
}
///
/// \brief 获取x值
/// \param col 对应的系列索引
/// \param series SAVectorDouble的指针
/// \return 成功返回true
/// \note 会设置x的名给系列
///
bool SACfxCsv2DDataParser::getXSeries(int col, SAVectorDouble *series)
{
    if(col > m_dataSeries.size ())
        return false;
    QVector<QPointF>& points = m_dataSeries[col]->m_series;
    std::for_each(points.begin(),points.end(),[series](QPointF& p){
        series->push_back(p.x());
    });
    series->setName(m_dataSeries[col]->m_xName);
    return true;
}
///
/// \brief 获取x值
/// \param col 对应的系列索引
/// \param series
/// \return
///
bool SACfxCsv2DDataParser::getXSeries(int col, QVector<double> &series)
{
    if(col > m_dataSeries.size ())
        return false;
    QVector<QPointF>& points = m_dataSeries[col]->m_series;
    std::for_each(points.begin(),points.end(),[&series](QPointF& p){
        series.push_back(p.x());
    });
    return true;
}

bool SACfxCsv2DDataParser::getYSeries(int col, SAVectorDouble *series)
{
    if(col > m_dataSeries.size ())
        return false;
    QVector<QPointF>& points = m_dataSeries[col]->m_series;
    std::for_each(points.begin(),points.end(),[series](QPointF& p){
        series->push_back(p.y());
    });
    series->setName(m_dataSeries[col]->m_yName);
    return true;
}

bool SACfxCsv2DDataParser::getYSeries(int col, QVector<double> &series)
{
    if(col > m_dataSeries.size ())
        return false;
    QVector<QPointF>& points = m_dataSeries[col]->m_series;
    std::for_each(points.begin(),points.end(),[&series](QPointF& p){
        series.push_back(p.y());
    });
    return true;
}

bool SACfxCsv2DDataParser::getXYSeries(int col, SAVectorDouble *x, SAVectorDouble *y)
{
    if(col > m_dataSeries.size ())
        return false;
    QVector<QPointF>& points = m_dataSeries[col]->m_series;
    std::for_each(points.begin(),points.end(),[x,y](QPointF& p){
        x->push_back(p.x());
        y->push_back(p.y());
    });
    x->setName(m_dataSeries[col]->m_xName);
    y->setName(m_dataSeries[col]->m_yName);
    return true;
}

int SACfxCsv2DDataParser::getRowCount() const
{
    return m_rowCount;
}

int SACfxCsv2DDataParser::getColumnCount() const
{
    return m_dataSeries.size ();
}
