// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/FPS.h>

using namespace me;
using namespace scene2d;

FPS::FPS( me::IGame * game, Effect::ptr effect, Anchor anchor, unify::V2< float > scale, unify::V2< float > offset )
	: TextElement( game, effect, std::string(), anchor, scale, offset )
{
}
		
void FPS::Update( IRenderer * renderer, const RenderInfo & renderInfo )
{
	SetText( unify::Cast< std::string >( renderInfo.GetFPS() ) );
	TextElement::Update( renderer, renderInfo );
}
		
