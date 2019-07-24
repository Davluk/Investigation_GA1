#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<string>
#include<vector>

#ifndef NODE_CPP
#define NODE_CPP

/*****************************************************************************
*	FULL: fill all the nodes with an operator until Depth-1, then put leafs  *
*	RAND: fill randomlly all the nodes										 *
*	HALF: left-> full-fill, rigth->rand-fill                                 *
*****************************************************************************/

#define FULL_FILL 0
#define RAND_FILL 1
#define HALF_FILL 2

/*********************************
*				  				 *
*	 		DATA{...}  			 *
*				  				 *
**********************************
*	left{NODE} 	**	rigth{NODE}	 *
*********************************/
template<typename T>
struct Node
{
    T data;
    Node<T>* left;
    Node<T>* unique;
    Node<T>* rigth;
    Node(){}
    explicit Node(Node* copy){ data	= copy->data; left = copy->left; unique=copy->unique; rigth = copy->rigth; }
    explicit Node(T _data){data=_data;left=NULL;unique=NULL;rigth=NULL;}
};

/**************************************
*	real initializator of the trees	  *  
* gets: 							  *
*	- selection for type of fill      *
*	- max depth 					  *
*	- a leaf generator				  *
*	- a operand generator			  *
**************************************/
template<typename T>
Node<T>* newNode(int _fill_selection_,int depth,T _data,bool (*is_binary)(T),T (*getLeaf)(),T (*getOp)())
{
	//||||||||||||||||||||||||VARIABLES||||||||||||||||||||||||||||||||//
	Node<T>* temp_node=new Node<T>();  temp_node->data = _data;  int coin=0;
	//||||verifys if finds on maxdepth -1 to finish the filling of the tree||||//
	if(depth==1)
		{ 
			if(is_binary(temp_node->data))
			{ temp_node->left = new Node<T>(getLeaf()); temp_node->rigth = new Node<T>(getLeaf()); goto _end_new_node; }
			else
			{ temp_node->unique= new Node<T>(getLeaf()); goto _end_new_node; }
		}
	else
	{
		//|||||||||||||||||fill recursively all the nodes||||||||||||||||||||||||||//
		switch(_fill_selection_)
		{
			case FULL_FILL:
				if(is_binary(temp_node->data))
				{
					temp_node->left  = newNode(FULL_FILL,depth-1,getOp(),is_binary,getLeaf,getOp);
					temp_node->rigth = newNode(FULL_FILL,depth-1,getOp(),is_binary,getLeaf,getOp);
				}else
				{ temp_node->unique = newNode(FULL_FILL,depth-1,getOp(),is_binary,getLeaf,getOp); }
			break;
			case RAND_FILL:
				if(is_binary(temp_node->data))
				{
					coin = rand()%4;
					if(coin>=2){ temp_node->left=new Node<T>(getLeaf());}
					else{ temp_node->left = newNode(RAND_FILL,depth-1,getOp(),is_binary,getLeaf,getOp); }
					coin = rand()%4;
					if(coin>=2){ temp_node->rigth=new Node<T>(getLeaf()); }
					else{ temp_node->rigth = newNode(RAND_FILL,depth-1,getOp(),is_binary,getLeaf,getOp); }
				}else
				{
					coin = rand()%4;
					if(coin>=2){ temp_node->unique=new Node<T>(getLeaf());}
					else{ temp_node->unique = newNode(RAND_FILL,depth-1,getOp(),is_binary,getLeaf,getOp); }
				}
			break;
			case HALF_FILL: 
				_def_Case_:
				if(is_binary(temp_node->data))
				{
					temp_node->left  = newNode(FULL_FILL,depth-1,getOp(),is_binary,getLeaf,getOp);
					coin = rand()%4;
					if(coin>=2){ temp_node->rigth=new Node<T>(getLeaf()); }
					else{ temp_node->rigth = newNode(RAND_FILL,depth-1,getOp(),is_binary,getLeaf,getOp); }
				}else
				{
					temp_node->unique = newNode(HALF_FILL,depth-1,getOp(),is_binary,getLeaf,getOp);
				}
			break;
			default:
			 	goto _def_Case_;
		}
	}
	_end_new_node:
	return temp_node;
}

/**************************************************************************************************
*																								  *
*	gets the evaluatoin of the function with X1,X2...Xn parameters 								  *
*	gets: 																						  *
*	- the tree to evaluate 																		  *
* 	- function to evaluate the data whish is obtained											  *
*  	- function to verify if the node is an internal node 										  *
*	- function to verify if the node has a variable, then obtains the data from that variable     *
*	- function to get the index of the variable in the array of variables 						  *
*	- function to get the index of the terminal in the array of terminals 						  *
* 	- function to get the data of the indexed terminal 											  *
* 	- function to get the index of the expresion in the array of expresions						  *
*																								  *
**************************************************************************************************/
template<typename T,typename U>
U evalFunction(Node<T>* _root_node,U (*executeBinExpresion)(int,U,U),U (*executeUnExpresion)(int,U),bool (*isNode)(T),bool (*is_binary)(T),bool (*isVar)(T),int (*getVarIndex)(T),U (*getTerminal)(T),int (*getExpresionIndex)(T),U* values)
{
	U left_operand;
	U rigth_operand;
	U unique_operand;
	if(is_binary(_root_node->data))
	{
		if(isNode(_root_node->left->data))
		{ left_operand = evalFunction(_root_node->left,executeBinExpresion,executeUnExpresion,isNode,is_binary,isVar,getVarIndex,getTerminal,getExpresionIndex,values); }
		else { 
			if(isVar(_root_node->left->data)) { left_operand = values[getVarIndex(_root_node->left->data)]; }
			else { left_operand = getTerminal(_root_node->left->data); }
		}

		if(isNode(_root_node->rigth->data))
		{ rigth_operand = evalFunction(_root_node->rigth,executeBinExpresion,executeUnExpresion,isNode,is_binary,isVar,getVarIndex,getTerminal,getExpresionIndex,values); }
		else { 
			if(isVar(_root_node->rigth->data)) { rigth_operand = values[getVarIndex(_root_node->rigth->data)]; }
			else { rigth_operand = getTerminal(_root_node->rigth->data); }
		}
		return executeBinExpresion(getExpresionIndex(_root_node->data),left_operand,rigth_operand);
	}else
	{
		if(isNode(_root_node->unique->data))
		{ unique_operand = evalFunction(_root_node->unique,executeBinExpresion,executeUnExpresion,isNode,is_binary,isVar,getVarIndex,getTerminal,getExpresionIndex,values); }
		else
		{
			if(isVar(_root_node->unique->data)){ unique_operand = values[ getVarIndex( _root_node->unique->data ) ]; }
			else{ unique_operand = getTerminal( _root_node->unique->data ); }
		}
		return executeUnExpresion( getExpresionIndex( _root_node->data ) , unique_operand );
	}
}

/********************************************************************
*																	*
*	the algorithm use the cuadratic error as a meassure of fitness	*
*																    *
********************************************************************/
template<typename T,typename U>
U cuadraticError(Node<T> *_current_indiv,U (*exBExp)(int,U,U),U (*exUExp)(int,U),bool (*isNode)(T),bool (*isBin)(T),bool (*isVar)(T),int (*getVrIn)(T),U (*getTer)(T),int (*getExpInd)(T),std::vector<U*> _list_of_values,int rows,int VARIABLE_SIZE)
{
	U error = (U)0;
	for(int index = 0;index<rows;index++)
	{
		//always the F(x) must be the last element
		U temp_var = pow(_list_of_values[index][VARIABLE_SIZE-1]-evalFunction(_current_indiv,exBExp,exUExp,isNode,isBin,isVar,getVrIn,getTer,getExpInd,_list_of_values[index]),2);
		if(!isnan(temp_var)){ error+=temp_var; }else{ return 1/0.0000000000000000000000000000000000000000000000000000000000000000000000001; }
	}
	error = sqrt(error);
	return error;
}


/********************************************
*											*
*	count the number of nodes in the tree 	*
*											*
********************************************/
template<typename T>
int nodeCounter(Node<T>* _root_node,bool (*isNode)(T),bool (*is_binary)(T))
{
	int counter = 1;
	if(is_binary(_root_node->data))
	{
		if(_root_node->left!=NULL && isNode(_root_node->left->data)){ counter+= nodeCounter(_root_node->left,isNode,is_binary); }
		if(_root_node->rigth!=NULL && isNode(_root_node->rigth->data)){ counter+= nodeCounter(_root_node->rigth,isNode,is_binary); }
	}else
	{
		if(_root_node->unique!=NULL && isNode(_root_node->unique->data)){ counter+= nodeCounter(_root_node->unique,isNode,is_binary);}
	}
	return counter;
}


/********************************************
*											*
*	count the number of nodes in the tree 	*
*											*
********************************************/
template<typename T>
int totalNodeCounter(Node<T>* _root_node,bool (*is_binary)(T))
{
	int counter = 1;
	if(is_binary(_root_node->data))
	{	
		if(_root_node->left!=NULL){ counter+= totalNodeCounter(_root_node->left,is_binary); }
		if(_root_node->rigth!=NULL){ counter+= totalNodeCounter(_root_node->rigth,is_binary); }
	}else
	{
		if(_root_node->unique!=NULL){ counter+= totalNodeCounter(_root_node->unique,is_binary); }
	}
	return counter;
}

/************************************************************************************
*																					*
*	gets the char representation of the function coded in the tree in pos order 	*
*																					*
************************************************************************************/
/*
template<typename T>
void PrintTree(Node<T>* _root_node,bool (*isNode)(T),bool (*is_binary)(T),std::basic_string<char> (*getCharRep)(T))
{
	if(is_binary(_root_node->data))
	{
		std::cout<<"( "<<getCharRep(_root_node->data)<<" ";
		if(_root_node->left!=NULL)
		{	
			if(isNode(_root_node->left->data))
			{ PrintTree(_root_node->left,isNode,is_binary,getCharRep); }
			else { std::cout<<getCharRep(_root_node->left->data)<<" "; }
		} 
		if(_root_node->rigth!=NULL)
		{	
			if(isNode(_root_node->rigth->data))
			{ PrintTree(_root_node->rigth,isNode,is_binary,getCharRep); }
			else { std::cout<<getCharRep(_root_node->rigth->data); }
		} 
		std::cout<<" )";
	}else
	{
		std::cout<<getCharRep(_root_node->data)<<"( ";
		if(_root_node->unique!=NULL)
		{	
			if(isNode(_root_node->unique->data))
			{ PrintTree(_root_node->unique,isNode,is_binary,getCharRep); }
			else { std::cout<<getCharRep(_root_node->unique->data); }
		}
		std::cout<<" )";
	}
}
*/
template<typename T>
void PrintTree(Node<T>* _root_node,bool (*isNode)(T),bool (*is_binary)(T),std::basic_string<char> (*getCharRep)(T))
{
	if(is_binary(_root_node->data))
	{
		std::cout<<"( ";
		if(_root_node->left!=NULL)
		{	
			if(isNode(_root_node->left->data))
			{ PrintTree(_root_node->left,isNode,is_binary,getCharRep); }
			else { std::cout<<getCharRep(_root_node->left->data)<<" "; }
		} 
		std::cout<<getCharRep(_root_node->data)<<" ";
		if(_root_node->rigth!=NULL)
		{	
			if(isNode(_root_node->rigth->data))
			{ PrintTree(_root_node->rigth,isNode,is_binary,getCharRep); }
			else { std::cout<<getCharRep(_root_node->rigth->data); }
		} 
		std::cout<<" )";
	}else
	{
		std::cout<<getCharRep(_root_node->data)<<"( ";
		if(_root_node->unique!=NULL)
		{	
			if(isNode(_root_node->unique->data))
			{ PrintTree(_root_node->unique,isNode,is_binary,getCharRep); }
			else { std::cout<<getCharRep(_root_node->unique->data); }
		}
		std::cout<<" )";
	}
}

/********************************************************
* 														*
*	go over the tree and returns the indexed subtree 	*
*														*
********************************************************/
template <typename T>
int getIndexedSubTree(int index,Node<T>* _root_node,Node<T>* OutNode,bool (*isNode)(T),bool (*is_binary)(T))
{
	int deep = index;
	if(index==0){ *OutNode = *_root_node; }
	else if(index>=1){
		if(is_binary(_root_node->data))
		{
			if(_root_node->left!=NULL && isNode(_root_node->left->data))
			{ deep = getIndexedSubTree(deep-1,_root_node->left,OutNode,isNode,is_binary); }
			if(_root_node->rigth!=NULL && isNode(_root_node->rigth->data))
			{ deep = getIndexedSubTree(deep-1,_root_node->rigth,OutNode,isNode,is_binary);}
		}else
		{
			if(_root_node->unique!=NULL && isNode(_root_node->unique->data))
			{ deep = getIndexedSubTree(deep-1,_root_node->unique,OutNode,isNode,is_binary); }
		}
	}
	return deep;
}


/********************************************************
* 														*
*	go over the tree and returns the indexed node 	 	*
*														*
********************************************************/

template<typename T>
int getIndexedNode(int index,Node<T>* _root_node,Node<T>* OutNode,bool (*is_binary)(T))
{
	int deep = index;
	if(index==0){ *OutNode = *_root_node; }
	else if(index>=1){
		if(is_binary(_root_node->data))
		{
			if(_root_node->left!=NULL)
			{ deep = getIndexedNode(deep-1,_root_node->left,OutNode,is_binary); }
			if(_root_node->rigth!=NULL)
			{ deep = getIndexedNode(deep-1,_root_node->rigth,OutNode,is_binary);}
		}else
		{
			if(_root_node->unique=NULL)
			{ deep = getIndexedNode(deep-1,_root_node->unique,OutNode,is_binary); }
		}
	}
	return deep;
}

/********************************************************************************
* 																				*
*	go over the tree and substitute the subtree passed in the indexed subtree 	*
* 																				*
********************************************************************************/
template <typename T>
int setIndexedSubTree(int index,Node<T>* _root_node,Node<T>* InNode,bool (*isNode)(T),bool (*is_binary)(T))
{
	int deep = index;
	if(index==0){ *_root_node=*InNode; }
	else if(index>=1){
		if(is_binary(_root_node->data))
		{
			if(_root_node->left!=NULL && isNode(_root_node->left->data))
			{ deep = setIndexedSubTree(deep-1,_root_node->left,InNode,isNode,is_binary); }
			if(_root_node->rigth!=NULL && isNode(_root_node->rigth->data))
			{ deep = setIndexedSubTree(deep-1,_root_node->rigth,InNode,isNode,is_binary);}
		}else
		{
			if(_root_node->unique!=NULL && isNode(_root_node->unique->data))
			{ deep = setIndexedSubTree(deep-1,_root_node->unique,InNode,isNode,is_binary);}
		}
	}
	return deep;
}

/********************************************************************************
* 																				*
*	go over the tree and substitute the new Node passed in the indexed node 	*
* 																				*
********************************************************************************/
template<typename T>
int setIndexedNode(int index,Node<T>* _root_node,Node<T>* InNode,bool (*is_binary)(T))
{
	int deep = index;
	if(index==0){ *_root_node=*InNode; }
	else if(index>=1){
		if(is_binary(_root_node->data))
		{
			if(_root_node->left!=NULL)
			{ deep = setIndexedNode(deep-1,_root_node->left,InNode,is_binary); }
			if(_root_node->rigth!=NULL)
			{ deep = setIndexedNode(deep-1,_root_node->rigth,InNode,is_binary);}
		}else
		{
			if(_root_node->unique!=NULL)
			{ deep = setIndexedNode(deep-1,_root_node->unique,InNode,is_binary);}
		}
	}
	return deep;
}

#endif
