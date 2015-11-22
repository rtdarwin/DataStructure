#ifndef BITREE_H_
#define BITREE_H_

typedef struct BitreeNode
{
	TElemType *l_child;
	TElemType  value;
	TElemType *parent;
	TElemType *r_child;
}BiTreeNode, *BiTree;

extern bool creat_bitree( const BiTree T, const istream& in );
extern void destory_bitree( BiTree T );
extern void clear_bitree( const BiTree T );
extern bool bitree_empty( const BiTree T );
extern int  bitree_depth( const BiTree T );
extern bool bitree_equal( const BiTree s, const BiTree t );
extern void bitree_exchange( const BiTree T );
extern TElemType root( const BiTree T );
extern TElemType parent( const BiTree T, const TElemType e );
extern TElemType left_child( const BiTree T, const TElemType e );
extern TElemType right_child( const BiTree T, const TElemType e );
extern TElemType left_sibling( const BiTree T, const TElemType e );
extern TElemType right_sibling( const BiTree T, const TElemType e );
extern int  cousin( const BiTree T, const TElemType e );  //print cousin and return number
//functions for users who understand the implementation of this Bitree
extern BitreeNode *_find_node( TElemType e ); 
extern bool pre_order_traverse( BiTree T, bool(*visit)(TElemType e) );
extern bool in_order_traverse( BiTree T, bool(*vist)(TElemType e) );
extern bool post_order_traverse( BiTree T, bool(*visit)(TElemType e) );
extern bool level_order_traverse( BiTree T, bool(*visit)(TElemType e) );
#endif
