math=require('math')
tr = require('tree')
inspect=require('inspect')

math.randomseed(os.time())
local mytree = tr.newTree(10)
tr.print_inorder(mytree)
print('\n'..tr.countNodes(mytree))
print('\n result: '..tr.EvalExpression(mytree))