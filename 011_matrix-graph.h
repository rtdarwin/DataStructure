#ifndef MATRIX_GRAPH_H_
#define MATRIX_GRAPH_H_
#include<iostream>
#include<utility>
#include<string>
#include<cstring>
#include<queue>
#include<exception>
#include<stdexcept>
#include<inttypes.h>



using namespace std;
//--------------------------- TEMPLATE CLASS ----------------------------------
#define INFINITY INT32_MAX

enum GraphKind_t { DiGraph, UnDiGraph, DiNetwork, UnDiNetwork };
/**
 *  vex_key_type : key_type
 *  vex_info_type : info_type
 *  arc_weight_type : bool OR other_type to justify DiGraph/Network
 *       edge is two arcs
 */
template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
class MGraph_basic{
public:
	MGraph_basic( const unsigned& given_vex_num, const unsigned& given_arc_num );
	MGraph_basic( const MGraph_basic& given_graph );
	~MGraph_basic( void );
	void operator= ( const MGraph_basic& given_graph );

	//Acess
	unsigned       arc_number() const { return arc_num; }
	unsigned       vex_number() const { return vex_num; }
	ostream       &print_vex( ostream& os ) const;
	ostream       &print_matrix( ostream& os ) const;
	vex_key_type  first_adj_vex( const vex_key_type& given_vex_key ) const;
	vex_key_type  next_adj_vex( const vex_key_type& given_vex_key,
				    const vex_key_type& former_adj_vex_key ) const;
	bool           is_adj( const vex_key_type& key_1, const vex_key_type& key_2 );
	unsigned       in_degree( const vex_key_type& key ) const;
	unsigned       out_degree( const vex_key_type& key ) const;
	vex_info_type &get_vex_info( const vex_key_type& key ) const;
	unsigned       get_location( const vex_key_type& key ) const;
	void           DFS_traverse( void ) const;
	void           BFS_traverse( void ) const;

	//Modifier
	istream       &read_content( istream& is );
	void           add_vex( const vex_key_type& new_key,
				const vex_info_type& new_vex_info );
	void           remove_vex( const vex_key_type& old_vex );
	void           add_arc( const vex_key_type& tail, const vex_key_type& head );
	void           add_arc( const vex_key_type& tail, const vex_key_type& head,
				const arc_weight_type& weight );
	void           remove_arc( const vex_key_type& tail, const vex_key_type& head );
	void           edit_vex_info( const vex_key_type& key,
				      const vex_info_type& new_info );
	void           edit_arc_weight( const vex_key_type& key1, const vex_key_type& key2,
					const arc_weight_type& new_weight );

private:
	void init_matrix_with_zero( void );
	void DFS( const unsigned& vex_loc, bool *visited ) const;
	void BFS( const unsigned& vex_loc, bool *visited ) const;
private:
	arc_weight_type ** arc_matrix;
	pair< vex_key_type, vex_info_type > * vexs;
	unsigned vex_num;
	unsigned arc_num;
};


//------------------------ TYPEDEF -----------------------------

typedef MGraph_basic< char, string, bool, DiGraph >        MDGraph;
typedef MGraph_basic< char, string, bool, UnDiGraph >      MUDGraph;
typedef MGraph_basic< char, string, int32_t, DiNetwork >   MDNetwork;
typedef MGraph_basic< char, string, int32_t, UnDiNetwork > MUDNetwork;

//----------------------- FUNCTION MEMBER ----------------------


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
MGraph_basic( const unsigned& given_vex_num, const unsigned& given_arc_num )
{
	vex_num = given_vex_num;
	arc_num = given_arc_num;
	vexs = new pair<vex_key_type, vex_info_type>[vex_num];
	arc_matrix = new arc_weight_type*[ vex_num ];
	for( unsigned i = 0; i < vex_num; i++ ) {
		arc_matrix[i] = new arc_weight_type[ vex_num ];
	}
	init_matrix_with_zero();
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
MGraph_basic( const MGraph_basic& given_graph )
{
	vex_num = given_graph.vex_num;
	arc_num = given_graph.arc_num;

	arc_matrix = new arc_weight_type*[ vex_num ];
	for( unsigned i = 0; i < vex_num; i++ ) {
		arc_matrix[i] = new arc_weight_type[ vex_num ];
		memcpy( arc_matrix[i], given_graph.arc_matrix[i], vex_num );
	}
	memcpy( arc_matrix, given_graph.arc_matrix, vex_num );
	vexs = new pair<vex_key_type, vex_info_type>[ vex_num ];
	memcpy( vexs, given_graph.vexs,
		sizeof( pair<vex_key_type, vex_info_type> ) * vex_num );
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
~MGraph_basic( void )
{
	for( unsigned i = 0; i < vex_num; i++ ) {
		delete []arc_matrix[ i ]; //delete the i-th row of the matrix
	}
	delete []arc_matrix;
	delete []vexs;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
operator= ( const MGraph_basic& given_graph )
{
	if( vex_num != given_graph.vex_num ) {
		throw runtime_error( "when using operator=, vex_number not equal" );
	}

	memcpy( arc_matrix, given_graph.arc_matrix, vex_num );
	for( unsigned i = 0; i < vex_num; i++ ) {
		memcpy( arc_matrix[i], given_graph.arc_matrix[i], vex_num );
	}
	memcpy( vexs, given_graph.vexs,
		sizeof( pair<vex_key_type, vex_info_type> ) * vex_num );
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
ostream&
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
print_vex( ostream& os ) const
{
	for( unsigned i = 0; i < vex_num; i++ ) {
		os << vexs[i].first << ": " << vexs[i].second << "\n";
	}
	return os;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
ostream&
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
print_matrix( ostream& os ) const
{
	for( unsigned i = 0; i < vex_num; i++ ) {
		for( unsigned j = 0; j < vex_num; j++ ) {
			os << arc_matrix[ i ][ j ] << " ";
		}
		os << "\n";
	}

	return os;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
istream&
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
read_content( istream& in )
{
	if( !in ) { throw runtime_error( "when reading content to graph, "
					 "istream invalid" ); }

	for( unsigned i = 0; i < vex_num; i++ )
		in >> vexs[i].first >> vexs[i].second;

	vex_key_type    key_1, key_2;
	arc_weight_type weight;
	for( unsigned i = 0; i < arc_num; i++ ) {
		in >> key_1 >> key_2;

cout << "add arc: " << key_1 << "->" << key_2 << endl;

		if( K == DiNetwork || K == UnDiNetwork ) {
			in >> weight;
			add_arc( key_1, key_2, weight );
		} else { add_arc( key_1, key_2 ); }
		
		if( K == UnDiGraph ) {
			add_arc( key_2, key_1 );
		} else if( K == UnDiNetwork ) {
			add_arc( key_2, key_1, weight );
		}

		--arc_num; //Bcos the add_arc() will increase arc_num
	}
	return in;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
add_vex( const vex_key_type& new_key, const vex_info_type& new_vex_info )
{
	++vex_num;
	pair< vex_key_type, vex_info_type > *new_vexs
		= new pair<vex_key_type, vex_info_type>[vex_num];
	/* 
	 * the pair< T1, T2 >[] don't consistent in the memory
	 * so have to use 'for' rather than memcpy()
	 */
	for( unsigned i = 0; i < vex_num -1; i++ ){ 
		new_vexs[ i ] = vexs[ i ];
	}
	delete []vexs;
	vexs = new_vexs;
	vexs[vex_num-1].first = new_key;
	vexs[vex_num-1].second = new_vex_info;

	arc_weight_type **new_arc_matrix = new arc_weight_type*[vex_num];
	memcpy( new_arc_matrix, arc_matrix, vex_num - 1 );
	for( unsigned i = 0; i < vex_num - 1; i++ ) {
		new_arc_matrix[ i ] = new arc_weight_type[ vex_num ];
		//for( unsigned j = 0; j < vex_num-1; j++ ){
		//	new_arc_matrix[ i ][ j ] = arc_matrix[ i ][ j ];
		//}
		memcpy( new_arc_matrix[ i ], arc_matrix[ i ],
			sizeof( arc_weight_type ) * (vex_num - 1 ) );
		delete []arc_matrix[ i ];
	}
	new_arc_matrix[ vex_num -1 ] = new arc_weight_type[ vex_num ];

	delete []arc_matrix;
	arc_matrix = new_arc_matrix;

	for( unsigned i = 0; i < vex_num; i++ ) {
		arc_matrix[ vex_num-1 ][ i ] = arc_matrix[ i ][ vex_num-1 ] = 0;
	}
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
remove_vex( const vex_key_type& key )
{
	vex_info_type info = get_info( key );
	arc_num -= ( in_degree(key) + out_degree(key) );

	unsigned loc = get_location( key );
	if( loc != --vex_num ) {
		for( unsigned i = 0; i < vex_num; i++ ) {
			arc_matrix[ loc ][ i ] = arc_matrix[ vex_num-1 ][ i ];
			arc_matrix[ i ][ loc ] = arc_matrix[ i ][ vex_num-1 ];
		}  // remain the room in matrix
		arc_matrix[ loc ][ loc ] = arc_matrix[ vex_num-1 ][ vex_num-1 ];

		pair< vex_key_type, vex_info_type > temp;
		temp.first  = vexs[vex_num-1].first;
		temp.second = vexs[vex_num-1].second;
		vexs[vex_num-1].first  = vexs[loc].first;
		vexs[vex_num-1].second = vexs[loc].second;
		vexs[loc].first  = vexs[vex_num-1].first;
		vexs[loc].second = vexs[vex_num-1].second;
		
	} // else: doing nothing except the vex_num
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
add_arc( const vex_key_type& tail, const vex_key_type& head )
{
	if( K == DiNetwork || K == UnDiNetwork ) {
		throw logic_error( "when adding arc to network, didn't provide a weight" );
	}
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	arc_matrix[ tail_loc ][ head_loc ] = true;
	if( K == UnDiGraph || K == UnDiNetwork ) {
		arc_matrix[ head_loc ][ tail_loc ] = true;
	}

	++arc_num;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
add_arc( const vex_key_type& tail, const vex_key_type& head,
	 const arc_weight_type& weight )
{
	if( K == DiGraph || K == UnDiGraph )
		throw logic_error( "when adding arc to graph (not network), "
				   "wrongly provide a weight" );
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	arc_matrix[ tail_loc ][ head_loc ] = weight;
	if( K == UnDiGraph || K == UnDiNetwork ) {
		arc_matrix[ head_loc ][ tail_loc ] = weight;
	}

	++arc_num;
}
	

template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
remove_arc( const vex_key_type& tail, const vex_key_type& head )
{
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	if( K == DiNetwork ) {
		arc_matrix[ tail_loc ][ head_loc ] = INFINITY;
	} else if( K == DiGraph ) {
		arc_matrix[ tail_loc ][ head_loc ] = false;
	} else if( K == UnDiGraph ) {
		arc_matrix[ tail_loc ][ head_loc ] 
			= arc_matrix[ head_loc ][ tail_loc ] = 0;
	} else if( K == UnDiNetwork ) {
		arc_matrix[ tail_loc ][ head_loc ] 
			= arc_matrix[ head_loc ][ tail_loc ] = INFINITY;
	}

	--arc_num;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
unsigned
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
in_degree( const vex_key_type& key ) const
{
	unsigned loc = get_location( key );
	unsigned count = 0;
	for( unsigned i = 0; i < vex_num; i++ ) {
		if( K == DiGraph || K == UnDiGraph ) {
			if( arc_matrix[ i ][ loc ] ) { ++count; }
		} else {
			if( arc_matrix[ i ][ loc ] != INFINITY ) { ++count; }
		}
	}
	
	return count;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
unsigned
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K > ::
out_degree( const vex_key_type& key ) const
{
	unsigned loc = get_location( key );
	unsigned count = 0;
	for( unsigned i = 0; i < arc_num; i++ ) {
		if( K == DiGraph || K == UnDiGraph ) {
			if( arc_matrix[ loc ][ i ]== true ) { ++count; }
		} else {
			if( arc_matrix[ loc ][ i ] != INFINITY ) { ++count; }
		}
	}
	
	return count;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
vex_info_type&
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
get_vex_info( const vex_key_type& key ) const
{
	for( unsigned i = 0; i < vex_num; i++ ) {
		if ( vexs[i].first == key ) {
			return vexs[i].second;
		}
	}

	throw runtime_error( "try to get info of a non-existing key" );
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
unsigned
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
get_location( const vex_key_type& key ) const
{
	for( unsigned i = 0; i < vex_num; i++ ) {
		if( vexs[i].first == key ) {
			return i;
		}
	}

	throw runtime_error( "try to get location of a non-existing key" );
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
DFS_traverse( void ) const
{
	bool *visited = new bool[ vex_num ];
	for( unsigned i = 0; i < vex_num; i++ ){
		visited[ i ] = false;
	}
	for( unsigned vex_loc = 0; vex_loc < vex_num; vex_loc++ ){
		if( ! visited[ vex_loc ] )
			DFS( vex_loc, visited );
	}
	delete []visited;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
BFS_traverse( void ) const
{
//Something wrong here.
	bool *visited = new bool[ vex_num ];
	for( unsigned i = 0; i < vex_num; i++ ){
		visited[ i ] = false;
	}
	deque< unsigned > for_traverse;
	for( unsigned i = 0; i < vex_num; i++ ){
		if( !visited[ i ] ){
			for_traverse.push_back( i );
			while( !for_traverse.empty() ){
				unsigned cur_loc = for_traverse.front();
				for_traverse.pop_front();
				cout << vexs[ cur_loc ].first << " ";
				visited[ cur_loc ] = true;
				for( unsigned w = first_adj_vex( vexs[ cur_loc ].first );
				     w;
				     w = next_adj_vex( vexs[ cur_loc ].first , w ) ) {
					unsigned w_loc = get_location( w );
					if( !visited[ w_loc ] )
						for_traverse.push_back( w_loc );
				}
			}
		}
	}
	delete []visited;
}

template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
DFS( const unsigned& vex_loc, bool *visited ) const
{
	if( ! visited ){ return; }

	cout << vexs[ vex_loc ].first << " ";
	visited[ vex_loc ] = true;
	for( vex_key_type w = first_adj_vex( vexs[ vex_loc ].first );
	     w;
	     w = next_adj_vex( vexs[ vex_loc ].first, w ) ) {
		unsigned next_loc = get_location( w );
		if( ! visited[ next_loc ] )
			DFS( next_loc, visited );
	}
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
edit_vex_info( const vex_key_type& key, const vex_info_type& new_info )
{
	unsigned loc = get_location( key );
	vexs[loc].second = new_info;
}


// Used only for Network class
template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
edit_arc_weight( const vex_key_type& tail, const vex_key_type& head,
	    const arc_weight_type& new_weight )
{
	unsigned tail_loc = get_location( tail );
	unsigned head_loc = get_location( head );
	arc_matrix[ tail_loc ][ head_loc ] = new_weight;
	if( K == UnDiNetwork ) {
		arc_matrix[ head_loc ][ tail_loc ] = new_weight;
	}
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
bool
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
is_adj( const vex_key_type& key_1, const vex_key_type& key_2 )
{
	unsigned loc_1 = get_location( key_1 );
	unsigned loc_2 = get_location( key_2 );

	for( unsigned i = 0; i < vex_num; i++ ) {
		if( K == DiGraph || K == UnDiGraph ) {
			if( arc_matrix[ loc_1 ][ loc_2 ]
			    || arc_matrix[ loc_2 ][ loc_1 ] ) { return true; }
		} else {
			if( arc_matrix[ loc_1 ][ loc_2] != INFINITY ) {
				return true;
			}
		}
	}

	return false;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
vex_key_type
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
first_adj_vex( const vex_key_type& given_vex_key ) const
{
	unsigned loc = get_location( given_vex_key );
	for( unsigned i = 0; i < vex_num; i++ ){
		if( arc_matrix[ loc ][ i ] )
			return vexs[ i ].first;
	}

	return 0; // -1 is not suitable
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
vex_key_type
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
next_adj_vex( const vex_key_type& given_vex_key,
	      const vex_key_type& former_adj_vex_key ) const
{
	unsigned given_vex_loc = get_location( given_vex_key );
	unsigned former_adj_loc = get_location( former_adj_vex_key );
	for( unsigned i = ++former_adj_loc; i < vex_num; i++ ){
		if( arc_matrix[ given_vex_loc ][ i ] ){
			return vexs[ i ].first;
		}
	}

	return 0;
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
MGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
init_matrix_with_zero( void )
{
	for( unsigned i = 0; i != vex_num; i++ ) {
		for( unsigned j = 0; j != vex_num; j++ ) {
			arc_matrix[i][j] = ( K == DiGraph || K == DiNetwork )?
						false : INFINITY;
		}
	}
}

#endif
