math = require('math')
inspect = require('inspect')
nodeManager={}

nodeManager.const={}
nodeManager.const.FULL_FILL=1
nodeManager.const.RAND_FILL=2
nodeManager.const.HALF_FILL=3

--[[
    #####################################################################################################
    #####################################################################################################
    #####################################################################################################
]]

function nodeManager:pushTree(fill_select,depth,_data,f_isBinaryOp,f_getLeaf,f_getOperand)
    if(self.INDIVS==nil)then
        self.INDIVS={}
        self.INDIVS[#self.INDIVS+1] = nodeManager:newTree(fill_select,depth,_data,f_isBinaryOp,f_getLeaf,f_getOperand)
    else 
        self.INDIVS[#self.INDIVS+1] = nodeManager:newTree(fill_select,depth,_data,f_isBinaryOp,f_getLeaf,f_getOperand)        
    end
end

function nodeManager:newTree(fill_select,depth,_data,f_isBinaryOp,f_getLeaf,f_getOperand)
    local T_node = {}
    T_node.data = _data
    if(depth==1)then
        if(f_isBinaryOp(_data))then
            T_node.Left     = nodeManager:newNode( f_getLeaf() )
            T_node.Right    = nodeManager:newNode( f_getLeaf() )
            T_node.Unique   = nil
        else
            T_node.Unique   = nodeManager:newNode( f_getLeaf() )
            T_node.Left     = nil
            T_node.Right    = nil
        end
    else
        case ={}
        case[nodeManager.const.FULL_FILL] = function()
            --print("FULL_FILL")
            if(f_isBinaryOp(T_node.data))then
                T_node.Left      = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                T_node.Right     = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                T_node.Unique    = nil
            else
                T_node.Left      = nil
                T_node.Right     = nil
                T_node.Unique    = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
            end
        end
        case[nodeManager.const.RAND_FILL] = function()
            --print("RAND_FILL")
            if(f_isBinaryOp(T_node.data))then
                if(math.random()>0.5)then
                    T_node.Left = nodeManager:newNode( f_getLeaf() ) 
                else  
                    T_node.Left = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                end
                if(math.random()>0.5)then
                    T_node.Right = nodeManager:newNode( f_getLeaf() ) 
                else  
                    T_node.Right = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                end
                T_node.Unique = nil
            else
                T_node.Left   = nil
                T_node.Right  = nil
                if(math.random()>0.5)then
                    T_node.Unique = nodeManager:newNode( f_getLeaf() )
                else  
                    T_node.Unique = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                end
            end
        end
        case[nodeManager.const.HALF_FILL]=function()
            --print("HALF_FILL")
            if(f_isBinaryOp(T_node.data))then
                T_node.Left = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                if(math.random()>0.5)then
                    T_node.Right = nodeManager:newNode( f_getLeaf() ) 
                else  
                    T_node.Right = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                end
                T_node.Unique = nil
            else
                T_node.Left   = nil
                T_node.Right  = nil
                if(math.random()>0.5)then
                    T_node.Unique = nodeManager:newNode( f_getLeaf() ) 
                else  
                    T_node.Unique = nodeManager:newTree(fill_select,depth-1,f_getOperand(),f_isBinaryOp,f_getLeaf,f_getOperand)
                end
            end
        end
        case[fill_select]()
    end
    return T_node
end

--[[

    ###########################################################################################################
    ###########################################################################################################
    ###########################################################################################################

]]

--[[
    ****************
    *     DATA     *
    ****************
    *   L   U   R  *
    ****************
]]
function nodeManager:newNode(_data)
    local temp_node = {}
    temp_node.Left  =nil
    temp_node.Right =nil
    temp_node.Unique=nil
    temp_node.data=_data
    return temp_node
end


function nodeManager:copyAllNode(_node)
    local c_node = {}
    c_node.Left     = _node.Left
    c_node.Right    = _node.Right
    c_node.Unique   = _node.Unique
    c_node.data = _node.data
    return c_node
end


function nodeManager:copyDataNode(_node)
    local c_node={}
    c_node.Left     =nil
    c_node.Right    =nil
    c_node.Unique   =nil
    c_node.data = _node.data
    return c_node
end

--[[    
    ##########################################################################################################
    ##########################################################################################################
    ##########################################################################################################
]]

function nodeManager:evalFunc(_node,f_evalBinOp,f_evalUnOp,f_isnode,f_isvar,f_isBinaryOp,f_getVarIndex,f_getTerminal,f_getExpresionIndex,values)
    --print(".")
    if(f_isBinaryOp(_node.data))then
        local left_operand
        local right_operand
        if( _node.Left~=nil and f_isnode(_node.Left.data))then
            left_operand = nodeManager:evalFunc(_node.Left,f_evalBinOp,f_evalUnOp,f_isnode,f_isvar,f_isBinaryOp,f_getVarIndex,f_getTerminal,f_getExpresionIndex,values)
        else
            if( _node.Left~=nil and f_isvar(_node.Left.data))then
                left_operand = values[f_getVarIndex(_node.Left.data)]
            else
                left_operand = f_getTerminal(_node.Left.data)
            end
        end
        if( _node.Right~=nil and f_isnode(_node.Right.data))then
            right_operand = nodeManager:evalFunc(_node.Right,f_evalBinOp,f_evalUnOp,f_isnode,f_isvar,f_isBinaryOp,f_getVarIndex,f_getTerminal,f_getExpresionIndex,values)
        else
            if( _node.Right~=nil and f_isvar(_node.Right.data))then
                right_operand = values[f_getVarIndex(_node.Right.data)]
            else
                right_operand = f_getTerminal(_node.Right.data)
            end
        end
        --if(type(right_operand)=='table' and right_operand~=nil)then print(inspect(right_operand))end
        --if(type(left_operand)=='table' and left_operand~=nil)then print(inspect(left_operand))end
        --print("a: "..left_operand.." b: "..right_operand)
        return f_evalBinOp( f_getExpresionIndex(_node.data) , left_operand , right_operand )
    else
        local unique_operand
        if(_node.Unique~=nil and f_isnode(_node.Unique.data))then
            unique_operand = nodeManager:evalFunc(_node.Unique,f_evalBinOp,f_evalUnOp,f_isnode,f_isvar,f_isBinaryOp,f_getVarIndex,f_getTerminal,f_getExpresionIndex,values)
        else
            if( _node.Unique~=nil and f_isvar(_node.Unique.data) )then
                unique_operand = values[f_getVarIndex(_node.Unique.data)]
            else
                unique_operand = f_getTerminal(_node.Unique.data)
            end
        end
        --print("a: "..unique_operand)
        return f_evalUnOp( f_getExpresionIndex(_node.data) , unique_operand)
    end
end



function nodeManager:cuadraticError(_node,f_evalBinOp,f_evalUnOp,f_isnode,f_isvar,f_isBinaryOp,f_getVarIndex,f_getTerminal,f_getExpresionIndex,l_values)
    local error = 0
    for index = 1,#l_values,1 do
        local result = nodeManager:evalFunc(_node,f_evalBinOp,f_evalUnOp,f_isnode,f_isvar,f_isBinaryOp,f_getVarIndex,f_getTerminal,f_getExpresionIndex,l_values[index])
        local fx = l_values[index][#l_values[index]]
        error = error + math.pow( fx-result , 2 )
    end
    return   math.sqrt( error/#l_values )

end

--[[
    ###########################################################################################################
    ###########################################################################################################
    ###########################################################################################################
]]

function nodeManager:nodeOpCounter(_node,f_isnode,f_isBinaryOp)
    local count = 1 
    if(f_isBinaryOp(_node.data))then
        if(_node.Left~=nil and f_isnode(_node.Left.data))then
            count = count + nodeManager:nodeOpCounter(_node.Left,f_isnode,f_isBinaryOp)
        end
        if(_node.Right~=nil and f_isnode(_node.Right.data))then
            count = count + nodeManager:nodeOpCounter(_node.Right,f_isnode,f_isBinaryOp)
        end
    else 
        if(_node.Unique~=nil and f_isnode(_node.Unique.data))then
            count = count + nodeManager:nodeOpCounter(_node.Unique,f_isnode,f_isBinaryOp)
        end
    end
    return count
end

function nodeManager:nodeAllCounter(_node,f_isnode,f_isBinaryOp)
    if(f_isBinaryOp(_node.data))then
        if(_node.Left~=nil)then
            if(f_isnode(_node.Left.data))then
                count = count + nodeManager:nodeAllCounter(_node.Left,f_isnode,f_isBinaryOp)
            else
                count = count + 1
            end
        end
        if(_node.Right~=nil)then
            if(f_isnode(_node.Right.data))then
                count = count + nodeManager:nodeAllCounter(_node.Right,f_isnode,f_isBinaryOp)
            else
                count = count + 1
            end
        end
    else  
        if(_node.Unique~=nil)then
            if(f_isnode(_node.Unique.data))then
                count = count + nodeManager:nodeAllCounter(_node.Unique,f_isnode,f_isBinaryOp)
            else
                count = count + 1
            end
        end
    end
return count
end

function nodeManager:printPosOrder(_node,f_isnode,f_isBinaryOp,f_getcharrep)
    if(f_isBinaryOp(_node.data))then
        io.write("( "..f_getcharrep(_node.data))
        if(_node.Left~=nil)then
            if(f_isnode(_node.Left.data))then
                io.write(" ");nodeManager:printPosOrder(_node.Left,f_isnode,f_isBinaryOp,f_getcharrep)
            else
                io.write(" "..f_getcharrep(_node.Left.data))
            end       
        end
        if(_node.Right~=nil)then
            if(f_isnode(_node.Right.data))then
                io.write(" ");nodeManager:printPosOrder(_node.Right,f_isnode,f_isBinaryOp,f_getcharrep)
            else
                io.write(" "..f_getcharrep(_node.Right.data))
            end
        end
        io.write(" )")       
    else
        io.write(f_getcharrep(_node.data).."(")
        if(_node.Unique~=nil)then
            if(f_isnode(_node.Unique.data))then
                io.write(" "); nodeManager:printPosOrder(_node.Unique,f_isnode,f_isBinaryOp,f_getcharrep)
            else
                io.write(" "..f_getcharrep(_node.Unique.data))
            end       
        end
        io.write(" )")
    end
end

function nodeManager:getIndexedSubTree(index,_node,_outNode,f_isnode,f_isBinaryOp)
    local deep = index
    if(index == 0)then
        _outNode = nodeManager:copyAllNode(_node)
        print("node reached")
    else
        if(index>=1)then
            if(f_isBinaryOp(_node.data))then
                if(_node.Left~=nil and f_isnode(_node.Left.data))then
                    deep = nodeManager:getIndexedSubTree(deep-1,_node.Left,_outNode,f_isnode,f_isBinaryOp)
                end
                if(_node.Right~=nil and f_isnode(_node.Right.data))then
                    deep = nodeManager:getIndexedSubTree(deep-1,_node.Right,_outNode,f_isnode,f_isBinaryOp)
                end
            else
                if(_node.Unique~=nil and f_isnode(_node.Unique.data))then
                    deep = nodeManager:getIndexedSubTree(deep-1,_node.Unique,_outNode,f_isnode,f_isBinaryOp)
                end
            end
        end
    end
    return deep;
end

-- this algorithm go through a tree inorder and without consider the one operation and two operation model
-- when the indexed node is reached the reference is saved in a internal variable
function nodeManager:getIndexedSubTreeIt( index , _node_root_index , f_isnode , f_isBinaryOp )
    local stackp = {}; local stacks = {};  local temp_node = self.INDIVS[_node_root]; local deep = index
    --[[while temp_node~=nil || #stackp<=0 do 
        while temp_node~=nil do 
            stackp[#stackp+1] = temp_node
            temp_node = temp_node.Left
        end
        temp_node = table.remove( stackp , #stackp )
        -- to something here
        temp_node =  temp_node.Right
    end]]
    ::gist_init::
        if( deep == 0 )then self.subT[#self.subT+1] = temp_node; goto gist_end end
        if( deep ~= index and #stacks==0 )then goto gist_end end
        if( temp_node==nil and f_isBinaryOp( temp_node ) )then
            if( temp_node.left ~=nil and f_isnode( temp_node.Left ) ) then goto gist_stt1 end        
            if( temp_node.Right~=nil and f_isnode( temp_node.Right ) )then goto gist_stt2 end
            
            temp_node = table.remove( stackp , #stackp ) ; table.remove( stacks , #stacks ) 
            
            if( stacks[#stacks]=='l' )then goto gist_sst2 end  
            ::gist_backUB::
                if( stacks[#stacks]=='u' )then temp_node = table.remove( stackp , #stackp ); table.remove( stacks , #stacks ) goto gist_backUB end
            ::gist_backRB::
                if( stacks[#stacks]=='r' )then temp_node = table.remove( stackp , #stackp ); table.remove( stacks , #stacks ) goto gist_backRB end

        else
            if( temp_node.Unique~=nil and f_isnode( temp_node.Unique ) )then goto gist_sst3 end

            temp_node = table.remove( stackp , #stackp ); table.remove( stacks , #stacks )
            
            if( stacks[#stacks]=='l' )then goto gist_sst2 end
            ::gist_backUU::
                if( stacks[#stacks]=='u' )then temp_node = table.remove( stackp , #stackp ); table.remove( stacks , #stacks ) goto gist_backUU end
            ::gist_backRU::
                if( stacks[#stacks]=='r' )then temp_node = table.remove( stackp , #stackp ); table.remove( stacks , #stacks ) goto gist_backRU end
        end
    ::gist_sst1:: stackp[#stackp+1]=temp_node; stacks[#stacks+1]='l'; temp_node = temp_node.Left; deep = deep-1;  goto gist_init 
    ::gist_sst2:: stackp[#stackp+1]=temp_node; stacks[#stacks+1]='r'; temp_node = temp_node.Right; deep = deep-1; goto gist_init
    ::gist_sst3:: stackp[#stackp+1]=temp_node; stacks[#stacks+1]='u'; temp_node = temp_node.Unique; deep = deep-1;goto gist_init
    ::gist_end:: 
end

function nodeManager:getIndexedNode(index,_node,_outNode,f_isnode,f_isBinaryOp)
    local deep = index
    if(index == 0)then
        _outNode = copyAllNode(_node)
    else
        if(index>=1)then
            if(f_isBinaryOp(_node.data))then
                if(_node.Left~=nil and f_isnode(_node.Left.data))then
                    if(f_isnode(_node.Left.data))then
                        deep = nodeManager:getIndexedSubTree(deep-1,_node.Left,_outNode,f_isnode,f_isBinaryOp)
                    else
                        deep = deep - 1
                    end
                end
                if(_node.Right~=nil and f_isnode(_node.Right.data))then
                    if(f_isnode(_node.Right.data))then
                        deep = nodeManager:getIndexedSubTree(deep-1,_node.Right,_outNode,f_isnode,f_isBinaryOp)
                    else
                        deep = deep - 1
                    end
                end
            else
                if(_node.Unique~=nil and f_isnode(_node.Unique.data))then
                    if(f_isnode(_node.Unique.data))then
                        deep = nodeManager:getIndexedSubTree(deep-1,_node.Unique,_outNode,f_isnode,f_isBinaryOp)
                    else
                        deep = deep - 1
                    end
                end
            end
        end
    end
    return deep;
end

function nodeManager:setIndexedSubTree(index,_node,_innode,f_isnode,f_isBinaryOp)
    local deep = index
    if(index == 0)then
        --_node = copyAllNode(_innode)
        _node , _innode = _innode , _node
    else
        if(index>=1)then
            if(f_isBinaryOp(_node.data))then
                if(_node.Left~=nil and f_isnode(_node.Left.data))then
                    deep = nodeManager:setIndexedSubTree(deep-1,_node.Left,f_isnode,f_isBinaryOp)
                end
                if(_node.Right~=nil and f_isnode(_node.Right.data))then
                    deep = nodeManager:setIndexedSubTree(deep-1,_node.Right,f_isnode,f_isBinaryOp)
                end
            else
                if(_node.Unique~=nil and f_isnode(_node.Unique.data))then
                    deep = nodeManager:setIndexedSubTree(deep-1,_node.Unique,f_isnode,f_isBinaryOp)
                end
            end
        end
    end
    return deep   
end

function nodeManager:setindexedNode(index,_node,_innode,f_isnode,f_isBinaryOp)
    local deep = index
    if(index == 0 )then
        _node = copyAllNode(_innode)
    else
        if(index>=1)then
            if(f_isBinaryOp(_node.data))then
                if(_node.Left~=nil)then
                    if(f_isnode(_node.Left.data))then
                        deep = nodeManager:setindexedNode(deep-1,_node.Left,_innode,f_isnode,f_isBinaryOp)
                    else
                        deep = deep - 1
                    end                
                end
                if(_node.Right~=nil)then
                    if(f_isnode(_node.Right.data))then
                        deep = nodeManager:setindexedNode(deep-1,_node.Right,_innode,f_isnode,f_isBinaryOp)
                    else
                        deep = deep - 1
                    end                
                end
            else
                if(_node.Unique~=nil)then
                    if(f_isnode(_node.Unique.data))then
                        deep = nodeManager:setindexedNode(deep-1,_node.Unique,_innode,f_isnode,f_isBinaryOp)
                    else
                        deep = deep - 1
                    end                
                end
            end
        end
    end
    return deep
end

return nodeManager