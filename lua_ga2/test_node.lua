math = require('math')
NM = require('node_manager')
inspect = require('inspect')

const={}
const.maxg = 100
const.pops = 100
const.inds= 3

const.m_rate = 0.5
const.c_rate = 0.9
const.c_prop = 0.7

const.f_select = NM.const.FULL_FILL
--const.m_select = GPM.const.BIT_MUT
--const.c_select = GPM.const.RAND_CROSS
--const.s_select = GPM.const.TOURN_SEL                                                   

local node_types = {}
node_types.Var = 0
node_types.Bop  = 1
node_types.Uop  = 2
node_types.Ter1 = 3
node_types.Ter2 = 4

local values = {}
function f(x) return x*x end

local terRep1 = { '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' }
local terRep2 = { 'sq(2)' , 'sq(3)', 'e' , 'PI'}
local BOpRep = { '+' , '-' , '*' , '/' , '^' }
local UOpRep = { 'Cos', 'Sin' , 'Exp' , 'Ln' }
local varRep = { 'x' , 'y' } 

function GLF()
    local temp_md = {}
    if(  math.random( 1 , 4 )>2 )then
        temp_md.data = math.random( 1, #terRep1 ) ; temp_md.type = node_types.Ter1
        --if( math.random( 1 , 4 )>2 )then 
        --    temp_md.data = math.random( 1, #terRep1 ) ; temp_md.type = node_types.Ter1
        --else
        --    temp_md.data = math.random( 1, #terRep2 ) ; temp_md.type = node_types.Ter2       
        --end
    else
        temp_md.data = math.random( 1, #varRep-1 ) ; temp_md.type = node_types.Var
    end
    return temp_md
end

function GOP()
    local temp_md = {}
    temp_md.data = math.random( 1 , #BOpRep ) ; temp_md.type = node_types.Bop  
    --if(math.random( 1 , 4 )>2)then 
    --   temp_md.data = math.random( 1 , #BOpRep ) ; temp_md.type = node_types.Bop        
    --else
    --    temp_md.data = math.random( 1 , #UOpRep ) ; temp_md.type = node_types.Uop
    --end
    return temp_md
end

function GCHR( m_data )
    local case={}
    case[node_types.Bop] =function() return BOpRep[m_data.data] end
    case[node_types.Uop] =function() return UOpRep[m_data.data] end
    case[node_types.Ter1]=function() return terRep1[m_data.data] end
    case[node_types.Ter2]=function() return terRep2[m_data.data] end
    case[node_types.Var] =function() return varRep[m_data.data] end
    return case[ m_data.type ]()
end

function IND( m_data ) return (m_data.type == node_types.Bop or m_data.type==node_types.Uop)  end
function IVR( m_data ) return m_data.type == node_types.Var end
function IBO( m_data ) return m_data.type == node_types.Bop end-- es una operaicon binaria

function GVI( m_data ) return m_data.data end
function GEI( m_data ) return m_data.data end
function GTR( m_data ) 
    if(m_data.type==node_types.Ter1)then
        return m_data.data
    else
        if(m_data.data==1)then return math.sqrt( 2 ) end
        if(m_data.data==2)then return math.sqrt( 3 ) end
        if(m_data.data==3)then return math.exp( 1 ) end
        if(m_data.data==4)then return math.pi end
    end
end

--it's not really nesesary yet
function INTERPRETER( m_data ) return m_data.data end

function ADD( a , b ) return a + b end
function SUB( a , b ) return a - b end
function MUL( a , b ) return a * b end
function DIV( a , b ) if(b==0)then return a/0.00000000000000001 else return a / b end end
function POW( a , b ) return math.pow( a , b ) end
function SIN( a ) return math.sin( a ) end 
function COS( a ) return math.cos( a ) end 
function EXP( a ) return math.exp( a ) end 
function LN( a ) return math.log( a , math.exp( 1 ) ) end

function EVBEX( selection , a , b )
    local case = {}
    case[ 1 ] = function () return ADD(a,b) end
    case[ 2 ] = function () return SUB(a,b) end
    case[ 3 ] = function () return MUL(a,b) end
    case[ 4 ] = function () return DIV(a,b) end
    case[ 5 ] = function () return POW(a,b) end
    return case[selection]()
end

function EVUEX( selection , a )
    local case = {}
    case[ 1 ] = function () return COS(a) end
    case[ 2 ] = function () return SIN(a) end
    case[ 3 ] = function () return EXP(a) end
    case[ 4 ] = function () return LN(a) end
    return case[selection]()
end

function main()
    math.randomseed( os.time() )
    for index = 1 , 20 , 1 do
        values[index]={}
        values[index][1] = 100*index / 7
        values[index][2] = f( values[index][1] )
    end
    local mynode1 = NM:newTree(const.f_select,const.inds,GOP(),IBO,GLF,GOP)
                    NM:pushTree(const.f_select,const.inds,GOP(),IBO,GLF,GOP)
     subtree1= {}
    local mynode2 = NM:newTree(const.f_select,const.inds,GOP(),IBO,GLF,GOP)
                    NM:pushTree(const.f_select,const.inds,GOP(),IBO,GLF,GOP)
     subtree2= {}

    local nodeCount1 = 0
    local nodeCount2 = 0
    local H_nodes1 = 0
    local H_nodes2 = 0

    --print(inspect(mynode1,{newline='\n'}))
    --print(inspect(mynode2,{newline='\n'}))
    
    print("INIT PROBE")
    NM:printPosOrder(NM.Indivs[1],IND,IBO,GCHR)
    print("\n")
    NM:printPosOrder(NM.Indivs[2],IND,IBO,GCHR)
    print("\n")

    print("evaluation_result1: "..NM:evalFunc(NM.Indivs[1],EVBEX,EVUEX,IND,IVR,IBO,GVI,GTR,GEI,values[1]))
    print("cuadraticError1: "..NM:cuadraticError(NM.Indivs[1],EVBEX,EVUEX,IND,IVR,IBO,GVI,GTR,GEI,values))

    print("evaluation_result2: "..NM:evalFunc(NM.Indivs[2],EVBEX,EVUEX,IND,IVR,IBO,GVI,GTR,GEI,values[1]))
    print("cuadraticError2: "..NM:cuadraticError(NM.Indivs[2],EVBEX,EVUEX,IND,IVR,IBO,GVI,GTR,GEI,values))

    nodeCount1 = NM:nodeOpCounter(NM.Indivs[1],IND,IBO)
    nodeCount2 = NM:nodeOpCounter(NM.Indivs[2],IND,IBO)

    H_nodes1 = math.ceil( nodeCount1/2 )
    H_nodes2 = math.ceil( nodeCount2/2 )

    print( "number of nodes 1: "..nodeCount1.." half_foor: "..H_nodes1)
    print( "number of nodes 2: "..nodeCount2.." half_foor: "..H_nodes2)

    print( "selected subtree1:")
    NM:getIndexedSubTreeIt( H_nodes1 , 1 , IND , IBO )
    --NM:printPosOrder( subtree1 , IND , IBO , GCHR )
    print(inspect(NM.subT[1]))
    print( "\n" )
    print( "selected subtree2:")
    NM:getIndexedSubTreeIt( H_nodes2 , 2 , IND , IBO )
    --NM:printPosOrder( subtree2 , IND , IBO , GCHR )
    print(inspect(NM.subT[2]))
    print("\n")

    --print("substitution")
    --NM:setIndexedSubTree( H_nodes1 , mynode1 , subtree2 , IND , IBO )
    --NM:setIndexedSubTree( H_nodes2 , mynode2 , subtree1 , IND , IBO )

    --NM:printPosOrder( mynode1 , IND , IBO , GCHR )
    --print("\n"){}
    --NM:printPosOrder( mynode2 , IND , IBO , GCHR )
end

main()