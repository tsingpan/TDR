TData
======
**多平台多语言的数据描述工具!**

- cmake 用于cmake工程的脚本文件
- source 源代码
- tutorial 使用例子
- language 语言包

问题和反馈
==========
点击这里 → [https://github.com/randyliu/TData/issues](https://github.com/randyliu/TData/issues)

安装
====
###TData

1.安装[Bison](http://www.gnu.org/software/bison/) 2.7.1

		Windows
			安装Cygwin并选中里面的Bison工具。
		Linux
			apt-get install bison

		OS X
			./configure
			make
			make install

2.安装[re2c](http://sourceforge.net/projects/re2c/) 0.13.5

		Windows
			下载re2c.exe放到本地的一个文件夹中， 并把文件夹路径加入path环境变量
		Linux
			apt-get install re2c
		OS X
			./configure
			make
			make install

3.安装[CMake](http://www.cmake.org/) 2.8.11.1

4.下载[TData](https://github.com/randyliu/TData)源代码

		git clone https://github.com/randyliu/TData

5.CMake生成工程文件

		使用方法请参考 → [http://www.cmake.org/](http://www.cmake.org/)

6.编译

		Windows
			打开Microsoft Visual Studio， 点击编译。
		Linux
			make
		OS X
			make

7.安装

		Windows
			在Microsoft Visual Studio中执行INSTALL项目。
		Linux
			make install
		OS X
			make install

8.配置环境变量

		配置TDATA_HOME环境变量为安装目录
		Windows
			新建一个环境变量TDATA_HOME=C:\Program Files\TData
		Linux
			export TDATA_HOME=/usr/local
		OS X
			export TDATA_HOME=/usr/local
