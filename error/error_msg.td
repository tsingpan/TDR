import "error/error_code.td"

const uint32 TD_MAX_ERROR_MSG_LENGTH = 1024;

struct TD_ERROR_MSG
{
	TD_ERROR_CODE error_code;
	string<TD_MAX_ERROR_MSG_LENGTH> error_msg;
};

struct TD_ERROR_MSG_LIBRARY
{
	uint32 error_list_num;
	vector<TD_ERROR_MSG, TD_ERROR_CODE_NUM> error_list(error_list_num);
};

