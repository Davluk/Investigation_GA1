local treeManager = {}

function treeManager:initTreeManager(operators,terminals,variables)
    self.op = operators
    self.tr = terminals
    self.vr = variables
end

function treeManager:NewTree(maxdepth)
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

return treeManager