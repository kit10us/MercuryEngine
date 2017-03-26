require "util"

function CreateShapeCube()
	cubeParameters = ShapeParameters( "cube" )
	cubeParameters:SetEffect( color3d )
	cubeParameters:SetSize3( Size3.New( 2, 2, 2 ) )
	cubeParameters:SetDiffuseFaces( Color.NewRed(), Color.NewGreen(), Color.NewBlue(), Color.NewRGB( 1, 1, 0 ), Color.NewRGB( 0, 1, 1 ), Color.NewRGB( 1, 0, 1 ) )
	return Geometry( MakeGeometryName( "cube", cubeParameters )
end

function CreateShapePointfield()
	pointfieldParameters = ShapeParameters( "pointfield" )
	pointfieldParameters:SetEffect( color3d )
	pointfieldParameters:SetMinorRadius( 0.5 )
	pointfieldParameters:SetMajorRadius( 1.0 )
	pointfieldParameters:SetCount( 1000 )
	pointfieldParameters:SetDiffuse( Color.NewWhite() )
	return Geometry( MakeGeometryName( "pointfield", pointfieldParameters )
end

function CreateShapePointring()
	pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 0.25 )
	pointringParameters:SetMajorRadius( 0.75 )
	pointringParameters:SetCount( 1000 )
	pointringParameters:SetDiffuse( Color.NewRed() )
	return Geometry( MakeGeometryName( "pointring", pointringParameters )
end

function CreateShapeDashring()
	dashringParameters = ShapeParameters( "dashring" )
	dashringParameters:SetEffect( color3d )
	dashringParameters:SetMinorRadius( 0.5 )
	dashringParameters:SetMajorRadius( 1.0 )
	dashringParameters:SetSize( 0.5 )
	dashringParameters:SetCount( 5 )
	dashringParameters:SetDiffuse( Color.NewGreen() )
	return Geometry( MakeGeometryName( "dashring", dashringParameters )
end

function CreateShapePyramid()
	pyramidParameters = ShapeParameters( "pyramid" )
	pyramidParameters:SetEffect( color3d )
	pyramidParameters:SetSize3( Size3.New( 2, 2, 2 ) )
	pyramidParameters:SetDiffuse( Color.NewRGB( 0, 1, 1 ) )
	return Geometry( MakeGeometryName( "pyramid", pyramidParameters )
end

function CreateShapeCircle()
	circleParameters = ShapeParameters( "circle" )
	circleParameters:SetEffect( color3d );
	circleParameters:SetSegments( 24 );
	circleParameters:SetRadius( 1.0 );
	circleParameters:SetDiffuse( Color.NewBlue() );
	return Geometry( MakeGeometryName( "circle", Geometry( circleParameters );
end

function CreateShapeSphere()
	sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( color3d )
	sphereParameters:SetSegments( 24 )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Color.NewRGB( 1, 1, 0 ) )
	return Geometry( MakeGeometryName( "sphere", sphereParameters )
end

function CreateShapeCylinder()
	cylinderParameters = ShapeParameters( "cylinder" )
	cylinderParameters:SetEffect( color3d )
	cylinderParameters:SetSegments( 24 )
	cylinderParameters:SetRadius( 1.0 )
	cylinderParameters:SetHeight( 2.0 )
	cylinderParameters:SetDiffuse( Color.NewRGB(1, 0, 1 ) )
	cylinderParameters:SetCaps( true )
	return Geometry( MakeGeometryName( "cylinder", cylinderParameters )
end

function CreateShapeTube()
	tubeParameters = ShapeParameters( "tube" )
	tubeParameters:SetEffect( color3d )
	tubeParameters:SetSegments( 24 )
	tubeParameters:SetMajorRadius( 1.0 )
	tubeParameters:SetMinorRadius( 0.5 )
	tubeParameters:SetHeight( 2.0 )
	tubeParameters:SetDiffuse( Color.NewRed() )
	return Geometry( MakeGeometryName( "tube", tubeParameters )
end

function CreateShapePlane()
	planeParameters = ShapeParameters( "plane" )
	planeParameters:SetEffect( color3d )
	planeParameters:SetSegments( 2 )
	planeParameters:SetSize2( Size2.New( 2.0, 2.0 ) )
	planeParameters:SetDiffuse( Color.NewRGB( 0, 1, 1 ) )
	return Geometry( MakeGeometryName( "plane", planeParameters ) )
end

function CreateShapeCone()
	coneParameters = ShapeParameters( "cone" )
	coneParameters:SetEffect( borgcubeEffect )
	coneParameters:SetSegments( 24 )
	coneParameters:SetRadius( 1 )
	coneParameters:SetHeight( 2 )
	coneParameters:SetCaps( true )
	return Geometry( MakeGeometryName( "cone", coneParameters ) )
end
