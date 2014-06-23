#ifndef _H_TDR_LIST
#define _H_TDR_LIST

#ifdef  __cplusplus
extern "C" {
#endif


typedef struct tdr_list_head_s
{
	struct tdr_list_head_s *next, *prev;
}tdr_list_head_t;

#define tdr_list_init(_head)\
{\
	(_head)->next = _head;\
	(_head)->prev = _head;\
}

#define	tdr_list_empty(list) ((list)->next == list)

#define __tdr_list_add(_new, _prev, _next)\
{\
	(_new)->next = (_next);\
	(_new)->prev = (_prev);\
	(_new)->next->prev = (_new);\
	(_new)->prev->next = (_new);\
}

#define tdr_list_add(_new, _head)\
{\
	__tdr_list_add((_new), (_head), ((_head)->next));\
}

#define tdr_list_add_tail(_new, _head)\
{\
	__tdr_list_add((_new), (_head)->prev, (_head));\
}

#define __tdr_list_del(_prev, _next)\
{\
	(_next)->prev = (_prev);\
	(_prev)->next = (_next);\
}

#define tdr_list_del(entry)\
{\
	__tdr_list_del((entry)->prev, (entry)->next);\
}

#ifdef  __cplusplus
}
#endif


#endif
