require "colors"

function MakeObjectName( prefix )
	local this = Scene()

	local name
	
	-- Find the first name available...
	for i = 0, 1000, 1 do
		name = prefix .. "_" .. i
		if not this:FindObject( name )  then		
			break
		end
	end	
	return name
end

function MakeGeometryName( prefix )
	local this = Scene()

	local name
	
	-- Find the first name available...
	for i = 0, 1000, 1 do
		name = prefix .. "_" .. i
		if not Geometry( name ) then		
			break
		end
	end	
	return name
end
