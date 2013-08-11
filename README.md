HotPot����С���ǵľ۱���
========================
�ṩ�����ݵĿ�ƽ̨�� ������֧�ֵȹ��ܣ� ���������Ļ�����

��װ
----
#windows
###1����װPHP
		���Ͱ汾:5.5.1
		��װ���ο���https://wiki.php.net/internals/windows/stepbystepbuild
		��Ҫ�������û�������PHP-DEVEL_DIRΪphp�ĸ�Ŀ¼

###2����װLUA
		���Ͱ汾:5.5.2
		��̬�汾��LUA���ο�](htp://sourceforge.net/projects/luabinaries/files/5.2.1/Windows%20Libraries/Static/)
		��Ҫ�������û�������LUA_DIRΪlua�ĸ�Ŀ¼
		
###3����װCMake
		���Ͱ汾��2.8.11.1
		������Windows�ϰ�װCMake���ο���http://www.cmake.org/

###4������HotPotԴ����
		git clone https://github.com/randyliu/HotPot

###5��CMake���ɹ����ļ�
		�ٶ�ʹ��Microsoft Visual Studio

###6���ù���Ա��������Microsoft Visual Studio


###7������INSTALL��Ŀ


����
----
###1������PHP
		��php.ini������php_hotscript.dll��չ��
			���ӣ�extension = "D:\build\source\php_hotscript\Debug\php_hotscript.dll"
		��php.ini������HotPot��װĿ¼
			����hotpot.dir="C:\Program Files\HotPot"

###2�����û�������
		���û�������
			HOTPOT_DIR="C:\Program Files\HotPot"
