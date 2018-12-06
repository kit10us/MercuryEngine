// Copyright (c) 2002 - 2018, Evil Quail LLC
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

std::string Shape::ToString( Shape::TYPE type )
{
	switch( type )
	{
		case Shape::Cube: return "cube";
		case Shape::PointField: return "pointField";
		case Shape::PointRing: return "pointRing";
		case Shape::DashRing: return "dashRing";
		case Shape::Pyramid: return "pyramid";
		case Shape::Circle: return "circle";
		case Shape::Sphere: return "sphere";
		case Shape::Cylinder: return "cylinder";
		case Shape::Tube: return "tube";
		case Shape::Plane: return "plane";
		case Shape::Cone: return "cone";
		case Shape::BeveledBox: return "bevelBox";
		default: return "<unknown>";
}
}