// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <sg/ShapeCreators.h>
#include <me/render/RenderMethod.h>
#include <me/render/TextureMode.h>
#include <me/exception/NotImplemented.h>
#include <me/render/BufferUsage.h>
#include <me/render/VertexUtil.h>
#include <unify/String.h>
#include <unify/Size3.h>
#include <unify/TexArea.h>
#include <unify/Angle.h>

using namespace me;
using namespace render;

const float PI = 3.14159265358979f;
const float PI2 = 6.28318530717959f;
const std::string DefaultBufferUsage = "Default";

std::shared_ptr< Mesh > sg::CreateShape( IRenderer * renderer, unify::Parameters & parameters )
{
	std::shared_ptr< Mesh > mesh( new Mesh( "Shape::" + Shape::ToString( parameters.Get< Shape::TYPE >( "type" ) ), renderer ) );
	CreateShape( renderer, mesh->GetPrimitiveList(), parameters );
	mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );
	return mesh;
}

void sg::CreateShape( me::render::IRenderer * renderer, me::render::PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	Shape::TYPE type = parameters.Get< Shape::TYPE >( "type" );

	switch( type )
	{
	case Shape::Cube:
		CreateShape_Cube( renderer, primitiveList, parameters );
		break;
	case Shape::PointField:
		CreateShape_PointField( renderer, primitiveList, parameters );
		break;
	case Shape::PointRing:
		CreateShape_PointRing( renderer, primitiveList, parameters );
		break;
	case Shape::DashRing:
		CreateShape_DashRing( renderer, primitiveList, parameters );
		break;
	case Shape::Pyramid:
		CreateShape_Pyramid( renderer, primitiveList, parameters );
		break;
	case Shape::Circle:
		CreateShape_Circle( renderer, primitiveList, parameters );
		break;
	case Shape::Sphere:
		CreateShape_Sphere( renderer, primitiveList, parameters );
		break;
	case Shape::Cylinder:
		CreateShape_Cylinder( renderer, primitiveList, parameters );
		break;
	case Shape::Tube:
		CreateShape_Tube( renderer, primitiveList, parameters );
		break;
	case Shape::Plane:
		CreateShape_Plane( renderer, primitiveList, parameters );
		break;
	case Shape::Cone:
		CreateShape_Cone( renderer, primitiveList, parameters );
		break;
	case Shape::BeveledBox:
		CreateShape_BeveledBox( renderer, primitiveList, parameters );
	default:
		throw unify::Exception( "Invalid Mesh m_shapeType! Failed to create m_shapeType!" );
	}
}
