function OnStart( me )
end

function OnUpdate( me )
	rotation = Update.GetDelta()
	xform  = me:Transform():RotateAbout( V3.New( 1, 0, 0 ), rotation )
end