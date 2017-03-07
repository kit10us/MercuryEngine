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

#include <me/scene2d/CanvasComponent.h>
#include <me/scene2d/FPS.h>

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

	// Add textures we will need for our effects, and terrain generation/modification.
	{
		GetManager< ITexture >()->Add( "grass", "grass01.jpg", "" );
	}
	
	// Add the main scene.
	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	Scene::ptr mainScene = sceneManager->AddScene( "main" );

	// Add 2d scene...
	// Add Canvas component...
	scene2d::CanvasComponent::ptr canvas( new scene2d::CanvasComponent( this ) );
	mainScene->AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
	canvas->GetLayer()->AddElement( scene2d::IElement::ptr( new scene2d::FPS( this, font2, scene2d::Anchor::BottomLeft ) ) );

	// Create map...
	TerrainMap * map = new TerrainMap( GetOS(), { 30, 30 }, { 2, 2 } );
	m_map.reset( map );
	auto land = mainScene->NewObject( "land" );
	land->AddComponent( m_map );

	// Load an effect, then modify it to fit our needs.
	{
		Effect::ptr effect = GetManager< Effect >()->Add( "grass", "EffectTextured.effect" );
		effect->SetTexture( 0, GetManager< ITexture >()->Find( "grass" ) );

		Terra::Parameters parameters;
		Terra * terra;
		parameters.Reset();
		parameters.SetSize( unify::Size< float >( 2, 2 ) );
		parameters.SetEffect( effect );
		parameters.SetPoints( unify::RowColumn< unsigned int >( 10, 10 ) );
		parameters.SetConstant( 0.0f );
		parameters.SetTexArea( unify::TexAreaFull() );
		terra = new Terra( GetOS()->GetRenderer( 0 ), parameters );
		map->SetInvalidGeometry( Geometry::ptr( terra ) );
	}

	// Add a camera...
	Object * camera = mainScene->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent( GetOS() ) ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 17, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );			

	auto cameraMotivator = new CameraMotivator( GetOS() );
	auto im = dynamic_cast< motivator::InputMotivator* >(cameraMotivator);
	auto com = dynamic_cast< scene::IObjectComponent* >(cameraMotivator);

	camera->AddComponent( IObjectComponent::ptr( new CameraMotivator( GetOS() ) ) );
}

void Adventure::Update( UpdateParams params )
{
}