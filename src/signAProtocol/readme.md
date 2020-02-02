# signAProtocol 介绍

`signAProtocol`库用于描述所有后台处理协议，这个包只负责协议的生成，数据封装成协议

核心的两个抽象类为：
- `SAAbstractProtocolMaker` 用于定义协议的生成接口
- `SAAbstractProtocolParse` 用于定义协议的解析接口

`SAAbstractProtocolMaker`只封装了setValue的相关内容
`SAAbstractProtocolParse`只封装了getValue的相关内容

# 关于协议

sa 协议分为xml和json两个版本

基于xml的交互协议

@startuml

Class01 <|-- Class02:泛化
Class03 <-- Class04:关联
Class05 *-- Class06:组合
Class07 o-- Class08:聚合
Class09 -- Class10

@enduml