# signADataProc

负责数据处理的进程

## 接口文档

### SA::ProtocolFunReq2DPointsDescribe
请求：
classid: SA::ProtocolTypeXml
funid: SA::ProtocolFunReq2DPointsDescribe
```xml
<sa type="xml" classid="2" funid="5">
 <values>
  <default-group>
    <item type="int" name="key"></item>
    <item type="QVariantList" name="points"></item>
  </default-group>
 </values>
</sa>
```

返回

```xml
<sa type="xml" classid="2" funid="5">
 <values>
  <default-group>
    <item type="int" name="key"></item>
    <item type="QVariantList" name="points"></item>
  </default-group>
 </values>
</sa>
```
