// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Sprite.h>

namespace dxi
{
	class SpriteList
	{
	public:
		SpriteList();
		~SpriteList() throw ();
		void AddSprite( const Sprite & sprite );
		void Update( unify::Seconds elapsed );
		void Render( const me::IRenderer * renderer, const me::RenderInfo & renderInfo );

	private:
		std::vector< Sprite > m_sprites;
	};
} // namespace dxi