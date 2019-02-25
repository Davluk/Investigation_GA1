#include<stdio.h>
#include<stdlib.h>
#include"infoType.cpp"

#ifndef TREEMANAGER_CPP
#define TREEMANAGER_CPP

class treeManager
{
	public:
	enum types {opr,term,var,func};
	int* operators;
	int* terminals;
	int* variables;
	char* repOperators;
	char* repVariables;
	float	(**functions)(float,float);
	treeManager(int* tree_operators,int* tree_terminals,int* tree_variables,char* rep_operators,char* rep_variables,float (**tree_functions)(float,float)){
		operators = tree_operators;
		terminals = tree_terminals;
		variables = tree_variables;
		repOperators = rep_operators;
		repVariables = rep_variables;
		functions = tree_functions;
	}
	inline int countElements(int* elements){ return (int)sizeof(elements)/sizeof(elements[0]); }
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