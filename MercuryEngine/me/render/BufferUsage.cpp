// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/exception/Exception.h>
#include <me/render/BufferUsage.h>
#include <unify/String.h>

using namespace me;
using namespace render;

BufferUsage::TYPE BufferUsage::FromString( std::string usage )
{
	if ( unify::string::StringIs( usage, "Default" ) )
	{
		return Default;
	}
	else if ( unify::string::StringIs( usage, "Immutable" ) )
	{
		return Immutable;
	}
	else if ( unify::string::StringIs( usage, "Dynamic" ) )
	{
		return Dynamic;
	}
	else if ( unify::string::StringIs( usage, "Staging" ) ) 
	{
		return Staging;
	}
	else
	{
		throw unify::Exception( "BufferUsage::FromString: Not a valid usage type!" );
	}
}

std::string BufferUsage::ToString( BufferUsage::TYPE usage )
{
	switch( usage )
	{
	case Default:
		return "Default";
	case Immutable:
		return "Immutable";
	case Dynamic:
		return "Dynamic";
	case Staging:
		return "Staging";
	default:
		throw unify::Exception( "BufferUsage::ToString: Not a valid usage type!" );
	}
}