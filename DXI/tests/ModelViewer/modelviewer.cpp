// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/loader/MeshLoader.h>
#include <dxi/loader/EffectLoader.h>
#include <dxi/dae/util/IEffectSolver.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	scene::Object::shared_ptr m_cameraObject;
	scene::Camera::shared_ptr m_camera;
	qlib::file::Path m_meshSource;
	scene::Object::shared_ptr m_mesh;
	unify::Angle m_rotation;
	float m_scale;
	float m_modelMovementScalar;

	enum CameraControl
	{
		CameraControlOrbit,
		CameraControlFree
	} m_cameraControl;
	
	// CameraControlOrbit...
	bool m_rotateAboutX;
	bool m_rotateAboutY;
	bool m_rotateAboutZ;

	boost::shared_ptr< class MyEffectSolver > m_effectSolver;

public:
	void Startup();
	void LoadMesh();
	void OnDragDrop( const std::vector< qlib::file::Path > & files, const unify::V2< float > & point );
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;

class MyEffectSolver : public dae::util::IEffectSolver
{
	Effect::shared_ptr m_color;
	Effect::shared_ptr m_textured;
public:

	MyEffectSolver( Game & game )
	{
		m_color = loader::EffectLoader( "color", "media/EffectColorN.xml", game.GetManagers() );
		m_textured = loader::EffectLoader( "textured", "media/EffectTextured.xml", game.GetManagers() );
	}

	dxi::Effect::shared_ptr GetEffect( const dae::Shading & shading ) const
	{
		if ( shading.GetDiffuse().GetType() == dae::Shading::Property::ColorType )
		{
			return m_color;
		}
		else
		{
			return m_textured;
		}
	}

	Effect & GetColorEffect()
	{
		return *m_color.get();
	}

	Effect & GetTexturedEffect()
	{
		return *m_textured.get();
	}
};


void MyGame::Startup()
{
	Game::Startup();

	// Setup camera...
	// Create an object to use as a camera, and a camera interface. Set the projection to a default projection.
	m_cameraObject.reset( new scene::Object ); // Necessary to store the camera object -somewhere- outside of the camera, as is weak_ptr in camera.
	m_camera.reset( new scene::Camera( m_cameraObject ) );
	m_camera->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );
	m_camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 1, 13 ) );
	m_camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
	m_cameraControl = CameraControlOrbit;
	m_rotateAboutX = false;
	m_rotateAboutY = true;

	if ( GetOS().GetCommandLine().size() != 0 )
	{
		m_meshSource = GetOS().GetCommandLine().at( 0 );
		LoadMesh();
	}
}

void MyGame::LoadMesh()
{
	if ( ! m_meshSource.empty() )
	{
		// Unload all current assests to ensure reloading is complete (in case assets have changed externally).
		m_mesh.reset();
		m_effectSolver.reset();
		GetTextureManager()->Clear();
		GetPixelShaderManager()->Clear();
		GetVertexShaderManager()->Clear();

		m_effectSolver.reset( new MyEffectSolver( *this ) ); // Force reloading effect and shaders.

		m_mesh.reset( new scene::Object( geo::Geometry::shared_ptr( loader::LoadMesh( m_meshSource, m_effectSolver.get(), GetManagers() ) ), unify::V3< float >( 0, 0, 0 ) ) );
		m_scale = 3 / m_mesh->GetGeometry()->ComputeBounds().Size().Length();
		m_mesh->SetGeometryMatrix( unify::Matrix::MatrixScale( m_scale ) );
		m_modelMovementScalar = m_mesh->GetGeometry()->ComputeBounds().Size().Length() / 100.0f;
	}
}

void MyGame::OnDragDrop( const std::vector< qlib::file::Path > & files, const unify::V2< float > & point )
{
	m_meshSource = files.at( 0 );
	LoadMesh();
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	if ( m_mesh )
	{
		if ( input.KeyPressed( Key::Key1 ) )
		{
			m_rotateAboutX = ! m_rotateAboutX;
		}
		else if ( input.KeyPressed( Key::Key2 ) )
		{
			m_rotateAboutY = ! m_rotateAboutY;
		}
		else if ( input.KeyPressed( Key::Key3 ) )
		{
			m_rotateAboutZ = ! m_rotateAboutZ;
		}
		if ( m_rotateAboutX || m_rotateAboutY || m_rotateAboutZ )
		{
			m_mesh->GetFrame().Rotate( unify::V3< float >( m_rotateAboutX ? 1.0f : 0.0f, m_rotateAboutY ? 1.0f : 0.0f, m_rotateAboutZ ? 1.0f : 0.0f ), unify::Angle::AngleInRadians( elapsed ) );
		}

		if ( input.KeyDown( Key::Left ) || input.KeyDown( Key::A ) )
		{
			m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( -1, 0, 0 ), elapsed * m_modelMovementScalar );
		}
		else if ( input.KeyDown( Key::Right ) || input.KeyDown( Key::D ) )
		{
			m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 1, 0, 0 ), elapsed * m_modelMovementScalar );
		}
		if ( input.KeyDown( Key::LShift ) )
		{
			m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, 1, 0 ), elapsed * m_modelMovementScalar );
		}
		else if ( input.KeyDown( Key::LCtrl ) )
		{
			m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, -1, 0 ), elapsed * m_modelMovementScalar );
		}
		if ( input.KeyDown( Key::Up ) || input.KeyDown( Key::W ) )
		{
			m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, 0, 1 ), elapsed * m_modelMovementScalar );
		}
		else if ( input.KeyDown( Key::Down ) || input.KeyDown( Key::S ) )
		{
			m_camera->GetObject()->GetFrame().Slide( unify::V3< float >( 0, 0, -1 ), elapsed * m_modelMovementScalar );
		}

		//m_camera->GetObject()->GetFrame().Rotate( unify::V3< float >( 0, 1.0f, 0 ), unify::Angle::AngleInRadians( input.MouseChangeX() * 0.001f ) );
		//m_camera->GetObject()->GetFrame().Rotate( unify::V3< float >( 1.0f, 0, 0 ), unify::Angle::AngleInRadians( input.MouseChangeY() * 0.001f ) );
		//m_camera->GetObject()->GetFrame().Rotate( unify::V3< float >( 0, 0, 1.0f ), unify::Angle::AngleInRadians( input.MouseChangeZ() * 0.001f ) );

		if ( input.KeyDown( Key::Minus ) )
		{
			m_mesh->SetGeometryMatrix( m_mesh->GetGeometryMatrix() * unify::Matrix::MatrixScale( 0.99f ) );
		}
		else if ( input.KeyDown( Key::Equals ) )
		{
			m_mesh->SetGeometryMatrix( m_mesh->GetGeometryMatrix() * unify::Matrix::MatrixScale( 1.01f ) );
		}

		{
			Effect colorEffect = m_effectSolver->GetColorEffect();
			D3DXHANDLE directionalLight = colorEffect.GetVertexShader()->GetConstantTable()->GetConstantByName( 0, "directionalLight" );
			if ( directionalLight )
			{
				int x(0);x;
			}
		}

		if ( m_mesh )
		{
			m_mesh->Update( elapsed, input );
		}
	}
				
	return Game::Update( elapsed, input );
}

void MyGame::Render()
{
	RenderInfo renderInfo;
	renderInfo.SetFinalMatrix( m_camera->GetMatrix() );

	if ( m_mesh ) m_mesh->Render( renderInfo );
}

void MyGame::Shutdown()
{
	m_mesh.reset();
	Game::Shutdown();
}
