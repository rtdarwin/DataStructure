#ifndef ROUND_ROBIN_QUEUE_H_

#define ROUND_ROBIN_QUEUE_H_
#include<cstdlib>

typedef int ElemType;

class RRqueue{
public:
//CONSTRUCTOR & DESTRUCTOR
	//Constructor
	RRqueue( size_t cap )
		: _front(0), _back(0), _size(0), _capacity(cap), _full(false) {
		_base = new ElemType[cap];
	}
	//Copy constructor
	RRqueue( const RRqueue& src_queue );
	//Destructor
	~RRqueue();
//PUBLIC INTERFACE
	size_t   capacity()const { return _capacity; }
	size_t   size()const { return _size; }
	bool     empty()const { return !_size; }
	bool     full()const { return _full; }
	ElemType front()const { return _base[_front]; }
	ElemType back()const { return _base[_back]; }
	void     push( ElemType val );
	void     pop();
	void     clear(){
			_front = _back = _size = 0;
			_full = false;
		}

private:
	ElemType* _base;
	size_t _front;
	size_t _back;
	size_t _size;
	size_t _capacity;
	bool   _full;
};

#endif
