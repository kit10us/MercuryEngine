// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/canvas/FPS.h>
#include <me/exception/FailedToCreate.h>

using namespace me;
using namespace canvas;
using namespace render;

FPS::FPS( me::game::IGame * gameInstance, Effect::ptr effect, Anchor anchor, unify::V2< float > scale, unify::V2< float > offset )
	: TextElement( gameInstance, effect, std::string(), anchor, scale, offset )
	, m_updateRate( 0.25f )
	, m_secondsTillUpdate( 0.0f )
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
	m_secondsTillUpdate -= params.renderInfo.GetDelta();
	if ( m_secondsTillUpdate > 0.0f ) return;
	
	m_secondsTillUpdate = m_updateRate;

	std::string average = unify::Cast< std::string >( params.renderInfo.GetFPS() );
	size_t p = average.find( '.' );
	average = average.substr( 0, p < 3 ? 5 : p + 2 );

	std::string instant = unify::Cast< std::string >( 1.0f / params.renderInfo.GetDelta() );
	p = instant.find( '.' );
	instant = instant.substr( 0, p < 3 ? 5 : p + 2 );

	SetText( instant + "/" + average );
	TextElement::Update( params );
}
		
