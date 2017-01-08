// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <MEWinMain.h>

#include <me/scene/SceneManager.h>
#include <me/scene/CameraComponent.h>
#include <me/scene2d/CanvasComponent.h>
#include <me/scene2d/FPS.h>

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
		Scene::ptr scene = sceneManager->Find( "scene1" );

		scene2d::CanvasComponent::ptr canvas( new scene2d::CanvasComponent( this ) );
		scene->AddComponent( canvas );

		Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
		canvas->GetLayer()->AddElement( scene2d::IElement::ptr( new scene2d::FPS( this, font2 ) ) );
	}
} game;

RegisterGame( game );

