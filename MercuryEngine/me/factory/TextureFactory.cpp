// Copyright (c) 2002 - 2018, Evil Quail LLC
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