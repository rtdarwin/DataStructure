#ifndef SEQUENTIAL_SPARSE_MATRIX_H_
#define SEQUENTIAL_SPARSE_MATRIX_H_


//Define the MatrixTriple used in SSparseMatrix
template< typename T > class SSparseMatrix;
template< typename T >
class MatrixTriple{
	friend class SSparseMatrix< T >;
	unsigned row, col;
	T elem;
};

//Define class SSparseMatrix
template< typename T >
class SSparseMatrix{
public:
	typedef T        value_type;
	typedef unsigned size_type;

public:
	//When creat an object, MUST give its row and col
	SSparseMatrix( size_t rows, size_t cols, size_t given_capacity = 0 )
		: _rows(rows), _cols(cols), _elem_total(0){
		if( given_capacity == 0 ){
			_capacity = 0.05 * _rows * _cols;
		}
		_node = new MatrixTriple<T>[ _capacity ];
		_elem_cout_in_col      = new int[_cols+1];
		_begin_position_of_col = new int[_cols+1];
		_elem_cout_in_row      = new int[_rows+1];
		_begin_position_of_row = new int[_rows+2]; //为在矩阵相乘时方便得到最后一行的终止位置，虚构第_rows+1行
		//Initilize all the array
		for( size_t cur_row = 1; cur_row <= _rows; cur_row++ ){
			_elem_cout_in_row[cur_row] = 0;
			_begin_position_of_row[cur_row] = 0;
		}
		for( size_t cur_col = 1; cur_col <= _cols; cur_col++ ){
			_elem_cout_in_col[cur_col] = 0;
			_begin_position_of_col[cur_col] = 0;
		}
	}
	//Copy constructor
	SSparseMatrix( const SSparseMatrix& src_matrix );
	//Destructor
	~SSparseMatrix(){
		delete []_node;
		delete []_elem_cout_in_col;
		delete []_begin_position_of_col;
		delete []_elem_cout_in_row;
		delete []_begin_position_of_row;
	}
	//Assign operator
	SSparseMatrix& operator= ( const SSparseMatrix& src_matrix );


	//Public function 
	size_t        read_content( void );
	bool          empty()const { return !_elem_total; }
	size_t        elem_total()const { return _elem_total; }
	size_t        capacity()const { return _capacity; }
	size_t        rows()const { return _rows; }
	size_t        cols()const { return _cols; }
	value_type    elem( size_t row_idx, size_t col_idx )const;
	void          clear();
	SSparseMatrix& transpose(){ _fast_transpose(); }
	SSparseMatrix& print_row( size_t row )const;
	SSparseMatrix& print()const;
	SSparseMatrix& multiply( const SSparseMatrix& matrix_b ){ _fast_multiply( matrix_b ); }
	bool operator== ( const SSparseMatrix& matrix_b ){ return _equal( matrix_b ); }
	bool operator!= ( const SSparseMatrix& matrix_b ){ return !(*this==matrix_b ); }
	//friend template ` operator* ` 
		//Is this usage correct?
	friend SSparseMatrix< T >& operator* < T > ( const SSparseMatrix< T >& matrix_a, const SSparseMatrix< T >& matrix_b );

private:

	size_t _rows; //coutn from 1
	size_t _cols;
	size_t _elem_total;
	size_t _capacity;

	MatrixTriple< T > *_node;	
	//the array used for fast-transpose
	//**Position begin from 0**
	int *_elem_cout_in_col;
	int *_begin_position_of_col;
	//the array used for fast-multiply
	int *_elem_cout_in_row;
	int *_begin_position_of_row;

private:
	void _generate_internal_data();
	bool _equal( const SSparseMatrix& matrix_b );
	void _fast_transpose();
	void _fast_multiply( const SSparseMatrix& matrix_b );
	void _extend_capacity( size_t times = 2 );
	void _push_back( size_t row, size_t col, T value );

};

#include"007_sequential-spare-matrix.cpp"
#endif
