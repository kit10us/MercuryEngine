// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "Generated Mesh and Camera"
/// goals and requirements:
/// * Illustrate the use of higher level objects, yet avoid higher level scene management.
/// * A major goal should be in reduction of size and complexity, with exception to the actual object creation - in other words, 
///   do use simplified loading of sub-components, like textures and effects, to reduce the complexity of code, do NOT use
///   simplified 3D object creation (such as constructing shapes from XML), so we can focus on how the 3D geometry is configured
///   yet avoid subjects covered in previous tests (like Effects).
/// </summary>


#include <dxi/core/Game.h>
#include <dxi/RenderState.h>
#include <dxi/factory/EffectFactories.h>
#include <dxi/Mesh.h>
#include <dxi/factory/ShapeFactory.h>
#include <dxi/pathing/Path.h>
#include <dxi/pathing/general/Split.h>
#include <dxi/pathing/general/Parallel.h>
#include <dxi/pathing/position/RotateAbout.h>
#include <dxi/pathing/orientation/LookAt.h>
#include <dxi/pathing/PathController.h>
#include <dxi/win/DXILib.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	unify::Angle m_rotation;
	scene::Object::shared_ptr m_cameraObject;
	scene::Camera::shared_ptr m_camera;

	dxi::Mesh::shared_ptr m_mesh;
	unify::V3< float > m_position[ 2 ];
	unify::Quaternion m_axis[ 3 ];

public:

	void Startup()
	{
		Game::Startup();

		// Setup camera...
		// Create an object to use as a camera, and a camera interface. Set the projection to a default projection.
		m_cameraObject.reset( new scene::Object ); // Necessary to store the camera object -somewhere- outside of the camera, as is weak_ptr in camera.
		m_camera.reset( new scene::Camera( m_cameraObject ) );
		m_camera->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );
		m_camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 0, 10 ) );

		// Load effect
		Effect::shared_ptr effect = GetManager< Effect >()->Add( "color_3d", "media/EffectColor.xml" );
		
		// Cube
        shapes::CubeParameters cubeParameters;
		cubeParameters.SetEffect( effect );
        cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
        cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		m_mesh = GetManager< Geometry >()->Add( "cube", dxi::shapes::CreateShape( cubeParameters ) );
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		unify::V3< float > eyePosition( 0, 0, -7 );
		unify::Matrix rotation = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ), m_rotation );
		rotation.TransformCoord( eyePosition );
		unify::Matrix view = unify::Matrix::MatrixLookAtLH( eyePosition, unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
		m_camera->GetObject()->GetFrame().SetMatrix( view );

		m_rotation += unify::Angle::AngleInRadians( elapsed );
		m_rotation.Fix360();

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		RenderInfo renderInfo;
		renderInfo.SetFinalMatrix( m_camera->GetMatrix() );

		m_mesh->Render( renderInfo, 0 );
	}

	void Shutdown()
	{
		Game::Shutdown();
	}
} game;

