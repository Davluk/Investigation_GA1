import nodes
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

class Indiv:
    def __init__(self,GenAlg=None,l_vals=None):
        if(l_vals==None or GenAlg==None):
            self.chrom=None
            self.fitness=None
        else:
            self.chrom = nodes.newNode(GenAlg.fill_s,GenAlg.ini)


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
            self.INDIVIDUALS.append()