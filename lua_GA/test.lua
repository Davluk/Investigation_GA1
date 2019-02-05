print(type(type(1)))

function perm(arg)
    if(arg==1)then return 1 else
        return arg * perm(arg - 1)
    end
end 

print(perm(4))