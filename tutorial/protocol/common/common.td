enum message_id_e
{
	E_MID_LOGIN_REQ			= 0,
	E_MID_LOGIN_RSP			= 1,
};

//这种注释不会出现在最终生成的文件中
/*
	这种注释也不会出现在最终生成的文件中
	但是以#开头的单行注释会出现在最终生成的文件中
*/
#用MESSAGE_ID_NUM来记录枚举类型message_id_e中的成员个数, count函数可以对struct, union, enum这三种类型使用

const uint32 MESSAGE_ID_NUM = count(message_id_e);
const string VERSION = "0.0.1";
const uint32 MAX_NAME_LENGTH = 255;
