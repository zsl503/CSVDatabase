# CSVDatabase
## 使用方法
以下为部分用法，具体可参考示例代码：https://github.com/zsl503/CommodityManagement/releases/tag/20231030

注意为了对中文有良好支持，每个字符串前都要有个L，例如L"id"
```C++
// #include <csvdb.h>

// 创建一个数据库
createDatabase(/*your data Path*/);

// 初始化一个数据库
Database db = Database(/*your data Path*/);

// 创建表 (id 主键 int型,name str型, password str型, createTime 日期)
db.createTable(/*table name*/,{
  Field(L"id", DATATYPE::INT, true, 0),
	Field(L"name", DATATYPE::STR, false, 1),
	Field(L"password", DATATYPE::STR, false, 2),
	Field(L"createTime", DATATYPE::DATE, false, 3)
})

// 获取所有表名列表
db.getTableList();

// 获取表
TableData td = db[/*table name*/];

// 选出id == 10
td[td["id"] == 10];

// 选出id >= 10 或 id < 5
td[td["id"] >= 10 || td["id"] < 5];

// 选出id*id >= 10 或 id < 5 且 name == "xx"
td[(td["id"]*td["id"] >= 10 || td["id"] < 5) && td["name"] == "xx"];

// 获取id和name列
td[td["id"] && td["name"]]

// 获取第10行
td[10]

// 相同列表相加
TableData td = td1 + td2

// 清空
td.clear()

// 更新表内id > 10 的name字段为“测试”
db.update(/*table name*/,{ L"name" },{CSVData(“测试”)},td[L"id"] > 10);
// 

```
