// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/Format.h>
#include <unify/String.h>

using namespace me;
using namespace render;

Format::TYPE Format::FromString( std::string format )
{
	using unify::string::StringIs;

	if ( StringIs( format, "FloatR32G32B32A32" ) )
	{
		return Format::FloatR32G32B32A32;
	}
	
	throw unify::Exception( "Invalid Format \"" + format + "\"!" );
}

std::string Format::ToString( TYPE format )
{
	switch( format )
	{
	case Format::FloatR32G32B32A32:
		return "FloatR32G32B32A32";
	}

	throw unify::Exception( "Invalid Format!" );
}
