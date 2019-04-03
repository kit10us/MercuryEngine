require "util"

function ShapeBox( effect )
	boxParameters = ShapeParameters( "box" )
	boxParameters:SetEffect( effect )
	boxParameters:SetSize3( Size3( 2, 2, 2 ) )
	boxParameters:SetDiffuseFaces( Red(), Green(), Blue(), RGB( 1, 1, 0 ), RGB( 0, 1, 1 ), RGB( 1, 0, 1 ) )
	return Geometry( MakeGeometryName( "box" ), boxParameters )
end

function ShapePointfield( effect )
	pointfieldParameters = ShapeParameters( "pointfield" )
	pointfieldParameters:SetEffect( effect )
	pointfieldParameters:SetMinorRadius( 0.5 )
	pointfieldParameters:SetMajorRadius( 1.0 )
	pointfieldParameters:SetCount( 1000 )
	pointfieldParameters:SetDiffuse( White() )
	return Geometry( MakeGeometryName( "pointfield" ), pointfieldParameters )
end

function ShapePointring( effect )
	pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( effect )
	pointringParameters:SetMinorRadius( 0.25 )
	pointringParameters:SetMajorRadius( 0.75 )
	pointringParameters:SetCount( 1000 )
	pointringParameters:SetDiffuse( Red() )
	return Geometry( MakeGeometryName( "pointring" ), pointringParameters )
end

function ShapeDashring( effect )
	dashringParameters = ShapeParameters( "dashring" )
	dashringParameters:SetEffect( effect )
	dashringParameters:SetMinorRadius( 0.5 )
	dashringParameters:SetMajorRadius( 1.0 )
	dashringParameters:SetSize( 0.5 )
	dashringParameters:SetCount( 5 )
	dashringParameters:SetDiffuse( Green() )
	return Geometry( MakeGeometryName( "dashring" ), dashringParameters )
end

function ShapePyramid( effect )
	pyramidParameters = ShapeParameters( "pyramid" )
	pyramidParameters:SetEffect( effect )
	pyramidParameters:SetSize3( Size3( 2, 2, 2 ) )
	pyramidParameters:SetDiffuse( RGB( 0, 1, 1 ) )
	return Geometry( MakeGeometryName( "pyramid" ), pyramidParameters )
end

function ShapeCircle( effect )
	circleParameters = ShapeParameters( "circle" )
	circleParameters:SetEffect( effect );
	circleParameters:SetSegments( 24 );
	circleParameters:SetRadius( 1.0 );
	circleParameters:SetDiffuse( Blue() );
	return Geometry( MakeGeometryName( "circle" ), circleParameters )
end

function ShapeSphere( effect )
	sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( effect )
	sphereParameters:SetSegments( 24 )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( RGB( 1, 1, 0 ) )
	return Geometry( MakeGeometryName( "sphere" ), sphereParameters )
end

function ShapeCylinder( effect )
	cylinderParameters = ShapeParameters( "cylinder" )
	cylinderParameters:SetEffect( effect )
	cylinderParameters:SetSegments( 24 )
	cylinderParameters:SetRadius( 1.0 )
	cylinderParameters:SetHeight( 2.0 )
	cylinderParameters:SetDiffuse( RGB(1, 0, 1 ) )
	cylinderParameters:SetCaps( true )
	return Geometry( MakeGeometryName( "cylinder" ), cylinderParameters )
end

function ShapeTube( effect )
	tubeParameters = ShapeParameters( "tube" )
	tubeParameters:SetEffect( effect )
	tubeParameters:SetSegments( 24 )
	tubeParameters:SetMajorRadius( 1.0 )
	tubeParameters:SetMinorRadius( 0.5 )
	tubeParameters:SetHeight( 2.0 )
	tubeParameters:SetDiffuse( Red() )
	return Geometry( MakeGeometryName( "tube" ), tubeParameters )
end

function ShapePlane( effect )
	planeParameters = ShapeParameters( "plane" )
	planeParameters:SetEffect( effect )
	planeParameters:SetSegments( 2 )
	planeParameters:SetSize2( Size2.New( 2.0, 2.0 ) )
	planeParameters:SetDiffuse( RGB( 0, 1, 1 ) )
	return Geometry( MakeGeometryName( "plane" ), planeParameters )
end

function ShapeCone( effect )
	coneParameters = ShapeParameters( "cone" )
	coneParameters:SetEffect( borgcubeEffect )
	coneParameters:SetSegments( 24 )
	coneParameters:SetRadius( 1 )
	coneParameters:SetHeight( 2 )
	coneParameters:SetCaps( true )
	return Geometry( MakeGeometryName( "cone" ), coneParameters )
end
