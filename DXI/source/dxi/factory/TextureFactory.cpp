// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/factory/TextureFactory.h>
#include <dxi/exception/FailedToCreate.h>

using namespace dxi;

TextureSourceFactory::TextureSourceFactory( core::Game * game )
	: m_game( game )
{
}

Texture * TextureSourceFactory::Produce( unify::Path source )
{
	if( !source.Empty() )
	{
		return new Texture( source );
	}

	throw exception::FailedToCreate( "No valid factory option provided for texture!" );
}