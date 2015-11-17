#include<iostream>
#include<cstdlib>
#include<cstring>

using std::cout;

//Copy Constructor
template< typename T >
SSparseMatrix< T >::SSparseMatrix( const SSparseMatrix& src_matrix ){
	//Copy all the private member
	_rows = src_matrix._row;
	_cols = src_matrix._cols;
	_elem_total = src_matrix._elem_total;
	_capacity = src_matrix._capacity;
	_node              = new MatrixTriple< T >[_elem_total];
	_elem_cout_in_col  = new int[_cols+1];
	_begin_pos_of_col  = new int[_cols+1];
	_elem_cout_in_row  = new int[_rows+1];
	_begin_pos_of_row  = new int[_rows+2];
	//Copy the array
	memcpy( _node, src_matrix._node, _elem_total );
	memcpy( _elem_cout_in_col, src_matrix._elem_cout_in_col, _cols+1 );
	memcpy( _begin_pos_of_col, src_matrix._begin_pos_of_col, _cols+1 );
	memcpy( _elem_cout_in_row, src_matrix._elem_cout_in_row, _rows+1 );
	memcpy( _begin_pos_of_row, src_matrix._begin_pos_of_row, _rows+2 );
}

//Assign operator
template< typename T >
SSparseMatrix< T >& SSparseMatrix::operator= ( const SSparseMatrix& src_matrix ){
	this->SSparseMatrix( src_matrix );
}

//Read Matrix from \istream is
template< typename T >
size_t SSparseMatrix< T >::read_content( istream& is ){
	value_type cur_e;
	for( int i = 0; i < _rows; i++ ){
		for( int j = 0; j < _cols; j ++ ){
			if( is >> cur_e, cur_e ){
				_push_back( i, j, cur_e );
			}
		}
	}
	_generate_internal_data();
}


//Print row
template< typename T >
void SSparseMatrix< T >::print_row( size_t row )const{
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
template< typename T >
void SSparseMatrix< T >::print()const{
	for( size_t i = 1; i <= _rows; i++ ){
		print_row( i );
		cout << endl;
	}
}


//Get elem
template< typename T >
T SSparseMatrix< T >::elem( size_t row_idx, size_t col_idx )const{
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

//Clear
template< typename T >
void SSparseMatrix< T >::clear(void){
	delete []_node;
	delete []_elem_cout_in_col;
	delete []_begin_pos_of_col;
	delete []_elem_cout_in_row;
	delete []_begin_pos_of_col;
	_node = _elem_cout_in_col = _begin_pos_of_col = _elem_cout_in_row = _begin_pos_of_row = NULL;
}

//Multiply
template< typename T >
SSparseMatrix< T > operator* ( const SSparseMatrix< T >& matrix_a, const SSparseMatrix< T >& matrix_b ){
	SSparseMatrix temp_matrix( matrix_a );
	temp_matrix.multiply( matrix_b );
	return temp_matrix;
}

//------------------------INTERNAL FUNCTION-----------------------

template< typename T>
void _generate_internal_data(void){
	for( size_t cur_pos = 0; cur_pos != _elem_total; cur_pos++ ){
		size_t j = _node[cur_pos].col;
		size_t i = _node[cur_pos].row;
		_elem_cout_in_col[j]++;
		_elem_cout_in_row[i]++;
	}

	_begin_pos_of_col[1] = 0;
	_begin_pos_of_row[1] = 0;
	for( size_t j = 2; j <= _cols; j++ ){
		_begin_pos_of_col[i] = _begin_pos_of_col[i-1] + _elem_cout_in_col[i-1];
	}
	for( size_t i = 2; i <= _rows; i++ ){
		_begin_pos_of_row[i] = _begin_pos_of_row[i-1] + _elem_cout_in_row[i-1];
	}
}
template< typename T >
bool SSparseMatrix< T >::_equal( const SSparseMatrix<T>& matrix_b ){
	if( _rows != matrix_b._rows 
		|| _cols != matrix_b._cols 
		|| _elem_total != matrix_b._elem_total ){
		return false;
	} else {
		for( int i = 0; i != _elem_total; i++ ){
			if( _node[i] != matrix_b._node[i] ){ return false; }
		}
	}
	return true;
}


template< typename T >
void SSparseMatrix< T >::_fast_transpose(){
	if( !_elem_total ) return;
	_generate_internal_data();
	cur_pos_of_col = new int[_cols+1];
	for( size_t j = 1; j <= _cols; j++ ){
		cur_pos_of_col[j] = _begin_pos_of_col[j];
	}
	//Begin transpose
	SSparseMatrix temp_matrix( *this );	//Is this ok?
	for( size_t src_pos = 0; src_pos != _elem_total; src_pos++ ){
		size_t j = _node[src_pos].col;
		size_t dest_pos = cur_pos_of_col[j];
		temp_matrix._node[dest_pos].row = _node[src_pos].col;
		temp_matrix._node[dest_pos].col = _node[src_pos].row;
		temp_matrix._node[dest_pos].elem = _node[src_pos].elem;
		++ cur_pos_of_col[j];
	}
	*this = temp_matrix;
}


template< typename T >
void SSparseMatrix< T >::_fast_multiply( const SSparseMatrix<T>& matrix_b ){
	//Exclude the situation that the a._cols != b._rows
	if( _cols != matrix_b._rows ){  return;  }
	_generate_internal_data();
	//Creat temp array to store the result & tempory matrix
	T *temp_result = new T[matrix_b._cols+1];
	for( size_t cur_col = 1; cur_col <= matrix_b._cols; cur_col++ ){
		temp_result[cur_col] = 0;
	}
	SSparseMatrix result_matrix( _rows, matrix_b._cols, _capacity );
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
			//将此元素与b中对应行的每个元素相乘，并放入temp_result 中
			while( cur_elem_pos_of_b++ < cur_row_end_of_b ){
				temp_col = matrix_b._node[cur_elem_pos_of_b].col;
				temp_result[temp_col] += _node[cur_elem_pos_of_a].elem
							 * matrix_b._node[cur_elem_pos_of_b].elem;
			}
		}
		//Compress and store
		for( size_t cur_col = 1; cur_col <= matrix_b._cols; cur_col++ ){
			if( temp_result[cur_col] ){
				//处理容量不足的情况
				if( result_matrix._elem_total + 1 == result_matrix._capacity - 1 ){
					result_matrix._extend_capacity();
				}
				result_matrix._push_back( cur_row, cur_col, temp_result[cur_col] );
			}
		}
	}
	*this = result_matrix;
}


//增加容量，一次1倍
template< typename T >
void SSparseMatrix< T >::_extend_capacity( size_t times ){
	realloc( _node, _capacity * times );
}


//Append elem
template< typename T >
SSparseMatrix<T>& SSparseMatrix< T >::_push_back( size_t row, size_t col, T value ){
	++_elem_total;
	if( _elem_total == _capacity - 1 ){  // _capacity-1 以确保有一个元素的余量
		_extend_capacity();
	}
	_node[_elem_total-1].row = row; //_elem_total couts from 1, but position couts from 0
	_node[_elem_total-1].col = col;
	_node[_elem_total-1].elem = value;
	return *this;
}
