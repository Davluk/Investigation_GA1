import nodes
import random
import math

HALF_CROSS=0
PROP_CROSS=1
RAND_CROSS=2

BIT_MUT=0
OPR_MUT=1
SUB_MUT=2
NO_MUT=3

TOUR_SEL=0
ROUL_SEL=1

class GenAlg:
    def __init__(self,fill_s,ms,cs,ss,pobSiz,indSiz,maxG,mutRt,crossRt,crossPrp,exeBin,exeUn,gtTr,gtOp,gtLf,gtVrI,gtExI,gtChr,isNd,isBin,isVr):
        self.INDIVIDUALS    =[]
        self.NEWINDDIVDUALS =[]
        self.maxG=maxG
        self.fill_s  =fill_s
        self.mut_s   =ms
        self.cross_s =cs
        self.sel_s   =ss
        self.pobSiz =pobSiz
        self.indSiz =indSiz
        self.mutRt  =mutRt
        self.crossRt=crossRt
        self.crossPrp=crossPrp
        self.exeBin =exeBin
        self.exeUn  =exeUn
        self.gtTr   =gtTr
        self.gtOp   =gtOp
        self.gtLf   =gtLf
        self.isNd   =isNd
        self.isBin  =isBin
        self.isVr   =isVr
        self.gtVrI  =gtVrI
        self.gtExI  =gtExI
        self.gtChr  =gtChr

    def initPob(self,l_vals):
        for index in range(self.pobSiz):
            self.INDIVIDUALS.append(Indiv(self,l_vals))
            if(self.INDIVIDUALS[index].fitness>200000000000000):
                flag=True
                while flag:
                    self.INDIVIDUALS[index]=Indiv(self,l_vals)
                    if(self.INDIVIDUALS[index].fitness>200000000000000):
                        flag=False
    def printPobStatus(self):
        for index in range(self.pobSiz):
            print('{\t',index,'\t',self.INDIVIDUALS[index].fitness,'\t}')
            if(self.INDIVIDUALS[index].fitness<100000):
                nodes.printPosOrd(self.INDIVIDUALS[index],self.isNd,self.isBin,self.gtChr)
            print('\n')
    def tournament(self,ind1,fit1,ind2,fit2):
        if(fit1<fit2):
            return ind1
        else:
            return ind2

    def SELECTION(self):
        pob_fit=[]
        total_fit=0

        for index in range(self.pobSiz):
            pob_fit.append(self.INDIVIDUALS[index].fitness)
        
        if(self.sel_s==ROUL_SEL):
            for index in range(self.pobSiz):
                pob_fit[index]=200000000000000+1-pob_fit[index]
                total_fit+=pob_fit[index]
            for index in range(self.pobSiz):
                pob_fit[index]=pob_fit[index]*self.pobSiz/total_fit
                if(index>0):
                    pob_fit[index]=pob_fit[index]+pob_fit[index-1]
            for ex_index in range(self.pobSiz):
                self.NEWINDDIVDUALS.append(Indiv())
                temp_random = random.random()*self.pobSiz
                for in_index in range(self.pobSiz):
                    self.NEWINDDIVDUALS[ex_index]=self.INDIVIDUALS[in_index]
                    if(pob_fit[in_index]>temp_random and in_index>0):
                        self.NEWINDDIVDUALS[ex_index]=self.INDIVIDUALS[in_index-1]
                        break

        elif(self.sel_s==TOUR_SEL):
            ind1=0
            temp1=0
            ind2=0
            temp2=0
            indexes=[]
            for index in range(self.pobSiz):
                indexes.append(index)
            while indexes.count()>0:
                ind1 = random.randint(0,indexes.count())
                temp1= indexes[ind1]
                indexes.remove(ind1)
                ind2 = random.randint(0,indexes.count())
                temp2= indexes[ind2]
                indexes.remove(ind2)
                self.NEWINDDIVDUALS.append(self.INDIVIDUALS[self.tournament(temp1,self.INDIVIDUALS[temp1].fitness,temp2,self.INDIVIDUALS[temp2].fitness)])
            for index in range(self.pobSiz):
                indexes.append(index)
            while indexes.count()>0:
                ind1 = random.randint(0,indexes.count())
                temp1= indexes[ind1]
                indexes.remove(ind1)
                ind2 = random.randint(0,indexes.count())
                temp2= indexes[ind2]
                indexes.remove(ind2)
                self.NEWINDDIVDUALS.append(self.INDIVIDUALS[self.tournament(temp1,self.INDIVIDUALS[temp1].fitness,temp2,self.INDIVIDUALS[temp2].fitness)])
        self.INDIVIDUALS=self.NEWINDDIVDUALS
        self.NEWINDDIVDUALS=[]
    def CROSSOVER(self,l_vals):
        parent_indexes=[]
        indexes=[]
        subTreeA=None
        subTreeB=None
        IndexA=0
        IndexB=0
        for index in range(self.pobSiz):
            indexes.append(index)
        for index in range(self.pobSiz):
            if(random.random()<=self.crossRt):
                tindex = random.randint(0,indexes.count())
                parent_indexes.append(indexes[tindex])
                indexes.remove(tindex)
        if(parent_indexes.count%2!=0):
            parent_indexes.pop()
        for index in range(int(parent_indexes.count/2)):
            parentA = parent_indexes[index*2]
            parentB = parent_indexes[index*2+1]
            if(self.INDIVIDUALS[parentA].fitness>0 and self.INDIVIDUALS[parentB]>0):
                parentAFit = self.INDIVIDUALS[parentA].fitness
                parentBFit = self.INDIVIDUALS[parentB].fitness
                lowest_fit = 0
                if(parentAFit<parentBFit):
                    lowest_fit=parentAFit
                else:
                    lowest_fit=parentBFit
                countOperandsA = nodes.nodeCount(self.INDIVIDUALS[parentA].chrom,self.isNd,self.isBin)
                countOperandsB = nodes.nodeCount(self.INDIVIDUALS[parentB].chrom,self.isNd,self.isBin)
                if(self.cross_s==HALF_CROSS):
                    IndexA=int(countOperandsA/2)
                    IndexB=int(countOperandsB/2)
                elif(self.cross_s==PROP_CROSS):
                    IndexA=int(countOperandsA*self.crossPrp)
                    IndexB=int(countOperandsB*self.crossPrp)
                elif(self.cross_s==RAND_CROSS):
                    tmp_rand = random.random()
                    IndexA = int(countOperandsA*tmp_rand)
                    IndexB = int(countOperandsB*tmp_rand)
                nodes.getIST(IndexA,self.INDIVIDUALS[parentA].chrom,subTreeA,self.isNd,self.isBin)
                nodes.getIST(IndexB,self.INDIVIDUALS[parentB].chrom,subTreeB,self.isNd,self.isBin)
                nodes.setIST(IndexA,self.INDIVIDUALS[parentA].chrom,subTreeB,self.isNd,self.isBin)
                nodes.setIST(IndexB,self.INDIVIDUALS[parentB].chrom,subTreeA,self.isNd,self.isBin)
                self.INDIVIDUALS[parentA].evalFitness(self,l_vals)
                self.INDIVIDUALS[parentB].evalFitness(self,l_vals)
                if(self.INDIVIDUALS[parentA].fitness>=lowest_fit and self.INDIVIDUALS[parentB].fitness>=lowest_fit):
                    nodes.setIST(IndexA,self.INDIVIDUALS[parentA].chrom,subTreeA,self.isNd,self.isBin)
                    nodes.setIST(IndexB,self.INDIVIDUALS[parentB].chrom,subTreeB,self.isNd,self.isBin)
                    self.INDIVIDUALS[parentA].evalFitness(self,l_vals)
                    self.INDIVIDUALS[parentB].evalFitness(self,l_vals)
    def MUTATION(self,l_vals):
        total_nodes = 0
        N_nodes= 0
        temp_rand1=0
        temp_rand2=0
        last_fitness=0
        temp_node = nodes.Node()
        temp_node_back= nodes.Node()

        for index in range(self.pobSiz):
            temp_random=random.random()
            if(temp_random<=self.mutRt and self.INDIVIDUALS[index].fitness>0):
                total_nodes=nodes.nodeCount( self.INDIVIDUALS[index].chrom, self.isNd, self.isBin)
                N_nodes = nodes.nodeCount(self.INDIVIDUALS[index].chrom,self.isNd,self.isBin)
                if(self.mut_s==BIT_MUT):
                    temp_rand1=random.randint(0,total_nodes-1)
                    nodes.getIN(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isBin)
                    temp_node_back.data=temp_node.data
                    last_fitness=self.INDIVIDUALS[index].fitness
                    if(self.isNd(temp_node.data)):
                        temp_node.data=self.gtOp()
                    else:
                        temp_node.data=self.gtLf()
                    nodes.setIN(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isBin)
                    self.INDIVIDUALS[index].evalFitness(self,l_vals)
                    if(self.INDIVIDUALS[index].fitness>last_fitness):
                        temp_node.data=temp_node_back.data
                        nodes.setIN(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isBin)
                        self.INDIVIDUALS[index].evalFitness(self,l_vals)
                elif(self.mut_s==OPR_MUT):
                    temp_rand1=random.randint(0,N_nodes-1)
                    nodes.getIST(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isNd,self.isBin)
                    temp_node_back.data = temp_node.data
                    last_fitness= self.INDIVIDUALS[index].fitness
                    temp_node.data=self.gtOp()
                    nodes.setIST(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isNd,self.isBin)
                    self.INDIVIDUALS[index].evalFitness(self,l_vals)
                    if(self.INDIVIDUALS[index].fitness>last_fitness):
                        temp_node.data=temp_node_back.data
                        nodes.setIST(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isNd,self.isBin)
                        self.INDIVIDUALS[index].evalFitness(self,l_vals)
                elif(self.mut_s==SUB_MUT):
                    temp_rand1=random.randint(0,N_nodes-1)
                    if(random.randint(0,500)==0):
                        temp_rand2=1
                    else:
                        temp_rand2=0
                    nodes.getIST(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isNd,self.isBin)
                    temp_node_back=temp_node
                    last_fitness=self.INDIVIDUALS[index].fitness
                    temp_node=nodes.newNode(self.fill_s,int(math.log2(nodes.totalNodeCount(temp_node,self.isBin)+temp_rand2)),self.gtOp(),self.isBin,self.gtLf,self.gtOp)
                    nodes.setIST(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isNd,self.isBin)
                    self.INDIVIDUALS[index].evalFitness(self,l_vals)
                    if(self.INDIVIDUALS[index].fitness>last_fitness):
                        temp_node=temp_node_back
                        nodes.setIST(temp_rand1,self.INDIVIDUALS[index].chrom,temp_node,self.isNd,self.isBin)
                        self.INDIVIDUALS[index].evalFitness(self,l_vals)
    def evalPob(self,l_vals):
        for index in range(self.pobSiz):
            self.INDIVIDUALS[index].evalFitness(index,l_vals)
    def KILLWORSTINDIVS(self,l_vals):
        for index in range(self.pobSiz):
            if(self.INDIVIDUALS[index].fitness>200000000000000):
                self.INDIVIDUALS[index]=Indiv(self,l_vals)
    def GENETICPROCESS(self,l_vals):
        advantage = 0
        adv_counter = 0
        temp_indiv = Indiv()
        for index in range(self.maxG):
            advantage=100*(1-float(self.maxG-index/self.maxG))
            if((advantage-adv_counter)>1):
                print('#')
                adv_counter+=1
                if(adv_counter%5==0):
                    print('.')
            self.KILLWORSTINDIVS(l_vals)
            self.SELECTION()
            self.CROSSOVER(l_vals)
            self.MUTATION(l_vals)
            self.evalPob(l_vals)
            temp_indiv = self.INDIVIDUALS[self.getBestIniv()]
            print(index,'\t: fitness : ',temp_indiv.fitness,'\t : count_ops : ',nodes.nodeCount(temp_indiv.chrom,self.isNd,self.isBin))
            if(temp_indiv.fitness<math.pow(10,-50)):
                break
    def getBestIniv(self):
        best_index = 0
        best_indiv = Indiv()
        best_indiv.fitness=math.pow(10,50)
        for index in range(self.pobSiz):
            if(best_indiv.fitness>self.INDIVIDUALS[index].fitness):
                best_index=index
                best_indiv=self.INDIVIDUALS[index]
        return best_index
class Indiv:
    def __init__(self,GenAlg=None,l_vals=None):
        if(l_vals==None or GenAlg==None):
            self.chrom=None
            self.fitness=None
        else:
            self.chrom   = nodes.newNode(GenAlg.fill_s,GenAlg.indSiz,GenAlg.gtOp(),GenAlg.isBin,GenAlg.gtLf,GenAlg.gtOp)
            self.fitness = nodes.cuadraticError(self.chrom,GenAlg.exeBin,GenAlg.exeUn,GenAlg.isNd,GenAlg.isBin,GenAlg.isVr,GenAlg.gtVrI,GenAlg.gtTr,GenAlg.gtExI,l_vals)
    def evalFitness(self,GenAlg=None,l_vals=None):
        self.fitness=nodes.cuadraticError(self.chrom,GenAlg.exeBin,GenAlg.exeUn,GenAlg.isNd,GenAlg.isBin,GenAlg.isVr,GenAlg.gtVrI,GenAlg.gtTr,GenAlg.gtExI,l_vals)

