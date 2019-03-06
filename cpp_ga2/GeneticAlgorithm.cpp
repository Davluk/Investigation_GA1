#include "Node.cpp"
#ifndef GEN_ALG
#define GEN_ALG

// all the methods is for tree tratment

#define HALF_CROSS 0
#define PROP_CROSS 1
#define RAND_CROSS 2

template<typename T>
struct GenAlg{
    Node<T>* population;
    GenAlg(){}
};

template<typename T>
struct Indiv
{
    Node<T>* chrom;
    float fitness; 
    Indiv(){}   
};

/************************************************************************************************************
*FS-> fill_selection; D -> depth; GO-> getOperand; GT -> getTerminal; EE-> evalExpresion; IN-> isNode       *
*IV-> isVar; GVI-> getVarIndex; GEI-> getExpresionIndex; val-> values to model                              *
************************************************************************************************************/
template<typename T,typename U>
Indiv<T>* newIndiv(int FS,int D,T (*GO)(),T (*GT)(),U (*EE)(int,U,U),bool (*IN)(T),bool (*IV)(T),int (*GVI)(T),int (*GEI)(T),int Fx,U* Val,size_t sizeVal)
{
    Indiv<T>* temp_indiv = new Indiv<T>();
    temp_indiv->chrom = newNode(FS,D,GO(),GT,GO);
    temp_indiv->fitness = cuadraticError(temp_indiv->chrom,IN,IV,GVI,GT,GEI,Fx,Val,sizeVal);
    return temp_indiv;
}
template<typename T>
void CrossIndivs(int Option,float porc,Indiv<T>* parentA,Indiv<T>* parentB,bool (*IN)())
{
    Node<T>* subIndivA;
    Node<T>* subIndivB;
    int countOperandsA = nodeCounter(parentA,IN);
    int countOperandsB = nodeCounter(parentB,IN);
    int Index_A=0;
    int Index_B=0;
    switch (Option){
        case HALF_CROSS: 
            Index_A = (int)countOperandsA/2; Index_b = (int)countOperandsb/2; break;
        case PROP_CROSS://porcentage proportional cross, the value is from 0 to 1
            Index_A = (int)countOperandsA*(porc); Index_b = (int)countOperandsb*(1-porc); break;    
        case RAND_CROSS://aleatory index
            Index_A = rand()%countOperandsA; Index_b = rand()%countOperandsb; break;    
    }
    /*gets the indexed operand over the trees*/
    getIndexedSubTree(Index_A,parentA,subIndivA,IN);
    getIndexedSubTree(Index_B,parentB,subIndivB,IN);
    /* interchange the indexed operands*/
    setIndexedSubTree(Index_A,parentA,subIndivB,IN);
    setIndexedSubTree(Index_B,parentB,subIndivA,IN);
}

#endif