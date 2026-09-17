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

unify::Result<ITexture::ptr> TextureSourceFactory::Produce( unify::Path source, unify::Parameters parameters )
{
	if( !source.IsEmpty() )
	{
		parameters.Set< std::string >( "source", source.ToString() );
		return m_game->GetOS()->GetRenderer(0)->ProduceT( parameters );
	}

	return unify::Failure{ "No valid factory option provided for texture!" };
}

unify::Result<ITexture::ptr> TextureSourceFactory::Produce( unify::Parameters parameters )
{
	return unify::Failure{ "Attempted to create texture from parameters." };
}