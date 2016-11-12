function OnStart()
end

function OnUpdate()
	local rot = Quaternion.NewRotationY( Update.GetDelta() * 2 )
	_object:Transform():PreMul( rot )
end