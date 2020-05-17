# signAProtocol 介绍

`signAProtocol`库用于描述所有后台处理协议，这个包只负责协议的生成，数据封装成协议

核心的两个抽象类为：

- `SAAbstractProtocolParse` 定义协议的解析接口


# 关于协议

sa 协议分为xml和json两个版本

## 基于xml的交互协议

```xml
<sa type="xml" classid="" funid="">
 <values>
  <default-group>
    <item type="int" name="value">1</item>
  </default-group>
  <group name="g">
    <item type="int" name="point-size">4</item>
    <item type="int" name="sequenceID">123</item>
    <item type="QVariantList" name="points">
       <item type="QPointF">1;2</item>
       <item type="QPointF">1;3</item>
       <item type="QPointF">2;3</item>
       <item type="QPointF">4;5</item>
    </item>
  </group>
 </values>
</sa>
```

sa协议主要用于描述一个树形数据结构，上述协议可表达为：

root
  |-value:1
  |-g
    |-point-size:4
    |-sequenceID:123
    |-points:[[1,2],[1,3],[2,3],[4,5]]

如果使用json，则表达为：
```json
{
    "sa": {
        "type": "xml",
        "calssid": "",
        "funid": ""
    },
    "values": {
        "default-group": [
            {
                "type": "int",
                "name": "value",
                "value": 1
            }
        ],
        "g": [
            {
                "type": "int",
                "name": "point-size",
                "value": 4
            },
            {
                "type": "int",
                "name": "sequenceID",
                "value": 4
            },
            {
                "type": "QVariantList",
                "name": "points",
                "value": [
                    {
                        "type": "QPointF",
                        "value": "1;2"
                    },
                    {
                        "type": "QPointF",
                        "value": "1;3"
                    },
                    {
                        "type": "QPointF",
                        "value": "2;3"
                    },
                    {
                        "type": "QPointF",
                        "value": "4;5"
                    }
                ]
            }
        ]
    }
}
```

从xml和json两种形式上看，xml是比json有优势的，当然json还可以进行优化，但xml也未尝不可，另外还有一点是，json往往是无序的，例如values字段下两个group，如果要表征group的顺序，json就需要用到数组，这样又无疑会复杂一层

有序的json表达应该如下：
```json
{
    "sa": {
        "type": "xml",
        "calssid": "",
        "funid": ""
    },
    "values": [
        {
            "name": "default-group",
            "items": [
                {
                    "type": "int",
                    "name": "value",
                    "value": 1
                }
            ]
        },
        {
            "name": "g",
            "items": [
                {
                    "type": "int",
                    "name": "point-size",
                    "value": 4
                },
                {
                    "type": "int",
                    "name": "sequenceID",
                    "value": 4
                },
                {
                    "type": "QVariantList",
                    "name": "points",
                    "value": [
                        {
                            "type": "QPointF",
                            "value": "1;2"
                        },
                        {
                            "type": "QPointF",
                            "value": "1;3"
                        },
                        {
                            "type": "QPointF",
                            "value": "2;3"
                        },
                        {
                            "type": "QPointF",
                            "value": "4;5"
                        }
                    ]
                }
            ]
        }
    ]
}
```

可见json可读性并不比xml高，因此，sa推荐使用xml协议，对协议进行了抽象，目前实现了xml的描述，暂无json描述的打算。

通过此协议表征一个有较强关系的数据结构，尤其是树形结构，主要是为了返回复杂关系的参数

# 主要接口

- `setFunctionID`/`getFunctionID`设置功能码，用于区分协议类型
- `setClassID`/`getClassID`设置类型码，用于区分协议大类
- `setValue`通过分组名和key设置一个参数，如果不输入分组名，会写入到默认分组，默认分组使用`getDefaultGroupValue`来获取
- `getValue`通过分组名和key获取一个参数

# 关于组
