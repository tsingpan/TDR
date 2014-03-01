/*
	¶ÁÈ¡Êı¾İ¿â
*/

import "common/common.td"

enum user_type
{
	e_admin,
	e_guest,
};

struct user_s
{
	int32 id;
	string<MAX_NAME_LENGTH> username;
	user_type type;
};
