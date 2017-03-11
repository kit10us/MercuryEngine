// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <MEWinMain.h>
#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Mesh.h>
#include <me/Terra.h>
#include <me/factory/EffectFactories.h>
#include <me/scene/GeometryComponent.h>

#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>

#include <TerrainMap.h>
#include <CameraMotivator.h>

using namespace me;

class Adventure : public me::Game
{
	TerrainMap::ptr m_map;
public:
	Adventure()
		: Game( "setup.xml" )
	{
	}

	void Startup() override;
	void Update( UpdateParams params ) override;
} game;

RegisterGame( game );

void Adventure::Startup()
{	  
	using namespace scene;
	
	// Add the main scene.
	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	Scene::ptr mainScene = sceneManager->AddScene( "main" );

	// Add Canvas component...
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( this ) );
	mainScene->AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( this, font2, canvas::Anchor::BottomLeft ) ) );

	// Create map...
	TerrainMap * map = new TerrainMap( { 30, 30 }, { 2, 2 } );
	m_map.reset( map );
	auto land = mainScene->NewObject( "land" );
	land->AddComponent( m_map );
	land->GetFrame().MoveBy( { -20 * 2 * 0.5f, 0, -20 * 2 * 0.5f } );

	// Reusable...
	Terra::Parameters parameters;
	parameters.SetSize( unify::Size< float >( 2, 2 ) );
	parameters.SetPoints( unify::RowColumn< unsigned int >( 10, 10 ) );
	parameters.SetConstant( 0.0f );	 


	// Load an effect, then modify it to fit our needs.
	{
		map->SetInvalidGeometry( GetManager< Geometry >()->Find( "invallid" ) );
	}

	// Add a grass ground...
	{
		GroundDesc::ptr ground( new GroundDesc{} );
		ground->AddDefault( GetManager< Geometry >()->Find( "grass" ) );
		map->AddGround( "grass", ground );
	}

	// Add a sand ground...
	{
		GroundDesc::ptr ground( new GroundDesc{} );
		ground->AddDefault( GetManager< Geometry >()->Find( "sand" ) );
		map->AddGround( "sand", ground );
	}

	for ( int i = 0; i < map->GetSize().height && i < map->GetSize().width; i++ )
	{
		int xp = i;
		int xn = map->GetSize().width - i - 1;
		int y = i;
		map->DrawOnMap( { xp, y }, "sand" );
		map->DrawOnMap( { xn, y }, "sand" );
	}


	// Add a camera...
	Object * camera = mainScene->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 17, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );			

	auto cameraMotivator = new CameraMotivator();
	auto im = dynamic_cast< motivator::InputMotivator* >(cameraMotivator);
	auto com = dynamic_cast< scene::IObjectComponent* >(cameraMotivator);

	camera->AddComponent( IObjectComponent::ptr( cameraMotivator ) );
}

void Adventure::Update( UpdateParams params )
{
}