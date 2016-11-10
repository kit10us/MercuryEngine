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
		void Render( const me::RenderInfo & renderInfo );
		void Update( unify::Seconds elapsed );

	private:
		std::vector< Sprite > m_sprites;
	};
} // namespace dxi