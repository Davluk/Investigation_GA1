local treeManager = {}

function treeManager:initTreeManager(operators,terminals,variables,operator_functions)
    self.op = operators
    self.tr = terminals
    self.vr = variables
    self.functions = operator_functions
end

function treeManager:NewTree(maxdepth,listOfValues,fx)
    local tree = {}
    local expr = {type='op',data=self.op[math.random( #self.op )]}
    tree.data = self:newNode(expr,self:GenRandomNode(),self:GenRandomNode(),maxdepth)
    return tree
end



function treeManager:GenRandomNode()
    local coin = math.random( 9 )
    if coin>6 then
        return {type='op',data=self.op[math.random( #self.op )]}
    else 
        if coin>3 then
            return {type='tr',data=self.tr[math.random( #self.tr )]}
        else
            return {type='vr',data=self.vr[math.random( #self.vr )]}
        end 
    end
end

function treeManager:newNode(expr,left,rigth,maxdepth)
    local node = {}
    node.expr = expr
    if(maxdepth==2)then 
        local coin = math.random( 4 )
        if(coin >=2)then node.left  = {type = 'tr',data = self.tr[math.random( #self.tr )]}
        else             node.left  = {type = 'vr',data = self.vr[math.random( #self.vr )]}
        end
        coin = math.random( 4 )
        if(coin >=2)then node.rigth = {type = 'tr',data = self.tr[math.random( #self.tr )]}
        else             node.rigth = {type = 'vr',data = self.vr[math.random( #self.vr )]}   
        end
    else
        if(left.type  == 'op')then node.left  = self:newNode(left, self:GenRandomNode(),self:GenRandomNode(),maxdepth-1)
        else node.left = left end
        if(rigth.type == 'op')then node.rigth = self:newNode(rigth,self:GenRandomNode(),self:GenRandomNode(),maxdepth-1)
        else node.rigth = rigth end
    end
    return node
end

function treeManager:printInOrder(tree)
    io.write('( ')
    if(tree.left.type ~= nil)then 
        io.write(tree.left.data) 
    else 
        self:printInOrder(tree.left)
    end
    io.write(tree.expr.data)
    if(tree.rigth.type ~= nil)
    then 
        io.write(tree.rigth.data) 
    else 
        self:printInOrder(tree.rigth)
    end
    io.write(' )')
end

function treeManager:EvaluateFunction(tree,currentVarValues)
    local left_operand 
    local rigth_operand 
    if(tree.left.type~=nil)then
        if(tree.left.type=='tr')then
            left_operand = tree.left.data        
        else
            left_operand = currentVarValues[tree.left.data]
        end
    else
        left_operand = treeManager:EvaluateFunction(tree.left,currentVarValues)
    end
    if(tree.rigth.type~=nil)then
        if(tree.rigth.type=='tr')then
            rigth_operand = tree.rigth.data        
        else
            rigth_operand = currentVarValues[tree.rigth.data]
        end
    else
        rigth_operand = treeManager:EvaluateFunction(tree.rigth,currentVarValues)
    end
    return self.functions[tree.expr.data](left_operand,rigth_operand)
end

function treeManager:countOperators(tree)
    local counter = 1
    if(tree.left.type ==nil)then counter = counter + treeManager:countOperators(tree.left) end
    if(tree.rigth.type==nil)then counter = counter + treeManager:countOperators(tree.rigth)end
    return counter
end

function treeManager:returnIndexedSubTree(tree,number)
    local counter = number
    local tempNumber = 0
    local indexedTree = nil
    if(number == 0)then return 1,tree end
    if(tree.left.type ==nil)then 
        tempNumber,indexedTree = treeManager:returnIndexedSubTree(tree.left,counter-1)
        counter = counter - tempNumber
    end
    if(tree.rigth.type==nil and counter > 0)then 
        tempNumber,indexedTree = treeManager:returnIndexedSubTree(tree.rigth,counter-1)
        counter = counter - tempNumber
    end
    return counter,indexedTree
end


function treeManager:getCuadraticError(tree,listOfValues,fx)
    local error=0
    for index = 1,#listOfValues,1 do
        error = error + math.pow(listOfValues[index][fx]- treeManager:EvaluateFunction(tree,listOfValues[index]),2)
    end
    return error/#listOfValues
end

return treeManager