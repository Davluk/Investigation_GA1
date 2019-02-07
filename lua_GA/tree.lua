tree = {}
--valor de la variable independiente
x=7.86
--variables de configuracion del algoritmo
option_types = {'string','number'}
option_functions={'+','-','*','/'}
option_terminals={x,x,x,1,2,3,4,5,6,7,8,9}

function ADD(a,b) return a+b end
function SUB(a,b) return a-b end
function MUL(a,b) return a*b end
function DIV(a,b) return a/b end

--asegurarse de que la asignacion de las funciones este en el orden en el que estan las epxresiones en "option_functions"
execute_functions = {}
execute_functions[option_functions[1]]=ADD
execute_functions[option_functions[2]]=SUB
execute_functions[option_functions[3]]=MUL
execute_functions[option_functions[4]]=DIV

tree.EvalExpression=function(_tree)
    local operand_a = 0
    local operand_b = 0
    if(type(_tree.Lleaf)~=option_types[2])then operand_a = tree.EvalExpression(_tree.Lleaf) else operand_a=_tree.Lleaf end
    if(type(_tree.Rleaf)~=option_types[2])then operand_b = tree.EvalExpression(_tree.Rleaf) else operand_b=_tree.Rleaf end
    return execute_functions[_tree.expr](operand_a,operand_b)
end

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

tree.print_inorder=function(_tree)
    io.write('(')
    if(type(_tree.Lleaf)==option_types[2])then io.write(_tree.Lleaf)
    else tree.print_inorder(_tree.Lleaf) 
    end
    io.write(_tree.expr)    
    if(type(_tree.Rleaf)==option_types[2])then io.write(_tree.Rleaf)
    else tree.print_inorder(_tree.Rleaf) 
    end
    io.write(')')
end


tree.countNodes= function(_tree)
    local counter = 1
    if(type(_tree.Lleaf)~=option_types[2])then counter = counter + tree.countNodes(_tree.Lleaf) end
    if(type(_tree.Rleaf)~=option_types[2])then counter = counter + tree.countNodes(_tree.Rleaf) end
    return counter
end

return tree