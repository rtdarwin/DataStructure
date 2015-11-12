#ifndef SQ_LIST_H_
#define SQ_LIST_H_
typedef int Length;
typedef int Loc;
typedef struct{
	int   int_value;
	float float_value;
}ElemType;
typedef struct sq_list_tag{
	ElemType *elem;
	Length length;
	size_t size;
} *sq_list;

static const int init_list_size = 100;
static const int incr_list_size = 10;

extern sq_list init_sq_list( sq_list list );
extern void destory_list( sq_list list );
extern void clear_list( sq_list list );
extern void sort_list( sq_list list );
extern bool list_empty( sq_list list );
extern Length list_length( sq_list list );
extern void  get_elem( sq_list list, Loc location, ElemType *elem );
extern Loc locate_elem( sq_list list, ElemType *elem );
extern void prior_elem( sq_list list, ElemType *cur_elem, ElemType *prior_elem );
extern void next_elem( sq_list list, ElemType *cur_elem, ElemType *next_elem );
extern void list_insert( sq_list list, Loc insert_loc, ElemType *insert_elem );
extern void list_delete( sq_list list, Loc del_loc );
extern void merge_list( sq_list list1, sq_list list2, sq_list merged_list );

#endif
