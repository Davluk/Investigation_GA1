NManager = require('node_manager')
math = require("math")

GPManager = {}--Genetic programming manager
GPManager.const = {}
GPManager.const.HALF_CROSS=0
GPManager.const.PROP_CROSS=1
GPManager.const.RAND_CROSS=2

GPManager.const.BIT_MUT=0
GPManager.const.OPER_MUT=1
GPManager.const.SUBT_MUT=2
GPManager.const.NO_MUT=3

GPManager.const.TOURN_SEL=0
GPManager.const.ROULT_SEL=1

function GPManager:newGPAlg( _fs , _ms , _cs , _ss , _pobs , _inds , _maxg , _mutrt , _crsrt , _crsprp , 
                            f_evbe , f_evue  , f_gtr , f_gop , f_glf , f_ind , f_ivr , f_ibo , f_gvi , f_gei, f_gchr )
    self = {}
    self.INDIVIDUALS={}
    self.max_gen = _maxg
    self.f_select = _fs
    self.m_select = _ms
    self.c_select = _cs
    self.s_select = _ss
    self.pob_size = _pobs
    self.ind_size = _inds
    self.mut_rate = _mutrt
    self.crs_rate = _crsrt
    self.crs_prop = _crsprp
    endself.EVBE = f_evbe
    endself.EVUE = f_evue
    self.GTR = f_gtr
    self.GOP = f_gop
    self.GLF = f_gop
    self.IND = f_ind
    self.IVR = f_ivr
    self.IBO = f_ibo
    self.GVI = f_gvi
    self.GEI = f_gei
    self.GCHR = f_gchr
end

function GPManager:initPobRec( l_vals , counter )
    if(counter==self.pob_size)then 
        return 
    else 
        self.INDIVIDUALS[counter] = GPManager:newIndiv(l_vals)
        GPManager:initPobRec( l_vals , counter + 1 )
    end
end

function GPManager:newIndiv( l_vals )
    local Indiv = {}
    Indiv.chrom = newTree( self.f_select , self.ind_size , self.GOP() , self.IBO , self.GLF , self.GOP )
    Indiv.fitness = NManager:cuadraticError( Indiv.chrom , self.EVBE, self.EVUE , self.IND , self.IVR , self.IBO , self.GVI , self.GTR , self.GEI , l_vals )
    return Indiv
end

function GPManager:evalFunc( indiv , l_vals )
    indiv.fitness = NManager:cuadraticError( Indiv.chrom , self.EVBE, self.EVUE , self.IND , self.IVR , self.GVI , self.GTR , self.GEI , l_vals )
end

function GPManager:printPobStatus()
    for index = 1, self.pob_size , 1 do
        print( "{\t"..index.."\t,\t"..self.INDIVIDUALS[index].fitness.."\t}" )
        if( self.INDIVIDUALS[index].fitness<10*self.pob_size )then
            NManager:printPosOrder( self.INDIVIDUALS[INDEX].chrom , self.IND  , self.IBO , self.GCHR )
            print("\n")
        end
    end
end

function GPManager:printNewPobStatus()
    for index = 1, self.pob_size , 1 do
        print( "{\t"..index.."\t,\t"..self.NEWINDIVIDUALS[index].fitness.."\t}" )
        if( self.NEWINDIVIDUALS[index].fitness<10*self.pob_size )then
            NManager:printPosOrder( self.NEWINDIVIDUALS[INDEX].chrom , self.IND , self.IBO , self.GCHR )
            print("\n")
        end
    end
end

function GPManager:tournament( ind1 , fit1 , ind2 , fit2 )
    if(fit1<fit2)then return ind1 else return ind2 end
end

function GPManager:SELECTION()
    local case={}
    case[GPManager.const.ROULT_SEL]=function()
        local total_fit = 0.0
        local max_fit = 0.0
        local pob_fit = {}
        local NEWINDIVIDUALS = {}
        for index = 1 , self.pob_size , 1 do pob_fit[index] = self.INDIVIDUALS[index].fitness end
        for index = 1 , self.pob_size , 1 do if( pob_fit[index]>max_fit )then  max_fit = pob_fit[index] end end
        for index = 1 , self.pob_size , 1 do pob_fit[index] = max_fit + 1 - pob_fit[index] ; total_fit = total_fit + pob_fit[index] end 
        for index = 1 , self.pob_size , 1 do pob_fit[index] = pob_fit[index]*self.pob_size/total_fit ; if( index > 1 )then pob_fit[index] = pob_fit[index]+pob_fit[index-1] end end
        for extindex = 1 , self.pob_size , 1 do 
            local temp_random = math.random()*self.pob_size
            for index = 1 , self.pob_size , 1 do
                NEWINDIVIDUALS[extindex] = self.INDIVIDUALS[index]
                if( pob_fit[index]> temp_random and index > 1 )then NEWINDIVIDUALS[extindex]= self.INDIVIDUALS[index-1] break end
            end
        end
        self.INDIVIDUALS = NEWINDIVIDUALS 
    end
    case[GPManager.const.TOURN_SEL]=function()
        ind_1 = 0
        ind_2 = 0
        counter = 0
        indexes = {}
        local NEWINDIVIDUALS = {}
        for index = 1 , self.pob_size , 1 do indexes[#indexes+1]=index end
        while( #indexes>0 )do
            ind_1 = math.random(1,#indexes); temp_ind1 = table.remove( indexes, ind_1 )
            ind_2 = math.random(1,#indexes); temp_ind2 = table.remove( indexes, ind_2 )
            NEWINDIVIDUALS[#NEWINDIVIDUALS+1]=self.INDIVIDUALS[tournament( temp_ind1 , self.INDIVIDUALS[temp_ind1].fitness , temp_ind2 , self.INDIVIDUALS[temp_ind2].fitness )]
        end
        for index = 1 , self.pob_size , 1 do indexes[#indexes+1]=index end
        while( #indexes>0 )do
            ind_1 = math.random(1,#indexes); temp_ind1 = table.remove( indexes, ind_1 )
            ind_2 = math.random(1,#indexes); temp_ind2 = table.remove( indexes, ind_2 )
            NEWINDIVIDUALS[#NEWINDIVIDUALS+1]=self.INDIVIDUALS[tournament( temp_ind1 , self.INDIVIDUALS[temp_ind1].fitness , temp_ind2 , self.INDIVIDUALS[temp_ind2].fitness )]
        end
        self.INDIVIDUALS = NEWINDIVIDUALS
    end
    case[self.s_select]()
end

function GPManager:CROSSOVER(l_vals)

    local parent_indexes = {}
    local temp_indexes = {}
    local indexA = 0
    local indexB = 0
    local countOperandsA = 0
    local countOperandsB = 0
    for index = 1, self.pob_size , 1 do temp_indexes[#temp_indexes+1] = index end
    for index = 1, self.pob_size , 1 do if(temp_random<=self.crs_rate)then parent_indexes[#parent_indexes+1]=table.remove( temp_indexes, index ) end end
    if(#parent_indexes%2 ~= 0)then parent_indexes[#parent_indexes+1]=table.remove( temp_indexes, math.random(1,#temp_indexes) ) end
    for index = 1, self.pob_size, 2 do
        
        local subtA = {}
        local subtB = {}
        local parent_A = parent_indexes[index]
        local parent_B = parent_indexes[index+1]
        if(self.INDIVIDUALS[parent_A].fitness>100000*self.pob_size and self.INDIVIDUALS[parent_B].fitness>100000*self.pob_size)then
        
            local parent_a_fit = self.INDIVIDUALS[parent_A].fittness
            local parent_b_fit = self.INDIVIDUALS[parent_B].fittness
            local countOperandsA = NManager:nodeOpCounter( self.INDIVIDUALS[parent_A].chrom , self.IND  , self.IBO )
            local countOperandsB = NManager:nodeOpCounter( self.INDIVIDUALS[parent_B].chrom , self.IND  , self.IBO )
            local case = {} 
            case[GPManager.const.HALF_CROSS]=function()
                indexA = math.floor( countOperandsA/2 ) ; indexB = math.floor( countOperandsB/2 )
            end
            case[GPManager.const.PROP_CROSS]=function()
                indexA = math.floor( countOperandsA*self.crs_prop ) ; indexB = math.floor( countOperandsB*self.crs_prop )
            end
            case[GPManager.const.RAND_CROSS]=function()
                local temp_rand = math.random()
                indexA = math.floor( countOperandsA*temp_rand ) ; indexB = math.floor( countOperandsB*temp_rand ) 
            end
            case[self.s_select]()
            NManager:getIndexedSubTree( indexA , self.INDIVIDUALS[parent_A].chrom , subtA , self.IND , self.IBO )
            NManager:getIndexedSubTree( indexB , self.INDIVIDUALS[parent_B].chrom , subtB , self.IND , self.IBO )

            NManager:setIndexedSubTree( indexA , self.INDIVIDUALS[parent_A].chrom , subtB , self.IND , self.IBO )
            NManager:setIndexedSubTree( indexB , self.INDIVIDUALS[parent_B].chrom , subtA , self.IND , self.IBO )
            GPManager:evalFunc( self.INDIVIDUALS[parent_A], l_vals )
            GPManager:evalFunc( self.INDIVIDUALS[parent_B], l_vals )
            if( self.INDIVIDUALS[parent_A].fitness>parent_a_fit and self.INDIVIDUALS[parent_B].fitness>parent_b_fit )then
                NManager:setIndexedSubTree( indexA , self.INDIVIDUALS[parent_A].chrom , subtA , self.IND , self.IBO )
                NManager:setIndexedSubTree( indexB , self.INDIVIDUALS[parent_B].chrom , subtB , self.IND , self.IBO )
            end
        end
    end
end

function GPManager:MUTATION()
    local total_nodes = 0
    local subt_total_nodes = 0
    local nodes = 0
    local temp_rand = 0
    local temp_node = {}
    for index = 1 , self.pob_size , 1 do 
        local temp_random = math.random()
        if( temp_random<= self.mut_rate and self.INDIVIDUALS[index].fitness>10*self.pob_size )then 
            total_nodes = NManager:nodeOpCounter( self.INDIVIDUALS[index].chrom , self.IND , self.IBO )
            nodes       = NManager:nodeAllCounter( self.INDIVIDUALS[index].chrom , self.IND , self.IBO )
            case = {}
            case[GPManager.const.BIT_MUT]=function()
                local temp_rand = math.random( 1 , total_nodes )
                NManager:getIndexedNode( temp_rand , self.INDIVIDUALS[index].chrom , temp_node , self.IND , self.IBO )
                if( self.IND(temp_node.data) )then
                    temp_node.data = self.GOP()
                else 
                    temp_node.data = self.GLF()
                end
                NManager:setIndexedNode( temp_rand , self.INDIVIDUALS[index].chrom , temp_node )
            end

            case[GPManager.const.OPER_MUT]=function()
                local temp_rand = math.random( 1 , nodes )
                NManager:getIndexedSubTree( temp_rand , self.INDIVIDUALS[index].chrom , temp_node , self.IND , self.IBO )
                temp_node.data = self.GOP()
                NManager:setIndexedSubTree( temp_rand , self.INDIVIDUALS[index].chrom , temp_node , self.IND , self.IBO )
            end

            case[GPManager.const.SUBT_MUT]=function()
                local temp_rand = math.random( 1 , nodes )
                NManager:getIndexedSubTree( temp_rand , self.INDIVIDUALS[index].chrom , temp_node , self.IND , self.IBO)
                temp_node = newTree( self.f_select , math.floor( math.log( NManager:nodeAllCounter(temp_node , self.IND , self.IBO ) ,2 ) ) , self.GOP() , self.GLF , self.GOP )
                NManager:setIndexedSubTree( temp_rand , self.INDIVIDUALS[index].chrom , temp_node , self.IND , self.IBO)
            end

            case[GPManager.const.NO_MUT]=function() end

            case[GPManager.m_select]()
        end
    end
end

function GPManager:evalPoblation( l_vals )
    for index = 1, self.pob_size , 1 do
        GPManager:evalFunc( self.INDIVIDUALS[index] , l_vals )
    end
end

function GPManager:KILLINDIVS( l_vals )
    for index = 1 , self.pob_size , 1 do 
        if( self.INDIVIDUALS[index].fitness > 100000000*self.pob_size )then
            self.INDIVIDUALS[index] = GPManager:newIndiv( l_vals )
        end
    end
end

function GPManager:GENETICPROSSES( l_vals )
    for index = 1, self.max_gen , 1 do
        GPManager:KILLINDIVS( l_vals )
        GPManager:SELECTION()
        GPManager:CROSSOVER( l_vals )
        GPManager:MUTATION()
        GPManager:evalPoblation( l_vals )
    end
end

function GPManager:printBestIndiv()
    local best_indiv = {}
    best_indiv.fitness = 1/0.0000000000000000000000000000000000001
    for index = 1 , self.pob_size , 1 do 
        if( best_indiv.fitness > self.INDIVIDUALS[index].fitness )then best_indiv = self.INDIVIDUALS[index] end
    end
    print( "fitness: "..best_indiv.fitness.."\n" ); NManager:printPosOrder( best_indiv.chrom , self.IND , self.IBO , self.GCHR )
end

return GPManager