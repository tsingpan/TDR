/*
	协议支持包含功能， 类似c语言中的#include命令， 但是由于生成文件的需要， 所以不能指定多个-I参数来寻找不同目录下的文件。
	所有的import路径都是从唯一的-s参数所表示的目录开始算起的。
*/

//让我们首先来看一个网络协议的例子
import "common/common.td"


struct login_req_s
{
	string<MAX_NAME_LENGTH> name;
	string<MAX_NAME_LENGTH> password;
	uint8			age;
};

typedef int32 bool_t;
struct login_rsp_s
{
		bool_t result;#0表示成功， 非0表示失败
if(result == 0)	uint64 session_id;#可以使用条件判断与据来规定何时出现这个元素， 合法的运算符有==, !=, &这三种。
};

union message_body_u(message_id_e selector)
{
E_MID_LOGIN_REQ:
	login_req_s login_req;	
E_MID_LOGIN_RSP:
	login_rsp_s login_rsp;
};

#定义网络协议
struct message_s
{
	message_id_e mid;
	message_body_u body(mid);
};


/*
	读取配置文件
*/
enum tconnd_instance_level_e
{
	e_low,
	e_high,
};
const uint32 TONND_CONFIG_NUM = 1024;
const uint32 IP_LENGTH = 16;
struct tconnd_instance_config_s
{
	tconnd_instance_level_e level;
	string<IP_LENGTH> ip;
	uint16 port;
	uint32 backlog;
	uint32 epoll_size;
};
struct tconnd_config_s
{
	string<MAX_NAME_LENGTH>					log_config;
	vector<tconnd_instance_config_s, TONND_CONFIG_NUM> 	instance_config;
};


/*
	读取道具表格
*/
enum item_type_e
{
	e_crystal,
	e_ectype,
	e_other,
};

union item_limit_u(item_type_e selector)
{
e_crystal:	uint32 level;
e_ectype:	uint32 mapid;
e_other:	uint32 gold;
};

const uint32 ITEM_MAX_LIMIT = 10;
struct item_table_s
{
	uint64 id;
	string<MAX_NAME_LENGTH> name;
	item_type_e type;
	vector<item_limit_u, ITEM_MAX_LIMIT> limit_list(type);#一般来说不要把union类型作为数组， 这里只是演示下union类型是可以作为数组的。
};

