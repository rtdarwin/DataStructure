#include<cstdlib>
#include<cstring>
#include"005_round-robin-queue.h"

RRqueue::RRqueue( const RRqueue& src_queue ){
	_capacity = src_queue._capacity;
	_front = src_queue._front;
	_back = src_queue._back;
	_size = src_queue._size;
	_full = src_queue.full();
	_base = new ElemType[_capacity];
	memcpy( _base, src_queue._base, _size );
}

RRqueue::~RRqueue(){
	delete []_base;
}

void RRqueue::push( ElemType val ){
	if( _full ){  return; }
	_size++;
	_base[_back] = val;
	_back = ( _back + 1 ) % _capacity;
	if( _front == _back ){	_full = true; }
}

void RRqueue::pop(){
	if( empty() ){	return; }
	_front = ( _front + 1 ) % _capacity;
	_full = false;
	_size--;
}

