简介
---------------------------------
NI Labview tdm/tdms文件读取Qt封装

说明
---------------------------------
此封装用于读取Labview数据文件*.tdm/*.tdms
QTDM.h用于读取文件
tdmfiletablemodel.h是对数据文件的table model封装，用于在tableView显示数据
TdmFileViewer.h为数据浏览窗口

使用方法
---------------------------------
把仓库所以文件拷贝到xx文件夹下（nilib）
在工程的pro文件中加入下面此句
include($$PWD/nilib/NI_tdm.pri)