#include<iostream>
class infoType
{
	public:
	int data;
	int type;
	infoType(){}
	infoType(infoType* copy){ 
		data = copy->data; 
	}	
	infoType(int node_Data,int node_type){ 
		data = node_Data; 
		type = node_type; 
	}
	~infoType();
};

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

	void printPreOrder(char (*getDataChar)(infoType),bool (*isNode)(infoType))
	{
		std::cout<<"( "<<getDataChar(data)<<" ";
		if(isNode(&left->data)){ left->printPreOrder(getDataChar,isNode); }
		else{ std::cout<<getDataChar(left->data)<<" "; }
		if(isNode(&rigth->data)){ rigth->printPreOrder(getDataChar,isNode); }
		else{ std::cout<<getDataChar(rigth->data)<<" "; }
		std::cout<<" )";
	}
};

/*
	- is node
	- get data
	- get leaf
	- getDataChar
*/
class treeManager
{
	public:
	enum types {opr,term,var,func};
	int* operators;
	int* terminals;
	int* variables;
	float	(**functions)(float,float);
	treeManager(int* tree_operators,int* tree_terminals,int* tree_variables,float (**tree_functions)(float,float)){
		operators = tree_operators;
		terminals = tree_terminals;
		variables = tree_variables;
		functions = tree_functions;
	}
	int countElements(int* elements){ int counter=0; while(true){ if(elements[counter]!=NULL){ counter++; }else{ return counter; } } }
	bool isNode(infoType temp_data) { return (temp_data.data==opr); }
	infoType getData()
	{
		int coin = rand()%4;
		infoType temp_Type;
		if(coin>2){
			coin = rand()%4;
			if(coin>2){
				temp_Type.data = terminals[rand()%countElements(terminals)];
				temp_Type.type = term;
			}else{
				temp_Type.data = variables[rand()%countElements(variables)];
				temp_Type.type = var;
			}
		}else{
			temp_Type.data = operators[rand()%countElements(operators)];
			temp_Type.type = opr;
		}
		return temp_Type;
	}

	infoType getLeaf()
	{
		int coin = rand()%4;
		infoType temp_Type;
		if(coin>2){
			temp_Type.data = terminals[rand()%countElements(terminals)];
			temp_Type.type = term;
		}else{
			temp_Type.data = variables[rand()%countElements(variables)];
			temp_Type.type = var;
		}
		return temp_Type;
	}

	~treeManager();
	
};