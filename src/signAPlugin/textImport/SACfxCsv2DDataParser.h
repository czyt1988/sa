#ifndef SACFXCSV2DDATAPARSER_H
#define SACFXCSV2DDATAPARSER_H
#include "SACsvParser.h"
#include <QPointF>
#include <QVector>
class SAVectorDouble;
class SAVectorPointF;
///
/// \brief 对ansys cfx导出的csv2d数据的解析
///
class SACfxCsv2DDataParser : public SACsvParser
{
public:
    SACfxCsv2DDataParser(QObject* par = nullptr);
    virtual ~SACfxCsv2DDataParser(){}
    //解析文本
    virtual bool parser();
    //列数
    int getColumnCount() const;
    //行数
    int getRowCount() const;

    QString getAt(int row,int column) const;

    int getSeriesSize(int col) const;
    //获取序列的引用
    QVector<QPointF> & getSeries(int col);
    const QVector<QPointF> & getSeries(int col) const;
    //获取序列的长度

    bool getSeries(int col,SAVectorPointF* series);
    bool getSeries(int col,QVector<QPointF>& series);   
    bool getXSeries(int col,SAVectorDouble* series);
    bool getXSeries(int col,QVector<double>& series);
    bool getYSeries(int col,SAVectorDouble* series);
    bool getYSeries(int col,QVector<double>& series);
    bool getXYSeries(int col,SAVectorDouble* x,SAVectorDouble* y);
    QString getSeriesName(int col);
    QString getSeriesXName(int col);
    QString getSeriesYName(int col);
private:
    class dataContainer{
    public:
        QString m_name;
        QString m_xName;
        QString m_yName;
        QVector<QPointF> m_series;
    };
    QVector<std::shared_ptr<dataContainer> > m_dataSeries;
    int m_rowCount;
};

#endif // SACFXCSV2DDATAPARSER_H
