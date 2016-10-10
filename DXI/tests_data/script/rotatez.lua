function OnStart( me )
end

function OnUpdate( me )
	local rotMatrix = Matrix.NewRotationZ( Update.GetDelta() * 2 )
	me:Transform():PreMul( rotMatrix )
end