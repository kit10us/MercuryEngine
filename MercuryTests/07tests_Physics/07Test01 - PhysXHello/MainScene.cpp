// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>

using namespace me;
using namespace scene;

MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
{	
}

void MainScene::OnStart()
{
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( GetGame(), font2 ) ) );
}
