# 构建说明 

## Window下构建步骤

下面将记录为windows下构建SA的流水记录：

总结出来有以下操作：
- 1、拉取项目
- 2、 运行`src/SARibbonBar/updateSARibbon.sh`拉取SARibbonBar
- 3、运行3rdParty.pro
- 4、运行sa.pro

完成


### 1.拉取`SA`项目

打开`gitbash`，`cd`到一个英文路径下，注意路径别有中文，敲入下面的脚本拉取`SA`

```shell
git clone https://github.com/czyt1988/sa.git
```
等待`SA`的拉取

![git clone https://github.com/czyt1988/sa.git](https://github.com/czyt1988/sa/raw/master/doc/build/01.png)

### 2.下载并构建第三方库

目前第三方库的构建已经自动集成到`sa3rdParty.pro`,运行`sa3rdParty.pro`即可构建,想了解详情可细看下面小点介绍

- 1.下载`FFTW`库

我已经把必要文件放置在`src/3rdParty/fftw`，此步骤可忽略
打开网址`FFTW For Windows` : [http://www.fftw.org/install/windows.html](http://www.fftw.org/install/windows.html)
下载对应的版本

![fftw for windows download page](https://github.com/czyt1988/sa/raw/master/doc/build/02.png)

虽然是用MinGW编译的，但是经测试vs2003到vs2015都可以正常使用，所以为了免去不必要麻烦，建议直接使用它们已经编译好的dll,这里我们后续需要用到`libfftw3-3.dll`,`lib`文件和头文件已经在`./src/signAScience/fftw`中配置好，理论不需要重新配置，若想重新配置`fftw`的`lib`请参考文档：[doc/buildFFTW.md](https://github.com/czyt1988/sa/tree/master/doc/buildFFTW.md)

- 2.下载GSL库

我已经把必要文件放置在`src/3rdParty/gsl`，此步骤可忽略

`GSL For Window`人家也像我一样很贴心的把所有东西都打包好了，包括dll和lib文件，你都不用费劲自己编译，下载地址见:[GSL for windows](http://gnuwin32.sourceforge.net/packages/gsl.htm)

`SA`把gsl相关文件放置在[./src/signAScience/gsl](https://github.com/czyt1988/sa/tree/master/src/signAScience/gsl)，包括头文件，lib文件，dll文件。
dll文件具体见[百度网盘-gsl文件](https://pan.baidu.com/s/1Y1xKO9eJELbFf8RtFafVgQ) 提取码：k5e5

- 3.QWT库的构建

为了省事，`SA`把`QWT`的源码都放置在[src/3rdParty/qwt/](https://github.com/czyt1988/sa/tree/master/src/3rdParty/qwt)中

要编译`qwt`只需要用`Qt Creator`运行`src\3rdParty\qwt\qwt.pro`，点build，等待10分钟即可，若不做特殊处理，将会在目录`src\3rdParty`上生成一个build-qwt-xx文件夹：

![build qwt folder](https://github.com/czyt1988/sa/raw/master/doc/build/03.png)

编译完后会自动在lib文件夹下建立对应版本的目录，如下图（msvc，Qt5.9为例）

![copy qwt lib to folder](https://github.com/czyt1988/sa/raw/master/doc/build/04.png)

编译的dll文件(release模式下qwt.dll，debug模式下qwtd.dll)将要放置到sa的`bin_xx_debug/release`目录下,这个目录需要先编译sa.pro才能生成

至此目前`SA`所需的第三方库准备完成，需要有如下内容：

- fftw:

根据系统位数会加载下面的一个

> src/3rdParty/fftw/fftw32/libfftw3-3.lib

> src/3rdParty/fftw/fftw64/libfftw3-3.lib

- gsl:

> src/signAScience/gsl/lib/libgsl.lib

> src/signAScience/gsl/lib/libgslcblas.lib

- qwt 会根据编译环境而不同(qwt编译的lib路径不要改变，编译sa.pro时会自动寻找)

> src/3rdParty/qwt/lib/{msvc/mingw32}/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}/libqwt.a 

> src/3rdParty/qwt/lib/{msvc/mingw32}/$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}/libqwtd.a

### 3.拉取SARibbon

在完成上述步骤后，通过gitbash或其他，进入`SA`目录：`src/SARibbonBar/`,运行`updateSARibbon.sh`

```shell
./updateSARibbon.sh
```

此脚本会自动把`SARibbon`拉取下来,这是`SA`界面的Ribbon控件库。

windows下，直接进入目录双击脚本也可以，如果没有安装gitbash，可以在[https://github.com/czyt1988/SARibbon](https://github.com/czyt1988/SARibbon)下载并把`SARibbon`文件夹放在`src/SARibbonBar`文件夹下，保证`SARibbon.pro`的位置在`src/SARibbonBar/SARibbon.pro`

不需要编译`SARibbon`库，因为`sa.pro`会自动编译此库。

若想单独编译构建`SARibbon`只需用Qt Creator打开`SARibbon.pro`并构建，若不更改配置，将会生成`src\SARibbonBar\SARibbon\bin_qtx.x.x_{debug/release}\`路径，其中包含一个示例程序和`SARibbonBar.lib`和`SARibbonBar.dll`

![build SARibbon](https://github.com/czyt1988/sa/raw/master/doc/build/05.png)

生成的`SARibbonBar.lib`文件位置请不要更改，`SA`项目将引用此路径。自此`SARibbonBar`构建完成。

### 4.构建`SA`

使用`Qt Creator` 打开`src/sa.pro`，点构建，保证以下库存在则可顺利完成`SA`的构建:

- 1. `qwt`库 ，根据编译环境不同和Qt版本不同会有不同路径，如Qt5.9.0下msvc编译将产生：`src/3rdParty/qwt/lib/msvc/5_9_0/qwt.lib`

- 2. `fftw`库，位于`src/signAScience/fftw/libfftw3-3.lib`,头文件位于`src/signAScience/fftw/fftw.h`

- 3. `gsl`库，位于`src/signAScience/gsl/lib/libgsl.lib`和`src/signAScience/gsl/lib/libgslcblas.lib`,头文件位于`src/signAScience/gsl/include/gsl/*.h`

- 4. `SARibbonBar`库的源码，`SARibbonBar`可以不需要提前编译，但必须保证源码SARibbon目录位于`src/SARibbonBar`文件夹下,若已经编译，不要改变编译生成的bin_qtx.x.x_{debug/release}文件夹名称

此时`sa`的结构目录为：

![sa tree](https://github.com/czyt1988/sa/raw/master/doc/build/06.png)

确认上述文件配置完成，自己通过`Qt Creator`构建`sa.pro`

构建完成后会有程序运行异常提示，同时生成`src/bin_qtx.x.x_{debug/release}`文件夹


![finish build](https://github.com/czyt1988/sa/raw/master/doc/build/07.png)

程序运行异常是因为缺少必要的dll，此时`src/bin_qtx.x.x_{debug/release}`文件夹如下图所示：

![copy dll](https://github.com/czyt1988/sa/raw/master/doc/build/08.png)

需要把必要的dll拷贝到此目录下：

需要拷贝的dll如下：

- 1. [可选]`src/bin_qtx.x.x_{debug/release}/lib`文件夹下的dll拷贝到`src/bin_qtx.x.x_{debug/release}`目录下,*Qt Creator+MinGW 并不需要此步骤，但Qt Creator+MSVC在一些版本里需要此步骤*

- 2. 之前构建`qwt`对应的dll拷贝到`src/bin_qtx.x.x_{debug/release}`目录下，debug模式为`qwtd.dll`,release模式下为`qwt.dll`

- 3. `FFTW`库的dll，此版本`SA`使用的是`libfftw3-3.dll`需要拷贝到`src/bin_qtx.x.x_{debug/release}`下(注意`FFTW`有double版本，float版本，long double版本，每个版本对应不同的dll，目前`SA`只用了double版本也就是`libfftw3-3.dll`)

- 4. `GSL`库的dll，`libgsl.dll`和`libgslcblas.dll`需要拷贝到`src/bin_qtx.x.x_{debug/release}`下

再运行`Qt Creator`的`build`即可看到程序运行

此时目录下文件应该如下图所示（Debug模式）

![sa dll files](https://github.com/czyt1988/sa/raw/master/doc/build/09.png)

软件运行起来的截图

![sa ](https://github.com/czyt1988/sa/raw/master/doc/build/ui.png)


## Ubuntu下布置

TODO
