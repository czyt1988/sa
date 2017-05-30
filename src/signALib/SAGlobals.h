#ifndef SAGLOBALS_H
#define SAGLOBALS_H
class SAAbstractDatas;

namespace SA {
    enum ThermoChartPARAM{
        T//< 温度
        ,P///< 压力
        ,S///< 熵
        ,H///< 焓
    };
    enum ThermoChartMODE{
        TS
        ,HS
        ,PT
    };
//    enum DataMode{
//        NotData///<不是数据
//        ,DoubleSeries///<数组
//        ,PointSeries///<点集
//        ,TDMSChannel
//    };
    enum ViewRange{
        InViewRange = 0x1///<在可视范围内
        ,OutViewRange = 0x2///< 不可视范围内
        ,AllRange = 0x3///< 所有范围
    };
    ///
    /// \brief 导出数据时的模式
    ///
    enum PickDataMode
    {
        XOnly///< 只导出x值，这样导出的数据为DataMode::DoubleSeries
        ,YOnly///< 只导出y值，这样导出的数据为DataMode::DoubleSeries
        ,XYPoint///< 导出xy值，这样导出的数据为DataMode::PointSeries
    };

    ///
    /// \brief 此类型用于标定图表的特殊内容，如波形图所包含的采样率采样点这些特殊数据,每个SAChartWidget都会保存有一份
    /// SAAbstractChartTypeData，SAAbstractChartTypeData里包含了ChartType信息，ChartType是为了方便快速查询
    /// 类型，而避免经常调用dynamic_cast
    /// \see SAAbstractChartTypeData::getType
    ///
    enum ChartType{
        NormalChart///< 正常
        ,WaveChart///< 波形图
        ,SpectrumChart///< 频谱图
        ,ThermodynamicsChart///< 热力学图
    };
    ///
    /// \brief 此类型用于标定一个mdi子窗口的属性，属于哪一类型的窗口，在mdi子窗口创建时设定在MdiSubWindow里，
    /// \see MdiSubWindow::setType
    ///
    enum SubWndType{
        UserChartWnd///< 用户自定义图
        ,Normal2DChartWnd///< 普通2d图形，不含其他信息
        ,TendencyChartWnd///< 趋势图
        ,ThermalDiagramWnd///< 热力学图
        ,SignalChartWnd///< 信号相关的图
        ,ThermalPropertiesWnd
        ,ValueDataView///< 数据表格图
        ,TimeFrequencyAnalysis///< 时频分析窗口
    };

    enum ProjectTreeMark
    {
        MARK_NONE = 0
        ,MARK_PROJECT = 0x80000000
        ,MARK_FOLDER = 0x40000000
        ,MARK_ITEM = 0x20000000
    };
    ///
    /// \brief 全局定义的信息类型，sa中有些类会发射错误或者一些提示信息，
    /// 一般为messageInformation信号，此信号的第二个参数为信息的分类
    /// 此枚举主要对这些信息进行分类
    ///
    enum MeaasgeType{
        NormalMessage = 0///< 普通消息
        ,WarningMessage///< 警告的消息
        ,ErrorMessage///< 出错的消息
        ,QuesstionMessage///< 疑问信息
    };

    ///
    /// \brief The DataType enum
    ///
    enum DataType
    {
        UnknowType = -1///< 不知类型
        ,DataLink = -2///<数据的连接类型

        ,CanBeDrawBoundaryDataTypeStart = 0///<可绘制范围 - start

        ,Variant = 1///< 基本类型


        ,VectorDouble=100///< 数组
        ,VectorPoint=200///< 点数组
        ,VectorInt=300///< 数组
        ,VectorVariant=400///< 数组
        ,VectorInterval=500///< 数组

        ,TableBoundaryBegin = 1000///< 表格开始范围
        ,TableVariant = 1001///< 任意类型表格
        ,TableDouble = 1002///< 数值表格
        ,TableBoundaryEnd = 2000///< 表格结束范围

        ,TdmsChannel = 2100///< tdms通道
        ,CanBeDrawBoundaryDataTypeEnd = 9000///<可绘制范围 - end

        ,FolderBoundaryBegin = 9100///<文件夹性质数据类型开始
        ,TdmsFile = 9105///< tdms文件
        ,TdmsGroup = 9106///< tdms组
        ,FolderBoundaryEnd = 9900///<文件夹性质数据类型结束
    };

    enum DataPackageType
    {
        DataPackageTypeStart = 1000000
        ,NormalDataPackage = 1000010 ///< 普通数据包
        ,DataPackageTypeEnd = 2000000
    };

    enum FunType
    {
        FunctionBoundaryDataTypeStart = 10000///< 函数类型
        ,FunctionFFt = 10010 ///< 傅里叶变换函数
        ,FunctionPolyfit = 10020 ///<多项式拟合函数
        ,FunctionFilter = 10030 ///<滤波函数
        ,FunctionStatistics = 10040 ///<统计学相关函数
        ,FunctionNum = 10050///< 数值计算相关函数
        ,FunctionBoundaryDataTypeEnd = 90000///< 函数类型结束

    };

    enum SizeType{
        Row=1
        ,Column=2
    };
    enum DimType{
        Dim0=0
        ,Dim1=1
        ,Dim2=2
        ,Dim3=3
    };

}

#define ROLE_ITEM_MARK Qt::UserRole + 1///< 标志角色
#define ROLE_ITEM_WIDGET Qt::UserRole + 10///< 关联窗口角色
#define ROLE_FOLDER_MARK Qt::UserRole + 20 ///< 文件夹标记
#define ROLE_DATATYPE_MARK Qt::UserRole + 30 ///< 数据标记,所有需要绘制的条目都需要设置这个ROLE_DATATYPE_MARK，用于标定DATAMODE \see DATAMODE
#define ROLE_ID Qt::UserRole + 40 ///< 数据标记

//#define MARK_PROJECT 0x80000000 ///< 顶层项目标记位
//#define MARK_FOLDER 0x40000000 ///< 文件夹标记位
//#define MARK_ITEM 0x20000000 ///< 项目标记位

//#define MARK_PROJECT_ID_BIT 0xFF ///< 顶层项目个数存放位置，MARK与此位&（于）操作后，即可提取该条目或文件夹所属的项目,因此项目的最大数目为0xFF

#define ICON_Project QIcon(":/treeItemIcon/res_treeItemIcon/Project.png")
#define ICON_folder QIcon(":/treeItemIcon/res_treeItemIcon/folder.png")
#define ICON_FolderUserChart QIcon(":/treeItemIcon/res_treeItemIcon/folder-ansys.png")
#define ICON_TdmsGroup QIcon(":/treeItemIcon/res_treeItemIcon/group.png")

#define ICON_folderOriginal QIcon(":/treeItemIcon/res_treeItemIcon/folder_original.png")
#define ICON_DataItem QIcon(":/treeItemIcon/res_treeItemIcon/dataItem.png")
#define ICON_Spectrum QIcon(":/treeItemIcon/res_treeItemIcon/spectrum.png")
#define ICON_UserChart QIcon(":/treeItemIcon/res_treeItemIcon/user_chart.png")
#define ICON_ThermalProperties QIcon(":/treeItemIcon/res_treeItemIcon/propertySearch.png")
#define ICON_ThermalTS QIcon(":/treeItemIcon/res_treeItemIcon/ts.png")
#define ICON_CountFrequency  QIcon(":/image/res_image/count_frequency.png")
#define ICON_TdmsChannel QIcon(":/treeItemIcon/res_treeItemIcon/channel.png")
#define ICON_ItemDouble QIcon(":/treeItemIcon/res_treeItemIcon/doubles.png")
#define ICON_ItemPoint QIcon(":/treeItemIcon/res_treeItemIcon/points.png")

#define SA_MIME_ProjectItem "sa/projectItem"
#define SA_MIME_ValueManagerItem "sa/ValueManagerItem"



#ifndef SA_ROLE_DATA_TYPE
#define SA_ROLE_DATA_TYPE (Qt::UserRole + 1299)
#endif
#ifndef SA_ROLE_DATA_ID
#define SA_ROLE_DATA_ID (Qt::UserRole + 1099)
#endif


//#ifndef SA_TYPE_CAN_BE_DRAW_BOUNDARY
//#define SA_TYPE_CAN_BE_DRAW_BOUNDARY 0
//#endif

//#ifndef SA_CLOUMN_TYPE_DOUBLE
//#define SA_CLOUMN_TYPE_DOUBLE 1
//#endif
//#ifndef SA_CLOUMN_TYPE_POINT
//#define SA_CLOUMN_TYPE_POINT 2
//#endif

//#ifndef SA_TYPE_TABLE_BEGIN//table文件标记的起始数据
//#define SA_TYPE_TABLE_BEGIN 1000
//#endif
//#ifndef SA_TYPE_TABLE_UNKNOW
//#define SA_TYPE_TABLE_UNKNOW -2
//#endif
//#ifndef SA_TYPE_TABLE_VARIANT
//#define SA_TYPE_TABLE_VARIANT 1001
//#endif
//#ifndef SA_TYPE_TABLE_DOUBLE
//#define SA_TYPE_TABLE_DOUBLE 1002
//#endif
//#ifndef SA_TYPE_TABLE_END//table文件标记的结束数据
//#define SA_TYPE_TABLE_END 2000
//#endif

//#ifndef SA_TYPE_TDMS_BEGIN //tdm文件标记的起始数据
//#define SA_TYPE_TDMS_BEGIN 100
//#endif
//#ifndef SA_TYPE_TDMS
//#define SA_TYPE_TDMS -100
//#endif
//#ifndef SA_TYPE_TDMS_GROUP
//#define SA_TYPE_TDMS_GROUP -101
//#endif
//#ifndef SA_TYPE_TDMS_CHANNEL
//#define SA_TYPE_TDMS_CHANNEL 102
//#endif



#ifndef SA_SIZE_TYPE_ROW
#define SA_SIZE_TYPE_ROW 1
#endif
#ifndef SA_SIZE_TYPE_COLUMN
#define SA_SIZE_TYPE_COLUMN 2
#endif

#ifndef SA_TYPE_FOLDER
#define SA_TYPE_FOLDER -1000
#endif

#endif // GLOBALS_H
