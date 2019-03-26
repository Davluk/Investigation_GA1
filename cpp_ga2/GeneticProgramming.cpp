#include "Node.cpp"
#include <vector>
#ifndef GEN_ALG
#define GEN_ALG

/* ************************************************************* 
*                                                              *
*           all the methods is for tree tratment               *
*                                                              *
***************************************************************/

/********************************************
*   constants for the genetic algorithm     *    
********************************************/
#define HALF_CROSS 0
#define PROP_CROSS 1
#define RAND_CROSS 2

/*
*
*/
#define LEAF_MUT 0
#define OPER_MUT 1
#define SUBT_BUT 2
#define NO_MUT 3

/*
*
*/
#define TOURN 0
#define ROULT 1


   

/*********************************************
*   structure for tratment with individuals  *
*********************************************/
template<typename T>
struct Indiv
{
    Node<T>* chrom;
    float fitness; 
    Indiv(){}   
};

/********************************************
*   structure for the genetic algorithm     *
********************************************/  
template<typename T,typename U>
struct GenAlg{
    Node<T>* subIndivA;//saves the direction of the B subtree
    Node<T>* subIndivB;//saves the direction of the A subtree 
    int Index_A;
    int Index_B;
    Indiv<T>* INDIVIDUALS;
    Indiv<T>* NEWINDIVIDUALS;
    int   poblation_size;
    int   indiv_init_size;
    int   selection_opt;
    int   fill_selection;
    int   m_selection;
    int   c_selection;
    int   s_selection;
    float mutation_rate;
    float crossover_rate;
    float cross_proportion;
    //functions releated with the treatment of the tree individuals
    U       (*EVE)(int,U,U);    // evaluate expresion
    U       (*GTR)(T);          //get terminal
    T       (*GOP)();           //get operand
    T       (*GLF)();           //get leaf
    bool    (*IND)(T);          //is a node?
    bool    (*IVR)(T);          //is a variable?
    int     (*GVI)(T);          //get variable index(the variables can be in disorder)
    int     (*GEI)(T);          //get expreison index(the expresions can be in disorder)
    char    (*GCHR)(T);
    GenAlg(){}
};

template<typename T,typename U>
GenAlg<T,U>* newGA(int fillS,int ms,int cs,int ss,int pobSiz,int indvSiz,float mutrt,float crossrt,float cross_prop,
 U (*eve)(int,U,U), U (*gtr)(T), T (*gop)(),T (*glf)(),bool (*ind)(T),bool (*ivr)(T),int (*gvi)(T),int (*gei)(T),char (*gchr)(T))
{   
    if(ss==TOURN && pobSiz%2!=0){printf("SI SELECCIONAS >>SELECCION POR TORNEO<< DEBES TENER UN NUMERO PAR DE INDIVIDUOS\n");exit(EXIT_FAILURE);}
    GenAlg<T,U>* tmpGA = new GenAlg<T,U>();
    tmpGA->subIndivA=new Node<T>();
    tmpGA->subIndivB=new Node<T>();
    tmpGA->Index_A=0.0f;
    tmpGA->Index_B=0.0f;
    tmpGA->INDIVIDUALS=new Indiv<T>[pobSiz];
    tmpGA->NEWINDIVIDUALS=new Indiv<T>[pobSiz];
    tmpGA->fill_selection = fillS;
    tmpGA->m_selection = ms;
    tmpGA->c_selection = cs;
    tmpGA->s_selection = ss;
    tmpGA->poblation_size = pobSiz;
    tmpGA->indiv_init_size = indvSiz;
    tmpGA->mutation_rate = mutrt;
    tmpGA->crossover_rate = crossrt;
    tmpGA->cross_proportion= cross_prop;
    tmpGA->EVE = eve;
    tmpGA->GTR = gtr;
    tmpGA->GOP = gop;
    tmpGA->GLF = glf;
    tmpGA->IND = ind;
    tmpGA->IVR = ivr;
    tmpGA->GVI = gvi;
    tmpGA->GEI = gei;
    tmpGA->GCHR = gchr;
    return tmpGA;
}

template<typename T, typename U,std::size_t SIZE>
void initPobRec(GenAlg<T,U>* GA,U (*_vals)[SIZE],size_t size_vals,int counter)
{
    //printf("%d ",counter);
    if(counter==GA->poblation_size){return;}
    else{
        GA->INDIVIDUALS[counter]=*newIndiv(GA,_vals,size_vals);
        //printf("index:\t%d\t",counter);PrintPosOrder(GA->INDIVIDUALS[counter].chrom,GA->IND,GA->GCHR);printf("fitness: \t%6.4f\n",GA->INDIVIDUALS[counter].fitness);
        initPobRec(GA,_vals,size_vals,counter+1);
    }
    return;
}

/***************************************************************************************************************
*   FS-> fill_selection; D -> depth; GO-> getOperand; GT -> getTerminal; EE-> evalExpresion; IN-> isNode       *
*   IV-> isVar; GVI-> getVarIndex; GEI-> getExpresionIndex; val-> values to model                              *
***************************************************************************************************************/
template<typename T,typename U,std::size_t  SIZE>
Indiv<T>* newIndiv(GenAlg<T,U>* GA,U (*Vals)[SIZE],size_t sizeVal)
{
    Indiv<T>* temp_indiv = new Indiv<T>();
    temp_indiv->chrom = newNode(GA->fill_selection,GA->indiv_init_size,GA->GOP(),GA->GLF,GA->GOP);
    temp_indiv->fitness = cuadraticError(temp_indiv->chrom,GA->EVE,GA->IND,GA->IVR,GA->GVI,GA->GTR,GA->GEI,Vals,sizeVal);
    return temp_indiv;
}

template<typename T,typename U,std::size_t SIZE>
void evalFitness(GenAlg<T,U>* GA,Indiv<T>* t_indiv,float (*values)[SIZE],size_t size_vals){
    t_indiv->fitness = cuadraticError(t_indiv->chrom,GA->EVE,GA->IND,GA->IVR,GA->GVI,GA->GTR,GA->GEI,values,size_vals);
}

/************************************************************************************
*   the function apply a kind of mutation to a individual with selection Option     *
************************************************************************************/
template<typename T,typename U>
void MutateIndiv(GenAlg<T,U>* GA,Indiv<T>* someIndiv)
{
    Indiv<T>* tempInd;
}

/****************************************************************************************
*   the funcitons interchange the subtree of both individuals with selection Option     *
****************************************************************************************/
template<typename T,typename U,std::size_t SIZE>
void CrossIndivs(GenAlg<T,U>* GA,Indiv<T>* parentA,Indiv<T>* parentB,float (*values)[SIZE],size_t size_vals)
{
    int countOperandsA = nodeCounter(parentA->chrom,GA->IND);
    int countOperandsB = nodeCounter(parentB->chrom,GA->IND);
    switch (GA->c_selection){
        case HALF_CROSS:// 50% of individual information will be interchanged 
            GA->Index_A = (int)countOperandsA/2; GA->Index_B = (int)countOperandsB/2; break;
        case PROP_CROSS://porcentage proportional cross, the value is from 0 to 1
            GA->Index_A = (int)countOperandsA*(GA->cross_proportion); GA->Index_B = (int)countOperandsB*(1 - GA->cross_proportion); break;    
        case RAND_CROSS://aleatory index
            GA->Index_A = rand()%countOperandsA; GA->Index_B = rand()%countOperandsB; break;
    }
    /*gets the indexed operand over the trees*/
    getIndexedSubTree(GA->Index_A,parentA->chrom,GA->subIndivA,GA->IND);
    getIndexedSubTree(GA->Index_B,parentB->chrom,GA->subIndivB,GA->IND);
    //printf("subtree: \n");PrintPosOrder(subIndivA,GA->IND,GA->GCHR);printf("\n");
    //printf("subtree: \n");PrintPosOrder(subIndivA,GA->IND,GA->GCHR);printf("\n");
    /* interchange the indexed operands*/
    setIndexedSubTree(GA->Index_A,parentA->chrom,GA->subIndivB,GA->IND);
    setIndexedSubTree(GA->Index_B,parentB->chrom,GA->subIndivA,GA->IND);
    evalFitness(GA,parentA,values,size_vals);
    evalFitness(GA,parentB,values,size_vals);
}

template<typename T,typename U,std::size_t SIZE>
void get_back_Cross(GenAlg<T,U>* GA,Indiv<T>* parentA,Indiv<T>* parentB,float (*_values)[SIZE],size_t size_vals)
{
    setIndexedSubTree(GA->Index_A,parentA->chrom,GA->subIndivA,GA->IND);
    setIndexedSubTree(GA->Index_B,parentB->chrom,GA->subIndivB,GA->IND);
    evalFitness(GA,parentA,_values,size_vals);
    evalFitness(GA,parentB,_values,size_vals);
}


/*##############################################################################
###############  Funciones para tomar informacion estadistica ##################
##############################################################################*/
template<typename T,typename U>
void printPobStatus(GenAlg<T,U>* GA)
{
    for(int index = 0;index<GA->poblation_size;index++)
    { 
        printf("{\t%d\t,\t%6.4f\t}",index,GA->INDIVIDUALS[index].fitness);
        if(index,GA->INDIVIDUALS[index].fitness<200.0f)PrintPosOrder(GA->INDIVIDUALS[index].chrom,GA->IND,GA->GCHR); 
        printf("\n");
    }
}

template<typename T,typename U>
void printNewPobStatus(GenAlg<T,U>* GA)
{
    for(int index = 0;index<GA->poblation_size;index++)
    { 
        printf("{\t%d\t,\t%6.4f\t}",index,GA->NEWINDIVIDUALS[index].fitness);
        if(index,GA->NEWINDIVIDUALS[index].fitness<200.0f)PrintPosOrder(GA->NEWINDIVIDUALS[index].chrom,GA->IND,GA->GCHR); 
        printf("\n");
    }
}

/*############################################################################
##############################################################################
###############     Funciones para el algoritmo genético    ##################
##############################################################################
############################################################################*/

int tournament(int ind1, float fit1, int ind2, float fit2)
{ return (fit1<fit2)?(ind1):(ind2); }

template<typename T,typename U>
void SELECTION(GenAlg<T,U>* GA)
{
    float pob_fitness[GA->poblation_size]; 
    float total_fit = 0.0f;
    float max_fit = 0.0f;

    for(int index =0;index<GA->poblation_size;index++)
    { pob_fitness[index] = GA->INDIVIDUALS[index].fitness; }

    switch(GA->s_selection)
    {
        case ROULT:
            for(int index=0;index<GA->poblation_size;index++)
            { if(pob_fitness[index]>max_fit)max_fit=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]= max_fit + 1 - pob_fitness[index]; total_fit+=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]=pob_fitness[index]*GA->poblation_size/total_fit; if(index>0){ pob_fitness[index]=pob_fitness[index]+pob_fitness[index-1]; } }
            for(int ext_index=0;ext_index<GA->poblation_size;ext_index++)
            {
                float temp_random = ((float)rand()/(float)(RAND_MAX+1))*GA->poblation_size;
                for(int index=0;index<GA->poblation_size;index++)
                { 
                    GA->NEWINDIVIDUALS[ext_index]=GA->INDIVIDUALS[index];
                    if(pob_fitness[index]>temp_random && index>0){ GA->NEWINDIVIDUALS[ext_index]=GA->INDIVIDUALS[index-1]; break; }  
                }
                printf("index:\t%d\t",ext_index);PrintPosOrder(GA->NEWINDIVIDUALS[ext_index].chrom,GA->IND,GA->GCHR);printf("\tfitness: \t%6.4f\n",GA->NEWINDIVIDUALS[ext_index].fitness);
            }
        break;
        case TOURN:
            int   ind_1 = 0;
            int   ind_2 = 0; 
            int counter = 0;
            std::vector<int> indexes; 

            for(int index=0;index<GA->poblation_size;index++)indexes.push_back(index);
            while(indexes.size()>0){
                ind_1= rand()%indexes.size(); int temp_ind1=indexes[ind_1]; indexes.erase(indexes.begin()+ind_1);
                ind_2= rand()%indexes.size(); int temp_ind2=indexes[ind_2]; indexes.erase(indexes.begin()+ind_2);
                GA->NEWINDIVIDUALS[counter++]=GA->INDIVIDUALS[tournament(temp_ind1,GA->INDIVIDUALS[temp_ind1].fitness,temp_ind2,GA->INDIVIDUALS[temp_ind2].fitness)];

            }
            for(int index=0;index<GA->poblation_size;index++)indexes.push_back(index);
            while(indexes.size()>0){
                ind_1= rand()%indexes.size(); int temp_ind1=indexes[ind_1]; indexes.erase(indexes.begin()+ind_1);
                ind_2= rand()%indexes.size(); int temp_ind2=indexes[ind_2]; indexes.erase(indexes.begin()+ind_2);
                GA->NEWINDIVIDUALS[counter++]=GA->INDIVIDUALS[tournament(temp_ind1,GA->INDIVIDUALS[temp_ind1].fitness,temp_ind2,GA->INDIVIDUALS[temp_ind2].fitness)];

            }
        break;
    }
    GA->INDIVIDUALS=GA->NEWINDIVIDUALS;
    GA->NEWINDIVIDUALS = new Indiv<T>[GA->poblation_size];
}

template<typename T, typename U,std::size_t SIZE>
void CROSSOVER(GenAlg<T,U>* GA,U (*_vals)[SIZE],size_t _val_size)
{
    std::vector<int> parent_indexes;
    float temp_random=0.0f;
    for(int index = 0;index<GA->poblation_size;index++)
    {
        temp_random = (float)rand()/(float)(RAND_MAX+1);
        if(temp_random<=GA->crossover_rate){ parent_indexes.push_back(index); }
    }
    if(parent_indexes.size()%2!=0){ parent_indexes.push_back(rand()%(GA->poblation_size-1)); }
    
    for(int index = 0;index<parent_indexes.size();index+=2)
    {
        int parent_1 = parent_indexes[index];
        int parent_2 = parent_indexes[index+1];

        float parent_a_fit = GA->INDIVIDUALS[parent_1].fitness;
        float parent_b_fit = GA->INDIVIDUALS[parent_2].fitness;
        
        CrossIndivs(GA,&GA->INDIVIDUALS[parent_1],&GA->INDIVIDUALS[parent_2],_vals,_val_size);

        float son1_fit = GA->INDIVIDUALS[parent_1].fitness;
        float son2_fit = GA->INDIVIDUALS[parent_2].fitness;
        
        if(son1_fit>parent_a_fit && son1_fit>parent_a_fit && son2_fit>parent_a_fit && son2_fit>parent_b_fit)
        {
            get_back_Cross(GA,&GA->INDIVIDUALS[parent_1],&GA->INDIVIDUALS[parent_2],_vals,_val_size);
        }
    }
}

#endif
