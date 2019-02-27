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
mytree1 = tr:NewTree(6,variable_values,'y')
mytree2 = tr:NewTree(6,variable_values,'y')

tr:printPosOrderIndex(mytree1)
print()
tr:printPosOrderIndex(mytree2)
print()

op1 = tr:countOperatorsIndex(mytree1)
op2 = tr:countOperatorsIndex(mytree2)

print("operators 1: "..op1)
print("operators 2: "..op2)

op1 = math.ceil( op1/2 )
op2 = math.ceil( op2/2 )

print("op1 half: "..op1)
print("op2 half: "..op2)

print("\nsubtree1_selected: ")
tr:getIndexedSubTreeIndex(mytree1,op1,1)
tr:printPosOrder(tr.tempSubTree1)
print(tr.tempSubTree1)

print("\nsubtree2_selected: ")
tr:getIndexedSubTreeIndex(mytree2,op2,2)
tr:printPosOrder(tr.tempSubTree2)
print(tr.tempSubTree2)

tr.tempSubTree1 = tr.tempSubTree2
print()
tr:printPosOrder(tr.tempSubTree1)
print()
tr:printPosOrderIndex(mytree1)
print()
--[[
print("\nnew_individual: ")
tr:setIndexedSubTree(mytree1.data,tr.tempSubTree,op1)--save the indexed tree on a temporal variable called "tempSubTree"
tr:printPosOrder(mytree1.data)
print()
--[[
]]



--print(inspect(mytree.data))
--[[tr.print_inorder(mytree)
print('\n'..tr.countNodes(mytree))
print('\n result: '..tr.EvalExpression(mytree))
]]