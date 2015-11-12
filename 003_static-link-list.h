#ifndef STATIC_LINK_LIST_H_
#define STATIC_LINK_LIST_H_

#include"002_complex.h"

class SL_list{
public:
	//Default constructor
	SL_list():first_using( -1 ), last_using( -1 ), first_remaining( 0 ), last_remaining( init_size ) {}
	//Use the initilized default copy constructor & destructor

	//Interface
	ElemType first() const{ if( first_using != -1 ){ return elem[first_using] } }
	ElemType last() const{ if( last_using != -1 ){ return elem[last_using] } }
	void insert( ElemType &elem_i );
	void remove( ElemType &elem_r );
	size_t length(){ return length }
	ElemType get_elem( Index index );
	bool empty(){ return !length }
	void print( Index index = -1 );
	void clear();
public:
	typedef Complex ElemType;
	typedef int Index;
	static const init_size = 100;

private:
	typedef struct node{
		ElemType elem;
		Index next;
	}ElemNode;
private:
	//Imulate no pointer, so use array rather than pointer
	//size of array is fixed, CANNOT change
	ElemNode elem[ init_size ];
	Index first_using; 
	Index last_using;
	Index first_remaining;
	Index last_remaining;
};

#endif
