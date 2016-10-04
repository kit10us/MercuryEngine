tim = 12
tim = tim + 1;

function OnStart()
   print( "Start called" )
   
   tim = tim + 1
   print( "Tim = " .. tim );
end

function OnUpdate()
   print( "Update called" )
end

--[[
function Missing()
end
--]]