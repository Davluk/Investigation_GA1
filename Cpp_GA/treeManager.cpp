#include<stdio.h>
#include<stdlib.h>
#include<iostream>

#ifndef TREEMANAGER_CPP
#define TREEMANAGER_CPP

class infoType
{
	public:
	int data;
	int type;
	infoType(){}
	infoType(infoType* copy){ 
		data = copy->data;
		type = copy->type; 
	}	
	infoType(int node_Data,int node_type){ 
		data = node_Data; 
		type = node_type; 
	}
	~infoType(){}
};

class node
{
	public:
	infoType data;
	node* left;
	node* rigth;
	
	node(){}
	~node(){}
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

class tree{
	public:
	node data;
	float fitness;
	tree(){}
	~tree(){}
	tree(tree* copy){
		data = copy->data;
		fitness = copy->fitness;
	}
	tree(int maxDepth,infoType (*newOperand)(),infoType (*newData)(),infoType (*newLeaf)(),bool (*isNode)(infoType)){
		data = new node(newOperand(),newData(),newData(),maxDepth,newData,newLeaf,isNode);
	}	
};

class treeManager
{
	public:
	enum types {opr,term,var};
	static int* operators;
	static int* terminals;
	static int* variables;
	static char* repOperators;
	static char* repVariables;

	static float	(**functions)(float,float);

	treeManager(){}
	~treeManager(){}
	treeManager(int* tree_operators,int* tree_terminals,int* tree_variables,char* rep_operators,char* rep_variables,float (**tree_functions)(float,float)){
		operators = tree_operators;
		terminals = tree_terminals;
		variables = tree_variables;
		repOperators = rep_operators;
		repVariables = rep_variables;
		functions = tree_functions;
	}

	tree* newTree(int maxDepth,float** current_values)
	{
		tree temp_tree(maxDepth,getOperand,getData,getLeaf,isNode);
		//evaluateExpression(&temp_tree.data);
	}

	/*float evaluateExpression(node *tempTree,float* current_values)
	{
		float left_operand=0.0f;
		float rigth_operand=0.0f;
		if(tempTree->left!=NULL && tempTree->left.data.type==opr){
			left_operand = evaluateExpression(tempTree->left,current_values);
		}else{
			if(tempTree->left!=NULL && tempTree->left.type==var){
				left_operand = current_values[tempTree->left.data.data];
			}else{
				left_operand = tempTree->left.data;			
			}
		}
		if(tempTree->rigth!=NULL && tempTree->rigth.data.type==opr){
			left_operand = evaluateExpression(tempTree->rigth,current_values);
		}else{
			if(tempTree->rigth!=NULL && tempTree->rigth.type==var){
				rigth_operand = current_values[tempTree->rigth.data.data];
			}else{
				rigth_operand = tempTree->left.data;			
			}
		}
		return functions[tempTree->data](left_operand,rigth_operand);
	}
	*/


	//la funcion debe de trabajar correctamente independientemente de el tipo de dato
	static int countElements(int* elements){ return static_cast<int>(sizeof(elements)/sizeof(elements[0])); }
	static bool isNode(infoType temp_data) { return (temp_data.data==opr); }
	static infoType getData(){
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

	static infoType getLeaf(){
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

	static infoType getOperand()
	{
		infoType temp_info;
		temp_info.data = operators[rand()%countElements(operators)];
		temp_info.type = term;
		return temp_info;
	}
	
	static char* getDataChar(infoType temp_data){
		char* temp_string = new char(' ');
		//it can be modificated for print some other strings for each terminal
		if(temp_data.type == term){ 
			sprintf(temp_string,"%d",temp_data.data); 
		}else{
			if(temp_data.type == opr){ 
				sprintf(temp_string,"%c",repOperators[temp_data.data]);
			}else{
				sprintf(temp_string,"%c",repVariables[temp_data.data]);
			}
		}
		return temp_string;
	}	
};

#endif