#include "Node.cpp"
#include <math.h>
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
#define BIT_MUT 0
#define OPER_MUT 1
#define SUBT_MUT 2
#define NO_MUT 3

/*
*
*/
#define TOURN 0
#define ROULT 1


   

/*********************************************
*   structure for tratment with individuals  *
*********************************************/
template<typename T,typename U>
struct Indiv
{
    Node<T>* chrom;
    U fitness; 
    Indiv(){}   
};

/********************************************
*   structure for the genetic algorithm     *
********************************************/  
template<typename T,typename U>
struct GenAlg{
    Indiv<T,U>* INDIVIDUALS;
    Indiv<T,U>* NEWINDIVIDUALS;
    int   max_generations;
    int   poblation_size;
    int   indiv_init_size;
    int   selection_opt;
    int   fill_selection;
    int   m_selection;
    int   c_selection;
    int   s_selection;
    U mutation_rate;
    U crossover_rate;
    U cross_proportion;
    //functions releated with the treatment of the tree individuals
    U       (*EVUE)(int,U);    // evaluate expresion
    U       (*EVBE)(int,U,U);    // evaluate expresion
    U       (*GTR)(T);          //get terminal
    T       (*GOP)();           //get operand
    T       (*GLF)();           //get leaf
    bool    (*IND)(T);          //is a node?
    bool    (*IBN)(T);          //is a binary operation?
    bool    (*IVR)(T);          //is a variable?
    int     (*GVI)(T);          //get variable index(the variables can be in disorder)
    int     (*GEI)(T);          //get expreison index(the expresions can be in disorder)
    std::string (*GCHR)(T);
    GenAlg(){}
};

template<typename T,typename U>
GenAlg<T,U>* newGA(int fillS,int ms,int cs,int ss,int pobSiz,int indvSiz,int max_g,float mutrt,float crossrt,float cross_prop,
 U (*evue)(int,U),U (*evbe)(int,U,U), U (*gtr)(T), T (*gop)(),T (*glf)(),bool (*ind)(T),bool (*ibn)(T),bool (*ivr)(T),int (*gvi)(T),int (*gei)(T),std::string (*gchr)(T))
{   
    if(ss==TOURN && pobSiz%2!=0){printf("SI SELECCIONAS >>SELECCION POR TORNEO<< DEBES TENER UN NUMERO PAR DE INDIVIDUOS\n");exit(EXIT_FAILURE);}
    GenAlg<T,U>* tmpGA = new GenAlg<T,U>();
    tmpGA->INDIVIDUALS=new Indiv<T,U>[pobSiz];
    tmpGA->NEWINDIVIDUALS=new Indiv<T,U>[pobSiz];
    tmpGA->max_generations = max_g;
    tmpGA->fill_selection = fillS;
    tmpGA->m_selection = ms;
    tmpGA->c_selection = cs;
    tmpGA->s_selection = ss;
    tmpGA->poblation_size = pobSiz;
    tmpGA->indiv_init_size = indvSiz;
    tmpGA->mutation_rate = mutrt;
    tmpGA->crossover_rate = crossrt;
    tmpGA->cross_proportion= cross_prop;
    tmpGA->EVUE = evue;
    tmpGA->EVBE = evbe;
    tmpGA->GTR = gtr;
    tmpGA->GOP = gop;
    tmpGA->GLF = glf;
    tmpGA->IND = ind;
    tmpGA->IBN = ibn;
    tmpGA->IVR = ivr;
    tmpGA->GVI = gvi;
    tmpGA->GEI = gei;
    tmpGA->GCHR = gchr;
    return tmpGA;   
}

template<typename T, typename U>
void initPobRec(GenAlg<T,U>* GA,int var_size,std::vector<U*> _vals,int _vals_size,int counter)
{
    if(counter%100 == 0)printf("\n");
    printf("o");
    if(counter==GA->poblation_size){return;}
    else{
        //printf("{%d,",counter);:
        int flag=1;
        do{
            GA->INDIVIDUALS[counter]=*newIndiv(GA,var_size,_vals,_vals_size);
            if(isinf(GA->INDIVIDUALS[counter].fitness)||GA->INDIVIDUALS[counter].fitness>200000000000000)
            { 
                flag = 1;
                delete GA->INDIVIDUALS[counter].chrom;
            }else{ flag = 0; }
        }
        while(flag);
        initPobRec(GA,var_size,_vals,_vals_size,counter+1);
    }
    return;
}

/***************************************************************************************************************
*   FS-> fill_selection; D -> depth; GO-> getOperand; GT -> getTerminal; EE-> evalExpresion; IN-> isNode       *
*   IV-> isVar; GVI-> getVarIndex; GEI-> getExpresionIndex; val-> values to model                              *
***************************************************************************************************************/
template<typename T,typename U>
Indiv<T,U>* newIndiv(GenAlg<T,U>* GA,int var_size,std::vector<U*> _vals,int _vals_size)
{
    Indiv<T,U>* temp_indiv = new Indiv<T,U>();
    temp_indiv->chrom = newNode(GA->fill_selection,GA->indiv_init_size,GA->GOP(),GA->IBN,GA->GLF,GA->GOP);
    temp_indiv->fitness = cuadraticError(temp_indiv->chrom,GA->EVBE,GA->EVUE,GA->IND,GA->IBN,GA->IVR,GA->GVI,GA->GTR,GA->GEI,_vals,_vals_size,var_size);
    return temp_indiv;
}

template<typename T,typename U>
void evalFitness(GenAlg<T,U>* GA,Indiv<T,U>* t_indiv,int var_size,std::vector<U*> _vals,int _vals_size){
    t_indiv->fitness = cuadraticError(t_indiv->chrom,GA->EVBE,GA->EVUE,GA->IND,GA->IBN,GA->IVR,GA->GVI,GA->GTR,GA->GEI,_vals,_vals_size,var_size);
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
        if(index,GA->INDIVIDUALS[index].fitness<(U)10*GA->poblation_size)PrintTree(GA->INDIVIDUALS[index].chrom,GA->IND,GA->IBN,GA->GCHR); 
        printf("\n");
    }
}

template<typename T,typename U>
void printNewPobStatus(GenAlg<T,U>* GA)
{
    for(int index = 0;index<GA->poblation_size;index++)
    { 
        printf("{\t%d\t,\t%6.4f\t}",index,GA->NEWINDIVIDUALS[index].fitness);
        if(index,GA->NEWINDIVIDUALS[index].fitness<(U)10*GA->poblation_size)PrintTree(GA->NEWINDIVIDUALS[index].chrom,GA->IND,GA->IBN,GA->GCHR); 
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
        case ROULT://problemas
            for(int index=0;index<GA->poblation_size;index++)
            { if(pob_fitness[index]>max_fit)max_fit=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]= max_fit + 1 - pob_fitness[index]; total_fit+=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]=pob_fitness[index]*GA->poblation_size/total_fit; if(index>0){ pob_fitness[index]=pob_fitness[index]+pob_fitness[index-1]; } }
            for(int ext_index=0;ext_index<GA->poblation_size;ext_index++)
            {
                float temp_random = ((float)(rand()%1000000)/(float)1000000)*GA->poblation_size;
                for(int index=0;index<GA->poblation_size;index++)
                { 
                    GA->NEWINDIVIDUALS[ext_index]=GA->INDIVIDUALS[index];
                    if(pob_fitness[index]>temp_random && index>0){ GA->NEWINDIVIDUALS[ext_index]=GA->INDIVIDUALS[index-1]; break; }  
                }
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
    GA->NEWINDIVIDUALS = new Indiv<T,U>[GA->poblation_size];
}

template<typename T, typename U>
void CROSSOVER(GenAlg<T,U>* GA,int var_size,std::vector<U*> _vals,int _vals_size)
{
    std::vector<int> parent_indexes;
    int indexA=0;
    int indexB=0;
    for(int index = 0;index<GA->poblation_size;index++)
    {
        float temp_random = (float)(rand()%1000000)/((float)1000000);
        if(temp_random<=GA->crossover_rate){ parent_indexes.push_back(index); }
    }
    if(parent_indexes.size()%2!=0){ parent_indexes.pop_back(); }
    Node<T>* subtreeA=new Node<T>;
    Node<T>* subtreeB=new Node<T>;
    for(int index = 0;index<parent_indexes.size();index+=2)
    {
        int parent_A = parent_indexes[index];
        int parent_B = parent_indexes[index+1];
        if(GA->INDIVIDUALS[parent_A].fitness>0 && GA->INDIVIDUALS[parent_B].fitness>0) 
        {
            float parent_A_fit = GA->INDIVIDUALS[parent_A].fitness;
            float parent_B_fit = GA->INDIVIDUALS[parent_B].fitness;
            float lowest_fit = (parent_A<parent_B)?parent_A:parent_B;
            int countOperandsA = nodeCounter(GA->INDIVIDUALS[parent_A].chrom,GA->IND,GA->IBN);
            int countOperandsB = nodeCounter(GA->INDIVIDUALS[parent_B].chrom,GA->IND,GA->IBN);

            switch (GA->c_selection){
            case HALF_CROSS:// 50% of individual information will be interchanged 
                indexA = (int)floor(countOperandsA/2); indexB = (int)floor(countOperandsB/2); break;
            case PROP_CROSS://porcentage proportional cross, the value is from 0 to 1
                indexA = (int)floor(countOperandsA*(GA->cross_proportion)); indexB = (int)floor(countOperandsB*(GA->cross_proportion)); break;    
            case RAND_CROSS://aleatory index
                float rand_n = (float)(rand()%1000000)/(float)1000000;
                indexA = (int)floor(countOperandsA*rand_n); indexB = (int)floor(countOperandsB*rand_n); break;
            }

            /*gets the indexed operand over the trees*/
            getIndexedSubTree(indexA,GA->INDIVIDUALS[parent_A].chrom,subtreeA,GA->IND,GA->IBN);
            getIndexedSubTree(indexB,GA->INDIVIDUALS[parent_B].chrom,subtreeB,GA->IND,GA->IBN);
            /* interchange the indexed operands*/
            setIndexedSubTree(indexA,GA->INDIVIDUALS[parent_A].chrom,subtreeB,GA->IND,GA->IBN);
            setIndexedSubTree(indexB,GA->INDIVIDUALS[parent_B].chrom,subtreeA,GA->IND,GA->IBN);
            evalFitness(GA,&GA->INDIVIDUALS[parent_A],var_size,_vals,_vals_size);
            evalFitness(GA,&GA->INDIVIDUALS[parent_B],var_size,_vals,_vals_size);
            if(GA->INDIVIDUALS[parent_A].fitness>=lowest_fit && GA->INDIVIDUALS[parent_B].fitness>=lowest_fit)
            {   /*UNDID THE ACTION*/
                setIndexedSubTree(indexA,GA->INDIVIDUALS[parent_A].chrom,subtreeA,GA->IND,GA->IBN);
                setIndexedSubTree(indexB,GA->INDIVIDUALS[parent_B].chrom,subtreeB,GA->IND,GA->IBN);
                evalFitness(GA,&GA->INDIVIDUALS[parent_A],var_size,_vals,_vals_size);
                evalFitness(GA,&GA->INDIVIDUALS[parent_B],var_size,_vals,_vals_size);
            }
        }
    }
    delete subtreeA;
    delete subtreeB;
}

template<typename T,typename U>
void MUTATION(GenAlg<T,U>* GA,int var_size,std::vector<U*> _vals,int _vals_size)
{
    int total_nodes = 0;//totalNodeCounter(indiv_mut->chrom);
    int subt_total_nodes=0;
    int nodes = 0;// nodeCounter(indiv_mut->chrom,GA->IND);
    int temp_rand=0;
    int temp_rand2=0;
    float last_fitness=0.0f;
    Node<T>* temp_node = new Node<T>();
    Node<T>* temp_node_back = new Node<T>();
    for(int index=0;index<GA->poblation_size;index++)
    {
        float temp_random = (float)(rand()%1000000)/((float)1000000);
        if(temp_random<=GA->mutation_rate&&GA->INDIVIDUALS[index].fitness>0){ 
            //MutateIndiv(GA,&GA->INDIVIDUALS[index]); 
            total_nodes = totalNodeCounter(GA->INDIVIDUALS[index].chrom,GA->IBN);
            nodes = nodeCounter(GA->INDIVIDUALS[index].chrom,GA->IND,GA->IBN);
            switch(GA->m_selection)
            {
                case BIT_MUT:
                    _bt_mt:  
                    temp_rand= rand()%total_nodes;
                    getIndexedNode(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IBN);
                    temp_node_back->data = temp_node->data;
                    last_fitness = GA->INDIVIDUALS[index].fitness;
                    if(GA->IND(temp_node->data))
                    { temp_node->data = GA->GOP(); }
                    else
                    { temp_node->data = GA->GLF(); }
                    setIndexedNode(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IBN);
                    evalFitness(GA,&GA->INDIVIDUALS[index],var_size,_vals,_vals_size);
                    if(GA->INDIVIDUALS[index].fitness>last_fitness)
                    {
                        temp_node->data=temp_node_back->data;
                        setIndexedNode(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IBN);
                        evalFitness(GA,&GA->INDIVIDUALS[index],var_size,_vals,_vals_size);
                    }
                break;
                case OPER_MUT:
                    temp_rand = rand()%nodes;
                    getIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND,GA->IBN);
                    temp_node_back->data = temp_node->data;
                    last_fitness = GA->INDIVIDUALS[index].fitness;
                    temp_node->data = GA->GOP();
                    setIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND,GA->IBN);
                    evalFitness(GA,&GA->INDIVIDUALS[index],var_size,_vals,_vals_size);
                    if(GA->INDIVIDUALS[index].fitness>last_fitness)
                    {
                        temp_node->data=temp_node_back->data;
                        setIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND,GA->IBN);
                        evalFitness(GA,&GA->INDIVIDUALS[index],var_size,_vals,_vals_size);
                    }
                break;
                case SUBT_MUT:
                    _sb_mt:
                    temp_rand = rand()%nodes;
                    temp_rand2 = (rand()%500)?(0):(1);
                    getIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND,GA->IBN);
                    temp_node_back = temp_node;
                    last_fitness = GA->INDIVIDUALS[index].fitness;
                    temp_node = newNode(GA->fill_selection,(int)(log2f(totalNodeCounter(temp_node,GA->IBN))+temp_rand2),GA->GOP(),GA->IBN,GA->GLF,GA->GOP);
                    setIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND,GA->IBN);
                    evalFitness(GA,&GA->INDIVIDUALS[index],var_size,_vals,_vals_size);
                    if(GA->INDIVIDUALS[index].fitness>last_fitness)
                    {
                        temp_node=temp_node_back;
                        setIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND,GA->IBN);
                        evalFitness(GA,&GA->INDIVIDUALS[index],var_size,_vals,_vals_size);
                    }
                break;
                case NO_MUT: return ;
                break;
            } 
        }
    } 
}

template<typename T,typename U>
void evalPoblation(GenAlg<T,U>* GA,int var_size,std::vector<U*> _vals,int _vals_size)
{
    for(int index = 0;index<GA->poblation_size;index++)
    {
        evalFitness(GA,&GA->INDIVIDUALS[index],var_size,_vals,_vals_size);
    }
}

template<typename T,typename U>
void KILLWORSTINDIVS(GenAlg<T,U>* GA,int var_size,std::vector<U*> _vals,int _vals_size)
{
    for(int index=0;index<GA->poblation_size;index++)
    {
        if(GA->INDIVIDUALS[index].fitness>200000000000000)
        {
            GA->INDIVIDUALS[index]=*newIndiv(GA,var_size,_vals,_vals_size);
        }
    }
}



template<typename T,typename U>
void GENETICPROSSES(GenAlg<T,U>* GA,int var_size,std::vector<U*> _vals,int _vals_size)
{   
    float advantage=0.0f;
    float best_fit=0.0f;
    int adv_counter = 0;
    Indiv<T,U>* temp_indiv=new Indiv<T,U>();
    for(int index = 0;index<GA->max_generations;index++)
    {
        advantage = 100*(1.0f-((float)GA->max_generations-(float)index)/(float)GA->max_generations);
        if((advantage-(float)adv_counter)>1.0f){printf("#");adv_counter++;if(adv_counter%5==0){printf(".");}}
        
        KILLWORSTINDIVS(GA,var_size,_vals,_vals_size);
        SELECTION(GA);
        CROSSOVER(GA,var_size,_vals,_vals_size);
        MUTATION(GA,var_size,_vals,_vals_size);
        evalPoblation(GA,var_size,_vals,_vals_size);
        *temp_indiv = GA->INDIVIDUALS[getBestIndiv(GA)];
        printf("%04d \t: fitness : %06.6f \t: count_ops : %d\n",index,temp_indiv->fitness,nodeCounter(temp_indiv->chrom,GA->IND,GA->IBN));
        if(temp_indiv->fitness<0.0000000000000000000000000000000000001)break;
    }
    printf("COMPlETE!!\n");
}

template<typename T,typename U>
int getBestIndiv(GenAlg<T,U>*  GA)  
{
    int best_index = 0;   
    Indiv<T,U>* best_indiv=new Indiv<T,U>(); 
   best_indiv->fitness=(U)1/0.000000000000000000000000000000000000000000001;
   for(int index=0;index<GA->poblation_size;index++)
   { if(best_indiv->fitness>GA->INDIVIDUALS[index].fitness){ best_index=index;*best_indiv = GA->INDIVIDUALS[index]; } }
   delete best_indiv;
   return best_index;
}

template<typename T,typename U>
float printBestIndiv(GenAlg<T,U>* GA)
{
   Indiv<T,U>* best_indiv=new Indiv<T,U>(); 
   best_indiv->fitness=(U)1/0.000000000000000000000000000000000000000000001;
   for(int index=0;index<GA->poblation_size;index++)
   {
        if(best_indiv->fitness>GA->INDIVIDUALS[index].fitness){ *best_indiv = GA->INDIVIDUALS[index]; } 
   }
   printf("fitness: %f\n",best_indiv->fitness);PrintTree(best_indiv->chrom,GA->IND,GA->IBN,GA->GCHR);printf("\n");
   float last_fitness = best_indiv->fitness;
   delete best_indiv;
   return last_fitness;
}



#endif
