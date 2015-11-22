#include<iostream>
#include"009_bi-tree.h"


bool creat_bitree( const BiTree T, const istream& in ){
}

void destory_bitree( BiTree T ){
	clear_bitree( T );
	T = NULL;
}

void clear_bitree( const BiTree T ){
	if( T->l_child == NULL && T->r_child == NULL ){
		delete T;
		return;
	}
	if( T->l_child ){ clear_bitree( T->l_child ); }
	if( T->r_child ){ clear_bitree( T->r_child ); }
}


bool bitree_empty( BiTree T ){
	return ( T==NULL );
}

int  bitree_depth( BiTree t ){
	if( !t ){ return 0; }
	return( bitree_depth(t->l_child) > bitree_depth(t->r_child) ?
		bitree_depth(t->l_child) + 1
		: bitree_depth(t->r_child) + 1 );
}

bool bitree_equal( BiTree s, BiTree t ){
	if( s == NULL && t == NULL ){  return true; }
	if( s && t && bitree_equal( s->l_child, t->l_child )
		&& bitree_equal( s->r_child, t->r_child ) ){
		return true;
	}
	return false;
}

void bitree_exchange( BiTree t ){
	if( !t->l_child && !t->r_child ){ return; }
	if( t->l_child ){ bitree_exchange( t->l_child ); }
	if( t->r_child ){ bitree_exchange( t->r_child ); }
	BitreeNode *temp = t->l_child;
	t->l_child = t->r_child;
	t->r_child = t->l_child;
}

TElemType root( BiTree T ){
	if( T ){ return T->value; }
	else { return TElemType(0); }
}

TElemType parent( BiTree T, TElemType e ){
	BitreeNode *cur_node = _find_node( e );
	if( cur_node && cur_node->parent ){
		return cur_node->parent->value;
	} else {
		return TElemType(0);
	}
}

TElemType left_child( BiTree T, TElemType e ){
	BitreeNode *cur_node = _find_node( e );
	if( cur_node && cur_node->l_child ){
		return cur_node->l_child->value;
	}else{
		return TElemType(0);
	}
}

TElemType right_child( BiTree T, TElemType e ){
	BitreeNode *cur_node = _find_node( e );
	if( cur_node && cur_node->r_child ){
		return cur_node->r_child->value;
	}else{
		return TElemType(0);
	}
}

TElemType left_sibling( BiTree T, TElemType e ){
	BitreeNode *cur_node = _find_node( e );
	if( cur_node && cur_node->parent && cur_node->parent->r_child ){
		return cur_node->parent->r_child->value;
	} else {
		return TElemType(0);
	}
}

TElemType right_sibling( BiTree T, TElemType e ){
	BitreeNode *cur_node = _find_node( e );
	if( cur_node && cur_node->parent && cur_node->parent->l_child ){
		return cur_node->parent->l_child->value;
	} else {
		return TElemType(0);
	}
}

int cousin( const BiTree t, const TElemType e ){
}

BitreeNode *_find_node( TElemType e ){
}

//If the traverse useful?
bool pre_order_traverse( BiTree T, bool(*visit)(TElemType e) ){
	if( T ){
		if( visit( T->value ) )
			if( pre_order_traverse( T->l_child, vist ) )
				if( pre_order_traverse( T->r_child, vist ) )
					return true;
		return false;
	}else{ return true; }
}

bool in_order_traverse( BiTree T, bool(*vist)(TElemType e) ){
}

bool post_order_traverse( BiTree T, bool(*visit)(TElemType e) ){
}

bool level_order_traverse( BiTree T, bool(*visit)(TElemType e) ){
}

