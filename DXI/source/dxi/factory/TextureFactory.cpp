// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/factory/TextureFactory.h>
#include <dxi/exception/FailedToCreate.h>

using namespace dxi;

TextureSourceFactory::TextureSourceFactory( core::IGame * game )
	: m_game( game )
{
}

Texture::ptr TextureSourceFactory::Produce( unify::Path source, void * data )
{
	if( !source.Empty() )
	{
		Texture::TextureParameters parameters;
		if ( data )
		{
			parameters = *(Texture::TextureParameters*)data;
		}
		return Texture::ptr( new Texture( m_game->GetOS()->GetRenderer(0), source, parameters ) );
	}

	throw exception::FailedToCreate( "No valid factory option provided for texture!" );
}