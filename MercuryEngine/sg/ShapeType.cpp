// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <sg/ShapeType.h>
#include <me/render/RenderMethod.h>
#include <unify/String.h>
#include <unify/Size3.h>

using namespace sg;

const float PI =  3.14159265358979f;
const float PI2 = 6.28318530717959f;

Shape::TYPE Shape::FromString( const std::string type )
{
	if( unify::StringIs( type, "cube" ) ) return Shape::Cube;
	if( unify::StringIs( type, "pointfield" ) ) return Shape::PointField;
	if( unify::StringIs( type, "pointring" ) ) return Shape::PointRing;
	if( unify::StringIs( type, "dashring" ) ) return Shape::DashRing;
	if( unify::StringIs( type, "pyramid" ) ) return Shape::Pyramid;
	if( unify::StringIs( type, "circle" ) ) return Shape::Circle;
	if( unify::StringIs( type, "sphere" ) ) return Shape::Sphere;
	if( unify::StringIs( type, "cylinder" ) ) return Shape::Cylinder;
	if( unify::StringIs( type, "tube" ) ) return Shape::Tube;
	if( unify::StringIs( type, "plane" ) ) return Shape::Plane;
	if( unify::StringIs( type, "cone" ) ) return Shape::Cone;
	return Shape::INVALID;
}
