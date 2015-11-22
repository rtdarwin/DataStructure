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

public:
	//When creat an object, MUST give its row and col
	SSparseMatrix( int rows, int cols, int given_capacity = 0 )
		: _rows(rows), _cols(cols), _elem_total(0){
		if( given_capacity == 0 ){ _capacity = 0.05 * _rows * _cols; }
		else{ _capacity = given_capacity; }
		_node = new MatrixTriple<T>[ _capacity ];
		_elem_cout_in_col = new int[_cols+1];
		_begin_pos_of_col = new int[_cols+1];
		_elem_cout_in_row = new int[_rows+1];
		_begin_pos_of_row = new int[_rows+2]; //为在矩阵相乘时方便得到最后一行的终止位置，虚构第_rows+1行
		//Initilize all the array
		for( int i = 1; i <= _rows; i++ ){
			_elem_cout_in_row[i] = 0;
			_begin_pos_of_row[i] = 0;
		}
		for( int j = 1; j <= _cols; j++ ){
			_elem_cout_in_col[j] = 0;
			_begin_pos_of_col[j] = 0;
		}
	}
	//Copy constructor
	SSparseMatrix( const SSparseMatrix& src_matrix );
	//Destructor
	~SSparseMatrix(){
		delete []_node;
		delete []_elem_cout_in_col;
		delete []_begin_pos_of_col;
		delete []_elem_cout_in_row;
		delete []_begin_pos_of_row;
	}
/*
	//Assign operator
	SSparseMatrix& operator= ( const SSparseMatrix& src_matrix );

*/

	//Public function 
	int     read_content( istream& is );
	bool    empty()const { return !_elem_total; }
	int     elem_total()const { return _elem_total; }
	int     capacity()const { return _capacity; }
	int     rows()const { return _rows; }
	int     cols()const { return _cols; }
	value_type   elem( int row_idx, int col_idx )const;
	void         clear();
	SSparseMatrix& transpose(){ _fast_transpose(); }
	SSparseMatrix& print_row( int row )const;
	SSparseMatrix& print()const;
	SSparseMatrix& multiply( const SSparseMatrix& matrix_b ){ _fast_multiply( matrix_b ); }
	bool operator== ( const SSparseMatrix& matrix_b ){ return _equal( matrix_b ); }
	bool operator!= ( const SSparseMatrix& matrix_b ){ return !(*this==matrix_b ); }
	//friend template ` operator* ` 
		//Is this usage correct? --- maybe not, gcc says that `declaration of operator* as non-function`
	template< class Type >
	friend SSparseMatrix< Type >& operator* ( const SSparseMatrix< Type >& matrix_a, const SSparseMatrix< Type >& matrix_b );


private:

	int _rows; //coutn from 1
	int _cols;
	int _elem_total;
	int _capacity; //cout from 1

	MatrixTriple< T > *_node;	
	//the array used for fast-transpose
	//**Position begin from 0**
	int *_elem_cout_in_col;
	int *_begin_pos_of_col;
	//the array used for fast-multiply
	int *_elem_cout_in_row;
	int *_begin_pos_of_row;

private:
	void _generate_internal_data();
	bool _equal( const SSparseMatrix& matrix_b );
	void _fast_transpose();
	void _fast_multiply( const SSparseMatrix& matrix_b );
	void _extend_capacity( int times = 2 );
	SSparseMatrix< T >& _push_back( int row, int col, T value );

};

#include"007_sequential-sparse-matrix.cpp"
#endif
