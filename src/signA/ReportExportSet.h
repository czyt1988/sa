#ifndef REPORTEXPORTSET_H
#define REPORTEXPORTSET_H
#include <list>

class SAFigureWindow;
class ChartReportSet
{
public:
    ChartReportSet(){
        isShowCharacteristicValue = isShowCharacteristicPoint = isShowCharacteristicPointsArray
                =true;
        isShowPointsCount=isShowMaxValue=isShowMinValue
                =isShowMean=isShowSum=isShowVar=isShowStd
                =isShowSkewness=isShowKurtosis=isShowPeakPeakValue

                =isShowMaxPoint=isShowMinPoint

                =isShowTop=isShowLow=true;
        topCount = lowCount = 10;
    }
    SAFigureWindow* widget;///< 记录要导出的图表的指针
    bool isShowCharacteristicValue;///< 是否显示特征值
    bool isShowPointsCount;///< 是否显示数据点数
    bool isShowMean;///< 是否显示平均值
    bool isShowSum;///< 是否显示和
    bool isShowVar;///< 是否显示方差
    bool isShowStd;///< 是否显示标准差
    bool isShowSkewness;///< 是否显示斜度
    bool isShowKurtosis;///< 是否显示峭度
    bool isShowPeakPeakValue;///< 是否显示峰峰值
    bool isShowMaxValue;///< 是否显示最大值
    bool isShowMinValue;///< 是否显示最小值


    bool isShowCharacteristicPoint;///< 是否显示特征点
    bool isShowMaxPoint;///< 是否显示最大点
    bool isShowMinPoint;///< 是否显示最小点

    bool isShowCharacteristicPointsArray;///< 是否显示特征序列
    bool isShowTop;///< 是否显示最大的序列值
    size_t topCount;///< 最大的序列值的个数
    bool isShowLow;///< 是否显示最小的序列
    size_t lowCount;///< 最小序列值的个数
};

class ReportExportSet
{
public:
    ReportExportSet(){}
    QString title;///< 标题
    QString extraMessage;///< 附加信息
    std::list<ChartReportSet> chartSet;///< 每个图表的详细设置
    ///
    /// \brief 添加一个图表设置信息，除了图表指针，其他都是默认值
    /// \param scw SAChartWidget指针
    ///
    void appendDefaultChartReportSet(SAChartWidget* scw)
    {
        ChartReportSet crs;
        crs.widget = scw;
        chartSet.push_back(crs);
    }
    ChartReportSet& getChartReportSetByWidgetPtr(SAChartWidget* w)
    {
        for(auto it = chartSet.begin();it != chartSet.end();++it)
        {
            if(it->widget == w)
                return *it;
        }
    }
};

#endif // REPORTEXPORTSET_H
