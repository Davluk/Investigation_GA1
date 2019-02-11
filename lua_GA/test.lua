math=require('math')
tr = require('tree2')
inspect=require('inspect')

variables = {'x'}
operators={'+','-','*','/'}
terminals={1,2,3,4,5,6,7,8,9}

math.randomseed(os.time())
tr:initTreeManager(operators,terminals,variables)
local mytree = tr:NewTree(10)
tr:printInOrder(mytree.data)
--print(inspect(mytree.data))
--[[tr.print_inorder(mytree)
print('\n'..tr.countNodes(mytree))
print('\n result: '..tr.EvalExpression(mytree))
]]