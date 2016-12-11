// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/FPS.h>

using namespace me;
using namespace scene2d;

FPS::FPS( me::IGame * game, Effect::ptr effect, Anchor anchor, unify::V2< float > scale, unify::V2< float > offset )
	: TextElement( game, effect, std::string(), anchor, scale, offset )
	, m_updateRate( 0.25f )
	, m_secondsTillUpdate( 0.0f )
{
}
		
void FPS::Update( IRenderer * renderer, const RenderInfo & renderInfo )
{
	m_secondsTillUpdate -= renderInfo.GetDelta();
	if ( m_secondsTillUpdate > 0.0f ) return;
	
	m_secondsTillUpdate = m_updateRate;

	std::string average = unify::Cast< std::string >( renderInfo.GetFPS() );
	size_t p = average.find( '.' );
	average = average.substr( 0, p < 3 ? 5 : p + 2 );

	std::string instant = unify::Cast< std::string >( 1.0f / renderInfo.GetDelta() );
	p = instant.find( '.' );
	instant = instant.substr( 0, p < 3 ? 5 : p + 2 );

	SetText( instant + "/" + average );
	TextElement::Update( renderer, renderInfo );
}
		
