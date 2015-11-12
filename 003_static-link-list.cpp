#ifndef STATIC_LINK_LIST_H_
#define STATIC_LINK_LIST_H_

#include"002_complex.h"

class SL_list{
public:
	//Default constructor
	SL_list():first_using( -1 ), last_using( -1 ), first_remaining( 0 ), last_remaining( init_size ), length( 0 ) {}
	//Use the initilized default copy constructor & destructor

	//Interface
	ElemType first() const{ if( first_using != -1 ){ return elem[first_using] } }
	ElemType last() const{ if( last_using != -1 ){ return elem[last_using] } }
	void insert( ElemType &elem_i );
	void remove( ElemType &elem_r );
	size_t length(){ return length }
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
	size_t length;
};

#endif
#include<iostream>


void SL_list::insert( ElemType &elem_i ){
}

void SL_list::remove( ElemType &elem_r );

ElemType get_elem( Index index ){
	//Wrong case
	if( index > length || index < 0 ){
//Something unhandled
	}
	//interation
	Index location = first_using;
	for( Index i = 0; i != index; ++i ){
		location = elem[location].next;
	}
	//Return
	return elem[location];
}

void SL_list::print( Index index ){
	if( index == -1 ){
		if( !empty ){
			for( Index i = first_using; i != last_using; i = elem[i].next ){
				cout<< elem[i].elem << endl;
			}
			//Print last elem
			cout << elem[last_using] << endl;
		}
	}else{
		//Wrong case
		if( index > length || index < 0 ){
			return;
		}
		//interation
		Index location = first_using;
		for( Index i = 0; i != index; ++i ){
			location = elem[location].next;
		}
		//Return
		return elem[location];
	}
}

void SL_list::clear(){
	first_using = -1;
	last_using = -1;
	first_remaining = 0;
	last_remaining = init_size;
}
