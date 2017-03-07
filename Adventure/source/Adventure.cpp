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
		TextureParameters parameters;
		parameters.renderable = true;
		parameters.lockable = true;
		parameters.min = Filtering::Point;
		parameters.mag = Filtering::Point;
		parameters.mip = Filtering::Point;
		GetManager< ITexture >()->Add( "4", "4.bmp", "", &parameters );
	}

	// Load an effect, then modify it to fit our needs.
	Effect::ptr landEffect = GetManager< Effect >()->Add( "4", "EffectTextured.effect" );
	landEffect->SetTexture( 0, GetManager< ITexture >()->Find( "4" ) );

	Terra::Parameters parameters;
	Terra * terra;
	parameters.Reset();
	parameters.SetSize( unify::Size< float >( 2, 2 ) );
	parameters.SetEffect( GetManager< Effect >()->Find( "4" ) ); // Note the use of FindEffect to simplify interaction with the Effect Manager.
	parameters.SetPoints( unify::RowColumn< unsigned int >( 10, 10 ) );
	parameters.SetConstant( 0.0f );
	parameters.SetTexArea( unify::TexAreaFull() );

	terra = new Terra( GetOS()->GetRenderer( 0 ), parameters );

	// Add the main scene.
	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	Scene::ptr mainScene = sceneManager->AddScene( "main" );

	// Add a camera...
	Object * camera = mainScene->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent( GetOS() ) ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );			

	auto cameraMotivator = new CameraMotivator( GetOS() );
	auto im = dynamic_cast< motivator::InputMotivator* >(cameraMotivator);
	auto com = dynamic_cast< scene::IObjectComponent* >(cameraMotivator);

	camera->AddComponent( IObjectComponent::ptr( new CameraMotivator( GetOS() ) ) );

	TerrainMap * map = new TerrainMap( GetOS(), { 10, 5 }, { 2, 2 } );
	m_map.reset( map );
	map->SetInvalidGeometry( Geometry::ptr( terra ) );
	auto land = mainScene->NewObject( "land" );
	land->AddComponent( m_map );
}

void Adventure::Update( UpdateParams params )
{
}