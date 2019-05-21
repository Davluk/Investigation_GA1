math = require('math')
NM = require('node_manager')
GPM = require('GeneticProgramming')

const={}
const.maxg = 100
const.pops = 100
const.inds= 3

const.m_rate = 0.5
const.c_rate = 0.9
const.c_prop = 0.7

const.f_select = NM.const.RAND_FILL
const.m_select = GPM.const.BIT_MUT
const.c_select = GPM.const.RAND_CROSS
const.s_select = GPM.const.TOURN_SEL

local node_types = {}
node_types.var = 0
node_types.op  = 1
node_types.term = 2

local values = {}
function f(x) return 9*x*x*x + 2*x*x + x + 4 end

local terRep = { '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' }
local opRep  = { '+' , '-' , '*' , '/' }
local varRep = { 'x' , 'y' } 

function GLF()
    local temp_md
    local temp_random = math.random( 1 , 4 )
    if( temp_random>2 )then
        temp_md.data = math.random( 1, #terRep ) ; temp_md.type = node_types.term
    else
        temp_md.data = math.random( 1, #varRep ) ; temp_md.type = node_types.var
    end
    return temp_md
end

function GOP()
    local temp_md = {}
    temp_md.data = math.random( 1 , #varRep ) ; temp_md.type = node_types.op
    return temp_md
end

function GCHR( m_data )
    local case={}
    case[node_types.op]  =function() return  opRep[m_data.data] end
    case[node_types.term]=function() return terRep[m_data.data] end
    case[node_types.var] =function() return varRep[m_data.data] end
    return case[ m_data.type ]()
end

function IND( m_data ) return m_data.type == node_types.op  end
function IVR( m_data ) return m_data.type == node_types.var end

function GVI( m_data ) return m_data.data end
function GEI( m_data ) return m_data.data end
function GTR( m_data ) return m_data.data end

function INTERPRETER( m_data ) return m_data.data end

function ADD( a , b ) return a + b end
function SUB( a , b ) return a - b end
function MUL( a , b ) return a * b end
function DIV( a , b ) if(b==0)then return a/0.00000000000000001 else return a / b end end

function EVEX( selection , a , b )
    local case = {}
    case[ 1 ]  = function () return ADD(a,b) end
    case[ 2 ]  = function () return SUB(a,b) end
    case[ 3 ]  = function () return MUL(a,b) end
    case[ 4 ]  = function () return DIV(a,b) end
    return case[selection]()
end

--[[function main()
    math.randomseed( os.time() )
    for index = 1 , 20 , 1 do
        values[index]={}
        values[index][0] = index / 1
        values[index][1] = f( values[index][0] )
    end
    local myGenAlg = GPM = GPM:newGPAlg( const.f_select , const.m_select , const.c_select , const.s_select , const.pops , const.inds , const.maxg , const.m_rate , const.c_rate , const.c_prop,
                                     EVEX, GTR , GOP , GLF , IND , IVR , GEI , GCHR )
    print("INIT POPULATION\n\n")
    GPM:initPobRec( values )
    GPM:printBestIndiv()
    GPM:GENETICPROSSES( values )
    GPM:printBestIndiv()
    print("hola mundo")
end]]

