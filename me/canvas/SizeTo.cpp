// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/canvas/SizeTo.h>
#include <unify/Exception.h>

using namespace me::canvas;

SizeTo::SizeTo( Type type, float value )
	: type{ type }
	, value{ value }
{
}

float SizeTo::GetSize( float parentSize, size_t totalWeight ) const
{
	switch ( type )
	{
	case Type::Pixels:
		return value;
	case Type::Percent:
		return parentSize * value;
	case Type::Weighted:
		return (float)totalWeight / value * parentSize;
	default:
		throw unify::Exception( "Invalied canvas SizeTo type!" );
	}
}
