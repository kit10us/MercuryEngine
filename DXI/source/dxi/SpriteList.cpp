// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/SpriteList.h>

using namespace dxi;

SpriteList::SpriteList()
{
}

SpriteList::~SpriteList()
{
}

void SpriteList::AddSprite( const Sprite & sprite )
{
	m_sprites.push_back( sprite );
}

void SpriteList::Render( RenderInfo renderInfo )
{
	for( auto sprite : m_sprites )
	{
		sprite.Render( renderInfo );
	}
}

void SpriteList::Update( unify::Seconds elapsed )
{
	for( auto sprite : m_sprites )
	{
		sprite.Update( elapsed );
	}
} 
