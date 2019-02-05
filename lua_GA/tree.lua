tree = {}
option_types = {'string','number'}
option_functions={'-','+','*','/'}
option_terminals={1,2,3,4,5,6,7,8,9}

tree.newNode=function(_expr,_left,_right,depth)
    local obj={}
    obj.expr=_expr
    if(type(_left)~=option_types[2])then
        if(depth==2)then 
            obj.Lleaf = tree.newNode(_left,option_terminals[math.random(#option_terminals)],option_terminals[math.random(#option_terminals)],depth-1)
        else
            local new_left  = (math.random(100)>50) and option_functions[math.random(#option_functions)] or option_terminals[math.random( #option_terminals )]
            local new_right = (math.random(100)>50) and option_functions[math.random(#option_functions)] or option_terminals[math.random( #option_terminals )]
            obj.Lleaf = tree.newNode(_left,new_left,new_right,depth-1)
        end
    else
        obj.Lleaf = _left
    end
    if(type(_right)~=option_types[2])then
        if(depth==2)then 
            obj.Rleaf = tree.newNode(_right,option_terminals[math.random(#option_terminals)],option_terminals[math.random(#option_terminals)],depth-1)
        else
            local new_left  = (math.random(100)>50) and option_functions[math.random(#option_functions)] or option_terminals[math.random( #option_terminals )]
            local new_right = (math.random(100)>50) and option_functions[math.random(#option_functions)] or option_terminals[math.random( #option_terminals )]
            obj.Rleaf = tree.newNode(_right,new_left,new_right,depth-1)
        end
    else
        obj.Rleaf = _right
    end
    return obj
end

tree.newTree=function(maxDepth)
    local new_left  = (math.random(100)>50) and option_functions[math.random(#option_functions)] or option_terminals[math.random( #option_terminals )]
    local new_right = (math.random(100)>50) and option_functions[math.random(#option_functions)] or option_terminals[math.random( #option_terminals )]
    local new_exp   = option_functions[math.random(#option_functions)]
    local tree = tree.newNode(new_exp,new_left,new_right,maxDepth)
    return tree
end

tree.print_preorder=function(_tree)
    io.write(_tree.expr)
    if(type(_tree.Lleaf)==option_types[2])then io.write(_tree.Lleaf)
    else tree.print_preorder(_tree.Lleaf) 
    end
    if(type(_tree.Rleaf)==option_types[2])then io.write(_tree.Rleaf)
    else tree.print_preorder(_tree.Rleaf) 
    end
end

--[[tree.string_to_print=function(_tree)
    if(type(_tree.Lleaf)==option_types[2])then io.write('~'.._tree.Lleaf..'\n')
    else io.write('~') tree.string_to_print(_tree.Lleaf) 
    end
    io.write(_tree.expr..'\n')
    if(type(_tree.Rleaf)==option_types[2])then io.write('~'.._tree.Rleaf..'\n')
    else io.write('~') tree.string_to_print(_tree.Rleaf) 
    end
end]]

return tree