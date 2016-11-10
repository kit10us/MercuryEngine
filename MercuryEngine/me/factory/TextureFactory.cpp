// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/factory/TextureFactory.h>
#include <me/exception/FailedToCreate.h>

using namespace me;

TextureSourceFactory::TextureSourceFactory( me::IGame * game )
	: m_game( game )
{
}

ITexture::ptr TextureSourceFactory::Produce( unify::Path source, void * data )
{
	if( !source.Empty() )
	{
		TextureParameters parameters;
		if ( data )
		{
			parameters = *(TextureParameters*)data;
		}
		parameters.source = source;
		return m_game->GetOS()->GetRenderer(0)->ProduceT( parameters );
	}

	throw exception::FailedToCreate( "No valid factory option provided for texture!" );
}