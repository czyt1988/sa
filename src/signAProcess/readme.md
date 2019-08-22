# signAProcess 介绍

`signAProcess`库用于描述所有后台处理类，其基类是`SAAbstractProcess`，这个类继承于`QObject`和`QRunnable`，这样每个继承`SAAbstractProcess`的类，都可以用在线程池中，`SAThreadProcessPool`线程池类就是用于管理`SAAbstractProcess`的封装。

这个类主要在`signADataProc`程序中调用

