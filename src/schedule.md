# 进度记录

## [2020/3/7] 开始完善SADataFeatureWidget的功能
- 实现多进程的曲线关键参数计算
- 实现计算进程的计算功能
- 实现基于sa协议的多进程计算

## [2020/3/1] 重构czy库到signAUtil库和signAScience库
- 把原来的czy库内容重新转移到signAUtil中，并修正所有库的命名
- 把原来的czy库的数学相关及信号处理相关移动到signAScience库中
- 统一命名为SA前缀
- 命名空间为SA，不考虑再深层级的命名空间
