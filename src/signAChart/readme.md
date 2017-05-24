- can not show layerout in first plot
- can not show SAVectorPoint in model



# 调试记录

## 考虑更换QtChart引擎

## 20161106

在window下signAChart可以顺利编译，但主程序signA链接时却发生错误，错误描述如下 ：

> D:\cloudDisk\Qt\signA-qwt\bin\libs/libsignAChart.a(d000953.o):(.text+0x0): multiple definition of `SAChart2D::qt_metacast(char const*)'
./debug\moc_SAChart2D.o:D:\Qt\qtApp\signA-qwt\build-signA-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/moc_SAChart2D.cpp:687: first defined here
> D:\cloudDisk\Qt\signA-qwt\bin\libs/libsignAChart.a(d000952.o):(.text+0x0): multiple definition of `SAChart2D::qt_metacall(QMetaObject::Call, int, void**)'
./debug\moc_SAChart2D.o:D:\Qt\qtApp\signA-qwt\build-signA-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/moc_SAChart2D.cpp:695: first defined here
> collect2.exe: error: ld returned 1 exit status
> mingw32-make[1]: *** [..\bin\signA.exe] Error 1

显示的是libsignAChart.a的链接错误，定位到SAChart2D这个类，是Q_OBJECT出现了重定义

但signAChart可以单独编译，也就是说signA里有SAChart2D这个类？

于是我查遍了signA工程中的文件，有个可疑类是SAPlotChart，他是继承SAChart2D

```cpp
class SAPlotChart : public SAChart2D
{
    Q_OBJECT
public:
    SAPlotChart(QWidget* parent = nullptr);
    virtual ~SAPlotChart();
    ......
}
```

但并没有检查出什么问题，下面打算建立一个分支，对其进行删减排除问题

