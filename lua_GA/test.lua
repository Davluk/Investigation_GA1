math=require('math')
tr = require('tree2')
inspect=require('inspect')

variables = {'x'}
variable_values = {{x=2,y=4}}
operators={'+','-','*','/'}
terminals={1,2,5,7,9}

function add(a,b)return a+b end
function sub(a,b)return a-b end
function mul(a,b)return a*b end
function div(a,b)return a/b end

operator_functions = {}
operator_functions[operators[1]]=add
operator_functions[operators[2]]=sub
operator_functions[operators[3]]=mul
operator_functions[operators[4]]=div

math.randomseed(os.time())
tr:initTreeManager(operators,terminals,variables,operator_functions)
local mytree1 = tr:NewTree(10,variable_values,'y')
local mytree2 = tr:NewTree(10,variable_values,'y')
tr:printPosOrder(mytree1.data)
print()
tr:printPosOrder(mytree2.data)
mytree1.fittness = tr:getCuadraticError(mytree1.data,variable_values,'y')
mytree2.fittness = tr:getCuadraticError(mytree2.data,variable_values,'y')
io.write('\n')
print("evaluation 1: "..tr:EvaluateFunction(mytree1.data,variable_values[1]))
print("evaluaiton 2: "..tr:EvaluateFunction(mytree2.data,variable_values[1]))
print("fit1: "..mytree1.fittness)
print("fit2: "..mytree2.fittness)

op1 = tr:countOperators(mytree1.data)
op2 = tr:countOperators(mytree2.data)
print("operators 1: "..op1)
print("operators 2: "..op2)
op1 = math.ceil( op1/2 )
op2 = math.ceil( op2/2 )
print("op1 half: "..op1)
print("op2 half: "..op2)

tr:setIndexedSubTree(mytree1.data,tr:getIndexedSubTree(mytree2.data,op2),op1)--save the indexed tree on a temporal variable called "tempSubTree"



--print(inspect(mytree.data))
--[[tr.print_inorder(mytree)
print('\n'..tr.countNodes(mytree))
print('\n result: '..tr.EvalExpression(mytree))
]]