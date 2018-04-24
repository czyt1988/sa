# 关于FFTW的构建 

## Window下构建步骤

下面将记录为windows下构建FFTW的流水记录：


- 1.下载`FFTW`库

打开网址`FFTW For Windows` : [http://www.fftw.org/install/windows.html](http://www.fftw.org/install/windows.html)
下载对应的版本

![fftw for windows download page](https://github.com/czyt1988/sa/raw/master/doc/build/02.png)

虽然是用MinGW编译的，但是经测试vs2003到vs2015都可以正常使用，所以为了免去不必要麻烦，建议直接使用它们已经编译好的dll

![fftw for windows files](https://github.com/czyt1988/sa/raw/master/doc/buildFFTW/01.png)

我们需要用到`libfftw3-3.dll`,但发现缺少lib文件，需要通过提供的`def`文件生成lib文件

下面摘自fftw官网：

> These DLLs were created by us, cross-compiled from GNU/Linux using MinGW; the 64-bit version is possible thanks to the mingw-w64 project. You should be able to call them from any compiler. In order to link to them from Visual C++, you will need to create .lib "import libraries" using the lib.exe program included with VC++. Run:

```shell
lib /def:libfftw3-3.def
lib /def:libfftw3f-3.def
lib /def:libfftw3l-3.def
```
> On Visual Studio 2008 in 64-bit mode, and possibly in other cases, you may need to specify the machine explicitly:

```shell
lib /machine:x64 /def:libfftw3l-3.def
```

对应vs用户运行对应的cmd

![vs cmd](https://github.com/czyt1988/sa/raw/master/doc/buildFFTW/02.png)

cd 到fftw对应的目录，运行`lib /def:libfftw3-3.def`

![build lib](https://github.com/czyt1988/sa/raw/master/doc/buildFFTW/03.png)

vs不指定平台默认生成32位的

此时会在目录下生成一个lib文件

![lib file](https://github.com/czyt1988/sa/raw/master/doc/buildFFTW/04.png)

其他环境看官网教程，由于lib文件几乎不会随环境变化而变化，因此，`SA`的`/src/czy/fftw`目录下已经上传了一个lib文件，如果自己按照上述步骤来进行，需要把生成的lib文件复制到`src/czy/fftw`文件夹下替换掉之前的lib文件，同时`fftw.h`的头文件也需要替换

![copy file](https://github.com/czyt1988/sa/raw/master/doc/buildFFTW/05.png)

自此FFTW算配置完成。

具体`SA`构建的文档请查看[编译SA说明](https://github.com/czyt1988/sa/tree/master/build.md)






