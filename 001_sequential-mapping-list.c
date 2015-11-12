#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"001_sequential-mapping-list.h"

static bool _same( ElemType *a, ElemType *b );
static bool _less( ElemType *a, ElemType *b );

//-------------------IMPLEMENTATION---------------------------------------------
sq_list init_sq_list( sq_list list ){
	list = ( sq_list )malloc( sizeof(struct sq_list_tag) );	
	list->elem = ( ElemType * )malloc( sizeof( ElemType ) * init_list_size );
	list->length = 0;
	list->size = init_list_size;
	return list;
}

void destory_list( sq_list list ){
	free( list->elem );
	free( list );
	list = NULL;
}

void clear_list( sq_list list ){
	free( list->elem );
	list->length = 0;
}

//For implementation.
void sort_list( sq_list a ){
}

bool list_empty( sq_list list ){
	return !(list->length);
}

Length list_length( sq_list list ){
	return list->length;
}

void get_elem( sq_list list, Loc location, ElemType *elem ){
	if( location >= 0 && location < list->length ){
		*elem = list->elem[location];
	}
}

Loc locate_elem( sq_list list, ElemType *elem ){
	for( int i = 0; i < list->length; i++ ){
		if( _same( elem, &list->elem[i])  )
			return i;
	}
}

void prior_elem( sq_list list, ElemType *cur_elem, ElemType *prior_elem ){
	Loc cur_elem_loc = locate_elem( list, cur_elem );
	if( cur_elem_loc != list->length ){
		get_elem( list, cur_elem_loc-1, prior_elem );
	}
}

void next_elem( sq_list list, ElemType *cur_elem, ElemType *next_elem ){
	Loc cur_elem_loc = locate_elem( list, cur_elem );
	if( cur_elem_loc <= list->length - 1 ){
		get_elem( list, cur_elem_loc+1, next_elem );
	}
}

void list_insert( sq_list list, Loc insert_loc, ElemType *insert_elem ){
	if( list->length == list->size ){
		list->elem = ( ElemType * )realloc( list->elem, sizeof( ElemType ) * ( list->size + incr_list_size ) );
		list->size += incr_list_size;
	}else{
		Loc cur_loc;
		for( cur_loc = list->length-1; cur_loc != insert_loc-1; --cur_loc ){
			list->elem[cur_loc + 1] = list->elem[cur_loc];
		}
		list->elem[insert_loc] = *insert_elem;
		++list->length;
	}
}

void list_delete( sq_list list, Loc del_loc ){
	for( Loc cur_loc = del_loc; cur_loc != list->length-1; cur_loc++ ){
		list->elem[cur_loc] = list->elem[cur_loc + 1];
	}
	--list->length;
}

void merge_list( sq_list list_a, sq_list list_b, sq_list merged_list ){
	//Suppose that the lists have been sorted
	//Suppose that all lists exist
	Length total_length = list_a->length + list_b->length;
	if( total_length > merged_list->size ){
		merged_list->elem = ( ElemType * )realloc( merged_list->elem, sizeof(ElemType) * ( total_length + 10 ) );
		merged_list->size = total_length + 10;
	}

	//Begin merge
	Length a = 0;
	Length b = 0;
	merged_list->length = total_length;
	while( a != list_a->length && b != list_b->length ){
		if( _less( &(list_a->elem[a]), &(list_b->elem[b]) ) ){
			merged_list->elem[a+b] = list_a->elem[a];
			++a;
		}else{
			merged_list->elem[a+b] = list_b->elem[b];
			++b;
		}
	}
	if( a == list_a->length ){
		while( b != list_b->length ){
			merged_list->elem[a+b] = list_b->elem[b];
			++b;
		}
	}else{
		while( a != list_a->length ){
			merged_list->elem[a+b] = list_a->elem[a];
			++a;
		}
	}
}
//---------------------Static function------------------------------------------
bool _same( ElemType *a, ElemType *b ){
	if( a->int_value == b->int_value 
		&&a->float_value == b->float_value ){
		return true;
	}else{
		return  false;
	}
}

bool _less( ElemType *a, ElemType *b ){
	if( a->int_value <= b->int_value ){
		return true;
	}else{
		return false;
	}
}
