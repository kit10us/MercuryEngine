// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/TextureMode.h>

using namespace me;

TextureMode::TYPE TextureMode::FromString( std::string mode )
{
	if( unify::StringIs( mode, "CORRECT" ) )
	{
		return Correct;
	}
	else if( unify::StringIs( mode, "WRAPPED" ) )
	{
		return Wrapped;
	}
	assert( 0 );
	throw 0;
}