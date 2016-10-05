axisIndex = 1
totalRotation = 0
--[[
unify::Angle rotation = unify::Angle::AngleInRadians( renderInfo.GetDelta() );
totalRotation += rotation;
if( totalRotation.Fix360() != 0 )
{
	rotation = totalRotation; // Left over
	axis++;
	if( axis >= 2 )
	{
		axis = 0;
	}
}
--]]

function OnStart()
	resources.add( "geometry", "cube", "media/ShapeCube.xml" )
	resources.add( "geometry", "pointfield", "media/ShapePointField.xml" )
	resources.add( "geometry", "pointring", "media/ShapePointRing.xml" )
	resources.add( "geometry", "dashring", "media/ShapeDashRing.xml" )
	resources.add( "geometry", "pyramid", "media/ShapePyramid.xml" )
	resources.add( "geometry", "circle", "media/ShapeCircle.xml" )
	resources.add( "geometry", "sphere", "media/ShapeSphere.xml" )
	resources.add( "geometry", "cylinder", "media/ShapeCylinder.xml" )
	resources.add( "geometry", "tube", "media/ShapeTube.xml" )
	resources.add( "geometry", "plane", "media/ShapePlane.xml" )
	resources.add( "geometry", "cone", "media/ShapeCone.xml" )

	scenes.add( "scene1" )
	
	scene.addobject( "scene1", "group" );
	
	scene.addobject( "scene1", "cube" )
	object.setgeometry( "scene1:cube", "cube" )
	object.setposition( "scene1:cube", V3.New( -4.5, 3, 0 ) )

	scene.addobject( "scene1", "pointfield" )
	object.setgeometry( "scene1:pointfield", "pointfield" )
	object.setposition( "scene1:pointfield", V3.New( -1.5, 3, 0 ) )
		
	scene.addobject( "scene1", "pointring" )
	object.setgeometry( "scene1:pointring", "pointring" )
	object.setposition( "scene1:pointring", V3.New( 1.5, 3, 0 ) )

	scene.addobject( "scene1", "dashring" )
	object.setgeometry( "scene1:dashring", "dashring" )
	object.setposition( "scene1:dashring", V3.New( 4.5, 3, 0 ) )

	scene.addobject( "scene1", "pyramid" )
	object.setgeometry( "scene1:pyramid", "pyramid" )
	object.setposition( "scene1:pyramid", V3.New( -4.5, 0, 0 ) )
		
	scene.addobject( "scene1", "circle" )
	object.setgeometry( "scene1:circle", "circle" )
	object.setposition( "scene1:circle", V3.New( -1.5, 0, 0 ) )

	scene.addobject( "scene1", "sphere" )
	object.setgeometry( "scene1:sphere", "sphere" )
	object.setposition( "scene1:sphere", V3.New( 1.5, 0, 0 ) )
	   
	scene.addobject( "scene1", "cylinder" )
	object.setgeometry( "scene1:cylinder", "cylinder" )
	object.setposition( "scene1:cylinder", V3.New( 4.5, 0, 0 ) )

	scene.addobject( "scene1", "tube" )
	object.setgeometry( "scene1:tube", "tube" )
	object.setposition( "scene1:tube", V3.New( -4.5, -3, 0 ) )
		
	scene.addobject( "scene1", "plane" )
	object.setgeometry( "scene1:plane", "plane" )
	object.setposition( "scene1:plane", V3.New( -1.5, -3, 0 ) )
		
	scene.addobject( "scene1", "cone" )
	object.setgeometry( "scene1:cone", "cone" )
	object.setposition( "scene1:cone", V3.New( 1.5, -3, 0 ) )

	scene.addobject( "scene1", "camera" )
	object.setposition( "scene1:camera", V3.New( 0, 5, -17 ) )
	object.lookat( "scene1:camera", V3.Zero() ) 

	scene.setcamera( "scene1", "camera" )

	scene.setsize( "scene1", game.getwidth(), game.getheight() )

	proj = matrix.makeperspectivefovlh( math.pi / 4.0, game.getaspectratiohw(), 1, 1000 )

	camera.setprojection( "scene1", proj ) 
end

function OnUpdate()
--[[
unify::Angle rotation = unify::Angle::AngleInRadians( renderInfo.GetDelta() );
totalRotation += rotation;
if( totalRotation.Fix360() != 0 )
{
	rotation = totalRotation; // Left over
	axis++;
	if( axis >= 2 )
	{
		axis = 0;
	}
}


	object.orbit( "scene1:camera", V3.New( 0, 0, 0 ), V2.New( 1, 0 ), update.getdelta() );
	object.lookat( "scene1:camera", V3.Zero() );
--]]

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

	--object.RotateAbout( "scene1:cube", axis, rotation )
end
