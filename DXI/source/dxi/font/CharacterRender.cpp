// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/CharacterRender.h>

using namespace dxi;
using namespace font;

CharacterRender::CharacterRender()
{
}

CharacterRender::CharacterRender( const Character & character, const unify::V2< float > & offset, float scale )
: m_character( character )
, m_offset( offset )
, m_scale( scale )
, m_size( character.GetSize().width * scale, character.GetSize().height * scale )
{
}

CharacterRender::~CharacterRender() throw ()
{
}

const unify::Size< float > & CharacterRender::GetSize() const
{
	return m_size;
}

void CharacterRender::Update( unify::Seconds elapsed )
{
	m_character.Update( elapsed );
}

void CharacterRender::Render( RenderInfo renderInfo, const unify::Matrix & origin, bool is3D )
{
	m_character.Render( renderInfo, origin, m_offset, m_scale, is3D );
}
