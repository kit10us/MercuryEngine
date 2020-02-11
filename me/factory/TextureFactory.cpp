// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/factory/TextureFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/render/IRenderer.h>

using namespace me;
using namespace render;

TextureSourceFactory::TextureSourceFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
{
}

ITexture::ptr TextureSourceFactory::Produce( unify::Path source, unify::Parameters parameters )
{
	if( !source.Empty() )
	{
		parameters.Set< std::string >( "source", source.ToString() );
		return m_game->GetOS()->GetRenderer(0)->ProduceT( parameters );
	}

	throw exception::FailedToCreate( "No valid factory option provided for texture!" );
}

ITexture::ptr TextureSourceFactory::Produce( unify::Parameters parameters )
{
	throw me::exception::FailedToCreate( "Attempted to create texture from parameters." );
}