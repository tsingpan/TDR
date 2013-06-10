#ifndef _H_XLIST
#define _H_XLIST

typedef struct tagXLIST_HEAD
{
	struct tagXLIST_HEAD *next, *prev;
}XLIST_HEAD;

#define init_xlist_head(_head)\
do\
{\
	(_head)->next = _head;\
	(_head)->prev = _head;\
}while(0)

#define __xlist_add(_new, _prev, _next)\
do\
{\
	(_new)->next = (_next);\
	(_new)->prev = (_prev);\
	(_new)->next->prev = (_new);\
	(_new)->prev->next = (_new);\
}while(0)

#define xlist_add(_new, _head)\
do\
{\
	__xlist_add((_new), (_head), ((_head)->next));\
}while(0)

#define xlist_add_tail(_new, _head)\
do\
{\
	__xlist_add((_new), (_head)->prev, (_head));\
}while(0)

#define __xlist_del(_prev, _next)\
do\
{\
	(_next)->prev = _prev;\
	(_next)->prev->next = (_next);\
}while(0)

#define xlist_del(entry)\
do\
{\
	__xlist_del((entry)->prev, (entry)->next);\
}while(0)

#define xlist_fix(_prev, _next)\
do\
{\
	(_prev)->next = _next;\
	(_prev)->next->prev = _prev;\
}while(0)

#endif //_H_XLIST
