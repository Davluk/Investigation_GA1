#include<stdio.h>
#include<stdlib.h>

#ifndef NODE_CPP
#define NODE_CPP

#define FULL_FILL 0
#define RAND_FILL 1
#define HALF_HALF 2

template<typename T>
struct Node
{
    T data;
    Node<T> *left;
    Node<T> *rigth;

    Node(){}
    explicit Node(Node* copy){ this.data	= copy->data; this.left = copy->left; this.rigth = copy->rigth; }
    explicit Node(T _data){this.data=_data;this.left=NULL;this.rigth=NULL;}
    Node(T _data,Node* _left,Node* _rigth){ this.data=_data;this.left=_left;this.rigth=_rigth;}
};

template<typename T>
Node<T> newNode(int _fill_selection_,int depth,T _data,T (*getData)(),T* (*getLeaf)(),T *(getOp)(),bool (*isNode)(T))
{
	Node<T> temp_node;
	temp_node.data = _data;
	int coin=0;
	if(depth==1){ temp_node.left(getLeaf()); temp_node.rigth(getLeaf()); goto _end_new_node;}
	else
	{
		switch(_fill_selection_)
		{
			case FULL_FILL:
				temp_node.left  = newNode(FULL_FILL,depth-1,getOp(),getData,getLeaf,getOp,isNode);
				temp_node.rigth = newNode(FULL_FILL,depth-1,getOp(),getData,getLeaf,getOp,isNode);
			break;
			case RAND_FILL:
				coin = rand()%4;
				if(coin>2){ temp_node.left(getLeaf());}
				else{ temp_node.left = newNode(RAND_FILL,depth-1,getOp(),getData,getLeaf,getOp,isNode); }
				coin = rand()%4;
				if(coin>2){ temp_node.rigth(getLeaf()); }
				else{ temp_node.rigth = newNode(RAND_FILL,depth-1,getOp(),getData,getLeaf,getOp,isNode); }
			break;
			case HALF_HALF: 
				_def_Case_:
				temp_node.left  = newNode(FULL_FILL,depth-1,getOp(),getData,getLeaf,getOp,isNode);
				coin = rand()%4;
				if(coin>2){ temp_node.rigth(getLeaf()); }
				else{ temp_node.rigth = newNode(RAND_FILL,depth-1,getOp(),getData,getLeaf,getOp,isNode); }
			break;
			default:
			 	goto _def_Case_;
		}
	}
	return temp_node;
}

template <typename T,typename U>
U getData(Node<T> _temp_node,U (*Interpreter)(T)){ return Interpreter(_temp_node.data); }

#endif
