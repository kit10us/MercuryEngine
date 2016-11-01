// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/scene/SceneManager.h>
#include <dxi/scene/CameraComponent.h>
#include <dxi/Mesh.h>
#include <dxi/Terra.h>
#include <dxi/factory/EffectFactories.h>
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>

/// <summary>
/// "Terra geometry creation"
/// goals and requirements:
/// * Example of the creation of Terra terrain geometry.
/// </summary>


using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	unify::Angle m_rotation;

public:
	void Startup();
	void Update( RenderInfo & renderInfo );
	void Render();
	void Shutdown();
} game;

RegisterGame( game );

void MyGame::Startup()
{
	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	scene::Scene::ptr mainScene = sceneManager->Add( "main" );

	scene::Object::ptr camera = mainScene->GetRoot()->AddChild( "camera" );
	camera->AddComponent( scene::IObjectComponent::ptr( new scene::CameraComponent( GetOS() ) ) );
	scene::CameraComponent * cameraComponent = unify::polymorphic_downcast< scene::CameraComponent * >( camera->GetComponent( "Camera" ).get() );
	cameraComponent->SetProjection(	unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, GetOS()->GetRenderer(0)->GetViewport().GetSize().AspectRatioWH(), 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 12, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// Add textures we will need for our effects, and terrain generation/modification.
	{
		Texture::TextureParameters parameters;
		parameters.renderable = true;
		parameters.lockable = true;
		parameters.min = Filtering::Point;
		parameters.mag = Filtering::Point;
		parameters.mip = Filtering::Point;
		GetManager< Texture >()->Add( "4", new Texture( GetOS()->GetRenderer( 0 ), "media/4.bmp", parameters ) );
	}

	// Load an effect, then modify it to fit our needs.
	Effect::ptr landEffect = GetManager< Effect >()->Add( "4", "media/EffectTextured.effect" );
	landEffect->SetTexture( 0, GetManager< Texture >()->Find( "4" ) );

	Terra::Parameters parameters;
	Terra * terra;
	parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
	parameters.SetEffect( GetManager< Effect >()->Find( "4" ) ); // Note the use of FindEffect to simplify interaction with the Effect Manager.
	parameters.SetPoints( unify::RowColumn< unsigned int >( 200, 140 ) );
	parameters.SetConstant( 0.0f );
	parameters.SetTexArea( unify::TexArea::Full() );

	//parameters.SetHeightMap( Terra::TextureOpMap( GetManager< Texture >()->Find( "land" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 0.7f, 0.25f, -1.0f ) ) );	
	parameters.SetHeightMap( Terra::TextureOpMap( GetManager< Texture >()->Find( "4" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 5.0f, 3.0f, 1.0f ) ) );

	//parameters.SetDiffuse( unify::ColorUnit::ColorUnitRed() );
	//parameters.SetDiffuses( unify::ColorUnit::ColorUnitBlack(), unify::ColorUnit::ColorUnitRed(), unify::ColorUnit::ColorUnitBlue(), unify::ColorUnit::ColorUnitWhite() );

	terra = new Terra( GetOS()->GetRenderer( 0 ), parameters );
	auto land = mainScene->GetRoot()->AddChild( "land" );
	land->SetGeometry( Geometry::ptr( terra ) );
}

void MyGame::Update( RenderInfo & renderInfo )
{
	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	scene::Object::ptr camera = sceneManager->Find( "main" )->FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}

void MyGame::Render()
{
}

void MyGame::Shutdown()
{
}
