#ifndef MATRIX_GRAPH_H_
#define MATRIX_GRAPH_H_
#include<iostream>
#include<utility>
#include<vector>
#include<string>
#include<cstring>
#include<exception>
#include<stdexcept>
#include<inttypes.h>


#define INFINITY INT32_MAX

using namespace std;

enum GraphKind_t { DiGraph, UnDiGraph, DiNetwork, UnDiNetwork };
/**
 *  T1 : key_type
 *  T2 : value_type
 *  T3 : bool OR other_type to justify DiGraph/Network
 *       edge is two arcs
 */
template< typename T1, typename T2, typename T3, GraphKind_t K >
class MGraph_basic{
public:
	typedef vector< pair<T1, T2> > vex_container_vector;
	typedef T1                     key_type;
	typedef T2                     value_type;
public:
	MGraph_basic( const unsigned& given_vex_num, const unsigned& given_arc_num );
	MGraph_basic( const MGraph_basic& given_graph );
	~MGraph_basic( void );
	void operator= ( const MGraph_basic& given_graph );

	unsigned  arc_number()const{ return arc_num; }
	unsigned  vex_number()const{ return vex_num; }
	void      print_vex( ostream& os )const;
	ostream  &print_matrix( ostream& os )const;

	istream  &read_content( istream& is );
	void      add_vex( const T1& new_key, const T2& new_vex_value );
	void      remove_vex( const T1& old_vex );
	void      add_arc( const T1& tail, const T1& head );
	void      add_arc( const T1& tail, const T1& head, const T3& power );
	void      remove_arc( const T1& tail, const T1& head );
	unsigned  in_degree( const T1& key );
	unsigned  out_degree( const T1& key );
	T2       &get_value( const T1& key );
	unsigned  get_location( const T1& key );
	void      edit_value( const T1& key, const T2& new_value );
	void      edit_power( const T1& key1, const T1& key2, const T3& new_power );
	void      adj_vex( const T1& key, vector< pair<T1, T2> >& vex_container );
	bool      is_adj( const T1& key_1, const T1& key_2 );

private:
	void init_matrix_with_zero( void );
private:
	T3 ** arc_matrix;
	vector<  pair<T1, T2>  > vexs; //map is a better choice, but it's too late when i know this.
	unsigned vex_num;
	unsigned arc_num;
};


//------------------------ TYPEDEF -----------------------------

typedef MGraph_basic< char, string, bool, DiGraph >        MDGraph;
typedef MGraph_basic< char, string, bool, UnDiGraph >      MUDGraph;
typedef MGraph_basic< char, string, int32_t, DiNetwork >   MDNetwork;
typedef MGraph_basic< char, string, int32_t, UnDiNetwork > MUDNetwork;

//----------------------- FUNCTION MEMBER ----------------------


template< typename T1, typename T2, typename T3, GraphKind_t K >
MGraph_basic< T1, T2, T3, K >::MGraph_basic( const unsigned& given_vex_num, const unsigned& given_arc_num ){
	vex_num = given_vex_num;
	arc_num = given_arc_num;
	arc_matrix = new T3*[ vex_num ];
	for( unsigned i = 0; i < vex_num; i++ ){
		arc_matrix[i] = new T3[ vex_num ];
	}
	init_matrix_with_zero();
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
MGraph_basic< T1, T2, T3, K >::MGraph_basic( const MGraph_basic& given_graph ){
	vex_num = given_graph.vex_num;
	arc_num = given_graph.arc_num;
	arc_matrix = new T3*[ vex_num ];
	for( unsigned i = 0; i < vex_num; i++ ){
		arc_matrix[i] = new T3[ vex_num ];
	}
	//Copy original arc_matrix
	memcpy( arc_matrix, given_graph.arc_matrix, vex_num * vex_num );
	vexs = given_graph.vexs;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
MGraph_basic< T1, T2, T3, K >::~MGraph_basic( void ){
	for( unsigned i = 0; i < vex_num; i++ ){
		delete []arc_matrix[ i ]; //delete the i-th row of the matrix
	}
	delete [] arc_matrix;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::operator= ( const MGraph_basic& given_graph ){
	if( vex_num != given_graph.vex_num ){
		throw runtime_error( "when using operator=, vex_number not equal" );
	}

	//memcpy( arc_matrix, given_graph.arc_matrix, vex_num * vex_num );
	memcpy( arc_matrix, given_graph.arc_matrix, vex_number );
	for( unsigned i = 0; i < vex_num; i++ ){
		memcpy( arc_matrix[i], given_graph.arc_matrix[i], vex_num );
	}
	vexs = given_graph.vexs;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::print_vex( ostream& os )const{
	for( unsigned i = 0; i < vex_num; i++ ){
		os << vexs[i].first() << vexs[i].second() << endl;
	}
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
ostream& MGraph_basic< T1, T2, T3, K >::print_matrix( ostream& os )const{
	for( unsigned i = 0; i < vex_num; i++ ){
		for( unsigned j = 0; j < vex_num; j++ ){
			os << arc_matrix[ i ][ j ];
		}
		os << endl;
	}

	return os;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
istream &MGraph_basic< T1, T2, T3, K >::read_content( istream& in ){
	if( !in ){ throw runtime_error( "istream invalid" ); }

	T1 temp_key;
	T2 temp_vex_value;
	for( unsigned i = 1; i <= vex_num; i++ ){
		in >> temp_key >> temp_vex_value;
		vexs.push_back( pair< T1, T2 > (temp_key, temp_vex_value) );
	}

	T1 key_1, key_2;
	T3 power;
	for( unsigned i = 1; i <= arc_num; i++ ){
		in >> key_1 >> key_2;
		add_arc( key_1, key_2 );
		if( K == DiNetwork || K == UnDiNetwork ){
			in >> power;
			add_arc( key_1, key_2, power );
		} else {
			add_arc( key_1, key_2 );
		}
		
		if( K == UnDiGraph ){
			add_arc( key_2, key_1 );
		} else if( K == UnDiNetwork ){
			add_arc( key_2, key_1, power );
		}

		int c;
		while( (in >> c) && c != '\n' )
			;
	}
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::add_vex( const T1& new_key, const T2& new_vex_value ){
	vexs.push_back( pair< T1, T2 > (new_key, new_vex_value) );

	++vex_num;
	T3 **new_arc_matrix = new T3*[vex_num];
	memcpy( new_arc_matrix, arc_matrix, vex_num - 1 );
	for( unsigned i = 0; i < vex_num; i++ ){
		new_arc_matrix[ i ] = new T3[ vex_num ];
		memcpy( new_arc_matrix[ i ], arc_matrix[ i ], vex_num - 1 );
		delete []arc_matrix[ i ];
	}
	delete []arc_matrix;
	arc_matrix = new_arc_matrix;

	for( unsigned i = 0; i < vex_num; i++ ){
		arc_matrix[ vex_num ][ i ] = arc_matrix[ i ][ vex_num ] = 0;
	}
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::remove_vex( const T1& key ){
	T2 value = get_value( key );
	arc_num -= ( in_degree(key) + out_degree(key) );

	unsigned loc = get_location( key );
	if( loc != vex_num-1 ){
		for( unsigned i = 0; i < vex_num; i++ ){
			arc_matrix[ loc ][ i ] = arc_matrix[ vex_num-1 ][ i ];
			arc_matrix[ i ][ loc ] = arc_matrix[ i ][ vex_num-1 ];
		}
		arc_matrix[ loc ][ loc ] = arc_matrix[ vex_num-1 ][ vex_num-1 ];

		replace( vexs.begin(), vexs.end(),
				pair< T1, T2 >(key,value), vexs.back() );
	} // else: just remain the room in matrix

	vector< pair<T1, T2> >::iterator loc_iter = find( vexs.begin(), vex.end(), pair<key,value> );
	vexs.erase( loc_inter );
	--vex_num;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::add_arc( const T1& tail, const T1& head ){
	if( K == DiNetwork || K == UnDiNetwork ){
		throw logic_error( "when adding arc to network, didn't provide a power" );
	}
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	arc_matrix[ tail_loc ][ head_loc ] = true;
	if( K == UnDiGraph || K == UnDiNetwork ){
		arc_matrix[ head_loc ][ tail_loc ] = true;
	}

	++arc_num;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::add_arc( const T1& tail, const T1& head, const T3& power ){
	if( K == DiGraph || K == UnDiGraph ){
		throw logic_error( "when adding arc to graph (not network), wrongly provide a power" );
	}
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	arc_matrix[ tail_loc ][ head_loc ] = power;
	if( K == UnDiGraph || K == UnDiNetwork ){
		arc_matrix[ head_loc ][ tail_loc ] = power;
	}

	++arc_num;
}
	

template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::remove_arc( const T1& tail, const T1& head ){
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	if( K == DiNetwork ){
		arc_matrix[ tail_loc ][ head_loc ] = INFINITY;
	} else if( K == DiGraph ){
		arc_matrix[ tail_loc ][ head_loc ] = false;
	} else if( K == UnDiGraph ){
		arc_matrix[ tail_loc ][ head_loc ] 
			= arc_matrix[ head_loc ][ tail_loc ] = 0;
	} else if( K == UnDiNetwork ){
		arc_matrix[ tail_loc ][ head_loc ] 
			= arc_matrix[ head_loc ][ tail_loc ] = INFINITY;
	}

	--arc_num;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
unsigned MGraph_basic< T1, T2, T3, K >::in_degree( const T1& key ){
	unsigned loc = get_location( key );
	unsigned count = 0;
	for( unsigned i = 0; i < arc_num; i++ ){
		if( K == DiGraph || K == UnDiGraph ){
			if( arc_matrix[ i ][ loc ] == true ){ ++count; }
		} else {
			if( arc_matrix[ i ][ loc ] != INFINITY ){ ++count; }
		}
	}
	
	return count;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
unsigned MGraph_basic< T1, T2, T3, K >::out_degree( const T1& key ){
	unsigned loc = get_location( key );
	unsigned count = 0;
	for( unsigned i = 0; i < arc_num; i++ ){
		if( K == DiGraph || K == UnDiGraph ){
			if( arc_matrix[ loc ][ i ]== true ){ ++count; }
		} else {
			if( arc_matrix[ loc ][ i ] != INFINITY ){ ++count; }
		}
	}
	
	return count;
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
T2& MGraph_basic< T1, T2, T3, K >::get_value( const T1& key ){
	for( vector< pair<T1, T2> >::iterator cur = vexs.begin();
				cur != vexs.end();
				cur++, count++ ){
		if( cur->first == key ){ return cur->second; }
	}

	throw runtime_error( "try to get value of a non-existing key" );
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
unsigned MGraph_basic< T1, T2, T3, K >::get_location( const T1& key ){
	int count = 0;
	for( vector< pair<T1, T2> >::iterator cur = vexs.begin();
				cur != vexs.end();
				cur++, count++ ){
		if( cur->first == key ){ return count; }
	}

	throw runtime_error( "try to get location of a non-existing key" );
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::edit_value( const T1& key, const T2& new_value ){
	T2& old_value = get_value( key );
	replace( vexs.begin(), vexs.end(), 
			pair< T1, T2 >(key,old_value), pair<key,new_value> );
}


//Used only for Network class
template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::edit_power( const T1& tail, const T1& head, const T3& new_power ){
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	arc_matrix[ tail_loc ][ head_loc ] = new_power;
	if( K == UnDiNetwork ){
		arc_matrix[ head_loc ][ tail_loc ] = new_power;
	}
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::adj_vex( const T1& key, vector< pair<T1, T2> >& vex_container ){
	unsigned loc = get_location( key );

	//In
	for( unsigned i = 0; i < vex_num; i++ ){
		if( K == UnDiGraph && arc_matrix[ i ][ loc ] == true ){
			vex_container.push_back( vexs[i] );
		} else if( K == UnDiNetwork && arc_matrix[ i ][ loc ] != INFINITY ){
			vex_container.push_back( vexs[i] );
		}
	}

	//DiGraph & DiNetwork 's Out
	if( K == DiGraph || K == DiNetwork ){
		for( unsigned i = 0; i < vex_num; i++ ){
			if( K == UnDiGraph && arc_matrix[ loc ][ i ] == true ){
				vex_container.push_back( vexs[i] );
			} else if( K == UnDiNetwork && arc_matrix[ loc ][ i ] != INFINITY ){
				vex_container.push_back( vexs[i] );
			}
		}
	}
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
bool MGraph_basic< T1, T2, T3, K >::is_adj( const T1& key_1, const T1& key_2 ){
	unsigned loc_1 = get_location( key_1 );
	unsigned loc_2 = get_location( key_2 );

	for( unsigned i = 0; i < vex_num; i++ ){
		if( K == DiGraph || K == UnDiGraph ){
			if( arc_matrix[ loc_1 ][ loc_2 ] || arc_matrix[ loc_2 ][ loc_1 ] ){
				return true;
			}
		} else {
			if( arc_matrix[ loc_1 ][ loc_2] != INFINITY ){
				return true;
			}
		}
	}
}


template< typename T1, typename T2, typename T3, GraphKind_t K >
void MGraph_basic< T1, T2, T3, K >::init_matrix_with_zero( void ){
	for( unsigned i = 0; i != vex_num; i++ ){
		for( unsigned j = 0; j != vex_num; j++ ){
			arc_matrix[i][j] = ( K == DiGraph || K == DiNetwork )?
						false : INFINITY;
		}
	}
}

#endif
