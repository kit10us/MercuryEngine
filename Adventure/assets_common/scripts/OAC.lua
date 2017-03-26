require "OA"

function OAC_SetPosition( size, position )
	local physics = MEPhysics()
	local collider = physics:CreateBoxCollider( size )
	
	collider:SetOnEnter( OA_SetPosition( position )	)
	return collider:AsObjectComponent()
end