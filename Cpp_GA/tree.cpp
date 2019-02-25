#include<stdio.h>
#include<iostream>
#include<string>
#include"node.cpp"
using namespace std;


#ifndef TREE_CPP
#define TREE_CPP

class tree{
	public:
	node* data;
	int fitness;
	tree(){}
	tree(tree* copy){
		data = copy->data;
		fitness = copy->fitness;
	}
	tree(){

	}
};

#endif
