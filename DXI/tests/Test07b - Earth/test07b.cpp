// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
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
	bool Update( RenderInfo & renderInfo );
	void Render();
	void Shutdown();
} game;

RegisterGame( game );

void MyGame::Startup()
{
	Game::Startup();

	scene::Scene::ptr mainScene = GetSceneManager()->Add( "main" );

	scene::Object::ptr camera = mainScene->GetRoot()->AddChild( "camera" );
	camera->AddComponent( scene::IObjectComponent::ptr( new scene::CameraComponent( GetOS() ) ) );
	scene::CameraComponent * cameraComponent = unify::polymorphic_downcast< scene::CameraComponent * >( camera->GetComponent( "Camera" ).get() );

	cameraComponent->SetProjection(
		unify::Matrix::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f,
			800 / 600
			, 1, 1000 ) );

	camera->GetFrame().SetPosition( unify::V3< float >( 0, 12, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
	
	// Add textures we will need for our effects, and terrain generation/modification.
	GetManager< Texture >()->Add( "land", new Texture( GetOS()->GetRenderer(0), "media/earthb.bmp", true, true ) );
	GetManager< Texture >()->Add( "cutout", new Texture( GetOS()->GetRenderer( 0 ), "media/EarthWaterCutoutMap.bmp", true, true ) );

	// Load an effect, then modify it to fit our needs.
	Effect::ptr landEffect = GetManager< Effect >()->Add( "land", "media/EffectTextured.effect" );
	//landEffect->SetBlend( Blend( Usage::False ) );
	landEffect->SetTexture( 0, GetManager< Texture >()->Find( "land" ) );

	Effect::ptr cutoutEffect = GetManager< Effect >()->Add( "cutout", "media/EffectTextured.effect" );
	//cutoutEffect->SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
	cutoutEffect->SetTexture( 0, GetManager< Texture >()->Find( "land" ) );

	// At this point the same effect has been loaded twice, modified to have different blending modes for the land and cutout effect.
	// Note that the same texture is shared, and internally all other possible assets are shared, such as shaders.

	Terra::Parameters parameters;
	Terra * terra;

    parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
    parameters.SetEffect( GetManager< Effect >()->Find( "land" ) ); // Note the use of FindEffect to simplify interaction with the Effect Manager.
    parameters.SetPoints( unify::RowColumn< unsigned int >( 200, 140 ) );
    parameters.SetConstant( 0.0f );
    parameters.SetTexArea( unify::TexArea::Full() );
	parameters.SetHeightMap( Terra::TextureOpMap( GetManager< Texture >()->Find( "land" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 0.7f, 0.25f, -1.0f ) ) );	
	//parameters.SetDiffuse( unify::ColorUnit::ColorUnitRed() );
	//parameters.SetDiffuses( unify::ColorUnit::ColorUnitBlack(), unify::ColorUnit::ColorUnitRed(), unify::ColorUnit::ColorUnitBlue(), unify::ColorUnit::ColorUnitWhite() );
	
	terra = new Terra( GetOS()->GetRenderer( 0 ), parameters );
	auto land = mainScene->GetRoot()->AddChild( "land" );
	land->SetGeometry( Geometry::ptr( terra ) );

	/*
	parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
    parameters.SetEffect( GetManager< Effect >()->Find( "cutout" ) );
    parameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
    parameters.SetConstant( 0.1f );
    parameters.SetTexArea( unify::TexArea::Full() );
	parameters.SetAlphaMap( Terra::TextureOpMap( GetManager< Texture >()->Find( "cutout" ), unify::ColorUnit::ColorUnitRed( 1.0f, 0.0f ) ) );
	terra = new Terra( GetOS()->GetRenderer( 0 ), parameters );
	auto cutout = mainScene->GetRoot()->AddChild( "cutout" );
	cutout->SetGeometry( Geometry::ptr( terra ) );
	*/
}

bool MyGame::Update( RenderInfo & renderInfo )
{
	scene::Object::ptr camera = GetSceneManager()->Find( "main" )->FindObject( "camera" );
	//camera->GetFrame().RotateAbout( unify::V3< float >( 0, 1, 0 ), unify::Angle::AngleInRadians( renderInfo.GetDelta() ) );
	return true;
}

void MyGame::Render()
{
}

void MyGame::Shutdown()
{
}
