// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/scene/Camera.h>
#include <dxi/geo/Mesh.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/geo/Terra.h>
#include <dxi/loader/EffectLoader.h>

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
	geo::GeometryGroup m_geometryGroup;
	unify::Angle m_rotation;
	scene::Object::shared_ptr m_cameraObject;
	scene::Camera::shared_ptr m_camera;

public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
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
	GetTextureManager()->Add( "land", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
	GetTextureManager()->Add( "cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

	// Load an effect, then modify it to fit our needs.
	Effect::shared_ptr landEffect = loader::EffectLoader( "land", "media/EffectTextured.xml", GetManagers() );
	landEffect->SetBlend( Blend( Usage::False ) );
	landEffect->SetTexture( 0, Find< Texture >( "land" ) );

	Effect::shared_ptr cutoutEffect = loader::EffectLoader( "cutout", "media/EffectTextured.xml", GetManagers() );
	cutoutEffect->SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
	cutoutEffect->SetTexture( 0, Find< Texture >( "land" ) );

	// At this point the same effect has been loaded twice, modified to have different blending modes for the land and cutout effect.
	// Note that the same texture is shared, and internally all other possible assets are shared, such as shaders.

	geo::Terra::Parameters parameters;
	geo::Terra * terra;

    parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
    parameters.SetEffect( Find< Effect >( "land" ) ); // Note the use of FindEffect to simplify interaction with the Effect Manager.
    parameters.SetRowsColumns( unify::RowColumn< unsigned int >( 200, 140 ) );
    parameters.SetConstant( 0.0f );
    parameters.SetTexArea( unify::TexArea::Full() );
	parameters.SetHeightMap( geo::Terra::TextureOpMap( Find< Texture >( "land" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 0.7f, 0.25f, -1.0f ) ) );
	terra = new geo::Terra( parameters );
	m_geometryGroup.Add( geo::Geometry::shared_ptr( terra ) );

	parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
    parameters.SetEffect( Find< Effect >( "cutout" ) );
    parameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
    parameters.SetConstant( 0.1f );
    parameters.SetTexArea( unify::TexArea::Full() );
	parameters.SetAlphaMap( geo::Terra::TextureOpMap( Find< Texture >( "cutout" ), unify::ColorUnit::ColorUnitRed( 1.0f, 0.0f ) ) );
	terra = new geo::Terra( parameters );
	m_geometryGroup.Add( geo::Geometry::shared_ptr( terra ) );
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	unify::V3< float > eyePosition( 0, 8, 7 );
	unify::Matrix rotation = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ), m_rotation );
	rotation.TransformCoord( eyePosition );
	unify::Matrix view = unify::Matrix::MatrixLookAtLH( eyePosition, unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	m_camera->GetObject()->GetFrame().SetModelMatrix( view );

	m_rotation += unify::Angle::AngleInRadians( elapsed );

	return Game::Update( elapsed, input );
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
