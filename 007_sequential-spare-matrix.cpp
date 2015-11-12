#include<iostream>
#include<cstdlib>
#include<cstring>

using std::cout;

//Constructor
template< class T >
SSpareMatrix::SSpareMatrix( const SSpareMatrix& src_matrix ){
	//Copy all the private member
	_rows = src_matrix._row;
	_cols = src_matrix._cols;
	_elem_total = src_matrix._elem_total;
	_capacity = src_matrix._capacity;
	_node                  = new MatrixTriple< T >[_elem_total];
	_elem_cout_in_col      = new int[_cols+1];
	_begin_pos_of_col = new int[_cols+1];
	_elem_cout_in_row      = new int[_rows+1];
	_begin_pos_of_row = new int[_rows+1];
	//Copy the array
	memcpy( _node, src_matrix._node, _elem_total );
	memcpy( _elem_cout_in_col, src_matrix._begin_pos_of_col, _cols );
	memcpy( _begin_pos_of_col, src_matrix._begin_pos_of_col, _cols );
	memcpy( _elem_cout_in_row, src_matrix._elem_cout_in_row, _rows );
	memcpy( _begin_pos_of_row, src_matrix._begin_pos_of_row, _rows );
}

//Assign operator
template< class T >
SSpareMatrix& SSpareMatrix::operator= ( const SSpareMatrix& src_matrix ){
	this->SSpareMatrix( const SSpareMatrix& src_matrix );
}

//Read Matrix from \istream is
template< class T >
size_t read_content( istream& is ){
	
}


//Print row
template< class T >
void SSpareMatrix::print_row( size_t row ){
	int cur_pos = _begin_pos_of_row[row];
	for( size_t cur_col = 1; cur_col != _cols; cur_col ++ ){
		if( _node[cur_pos+i].col = cur_col+1 ){
			cout << _node[cur_pos].elem << " ";
			++cur_pos;
		}else{
			cout << "0 ";
		}
	}
}

//Print Matrix
template< class T >
void SSpareMatrix::print(){
	for( size_t i = 1; i <= _rows; i++ ){
		print_row( i );
		cout << endl;
	}
}


//Get elem
template< class T >
T SSpareMatrix::elem( size_t row_idx, size_t col_idx ){
	if( row_idx <= _rows && col_idx <= _cols ){
		size_t cur_pos = _begin_pos_of_row[row_idx];
		for( size_t cur_col = 1; cur_col <= _cols; cur_col ++ ){
			if( _node[cur_pos].col = col_idx ){
				return _node[cur_pos].elem;
			}
		}
	}else{
		return T( 0 );
	}
}

//Multiply
SSpareMatrix& operator* ( const SSpareMatrix& matrix_a, const SSpareMatrix& matrix_b ){
	SSpareMatrix temp_matrix( matrix_a );
	temp_matrix.multiply( matrix_b );
	return temp_matrix;
}

//------------------------INTERNAL FUNCTION-----------------------

template< class T >
bool _equal( const SSpareMatrix<T>& matrix_b ){
}


template< class T >
void SSpareMatrix::_fast_transpose(){
	//Exclude the suitation matrix is empty
	if( !_elem_total ) return;
	//计算 _elem_cout_in_col[] 
	for( size_t cur_pos = 0; cur_pos != _elem_total; cur_pos++ ){
		size_t cur_col = _node[cur_pos].col;
		_elem_cout_in_col[cur_col]++;
	}
	//累加 _elem_cout_in_col[] 以计算 _begin_pos_of_col[] 并将其副本放入 cur_pos_of_col 
	cur_pos_of_col = new int[_cols+1];
	cur_pos_of_col[1] = _begin_pos_of_col[1] = 0;
	for( size_t cur_col = 2; cur_col <= _cols; cur_col++ ){
		cur_pos_of_col[cur_col] = _begin_pos_of_col[cur_col] = _begin_pos_of_col[cur_col-1] + _elem_cout_in_col[cur_col-1];
	}
	//BEGIN TRANSPOSE
	SSpareMatrix temp_matrix( *this );	//Is this ok?
	for( size_t src_pos = 0; src_pos != _elem_total; src_pos++ ){
		size_t cur_col = _node[src_pos].col;
		size_t dest_pos = src_pos_of_col[cur_col];
		temp_matrix._node[dest_pos].row = _node[src_pos].col;
		temp_matrix._node[dest_pos].col = _node[src_pos].row;
		temp_matrix._node[dest_pos].elem = _node[src_pos].elem;
		++ cur_pos_of_col[cur_col];
	}
	//
	*this = temp_matrix;
}


template< class T >
void SSpareMatrix::_fast_multiply( const SSpareMatrix<T>& matrix_b ){
	//Exclude the situation that the a._cols != b._rows
	if( _cols != matrix_b._rows ){  return;  }
	//创建临时存储未压缩当前行的数组以及临时矩阵
	T *temp_result_row = new T[matrix_b._cols+1];
	for( size_t cur_col = 1; cur_col <= matrix_b._cols; cur_col++ ){
		temp_result[cur_col] = 0;
	}
	SSpareMatrix result_matrix( _rows, matrix_b._cols, _capacity );
	//依次计算result-matrix的每一行(以metrix_a 的行)
	for( size_t cur_row = 1; cur_row <= _rows; cur_row++ ){
		size_t cur_elem_pos_of_a = _begin_pos_of_row[cur_row];
		size_t cur_row_end_of_a  = _begin_pos_of_row[cur_row+1];
		//Deal with every elem in this row
		while( cur_elem_pos_of_a++ < cur_row_end_of_a ){
			//找到这个元素在b中对应的行号
			col_of_cur_elem = _node[cur_elem_pos_of_a].col;
			//确定b中行的起始位置和结束位置
			size_t cur_elem_pos_of_b = matrix_b._begin_pos_of_row[col_of_a_elem];
			size_t cur_row_end_of_b  = matrix_b._begin_pos_of_row[col_of_a_elem+1];
			//将此元素与b中对应行的每个元素相乘，并放入temp_result_row 中
			while( cur_elem_pos_of_b++ < cur_row_end_of_b ){
				temp_col = matrix_b._node[cur_elem_pos_of_b].col;
				temp_result[temp_col] += _node[cur_elem_pos_of_a].elem
							 * matrix_b._node[cur_elem_pos_of_b].elem;
			}
		}
		//Compress and store
		for( size_t cur_col = 1; cur_col <= matrix_b._cols; cur_col++ ){
			if( temp_result_row[cur_col] ){
				//处理容量不足的情况
				if( result_matrix._elem_total + 1 == result_matrix._capacity ){
					result_matrix._extend_capacity();
				}
				result_matrix._pop_back( cur_row, cur_col, temp_result_row[cur_col] );
			}
		}
	}
	*this = result_matrix;
}


//增加容量，一次1倍
template< class T >
void SSpareMatrix::_extend_capacity( size_t times ){
	realloc( m._node, m._capacity * times );
}


//Append elem
template< class T >
void SSpareMatrix::_push_back( size_t row, size_t col, T value ){
	++_elem_total;
	_node[_elem_total].row = row;
	_node[_elem_total].col = col;
	_node[_elem_total].elem = value;
}
