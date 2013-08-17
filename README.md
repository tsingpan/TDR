HotPot
========================
**这里是一个火锅， 里面很多乱七八糟的新奇玩意!**

- hotpot 跨平台模块， 支持Windows Linux OSX。
- hotprotocol 跨语言协议模块， 支持C, Php, ObjC。
- hotscript 用于显示对象的脚本语言， 支持C和php扩展。

### 问题和反馈
点击这里 → [https://github.com/randyliu/HotPot/issues](https://github.com/randyliu/HotPot/issues)

###安装
1、安装Bison
		http://www.gnu.org/software/bison/
		版本2.4.1

		Windows:
			安装Cygwin, http://www.cygwin.com/
			选中里面的Bison工具
		Linux:
			apt-get install bison
		OS X:
			./configure
			make
			make install			

2、安装RE2C
		http://sourceforge.net/projects/re2c/
		版本0.13.5

		Windows:		
			下载re2c.exe放到本地的一个文件夹中， 并把文件夹路径加入path环境变量
		Linux:
			apt-get install re2c
		OS X:
			./configure
			make
			make install

3、安装CMake
		http://www.cmake.org/
		版本：2.8.11.1

4、下载HotPot源代码
		git clone https://github.com/randyliu/HotPot

5、CMake生成工程文件
		http://www.cmake.org/
		参考CMake使用方法

6、编译
		Windows:
			打开Microsoft Visual Studio， 点击编译。
		Linux:
			make
		OS X:
			make

7、安装
		Windows:
			在Microsoft Visual Studio中执行INSTALL项目。
		Linux:
			make install
		OS X:
			make install

8、配置环境变量
		配置HOTPOT_DIR环境变量为安装目录
		Windows:
			新建一个环境变量HOTPOT_DIR=C:\Program Files\HotPot
		Linux:
			export HOTPOT_DIR=/usr/local
		OS X:
			export HOTPOT_DIR=/usr/local

二、PHP扩展的安装
1、安装PHP扩展
		php版本5.5.1		
		source\hotscript\php\ext\hotscript目录下是一个标准的php扩展， 安装请参考php文档
		http://www.php.net/manual/zh/install.php

2、配置PHP
		在php.ini中添加HotPot安装目录:
			hotpot.dir="C:\Program Files\HotPot"

三、hotprotocol-java的安装
	待完善

四、hotprotocol-cocoa的安装
	待完善
