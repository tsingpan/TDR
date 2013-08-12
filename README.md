HotPot——小星星的聚宝盆
========================
提供了数据的跨平台， 跨语言支持等功能， 是其他库的基础。

安装
----
#Windows
###1、安装CMake
		最低版本：2.8.11.1
		如何在Windows上安装CMake请参考：http://www.cmake.org/

###2、下载HotPot源代码
		git clone https://github.com/randyliu/HotPot

###3、CMake生成工程文件
		假定使用Microsoft Visual Studio

###4、用管理员身份启动Microsoft Visual Studio

###5、运行INSTALL项目

###6、安装PHP扩展
		source\hotscript\php\ext\hotscript目录下是一个标准的php扩展，安装方法请参考：
			https://wiki.php.net/internals/windows/stepbystepbuild

###7、配置PHP
		在php.ini中添加HotPot安装目录:
			hotpot.dir="C:\Program Files\HotPot"

###8、配置环境变量
			HOTPOT_DIR="C:\Program Files\HotPot"
