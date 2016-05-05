// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <bi/XMLConvert.h>
#include <dxi/XMLConvert.h>
#include <bi/Convert.h>

bool bi::XMLConvertShape( const qxml::Element * element, boost::shared_ptr< btCollisionShape > & shape )
{
	std::string shapeType = element->GetStringAttribute( "type" );

	if( shapeType == "box" )
	{
		unify::V3< float > halfExtents;
		if( ! element->HasElements( "halfExtents" ) || ! dxi::XMLConvert( element->GetElement( "halfExtents" ), halfExtents ) )
		{
			throw std::exception( "XMLConvertShape: Box missing halfExtents!" );
		}
		shape.reset( new btBoxShape( bi::convert( halfExtents ) ) );
		return true;
	}
	else if( shapeType == "sphere" )
	{
		float radius = element->GetFloatAttribute( "radius" );
		shape.reset( new btSphereShape( radius ) );
		return true;
	}
	else if( shapeType == "cylinder" )
	{
		unify::V3< float > halfExtents;
		if( ! element->HasElements( "halfExtents" ) || ! dxi::XMLConvert( element->GetElement( "halfExtents" ), halfExtents ) )
		{
			throw std::exception( "XMLConvertShape: Sphere missing halfExtents!" );
		}
		shape.reset( new btCylinderShape( bi::convert( halfExtents ) ) );
		return true;
	}
	else if( shapeType == "cone" )
	{
		float radius = element->GetFloatAttribute( "radius" );
		float height = element->GetFloatAttribute( "height" );
		shape.reset( new btConeShape( radius, height ) );
		return true;
	}
	else if( shapeType == "capsule" )
	{
		float radius = element->GetFloatAttribute( "radius" );
		float height = element->GetFloatAttribute( "height" );
		shape.reset( new btCapsuleShape( radius, height ) );
		return true;
	}	
	else if( shapeType == "staticplane" )
	{
		unify::V3< float > planeNormal;
		if( ! element->HasElements( "planeNormal" ) || ! dxi::XMLConvert( element->GetElement( "planeNormal" ), planeNormal ) )
		{
			throw std::exception( "XMLConvertShape: Sphere missing halfExtents!" );
		}
		float planeConstant = element->GetFloatAttribute( "planeConstant" );
		shape.reset( new btStaticPlaneShape( bi::convert( planeNormal ), planeConstant ) );
		return true;
	}

	return false;
}
