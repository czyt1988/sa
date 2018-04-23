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

以下为需要自行构建的：

- [qwt - 绘图库](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qwt)  ->`QWT`的官网地址如下： [http://qwt.sourceforge.net/](http://qwt.sourceforge.net/) 另外 [ 具体构建方法可参考这个链接](http://blog.csdn.net/czyt1988/article/details/11999985)
- [GSL - 数学计算库](https://github.com/czyt1988/sa/tree/master/src/czy/gsl) -> `GSL`是很出名的数学库，SA的window版本使用的是`GSL For Window`:[GSL for window官网](http://gnuwin32.sourceforge.net/packages/gsl.htm) `GSL`的官网见： [GSL 官网](http://ftp.gnu.org/gnu/gsl/) 由于sa的window版本使用的是GSL for window，可以直接下载编译好的二进制使用。
- [fftw 快速傅里叶变换库](https://github.com/czyt1988/sa/tree/master/src/czy/fftw) fftw号称世界最快的傅里叶变换库，其官网见：[http://www.fftw.org/](http://www.fftw.org/) 可以下载官网提供的编译好的dll作为window版本的使用

以下为不需要自己构建的：

- [qti - qti软件的部分类](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qti)
- [qtcolorpicker - 颜色拾取按钮](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qtcolorpicker)
- [ctk - ctk项目的一些类](https://github.com/czyt1988/sa/tree/master/src/3rdParty/ctk)
- [Qt的隐藏控件组qtpropertybrowser 激活方法见此链接](https://blog.csdn.net/czyt1988/article/details/78140050)

可能用到的库，目前还没用

-[qwtplot3d 3d绘图的支持](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qwtplot3d)
-[quazip 一个Qt封装的zip库](https://github.com/czyt1988/sa/tree/master/src/3rdParty/quazip) 其官网：[http://quazip.sourceforge.net/](http://quazip.sourceforge.net/)

# 编译

## 1.第三方库编译及配置

### qwt库的编译

- 使用Qt Creator打开`\src\3rdParty\qwt\qwt.pro`文件，编译

编译结果生成的lib文件按照Qt的版本放置在如下对应的目录下：

> Window+Qt+MinGW版本的把lib文件(libqwt.a和libqwtd.a)放置到目录`src\3rdParty\qwt\lib\x86\mngw32\`下 

> Window+Qt+MSVC版本的把lib文件(qwt.lib和qwtd.lib)放置到目录`src\3rdParty\qwt\lib\x86\msvc\`下 

编译的dll文件(release模式下qwt.dll，debug模式下qwtd.dll)放置到sa的运行目录下,如果不做特殊设置，编译`sa.pro`时将会在`src`文件夹下生成`bin`目录,如果做了`shadow build`，将在指定文件夹下生成`bin`文件夹

### GSL for windows的安装

去 [GSL for window官网](http://gnuwin32.sourceforge.net/packages/gsl.htm)下载二进制包或者使用源码自己编译也可以，`sa`中已经包含了gsl的lib和dll文件位于`src\czy\gsl\GnuWin32.7z`中

把`GSL`的lib文件放置到路径：`src\czy\gsl\lib\`下面
编译的dll文件(libgsl.dll和libgslcblas.dll)放置到sa的运行目录下,如果不做特殊设置，编译`sa.pro`时将会在`src`文件夹下生成`bin`目录,如果做了`shadow build`，将在指定文件夹下生成`bin`文件夹

### FFTW的安装

去官网下载`FFTW`的最新版：[http://www.fftw.org/](http://www.fftw.org/)，windows版本可以直接下载编译好的dll:[http://www.fftw.org/install/windows.html](http://www.fftw.org/install/windows.html)

把`FFTW`的lib文件放置到路径:`src\czy\fftw\`下面
编译的dll文件(libfftw3-3.dll)放置到sa的运行目录下,如果不做特殊设置，编译`sa.pro`时将会在`src`文件夹下生成`bin`目录,如果做了`shadow build`，将在指定文件夹下生成`bin`文件夹

自此三个主要的第三方库编译完成，其中只有`qwt`需要自行编译，`GSL`和`FFTW`都有官方提供编译好的二进制

## 2.SARibbon的编译 

SARibbon是sa项目写的ribbon界面，又单独独立成一个项目，因此，需要去github重新拉取SARibbon编译

SARibbon项目的地址为：[https://github.com/czyt1988/SARibbon](https://github.com/czyt1988/SARibbon)

拉取方法最好直接运行`src\SARibbonBar\updateSARibbon.sh`的脚本，脚本运行完就会自动拉取SARibbon项目，且别改文件夹名字

拉取的文件夹`SARibbon`放置在`src\SARibbonBar\`目录下

- SARibbon编译只需用Qt Creator 运行`SARibbon.pro`即可，编译完会生成lib和对应dll，lib文件位于`src\SARibbonBar\SARibbon\bin\`下面

- `SARibbonBar.pri`文件，位于`src\SARibbonBar\SARibbon\SARibbonBar.pri`下

- SARibbon的所有头文件位于`src\SARibbonBar\SARibbon\src\SARibbonBar\`下面

## 3.SA的编译

Qt Creator打开`sa.pro`并运行build