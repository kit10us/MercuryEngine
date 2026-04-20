// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/ResourceType.h>
#include <unify/String.h>
#include <unify/Exception.h>

using namespace me;
using namespace render;

ResourceType::TYPE ResourceType::FromString( std::string usage )
{
	if ( unify::String::StringIs( usage, "PixelShader" ) )
	{
		return PixelShader;
	}
	else if ( unify::String::StringIs( usage, "VertexShader" ) )
	{
		return VertexShader;
	}
	else if ( unify::String::StringIs( usage, "ComputeShader" ) )
	{
		return ComputeShader;
	}
	else if ( unify::String::StringIs( usage, "DomainShader" ) ) 
	{
		return DomainShader;
	}
	else if( unify::String::StringIs( usage, "GeometryShader" ) )
	{
		return GeometryShader;
	}
	else
	{
		throw unify::Exception( "ResourceType::FromString: Not a valid usage type!" );
	}
}

std::string ResourceType::ToString( ResourceType::TYPE usage )
{
	switch( usage )
	{
	case PixelShader:
		return "PixelShader";

	case VertexShader:
		return "VertexShader";

	case ComputeShader:
		return "ComputeShader";

	case DomainShader:
		return "DomainShader";

	case GeometryShader:
		return "GeometryShader";

	default:
		throw unify::Exception( "ResourceType::ToString: Not a valid usage type!" );
	}
}