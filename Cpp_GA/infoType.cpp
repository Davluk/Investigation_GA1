#ifndef INFOTYPE_CPP
#define INFOTYPE_CPP

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

#endif