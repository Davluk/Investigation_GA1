#include "infoType.cpp"
#include <iostream>
#ifndef NODE_CPP
#define NODE_CPP

class node
{
	public:
	infoType data;
	node* left;
	node* rigth;
	
	node(){}
	node(node* copy)
	{
		data = copy->data;
		left = copy->left;
		rigth = copy->rigth;
	}
	node(infoType node_data){
		data = node_data; 
	}
	node(infoType node_data,infoType left_data,infoType rigth_data,int depth,infoType (*getData)(),infoType (*getLeaf)(),bool (*isNode)(infoType))
	{
		data = node_data;
		if(depth==2)
		{
			left = new node(getLeaf());
			rigth= new node(getLeaf());
		}else{
			if(isNode(left_data )){ left  = new node(left_data ,getData(),getData(),depth-1,getData,getLeaf,isNode); }
			else{ left  = new node(left_data ); }
			if(isNode(rigth_data)){ rigth = new node(rigth_data,getData(),getData(),depth-1,getData,getLeaf,isNode); }
			else{ rigth = new node(rigth_data); }
		}
	}

	void printPreOrder(char* (*getDataChar)(infoType),bool (*isNode)(infoType))
	{
		std::cout<<"( "<<getDataChar(data)<<" ";
		if(isNode(&left->data)){ left->printPreOrder(getDataChar,isNode); }
		else{ std::cout<<getDataChar(left->data)<<" "; }
		if(isNode(&rigth->data)){ rigth->printPreOrder(getDataChar,isNode); }
		else{ std::cout<<getDataChar(rigth->data)<<" "; }
		std::cout<<" )";
	}
};

#endif