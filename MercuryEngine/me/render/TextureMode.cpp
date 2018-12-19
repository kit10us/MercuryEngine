// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/TextureMode.h>

using namespace me;
using namespace render;

TextureMode::TYPE TextureMode::FromString( std::string mode )
{
	if( unify::string::StringIs( mode, "CORRECT" ) )
	{
		return Correct;
	}
	else if( unify::string::StringIs( mode, "WRAPPED" ) )
	{
		return Wrapped;
	}
	assert( 0 );
	throw 0;
}