require "OA"

function OAC_OnEnter_SetPosition( size, position )
	local physics = MEPhysics()
	local collider = physics:CreateBoxCollider( size )
	
	collider:SetOnEnter( OA_SetPosition( position )	)
	return collider:AsObjectComponent()
end

function OAC_OnExit_SetPosition( size, position )
	local physics = MEPhysics()
	local collider = physics:CreateBoxCollider( size )
	
	collider:SetOnExit( OA_SetPosition( position )	)
	return collider:AsObjectComponent()
end