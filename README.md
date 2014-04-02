TData
======
TData编译器可以生成TData数据描述语言， 在各种平台和语言下的对应代码。
TData数据描述语言的手册， 请点击这里 → [https://github.com/TDorm/TData/wiki](https://github.com/TDorm/TData/wiki)

特性
====
	编译器使用re2c生成词法分析代码， bison生成语法分析代码。 使用C语言编写， 无依赖库， 具有很高的可移植性， 支持Windows, Linux, Unix(OsX)平台。

问题和反馈
==========
点击这里 → [https://github.com/randyliu/TData/issues](https://github.com/randyliu/TData/issues)

安装
====
	由于跨平台的需要， 使用[CMake](http://www.cmake.org)作为构建工具， Linux下面一个典型的安装过程如下：

	mkdir build
	git clone https://github.com/TDorm/TData TData
	cd build
	cmake ../TData
	make
	sudo make install
