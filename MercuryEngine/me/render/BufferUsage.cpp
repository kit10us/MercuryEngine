// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/exception/FailedToCreate.h>
#include <me/exception/FailedToLock.h>
#include <me/exception/OutOfBounds.h>
#include <me/render/BufferUsage.h>
#include <unify/String.h>
#include <assert.h>

using namespace me;
using namespace render;

BufferUsage::TYPE BufferUsage::FromString( const std::string & usage )
{
	if ( unify::StringIs( usage, "Default" ) )
	{
		return Default;
	}
	else if ( unify::StringIs( usage, "Immutable" ) )
	{
		return Immutable;
	}
	else if ( unify::StringIs( usage, "Dynamic" ) )
	{
		return Dynamic;
	}
	else if ( unify::StringIs( usage, "Staging" ) ) 
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