// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <MEWinMain.h>

#include <me/scene/SceneManager.h>
#include <me/object/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>

class MyGame : public me::Game
{
public:
	MyGame()
		: Game( "setup_physx.xml" )
	{
	}

	void Startup() override
	{
		using namespace me;
		using namespace scene;
		
		SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
		Scene::ptr scene = sceneManager->FindScene( "scene1" );

		canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( this ) );
		scene->AddComponent( canvas );

		Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
		canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( this, font2 ) ) );
	}
} game;

RegisterGame( game );

