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
	std::for_each( m_sprites.begin(), m_sprites.end(), std::bind2nd( std::mem_fun_ref( &Sprite::Render ), renderInfo ) );
}

void SpriteList::Update( unify::Seconds elapsed )
{
	std::for_each( m_sprites.begin(), m_sprites.end(), std::bind2nd( std::mem_fun_ref( &Sprite::Update ), elapsed ) );
} 
