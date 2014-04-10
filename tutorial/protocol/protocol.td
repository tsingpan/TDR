/*
	协议支持包含功能， 类似c语言中的#include命令， 但是由于生成文件的需要， 所以不能指定多个-I参数来寻找不同目录下的文件。
	所有的import路径都是从唯一的-s参数所表示的目录开始算起的。
*/

//让我们首先来看一个网络协议的例子
import "common/common.td"
//重复包含一个文件， 并不会出错， 因为编译器只会展开第一个文件。
import "common/common.td"


struct login_req
{
	string<MAX_NAME_LENGTH> name;
	string<MAX_NAME_LENGTH> password;
	uint8			age;
};

typedef int32 bool_t;
struct login_rsp
{
		bool_t result;#0表示成功， 非0表示失败
if(result == 0)	uint64 session_id;#可以使用条件判断与据来规定何时出现这个元素， 合法的运算符有==, !=, &这三种。
};

union message_body(message_id selector)
{
E_MID_LOGIN_REQ:
	login_req login_req;	
E_MID_LOGIN_RSP:
	login_rsp login_rsp;
};

#定义网络协议
struct message
{
	message_id mid;
	message_body body(mid);
};


/*
	读取配置文件
*/
enum tconnd_instance_level
{
	e_low,
	e_high,
};
const uint32 TONND_CONFIG_NUM = 1024;
const uint32 IP_LENGTH = 16;
struct tconnd_instance_config
{
	tconnd_instance_level level;
	string<IP_LENGTH> ip;
	uint16 port;
	uint32 backlog;
	uint32 epoll_size;
};
struct tconnd_config
{
	string<MAX_NAME_LENGTH>					log_config;
	vector<tconnd_instance_config, TONND_CONFIG_NUM> 	instance_config;
};


/*
	读取道具表格
*/
enum item_type
{
	crystal,
	other,
};

const uint32 ITEM_NAME_LENGTH = 1024;
const uint32 ITEM_VEC_NUM = 10;
struct item_table
{
	uint32 						ID;
	string<ITEM_NAME_LENGTH>			ItemName;
	int32						Metempsychosis;
	uint32						UseTime;
	uint32						Bill;
	uint16						ReplaceItem;
	int32						Medals;
	int32						Value;
	int32						ReserveMoneyFlag;
	uint8						Quality;
	item_type					ItemType;
	uint32						UseSingTime;
	uint32						UserTarget;
	uint32						CanMoved;
	uint32						CanDeleted;
	uint32						CanTrade;
	uint32						CanSold;
	uint32						CanStored;
	uint32						CanLocked;
	uint32						CanBound;
	uint32						IsExclusive;
	uint32						IsExpendable;
	uint32						ColdDownType;
	uint32						ColdDownTime;
	uint32						CanDrop;
	uint32						DecomposePackID;
	uint32						IsMerge;
	vector<string<ITEM_NAME_LENGTH>, ITEM_VEC_NUM>	vec;
};

