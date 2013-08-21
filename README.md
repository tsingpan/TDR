HotPot
======
**这是一个火锅， 里面有很多新奇玩意!**

- HotPlatform 跨平台基础模块， 支持Windows,Linux,OS X。
- HotProtocol 跨语言协议模块，支持C,PHP,ObjC。
- HotLib 链表， 词法分析器， 错误码处理等
- HotData 数据描述语言。
- HotScript 文本处理工具。

问题和反馈
==========
点击这里 → [https://github.com/randyliu/HotPot/issues](https://github.com/randyliu/HotPot/issues)

安装
====
###HotPot-C

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

3.安装[Python](http://www.python.org/) 3.3.2

4.安装[CMake](http://www.cmake.org/) 2.8.11.1

5.下载[HotPot](https://github.com/randyliu/HotPot)源代码

		git clone https://github.com/randyliu/HotPot

6.CMake生成工程文件

		使用方法请参考 → [http://www.cmake.org/](http://www.cmake.org/)

7.编译

		Windows
			打开Microsoft Visual Studio， 点击编译。
		Linux
			make
		OS X
			make

8.安装

		Windows
			在Microsoft Visual Studio中执行INSTALL项目。
		Linux
			make install
		OS X
			make install

9.配置环境变量

		配置HOTPOT_DIR环境变量为安装目录
		Windows
			新建一个环境变量HOTPOT_DIR=C:\Program Files\HotPot
		Linux
			export HOTPOT_DIR=/usr/local
		OS X
			export HOTPOT_DIR=/usr/local

###HotScript-PHP
关于PHP的更多信息， 请参考[PHP手册](http://www.php.net/manual/zh/index.php)

1.安装

		source/hotscript/php/ext/hotscript目录下是一个标准的php扩展, 请按照php所提供的方法进行安装。

2.php.ini

		添加HotPot安装目录 hotpot.dir="C:\Program Files\HotPot"
		添加HotScript的工作目录 hotscript.workingdir = d:/


###HotProtocol-Java
		待完善

###HotProtocol-Cocoa
		待完善
