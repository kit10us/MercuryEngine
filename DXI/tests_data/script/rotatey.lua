function OnStart( me )
end

function OnUpdate( me )
	local rotMatrix = Matrix.NewRotationY( Update.GetDelta() * 2 )
	me:Transform():PreMul( rotMatrix )
end