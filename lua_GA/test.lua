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

--math.randomseed(os.time())
tr:initTreeManager(operators,terminals,variables,operator_functions)
local mytree = tr:NewTree(10,variable_values,'y')
tr:printPosOrder(mytree.data)
mytree.fittness = tr:getCuadraticError(mytree.data,variable_values,'y')
io.write('\n')
print(tr:EvaluateFunction(mytree.data,variable_values[1]))
print(mytree.fittness)

op = tr:countOperators(mytree.data)
print(op)
op = math.ceil( op/2 )
print(op)

mytree2 = tr:returnIndexedSubTree(mytree.data,op)
--tr:printInOrder(mytree2.data)

--print(inspect(mytree.data))
--[[tr.print_inorder(mytree)
print('\n'..tr.countNodes(mytree))
print('\n result: '..tr.EvalExpression(mytree))
]]