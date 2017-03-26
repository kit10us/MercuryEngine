function OA_SetPosition( position )
	local element = XMLElement( "SetPosition" )
	
	if type( position ) == "Object" then	
		element:AddAttribute( "type", "object" )
		element:AddText( position:GetName() )
	elseif type( position ) == "V3" then
		element:AddAttribute( "type", "absolute" )
		element:AddText( tostring( position ) )	
	end
	return ObjectAction( element )
end
