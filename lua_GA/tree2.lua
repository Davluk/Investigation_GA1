local treeManager = {}

--verified
function treeManager:initTreeManager(operators,terminals,variables,operator_functions)
    self.op = operators
    self.tr = terminals
    self.vr = variables
    self.functions = operator_functions
end


--verified
function treeManager:NewTree(maxdepth,listOfValues,fx)
     if (self.trees==nil) then self.trees = {} end
    local expr = {type='op',data=self.op[math.random( #self.op )]}
    self.trees[#self.trees+1]={}
    self.trees[#self.trees].data = self:newNode(expr,self:GenRandomNode(),self:GenRandomNode(),maxdepth)
    self.trees[#self.trees].fitness = self:getCuadraticErrorIndex(#self.trees,listOfValues,fx)
    return #self.trees
end

--???
function treeManager:NewSubTree(maxdepth,listOfValues,fx)
    local expr = {type='op',data=self.op[math.random( #self.op )]}
    return self:newNode(expr,self:GenRandomNode(),self:GenRandomNode(),maxdepth)
end


--verified
function treeManager:GenRandomNode()
    local coin = math.random( 10 )
    if coin>5 then
        return {type='op',data=self.op[math.random( #self.op )]}
    else 
        coin = math.random( 10 )
        if coin>5 then
            return {type='tr',data=self.tr[math.random( #self.tr )]}
        else
            return {type='vr',data=self.vr[math.random( #self.vr )]}
        end 
    end
end

--vefified
function treeManager:newNode(expr,left,rigth,maxdepth)
    local node = {}
    node.expr = expr
    if(maxdepth==2)then 
        local coin = math.random( 4 )
        if(coin >2)then node.left  = {type = 'tr',data = self.tr[math.random( #self.tr )]}
        else             node.left  = {type = 'vr',data = self.vr[math.random( #self.vr )]}
        end
        coin = math.random( 4 )
        if(coin >2)then node.rigth = {type = 'tr',data = self.tr[math.random( #self.tr )]}
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


--verified
function treeManager:printInOrderIndex(index) self:printInOrder(self.trees[index].data)end
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


--verified
function treeManager:printPosOrderIndex(index) self:printPosOrder(self.trees[index].data) print() print("fitness["..index.."]: "..self.trees[index].fitness) end
function treeManager:printPosOrder(tree)
    io.write("("..tree.expr.data.." ")
    if(tree.left.type ~= nil)then 
        io.write(tree.left.data.." ") 
    else 
        self:printPosOrder(tree.left)
    end

    if(tree.rigth.type ~= nil)
    then 
        io.write(tree.rigth.data.." ") 
    else 
        self:printPosOrder(tree.rigth)
    end
    io.write(")")
end

function treeManager:getCuadraticErrorIndex(index,listOfValues,fx) return self:getCuadraticError(self.trees[index],listOfValues,fx)end
function treeManager:getCuadraticError(tree,listOfValues,fx)
    local error=0
    for index = 1,#listOfValues,1 do
        error = error + math.pow(listOfValues[index][fx]- treeManager:EvaluateFunction(self.trees[index].data,listOfValues[index]),2)
    end
    return error/#listOfValues
end

--verified
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


--verified
function treeManager:countOperatorsIndex(index)return self:countOperators(self.trees[index].data)end
function treeManager:countOperators(tree)
    local counter = 1
    if(tree.left.type ==nil)then counter = counter + treeManager:countOperators(tree.left) end
    if(tree.rigth.type==nil)then counter = counter + treeManager:countOperators(tree.rigth)end
    return counter
end


--verified
function treeManager:getIndexedSubTreeIndex(index,number,selection)self:getIndexedSubTree(self.trees[index].data,number,selection)end
function treeManager:getIndexedSubTree(tree,number,selection)
    local deep = number
    if(number>=0)then 
        self["tempSubTree"..selection]=nil 
        self["tempSubTree"..selection]=tree 
        if(tree.left==nil and tree.rigth==nil)then return number end    
    else
        if(number>=1)then     
            if(tree.left~=nil and tree.left.type==nil)then deep = treeManager:getIndexedSubTree(tree.left,deep-1,selection) end
            if(tree.rigth~=nil and tree.rigth.type==nil and deep>=1)then deep = treeManager:getIndexedSubTree(tree.rigth,deep-1,selection) end
        end     
    end
    return deep
end

--[[
function treeManager:setIndexedSubTree(tree,treeSubstitute,number)
    local deep = number
    --print("set - expr: "..tree.expr.data.."| number: "..number)
    if(number>=1)then
        if(tree.left~=nil and tree.left.type~=nil and tree.rigth~=nil and tree.rigth.type~=nil)then
            return number
        end
        if(tree.left~=nil and tree.left.type==nil)then
            deep = treeManager:setIndexedSubTree(tree.left,treeSubstitute,number-1)
        end
        if(tree.rigth~=nil and tree.rigth.type==nil and deep>0)then
            deep = treeManager:setIndexedSubTree(tree.rigth,treeSubstitute,deep-1)
        end
    else
        print(tree)
        print(treeSubstitute)
        local _subtree = table.unpack(treeSubstitute)
        tree=_subtree
    end
    return deep
end
]]


return treeManager