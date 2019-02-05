tree = {}
options_types = {'string','number'}
options_functions={'-','+','*','/'}
options_terminals={1,2,3,4,5,6,7,8,9}

tree.newNode=function(_expr,_left,_right,depth)
    local obj={}
    obj.expr=_expr
    if(type(_left)~=options_types[2])then
        if(depth==2)then 
            obj.Lleaf = tree.newNode(_left,options_terminals[math.random(#option_terminals)],options_terminals[math.random(#option_terminals)],depth-1)
        else
            local new_left  = (math.random(100)>50) and options_functions[math.random(#options_functions)] or option_terminals[math.random( #option_terminals )]
            local new_right = (math.random(100)>50) and options_functions[math.random(#options_functions)] or option_terminals[math.random( #option_terminals )]
            obj.Lleaf = tree.newNode(_left,new_left,new_right,depth-1)
        end
    else
        obj.Lleaf = _left
    end
    if(type(_right)~=options_types[2])then
        if(depth==2)then 
            obj.Rleaf = tree.newNode(_right,options_terminals[math.random(#option_terminals)],options_terminals[math.random(#option_terminals)],depth-1)
        else
            local new_left  = (math.random(100)>50) and options_functions[math.random(#options_functions)] or option_terminals[math.random( #option_terminals )]
            local new_right = (math.random(100)>50) and options_functions[math.random(#options_functions)] or option_terminals[math.random( #option_terminals )]
            obj.Rleaf = tree.newNode(_right,new_left,new_right,depth-1)
        end
    else
        obj.Rleaf = _right
    end
    return obj
end

tree.newSubTree=function(exp,right,left)

end

return tree