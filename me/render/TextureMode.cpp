// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/TextureMode.h>

using namespace me;
using namespace render;

TextureMode::TYPE TextureMode::FromString( std::string mode )
{
	if( unify::String::StringIs( mode, "CORRECT" ) )
	{
		return Correct;
	}
	else if( unify::String::StringIs( mode, "WRAPPED" ) )
	{
		return Wrapped;
	}
	else
	{
		throw 0; // Should never get here. Throw for uncaught exception.
	}
}