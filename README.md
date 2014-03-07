TData
======
**之前我们在C语言的一个头文件item.h里定义了一个结构体item_table_s表示游戏的道具**

	enum item_type_e
	{
		e_crystal,
		e_ectype,
		e_other,
	};

	union item_limit_u
	{
		uint32 level;
		uint32 mapid;
		uint32 gold;
	};

	const uint32 ITEM_MAX_LIMIT = 10;
	struct item_table_s
	{
		uint64 id;
		string name;
		item_type_e type;
		vector<item_limit_u> limit_list;
	};

**有时需要从mysql中读取玩家身上的道具**

	此处省略若干服务器读取mysql数据库的代码。


**有时需要用网络协议把这个道具发送给客户端**

	此处省略若干行服务器序列化对象到缓存的代码。
	此处省略若干行客户端反序列化缓存到结构体的代码。

**有时服务器需要读取策划同学填写的道具表资源Excel文件**

	此处省略若干行服务器读取Excel代码。
	此处省略若干行客户端读取Excel代码。

**有时运维网站需要一个xml描述的道具**

	此处省略若干行对象转换为xml的代码。

**这时突然发现之前写的数据转换代码居然长得很类似！**

**更加丧心病狂的是策划刚刚提出修改道具表格， 于是又得把所有代码修改一遍……**


**有了TData之后, 只需要把item.h改名为item.td， 然后输入tdata -gen all item.td就可以把前面所有的无聊代码生成出来了！**

TData的说明文档, 请点击这里 → [https://github.com/TDorm/TData/wiki](https://github.com/TDorm/TData/wiki)

特性
====
	编译器无依赖库， 使用C语言编写， 可以支持Windows, Linux, OSX平台。

问题和反馈
==========
点击这里 → [https://github.com/randyliu/TData/issues](https://github.com/randyliu/TData/issues)

安装
====
- 由于跨平台的需要， 请先安装[CMake](http://www.cmake.org/), 由于代码本身并没有任何依赖库所以安装过程极为简单。

	shell> mkdir build
	shell> git clone https://github.com/TDorm/TData TData
	shell> cd build
	shell> cmake ../TData
	shell> make
	shell> sudo make install
