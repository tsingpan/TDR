HotPot——小星星的聚宝盆
========================
提供了数据的跨平台， 跨语言支持等功能， 是其他库的基础。

安装
----
#windows
###1、安装PHP
		最低版本:5.5.1
		安装请参考：https://wiki.php.net/internals/windows/stepbystepbuild
		不要忘记配置环境变量PHP-DEVEL_DIR为php的根目录

###2、安装LUA
		最低版本:5.5.2
		[静态版本的LUA请参考](http://sourceforge.net/projects/luabinaries/files/5.2.1/Windows%20Libraries/Static/)
		不要忘记配置环境变量LUA_DIR为lua的根目录
		
###3、安装CMake
		最低版本：2.8.11.1
		如何在Windows上安装CMake请参考：http://www.cmake.org/

###4、下载HotPot源代码
		git clone https://github.com/randyliu/HotPot

###5、CMake生成工程文件
		假定使用Microsoft Visual Studio

###6、用管理员身份启动Microsoft Visual Studio


###7、运行INSTALL项目


配置
----
###1、配置PHP
		在php.ini中添加php_hotscript.dll扩展。
			例子：extension = "D:\build\source\php_hotscript\Debug\php_hotscript.dll"
		在php.ini中添加HotPot安装目录
			例子hotpot.dir="C:\Program Files\HotPot"

###2、配置环境变量
		配置环境变量
			HOTPOT_DIR="C:\Program Files\HotPot"
