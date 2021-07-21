// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/canvas/FPS.h>
#include <me/exception/FailedToCreate.h>

using namespace me::game;
using namespace me::canvas;
using namespace me::render;

FPS::FPS( IGame * gameInstance, Effect::ptr effect, Anchor anchor, unify::V2< float > scale, unify::V2< float > offset )
	: TextElement( gameInstance, effect, std::string(), anchor, scale, offset )
	, m_updateRate{ unify::TimeDeltaInSeconds( 0.25f ) }
	, m_timeTillUpdate{ unify::TimeDelta() }
{
	if ( !effect->GetTexture( 0 )->GetSpriteDictionary().HasAscii( '=' )
		|| !effect->GetTexture( 0 )->GetSpriteDictionary().HasAscii( '.' )
		|| !effect->GetTexture( 0 )->GetSpriteDictionary().HasAsciiNumeric() )
	{
		throw me::exception::FailedToCreate(
			"Specified font, \"" + effect->GetTexture( 0 )->GetParameters()->source.ToString()
			+ "\", is missing too many characters to be used with FPS Canvas element! (requires '=', '.', and 0 to 9)" 
		);
	}
}
		
void FPS::Update( const UpdateParams & params )
{
	m_timeTillUpdate -= params.renderInfo.GetDelta();
	if (m_timeTillUpdate > unify::TimeDelta())
	{
		return;
	}
	
	m_timeTillUpdate = m_updateRate;

	std::string average = unify::Cast< std::string >( params.renderInfo.GetFPS() );
	size_t p = average.find( '.' );
	average = average.substr( 0, p < 3 ? 5 : p + 2 );

	std::string instant = unify::Cast< std::string >( 1.0f / params.renderInfo.GetDelta().GetSeconds() );
	p = instant.find( '.' );
	instant = instant.substr( 0, p < 3 ? 5 : p + 2 );

	SetText( instant + "/" + average );
	TextElement::Update( params );
}
		
