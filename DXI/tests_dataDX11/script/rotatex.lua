function OnStart()
end

function OnUpdate()
	local rotMatrix = Matrix.NewRotationX( Update.GetDelta() * 2 )
	_object:Transform():PreMul( rotMatrix )
end