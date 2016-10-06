// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/scene/Camera.h>
#include <dxi/Mesh.h>
#include <dxi/GeometryGroup.h>
#include <dxi/Terra.h>
#include <dxi/factory/EffectFactories.h>
#include <dxi/win/DXILib.h>

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
	GeometryGroup m_geometryGroup;
	unify::Angle m_rotation;
	scene::Object::shared_ptr m_cameraObject;
	scene::Camera::shared_ptr m_camera;

public:
	void Startup();
	bool Update( RenderInfo & renderInfo, IInput & input );
	void Render();
	void Shutdown();
} game;


void MyGame::Startup()
{
	Game::Startup();

	// Setup camera...
	m_cameraObject.reset( new scene::Object );
	m_camera.reset( new scene::Camera );
	m_camera->SetObject( m_cameraObject );
	m_camera->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );

	// Add textures we will need for our effects, and terrain generation/modification.
	GetManager< Texture >()->Add( "land", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
	GetManager< Texture >()->Add( "cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

	// Load an effect, then modify it to fit our needs.
	Effect::ptr landEffect = GetManager< Effect >()->Add( "land", "media/EffectTextured.xml" );
	landEffect->SetBlend( Blend( Usage::False ) );
	landEffect->SetTexture( 0, GetManager< Texture >()->Find( "land" ) );

	Effect::ptr cutoutEffect = GetManager< Effect >()->Add( "cutout", "media/EffectTextured.xml" );
	cutoutEffect->SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
	cutoutEffect->SetTexture( 0, GetManager< Texture >()->Find( "land" ) );

	// At this point the same effect has been loaded twice, modified to have different blending modes for the land and cutout effect.
	// Note that the same texture is shared, and internally all other possible assets are shared, such as shaders.

	Terra::Parameters parameters;
	Terra * terra;

    parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
    parameters.SetEffect( GetManager< Effect >()->Find( "land" ) ); // Note the use of FindEffect to simplify interaction with the Effect Manager.
    parameters.SetRowsColumns( unify::RowColumn< unsigned int >( 200, 140 ) );
    parameters.SetConstant( 0.0f );
    parameters.SetTexArea( unify::TexArea::Full() );
	parameters.SetHeightMap( Terra::TextureOpMap( GetManager< Texture >()->Find( "land" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 0.7f, 0.25f, -1.0f ) ) );
	terra = new Terra( parameters );
	m_geometryGroup.Add( Geometry::shared_ptr( terra ) );

	parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
    parameters.SetEffect( GetManager< Effect >()->Find( "cutout" ) );
    parameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
    parameters.SetConstant( 0.1f );
    parameters.SetTexArea( unify::TexArea::Full() );
	parameters.SetAlphaMap( Terra::TextureOpMap( GetManager< Texture >()->Find( "cutout" ), unify::ColorUnit::ColorUnitRed( 1.0f, 0.0f ) ) );
	terra = new Terra( parameters );
	m_geometryGroup.Add( Geometry::shared_ptr( terra ) );
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	unify::V3< float > eyePosition( 0, 8, 7 );
	unify::Matrix rotation = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ), m_rotation );
	rotation.TransformCoord( eyePosition );
	unify::Matrix view = unify::Matrix::MatrixLookAtLH( eyePosition, unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	m_camera->GetObject()->GetFrame().SetMatrix( view );

	m_rotation += unify::Angle::AngleInRadians( elapsed );

	return Game::Update( renderInfo, input );
}

void MyGame::Render()
{
	RenderInfo renderInfo;
	renderInfo.SetFinalMatrix( m_camera->GetMatrix() );
	m_geometryGroup.Render( renderInfo, 0 );
}

void MyGame::Shutdown()
{
	m_geometryGroup.Clear();
	Game::Shutdown();
}
