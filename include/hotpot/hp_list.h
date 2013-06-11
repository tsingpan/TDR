#ifndef _H_HP_LIST
#define _H_HP_LIST

typedef struct tagHP_LIST_HEAD
{
	struct tagHP_LIST_HEAD *next, *prev;
}HP_LIST_HEAD;

#define init_hp_list_head(_head)\
do\
{\
	(_head)->next = _head;\
	(_head)->prev = _head;\
}while(0)

#define __hp_list_add(_new, _prev, _next)\
do\
{\
	(_new)->next = (_next);\
	(_new)->prev = (_prev);\
	(_new)->next->prev = (_new);\
	(_new)->prev->next = (_new);\
}while(0)

#define hp_list_add(_new, _head)\
do\
{\
	__hp_list_add((_new), (_head), ((_head)->next));\
}while(0)

#define hp_list_add_tail(_new, _head)\
do\
{\
	__hp_list_add((_new), (_head)->prev, (_head));\
}while(0)

#define __hp_list_del(_prev, _next)\
do\
{\
	(_next)->prev = _prev;\
	(_next)->prev->next = (_next);\
}while(0)

#define hp_list_del(entry)\
do\
{\
	__hp_list_del((entry)->prev, (entry)->next);\
}while(0)

#define hp_list_fix(_prev, _next)\
do\
{\
	(_prev)->next = _next;\
	(_prev)->next->prev = _prev;\
}while(0)

#endif //_H_XLIST
