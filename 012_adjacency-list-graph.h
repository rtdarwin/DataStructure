#ifndef ADJACENCY_LIST_GRAPH_H_
#define ADJACENCY_LIST_GRAPH_H_
#include<iostream>
#include<utility>
#include<string>
#include<deque>
#include<cstring>
#include<exception>
#include<stdexcept>
#include<inttypes.h>


using namespace std;
//--------------------------- TEMPLATE CLASS ----------------------------------
#define INFINITY INT32_MAX

enum GraphKind_t { DiGraph, UnDiGraph, DiNetwork, UnDiNetwork };

//forward declaration to make friends  :)
template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K
> class ALGraph_basic;

//ArcNode
template< typename arc_weight_type >
class ArcNode{
private:
	template< typename vex_key_t, typename vex_info_t,
		  typename arc_weight_t, GraphKind_t K
	> friend class ALGraph_basic;

	unsigned         location_in_list;
	ArcNode         *next;
	arc_weight_type  arc_weight;
};

//VexNode
template< typename vex_key_type,
	  typename vex_info_type,
	  typename arc_weight_type
> class VexNode{
private:
	template< typename vex_key_t, typename vex_info_t,
		  typename arc_weight_t, GraphKind_t K
	> friend class ALGraph_basic;

	/* Not define a customed assign-operator=
	 * so when copy a VexNode, just copy the pointer, not the content
	 * the content first_arc points to is handled by the ALGraph_basic
	 */

	vex_key_type                key;
	vex_info_type               info;
	ArcNode< arc_weight_type>  *first_arc;
};

//ALGraph_basic
template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K
> class ALGraph_basic{
public:
	ALGraph_basic( const unsigned& given_vex_num, const unsigned& given_arc_num );
	ALGraph_basic( const ALGraph_basic& given_graph );
	~ALGraph_basic( void );
	void operator= ( const ALGraph_basic& given_graph );

	//Acess
	unsigned       arc_number() const { return arc_num; }
	unsigned       vex_number() const { return vex_num; }
	ostream       &print_vex( ostream& os ) const;
	ostream       &print_adj_list( ostream& os ) const;
	vex_key_type   first_adj_vex( void ) const;
	vex_key_type   next_adj_vex( const vex_key_type& former_vex ) const;
	bool           is_adj( const vex_key_type& key_1,
			       const vex_key_type& key_2 ) const;
	unsigned       in_degree( const vex_key_type& key ) const;
	unsigned       out_degree( const vex_key_type& key ) const;
	vex_info_type  get_info( const vex_key_type& key ) const;
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
				const arc_weight_type& power );
	void           remove_arc( const vex_key_type& tail, const vex_key_type& head );
	void           edit_vex_info( const vex_key_type& key, 
				      const vex_info_type& new_info );
	void           edit_power( const vex_key_type& key1, const vex_key_type& key2, 
				   const arc_weight_type& new_power );
private:
	void init_vex_node( void );
	void DFS( const unsigned& vex_loc, bool *visited ) const;
	void BFS( const unsigned& vex_loc, bool *visited ) const;

private:
	VexNode< vex_key_type, vex_info_type, arc_weight_type >  *vexs;
	unsigned  vex_num;
	unsigned  arc_num;
};


//------------------------ TYPEDEF -----------------------------

typedef ALGraph_basic< char, string, bool, DiGraph >        MDGraph;
typedef ALGraph_basic< char, string, bool, UnDiGraph >      MUDGraph;
typedef ALGraph_basic< char, string, int32_t, DiNetwork >   MDNetwork;
typedef ALGraph_basic< char, string, int32_t, UnDiNetwork > MUDNetwork;

//------- ALGRAPH_BASIC - FUNCTION MEMBER ----------------------

template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
ALGraph_basic( const unsigned& given_vex_num, const unsigned& given_arc_num )
{
	vex_num = given_vex_num;
	arc_num = given_arc_num;
	vexs = new VexNode< vex_key_type, vex_info_type, arc_weight_type >[ vex_num ];
	init_vex_node();
}

template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
ALGraph_basic( const ALGraph_basic& given_graph )
{
	vex_num = given_graph.vex_num;
	arc_num = given_graph.arc_num;
	vexs = new VexNode< vex_key_type, vex_info_type, arc_weight_type >[ vex_num ];
	//the vexs in memory maybe not consistent
	for( unsigned i = 0; i < vex_num; i++ ){
		vexs[ i ] = given_graph.vexs[ i ];
	}
	//
	for( int i = 0; i < vex_num; i++ ){
		ArcNode< arc_weight_type > *src_first_arc = vexs[ i ].first_arc;
		if( src_first_arc ){
			ArcNode< arc_weight_type > *dest_first_arc
				= new ArcNode< arc_weight_type >;
			*dest_first_arc = *src_first_arc;
			vexs[ i ].first_arc = dest_first_arc;

			ArcNode< arc_weight_type > *dest_former_arc = dest_first_arc;
			ArcNode< arc_weight_type > *src_cur_arc
				= dest_former_arc->next;
			while( !src_cur_arc ){
				ArcNode< arc_weight_type > *dest_add_arc
					= new ArcNode< arc_weight_type >;
				*dest_add_arc = *src_cur_arc;
				dest_former_arc->next = dest_add_arc;
				dest_former_arc = dest_add_arc;
				src_cur_arc = src_cur_arc->next;
			}
		}
	}
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
~ALGraph_basic( void )
{
	for( unsigned i = 0; i < vex_num; i++ ){
		ArcNode< arc_weight_type > *cur = vexs[ i ].first_arc;
		while( !cur ){
			ArcNode< arc_weight_type > *next = cur->next;
			delete cur;
			cur = next;
		}
	}

	delete []vexs;
}

template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
operator= ( const ALGraph_basic& given_graph )
{
	if( vex_num != given_graph.vex_num || arc_num != given_graph.arc_num )
		throw runtime_error( "when assign a graph to another"
				     "vex_num or arc_num doesn't match" );
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
ostream&
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
print_vex( ostream& os ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
ostream&
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
print_adj_list( ostream& os ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
vex_key_type
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
first_adj_vex( void ) const
{
///.///
}



template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
vex_key_type
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
next_adj_vex( const vex_key_type& former_vex ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
bool
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
is_adj( const vex_key_type& key_1, const vex_key_type& key_2 ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
unsigned
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
in_degree( const vex_key_type& key ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
unsigned
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
out_degree( const vex_key_type& key ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
vex_info_type
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
get_info( const vex_key_type& key ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
unsigned
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
get_location( const vex_key_type& key ) const
{
///.///
}


template< typename vex_key_type, typename vex_info_type,
	  typename arc_weight_type, GraphKind_t K >
void
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
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
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
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
				cout << vexs[ cur_loc ].key << " ";
				visited[ cur_loc ] = true;
				for( unsigned w
					= first_adj_vex( vexs[ cur_loc ].key );
				     w;
				     w = next_adj_vex( vexs[ cur_loc ].key , w ) ) {
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
ALGraph_basic< vex_key_type, vex_info_type, arc_weight_type, K >::
DFS( const unsigned& vex_loc, bool *visited ) const
{
	if( ! visited ){ return; }

	cout << vexs[ vex_loc ].first << " ";
	visited[ vex_loc ] = true;
	for( vex_key_type w = first_adj_vex( vexs[ vex_loc ].key );
	     w;
	     w = next_adj_vex( vexs[ vex_loc ].key, w ) ) {
		unsigned next_loc = get_location( w );
		if( ! visited[ next_loc ] )
			DFS( next_loc, visited );
	}
}

#endif
