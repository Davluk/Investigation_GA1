#include"node.cpp"
#ifndef TREE_CPP
#define TREE_CPP

class tree{
	public:
	node data;
	float fitness;
	tree(){}
	tree(tree* copy){
		data = copy->data;
		fitness = copy->fitness;
	}
	tree(int maxDepth,infoType (*newOperand)(),infoType (*newData)(),infoType (*newLeaf)(),bool (*isNode)(infoType)){
		data = new node(newOperand(),newData(),newData(),maxDepth,newData,newLeaf,isNode);
	}	
};

#endif
