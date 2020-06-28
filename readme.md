# 简介

这是一个数据预处理软件，主要负责数据挖掘的数据清洗工作，如对一些科研实验数据的清洗和预处理

PS:此软件alpha版都还没做好

# 已有的一些功能截图

- 界面总览

![](https://github.com/czyt1988/sa/raw/master/doc/screenshot/01.gif)

- 曲线数据实时抓取

![](https://github.com/czyt1988/sa/raw/master/doc/screenshot/02.gif)

- 曲线区域修正功能

![](https://github.com/czyt1988/sa/raw/master/doc/screenshot/03.gif)

# 第三方库介绍

sa使用了如下第三方库，在进行编译前请先配置好，否则无法编译。

目前版本的第三方库的代码都在[/src/3rdParty](https://github.com/czyt1988/sa/raw/master/src/3rdParty)文件夹下，由于像`qwtplot3d`已经不维护因此这里索性把第三方库的代码都放到此目录下，后续考虑移除

目前已经使用到如下第三方库或类：

以下为需要自行构建的（我已经为大家配置好了，windows下直接用就行）：

- [qwt - 绘图库](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qwt)  ->`QWT`的官网地址如下： [http://qwt.sourceforge.net/](http://qwt.sourceforge.net/) 另外 [ 具体构建方法可参考这个链接](http://blog.csdn.net/czyt1988/article/details/11999985)
- [Qt的qtpropertybrowser](https://blog.csdn.net/czyt1988/article/details/78140050) Qt的属性表控件，本来是可以不用编译的，但高版本qt（5.9之后）位置就变了，这里就作为需构建行列
- [GSL - 数学计算库](https://github.com/czyt1988/sa/tree/master/src/czy/gsl) -> `GSL`是很出名的数学库，SA的window版本使用的是`GSL For Window`:[GSL for window官网](http://gnuwin32.sourceforge.net/packages/gsl.htm) `GSL`的官网见： [GSL 官网](http://ftp.gnu.org/gnu/gsl/) 由于sa的window版本使用的是GSL for window，可以直接下载编译好的二进制使用。
- [fftw 快速傅里叶变换库](https://github.com/czyt1988/sa/tree/master/src/czy/fftw) fftw号称世界最快的傅里叶变换库，其官网见：[http://www.fftw.org/](http://www.fftw.org/) 可以下载官网提供的编译好的dll作为window版本的使用

GSL和FFTW都已经备好，其实也无需构建，直接用现成的lib

以下为不需要自己构建的：

- [qti - qti软件的部分类](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qti)
- [qtcolorpicker - 颜色拾取按钮](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qtcolorpicker)
- [ctk - ctk项目的一些类](https://github.com/czyt1988/sa/tree/master/src/3rdParty/ctk)
- [CRCpp](https://github.com/d-bahr/CRCpp) 一个crc库

可能用到的库，目前还没用

-[qwtplot3d 3d绘图的支持](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qwtplot3d)
-[quazip 一个Qt封装的zip库](https://github.com/czyt1988/sa/tree/master/src/3rdParty/quazip) 其官网：[http://quazip.sourceforge.net/](http://quazip.sourceforge.net/)

# 编译

编译说明请看此文档:

[sa 编译说明:build.md](https://github.com/czyt1988/sa/tree/master/build.md)