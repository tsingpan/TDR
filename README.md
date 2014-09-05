The Data Representation
=======================

TDR = [DDL](http://en.wikipedia.org/wiki/Data_Definition_Language) + [Compiler](http://en.wikipedia.org/wiki/Compiler)

详细手册， 请点击这里 → [https://github.com/randyliu/TDR/wiki](https://github.com/randyliu/TDR/wiki)


特性
----
- 使用C语言编写， 不链接任何第三方库， 具有很高的可移植性。
- 具有严谨的语法结构，使用[re2c](http://www.re2c.org/)和[bison](http://www.gnu.org/software/bison/)进行编译前端的处理。


问题和反馈
----------
点击这里 → [https://github.com/randyliu/TDR/issues](https://github.com/randyliu/TDR/issues)

安装
----
详细的安装说明， 请点击这里 → [https://github.com/randyliu/TDR/wiki/Installation](https://github.com/randyliu/TDR/wiki/Installation)

Linux下的安装过程如下：

	git clone https://github.com/randyliu/TDR tdr
	mkdir build
	cd build
	cmake ../tdr
	make
	sudo make install
