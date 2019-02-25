#include<iostream>
template <class T>
class node
{
	T data;
	node* left;
	node* rigth;
	node(){}
	node(node* copy)
	{
		this.data = copy.data;
		this.left = copy.left;
		this.rigth = copy.rigth;
	}
	node(T node_data){
		this.data = node_data; 
	}
	node(T node_data,T left_data,T rigth_data,int depth,T (*getData)(),T (*getLeaf)(),bool (*isNode)(T))
	{
		this.data = node_data;
		if(depth==2)
		{
			this.left = new node(getLeaf());
			this.rigth= new node(getLeaf());
		}else{
			if(isNode(left_data )){ this.left  = new node(left_data ,getData(),getData(),depth-1,getLeaf,isNode); }
			else{ this.left  = new node(left_data ); }
			if(isNode(rigth_data)){ this.rigth = new node(rigth_data,getData(),getData(),depth-1,getLeaf,isNode); }
			else{ this.rigth = new node(rigth_data); }
		}
	}
	void printPreOrder(char (*getDataChar)(T),bool (*isNode)(T))
	{
		std::cout<<"( "<<getData(this.data)<<" ";
		if(isNode(this.left)){ this.left->printPreOrder(getDataChar,isNode); }
		else{ std::cout<<getDataChar(this.left)<<" "; }
		if(isNode(this.rigth)){ this.rigth->printPreOrder(getDataChar,isNode); }
		else{ std::cout<<getDataChar(this.rigth)<<" "; }
		std::cout<<" )";
	}
};

class treeManager
{
public:
	treeManager(){

	}
	~treeManager();
	
};