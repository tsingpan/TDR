The Data Representation
======
TDR = [DDL](http://en.wikipedia.org/wiki/Data_Definition_Language) + [Compiler](http://en.wikipedia.org/wiki/Compiler)

详细手册， 请点击这里 → [https://github.com/randyliu/TDR/wiki](https://github.com/randyliu/TDR/wiki)

特性
====

	编译器使用re2c生成词法分析代码， bison生成语法分析代码。 
	使用C语言编写， 无第三方库依赖， 具有很高的可移植性， 支持Windows, Linux, Unix(Mac OSX)平台。

问题和反馈
==========
点击这里 → [https://github.com/randyliu/TDR/issues](https://github.com/randyliu/TDR/issues)

安装
====
详细的安装说明， 请点击这里 → [https://github.com/randyliu/TDR/wiki/Installation](https://github.com/randyliu/TDR/wiki/Installation)

Linux下面一个典型的安装过程如下：

	git clone https://github.com/randyliu/TDR tdr
	mkdir build
	cd build
	cmake ../tdr
	make
	sudo make install
