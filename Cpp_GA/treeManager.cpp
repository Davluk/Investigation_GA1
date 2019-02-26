#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include"tree.cpp"

#ifndef TREEMANAGER_CPP
#define TREEMANAGER_CPP

class treeManager
{
	public:
	enum types {opr,term,var};
	int* operators;
	int* terminals;
	int* variables;
	char* repOperators;
	char* repVariables;

	float	(**functions)(float,float);

	infoType getData();
	infoType getLeaf();
	bool isNode(infoType);

	treeManager(int* tree_operators,int* tree_terminals,int* tree_variables,char* rep_operators,char* rep_variables,float (**tree_functions)(float,float)){
		operators = tree_operators;
		terminals = tree_terminals;
		variables = tree_variables;
		repOperators = rep_operators;
		repVariables = rep_variables;
		functions = tree_functions;
	}

	tree* newTree(int maxDepth,float** current_values,infoType (*newOperand)(),infoType (*newData)(),infoType (*newLeaf)(),bool (*checkNode)(infoType))
	{
		tree temp_tree(maxDepth,newOperand,newData,newLeaf,checkNode);
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
	int countElements(int* elements){ return static_cast<int>(sizeof(elements)/sizeof(elements[0])); }
	bool isNode(infoType temp_data) { return (temp_data.data==opr); }
	infoType getData(){
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

	infoType getLeaf(){
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
	
	infoType getOperand()
	{
		infoType temp_info;
		temp_info.data = operators[rand()%countElements(operators)];
		temp_info.type = opr;
		return temp_info;
	}

	char* getDataChar(infoType temp_data){
		char* temp_string = " ";
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

	~treeManager();
	
};

#endif