// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/RenderState.h>
#include <dxi/pathing/Path.h>
#include <dxi/pathing/general/Split.h>
#include <dxi/pathing/general/Parallel.h>
#include <dxi/pathing/position/RotateAbout.h>
#include <dxi/pathing/orientation/LookAt.h>
#include <dxi/pathing/PathController.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	scene::Object::shared_ptr m_cameraObject;
	scene::Camera::shared_ptr m_camera;

	scene::Object::shared_ptr m_cube;
	scene::Object::shared_ptr m_orbitXLow;
	scene::Object::shared_ptr m_orbitXHigh;
	scene::Object::shared_ptr m_orbitYLow;
	scene::Object::shared_ptr m_orbitYHigh;
	scene::Object::shared_ptr m_orbitZLow;
	scene::Object::shared_ptr m_orbitZHigh;

	enum CameraControl
	{
		CameraControlOrbit,
		CameraControlFree
	} m_cameraControl;
	
	// CameraControlOrbit...
	bool m_rotateAboutX;
	bool m_rotateAboutY;
	bool m_rotateAboutZ;

public:

	void Startup()
	{
		Game::Startup();

		// Setup camera...
		// Create an object to use as a camera, and a camera interface. Set the projection to a default projection.
		m_cameraObject.reset( new scene::Object ); // Necessary to store the camera object -somewhere- outside of the camera, as is weak_ptr in camera.
		m_camera.reset( new scene::Camera( m_cameraObject ) );
		m_camera->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 100 ) );
		m_camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 4, -12 ) );
		m_camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

		m_cameraControl = CameraControlFree;

		// Color effect
		VertexDeclaration colorVD( FVF::XYZ | FVF::Diffuse ); 
		GetPixelShaderManager()->LoadFromFile( "color_3d", "media/shaders/color3d.shader", "ps_main", "ps_1_1" );
		GetVertexShaderManager()->LoadFromFile( "color_3d", "media/shaders/color3d.shader", "vs_main", "vs_1_1", colorVD );
		Effect::shared_ptr effect = GetEffectManager()->Add( "color_3d", new Effect );
		effect->SetPixelShader( GetPixelShaderManager()->Find( "color_3d" ) );
		effect->SetVertexShader( GetVertexShaderManager()->Find( "color_3d" ) );
		
		// Cube...
        shapes::CubeParameters cubeParameters;
		cubeParameters.SetEffect( effect );
        cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
        cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		m_cube.reset( new scene::Object( geo::Mesh::shared_ptr( new geo::Mesh( cubeParameters ) ) ) );

		// Balls...
		float distance = 3.0f;
        shapes::SphereParameters sphereParameters;
		sphereParameters.SetEffect( effect );
        sphereParameters.SetRadius( 0.30f );
        sphereParameters.SetDiffuse( unify::Color::ColorRed() );
		m_orbitXLow.reset( new scene::Object( geo::Mesh::shared_ptr( new geo::Mesh( sphereParameters ) ) , unify::V3< float >( -distance, 0, 0 ) ) );
		m_orbitXHigh.reset( new scene::Object( geo::Mesh::shared_ptr( new geo::Mesh( sphereParameters ) ) , unify::V3< float >( distance, 0, 0 ) ) );

		sphereParameters.SetDiffuse( unify::Color::ColorGreen() );
		m_orbitYLow.reset( new scene::Object( geo::Mesh::shared_ptr( new geo::Mesh( sphereParameters ) ) , unify::V3< float >( 0, -distance, 0 ) ) );
		m_orbitYHigh.reset( new scene::Object( geo::Mesh::shared_ptr( new geo::Mesh( sphereParameters ) ) , unify::V3< float >( 0, distance, 0 ) ) );

		sphereParameters.SetDiffuse( unify::Color::ColorBlue() );
		m_orbitZLow.reset( new scene::Object( geo::Mesh::shared_ptr( new geo::Mesh( sphereParameters ) ) , unify::V3< float >( 0, 0, -distance ) ) );
		m_orbitZHigh.reset( new scene::Object( geo::Mesh::shared_ptr( new geo::Mesh( sphereParameters ) ) , unify::V3< float >( 0, 0, distance ) ) );
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		m_orbitXLow->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) * unify::Angle::AnglePI2() );
		m_orbitXLow->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) ); // Test that the look at doesn't affect position.


		m_orbitXHigh->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) * unify::Angle::AnglePI2() );
		m_orbitYLow->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 0, 1 ), unify::Angle::AngleInRadians( elapsed ) * unify::Angle::AnglePI2() );
		m_orbitYHigh->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 0, 1 ), unify::Angle::AngleInRadians( elapsed ) * unify::Angle::AnglePI2() );
		m_orbitZLow->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 1 ), unify::Angle::AngleInRadians( elapsed ) * unify::Angle::AnglePI2() );
		m_orbitZHigh->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 1 ), unify::Angle::AngleInRadians( elapsed ) * unify::Angle::AnglePI2() );


		float modelMovementScalar = 1.0f;
		if ( input.KeyPressed( Key::Key0 ) )
		{
			m_cameraControl = CameraControlOrbit;
		}
		else if ( input.KeyPressed( Key::Key1 ) )
		{
			m_cameraControl = CameraControlFree;
		}

		switch( m_cameraControl )
		{
		case CameraControlOrbit:
			{
				if ( m_rotateAboutX || m_rotateAboutY || m_rotateAboutZ )
				{
					m_camera->GetObject()->GetFrame().RotateAbout( unify::V3< float >( m_rotateAboutX ? 1.0f : 0.0f, m_rotateAboutY ? 1.0f : 0.0f, m_rotateAboutZ ? 1.0f : 0.0f ), unify::Angle::AngleInRadians( elapsed ) );
					m_camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
				}
			}
			break;
		case CameraControlFree:
			{
				if ( input.KeyDown( Key::Left ) )
				{
					m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( -1, 0, 0 ), elapsed * modelMovementScalar );
				}
				else if ( input.KeyDown( Key::Right ) )
				{
					m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 1, 0, 0 ), elapsed * modelMovementScalar );
				}
				else if ( input.KeyDown( Key::A ) )
				{
					m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, 1, 0 ), elapsed * modelMovementScalar );
				}
				else if ( input.KeyDown( Key::Z ) )
				{
					m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, -1, 0 ), elapsed * modelMovementScalar );
				}
				else if ( input.KeyDown( Key::Up ) )
				{
					m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, 0, 1 ), elapsed * modelMovementScalar );
				}
				else if ( input.KeyDown( Key::Down ) )
				{
					m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, 0, -1 ), elapsed * modelMovementScalar );
				}

				m_camera->GetObject()->GetFrame().Rotate( unify::V3< float >( 0, 1.0f, 0 ), unify::Angle::AngleInRadians( input.MouseChangeX() *  0.01f ) );
				m_camera->GetObject()->GetFrame().Rotate( unify::V3< float >( 1.0f, 0, 0 ), unify::Angle::AngleInRadians( input.MouseChangeY() *  0.01f ) );
				m_camera->GetObject()->GetFrame().Rotate( unify::V3< float >( 0, 0, 1.0f ), unify::Angle::AngleInRadians( input.MouseChangeZ() ) );
			}
			break;
		}

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		RenderInfo renderInfo;
		renderInfo.SetFinalMatrix( m_camera->GetMatrix() );

		m_cube->Render( renderInfo );
		m_orbitXLow->Render( renderInfo );
		m_orbitXHigh->Render( renderInfo );
		m_orbitYLow->Render( renderInfo );
		m_orbitYHigh->Render( renderInfo );
		m_orbitZLow->Render( renderInfo );
		m_orbitZHigh->Render( renderInfo );
	}

	void Shutdown()
	{
		Game::Shutdown();
	}
} game;

