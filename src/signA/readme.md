sa 的ui程序
#正在进行
- 完成XML配置文件读取的类
- 添加一个项目设置对话框
- 格式化mainwindow的函数
- 变量选择窗口完善
- 添加一个全局参数设置类(单例)，用于保存全局一些设定，可以跨模块调用

#已知问题：
- 主窗口关闭，如果有NUll父窗口的widget，它将会hold住线程，然主窗口只是隐藏
解决方法，在关闭事件中把所有插件卸载但会影响关闭时间
- 时频分析窗口导入数据时会触发报告
- 时频分析扫频数据不刷新
- 文本导入时，忽略了文件头，其他也忽略



#当前的任务：(2017-02-28)

#已经完成进度：
- 完成SAData系列二进制保存的逻辑，使得sa支持项目保存
- 变量窗口给SAFunPlugin的函数使用
- 实现一个变量选择窗口，可以让用户选择变量，需求如下：
- 把signa里所有函数相关操作移动到插件中 remove function model
- 曲线选择窗口默认选中变量管理器选中的曲线
- MainWindow抽象出接口，saUI继承此抽象，在模块构建ui时传入此多重继承的抽象
- 完成txtImport的修订
- SAValueManager添加一个返回shareptr的接口
- SAUIReflection的强化，实现界面的操作
- 把SAUIInterface转移到signACommonUI
- 完善textImport，添加文本预览tab
- 完成SATextReadWriter的多线程写入文件功能，并结合到SALog中
