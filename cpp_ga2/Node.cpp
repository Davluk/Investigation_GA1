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
    Node<T>* left;
    Node<T>* rigth;

    Node(){}
    explicit Node(Node* copy){ data	= copy->data; left = copy->left; rigth = copy->rigth; }
    explicit Node(T _data){data=_data;left=NULL;rigth=NULL;}
};

template<typename T>
Node<T>* newNode(int _fill_selection_,int depth,T _data,T (*getLeaf)(),T (*getOp)())
{
	Node<T>* temp_node=new Node<T>();
	temp_node->data = _data;
	int coin=0;
	if(depth==1){ temp_node->left =new Node<T>(getLeaf()); temp_node->rigth =new Node<T>(getLeaf()); goto _end_new_node;}
	else
	{
		switch(_fill_selection_)
		{
			case FULL_FILL:
				temp_node->left  = newNode(FULL_FILL,depth-1,getOp(),getLeaf,getOp);
				temp_node->rigth = newNode(FULL_FILL,depth-1,getOp(),getLeaf,getOp);
			break;
			case RAND_FILL:
				coin = rand()%4;
				if(coin>2){ temp_node->left=new Node<T>(getLeaf());}
				else{ temp_node->left = newNode(RAND_FILL,depth-1,getOp(),getLeaf,getOp); }
				coin = rand()%4;
				if(coin>2){ temp_node->rigth=new Node<T>(getLeaf()); }
				else{ temp_node->rigth = newNode(RAND_FILL,depth-1,getOp(),getLeaf,getOp); }
			break;
			case HALF_HALF: 
				_def_Case_:
				temp_node->left  = newNode(FULL_FILL,depth-1,getOp(),getLeaf,getOp);
				coin = rand()%4;
				if(coin>2){ temp_node->rigth=new Node<T>(getLeaf()); }
				else{ temp_node->rigth = newNode(RAND_FILL,depth-1,getOp(),getLeaf,getOp); }
			break;
			default:
			 	goto _def_Case_;
		}
	}
	_end_new_node:
	return temp_node;
}

template<typename T>
void PrintPosOrder(Node<T>* _root_node,bool (*isNode)(T),char (*getCharRep)(T))
{
	printf("( %c ",getCharRep(_root_node->data));
	if(_root_node->left!=NULL)
	{	
		if(isNode(_root_node->left->data))
		{ PrintPosOrder(_root_node->left,isNode,getCharRep); }
		else { printf("%c ",getCharRep(_root_node->left->data)); }
	} 
	if(_root_node->rigth!=NULL)
	{	
		if(isNode(_root_node->rigth->data))
		{ PrintPosOrder(_root_node->rigth,isNode,getCharRep); }
		else { printf("%c",getCharRep(_root_node->rigth->data)); }
	} 
	printf(" )");
}

template <typename T,typename U>
U getData(Node<T> _temp_node,U (*Interpreter)(T)){ return Interpreter(_temp_node->data); }

template <typename T>
int findIndexedSubTree(int index,Node<T>* _tree_root,Node<T>* OutNode,bool (*isNode)(T))
{
	int deep = index;
	if(index>=0){ OutNode = _tree_root; }
	else{
		if(index>=1){
			if(_tree_root->left!=NULL && isNode(_tree_root->left->data))
			{ deep = findIndexedSubTree(deep-1,_tree_root->left,OutNode,isNode); }
			if(_tree_root->rigth!=NULL && isNode(_tree_root->rigth->data))
			{ deep = findIndexedSubTree(deep-1,_tree_root->rigth,OutNode,isNode);}
		}
	}
	return deep;
}

#endif
