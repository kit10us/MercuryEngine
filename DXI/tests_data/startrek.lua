axisIndex = 1
totalRotation = 0

function OnStart()
	resources.add( "geometry", "enterprise", "media/enterprise.dae" )	
	
	scene1 = Scene( "scene1" )
	root = scene1:FindObject( "root" )
	
	-- Add camera...
	camera = root:AddChild( "camera" )
	scene1:SetCamera( camera )

	camera:SetPosition( V3.New( 0, 5, -17 ) )
	camera:LookAt( V3.Zero() ) 

	scene1:SetSize( game.getwidth(), game.getheight() )

	proj = matrix.makeperspectivefovlh( math.pi / 4.0, game.getwidth()/ game.getheight(), 1, 1000 )

	Camera.SetProjection( "scene1", proj ) 
	
	enterprise = root:AddChild( "enterprise" )
	enterprise:SetGeometry( "enterprise" )
	scale = 8.0 / enterprise:GetSize()
	enterprise:Scale( scale * 0.25 )
end

function OnUpdate()
	rotation = Update.GetDelta()
	
	totalRotation = totalRotation + rotation
	pi2 = 3.1415926535 * 2
	if totalRotation > pi2 then
		totalRotation = totalRotation - pi2
		rotation = totalRotation -- Left over
		axisIndex = axisIndex + 1
		if axisIndex >= 3 then
			axisIndex = 0
		end
	end

	axis = V3.Zero()
	if axisIndex == 0 then
		axis.x = 1
	elseif axisIndex == 1 then
		axis.y = 1
	elseif axisIndex == 2 then
		axis.z = 1
	end

	camera:RotateAbout( axis, rotation )
end
