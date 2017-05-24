#ifndef DEF_STR_H
#define DEF_STR_H

//用于定义字符串的宏

//#define STRdef_pro_group_BaseInfo QObject::tr("basic information")//基本信息
//#define STRdef_pro_title QStringLiteral("图表标题")
//#define STRdef_pro_xAxisName QStringLiteral("x轴名称")
//#define STRdef_pro_yAxisName QStringLiteral("y轴名称")

//#define STRdef_pro_SamRate QStringLiteral("采样频率")

//#define STRdef_pro_group_lineInfo QStringLiteral("曲线信息")

//#define STRdef_pro_group_xAxisInfo QStringLiteral("x轴信息")
//#define STRdef_pro_xMax QStringLiteral("x轴最大值")
//#define STRdef_pro_xMin QStringLiteral("x轴最小值")
//#define STRdef_pro_xAvg QStringLiteral("x轴均值")
//#define STRdef_pro_xTopPeak QStringLiteral("x轴上峰值")
//#define STRdef_pro_xBottomPeak QStringLiteral("x轴下峰值")

//#define STRdef_pro_group_yAxisInfo QStringLiteral("y轴信息")
//#define STRdef_pro_yMax QStringLiteral("y轴最大值")
//#define STRdef_pro_yMin QStringLiteral("y轴最小值")
//#define STRdef_pro_yAvg QStringLiteral("y轴均值")
//#define STRdef_pro_yTopPeak QStringLiteral("y轴上峰值")
//#define STRdef_pro_yBottomPeak QStringLiteral("y轴下峰值")

//#define STRdef_pro_WaveGroup QStringLiteral("波形数据信息")
//#define STRdef_pro_SpectrumGroup QStringLiteral("频谱分析")

//#define STRdef_Spectrum QStringLiteral("频谱")
//#define STRdef_PowerSpectrum QStringLiteral("功率谱")
//#define STRdef_CountFrequency QStringLiteral("频率统计")


//#ifndef TREE_ITEM_ROLE_DataMark_PropertyMark//用于记录条目标志TREE_ITEM_MARK_Project，TREE_ITEM_MARK_Folder，等
//#define TREE_ITEM_ROLE_DataMark_PropertyMark (Qt::UserRole + 10)
//#endif

//#ifndef TREE_ITEM_DataMark_WidgetPtr//用于记录窗口指针
//#define TREE_ITEM_DataMark_WidgetPtr (Qt::UserRole + 30)
//#endif

////以下是图标的宏设定
//#ifndef TREE_ITEM_ICON_Project
//#define TREE_ITEM_ICON_Project QStringLiteral("treeItem_Project")
//#endif
//#ifndef TREE_ITEM_ICON_folder
//#define TREE_ITEM_ICON_folder QStringLiteral("treeItem_folder")
//#endif
//#ifndef TREE_ITEM_ICON_folderOriginal
//#define TREE_ITEM_ICON_folderOriginal QStringLiteral("treeItem_folder_Original")
//#endif
//#ifndef TREE_ITEM_ICON_folderAnsys
//#define TREE_ITEM_ICON_folderAnsys QStringLiteral("treeItem_folder_ansys")
//#endif
//#ifndef TREE_ITEM_ICON_TdmsGroup
//#define TREE_ITEM_ICON_TdmsGroup QStringLiteral("treeItem_group")
//#endif
//#ifndef TREE_ITEM_ICON_TdmsChannel
//#define TREE_ITEM_ICON_TdmsChannel QStringLiteral("treeItem_channel")
//#endif
//#ifndef TREE_ITEM_ICON_DataItem
//#define TREE_ITEM_ICON_DataItem QStringLiteral("treeItem_dataItem")
//#endif
//#ifndef TREE_ITEM_ICON_Spectrum //和谱有关的条目
//#define TREE_ITEM_ICON_Spectrum QStringLiteral("treeItem_spectrum")
//#endif
//#ifndef TREE_ITEM_ICON_UserChart
//#define TREE_ITEM_ICON_UserChart QStringLiteral("treeItem_user_chart")
//#endif
//#ifndef TREE_ITEM_ICON_ThermalProperties
//#define TREE_ITEM_ICON_ThermalProperties QStringLiteral("treeItem_thermal_properties")
//#endif
//#ifndef TREE_ITEM_ICON_ThermalTS
//#define TREE_ITEM_ICON_ThermalTS QStringLiteral("treeItem_thermal_ts")
//#endif
////以下是通用文字定义
//#ifndef TREE_ITEM_STRING_folderOriginal
//#define TREE_ITEM_STRING_folderOriginal QStringLiteral("原始数据")
//#endif
//#ifndef TREE_ITEM_STRING_folderAnsys
//#define TREE_ITEM_STRING_folderAnsys QStringLiteral("分析")
//#endif
//#ifndef TREE_ITEM_STRING_folderUserChart
//#define TREE_ITEM_STRING_folderUserChart QStringLiteral("图例")
//#endif

//#ifndef TREE_ITEM_MARK_Project//标定这个条目是项目--项目位于根节点
//#define TREE_ITEM_MARK_Project (1U << 1)
//#endif
//#ifndef TREE_ITEM_MARK_Folder//标定这个条目属于文件夹类
//#define TREE_ITEM_MARK_Folder (1U << 2)
//#endif
//#ifndef TREE_ITEM_MARK_Item//标定这个条目属于条目类
//#define TREE_ITEM_MARK_Item (1U << 3)
//#endif
//#ifndef TREE_ITEM_MARK_IsTDMSProject//标定这个条目属于tdms文件
//#define TREE_ITEM_MARK_IsTDMSProject (1U << 4)
//#endif
//#ifndef TREE_ITEM_MARK_FolderOriginal//标定这个原始数据文件夹
//#define TREE_ITEM_MARK_FolderOriginal (1U << 5)
//#endif
//#ifndef TREE_ITEM_MARK_folderAnsys//标定这个分析数据文件夹
//#define TREE_ITEM_MARK_folderAnsys (1U << 6)
//#endif
//#ifndef TREE_ITEM_MARK_folderUserChart//标定这个分析数据文件夹
//#define TREE_ITEM_MARK_folderUserChart (1U << 7)
//#endif









#endif // DEF_STR_H
