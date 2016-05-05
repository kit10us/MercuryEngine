// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/loader/MeshLoader.h>
#include <dxi/loader/EffectLoader.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	scene::Object::shared_ptr m_cameraObject;
	scene::Camera::shared_ptr m_camera;
	scene::Object::shared_ptr m_meshFromShape;
	scene::Object::shared_ptr m_meshFromXML;
	scene::Object::shared_ptr m_meshFromX;
	scene::Object::shared_ptr m_meshFromASE;
    scene::Object::shared_ptr m_meshFromDAE;
	unify::Angle m_rotation;

public:
	void Startup();
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
		m_color = loader::EffectLoader( "color", "media/EffectColor.xml", game.GetManagers() );
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
};

void MyGame::Startup()
{
	Game::Startup();
	
	// Setup camera...
	// Create an object to use as a camera, and a camera interface. Set the projection to a default projection.
	m_cameraObject.reset( new scene::Object ); // Necessary to store the camera object -somewhere- outside of the camera, as is weak_ptr in camera.
	m_camera.reset( new scene::Camera( m_cameraObject ) );
	m_camera->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );
	m_camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 5, 13 ) );

	loader::EffectLoader( "color_3d", "media/EffectColor.xml", GetManagers() );
	Effect::shared_ptr color3DEffect = GetEffectManager()->Find( "color_3d" );

	// From dynamically generated geometry (shape creator)...
	shapes::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	m_meshFromShape.reset( new scene::Object( geo::Geometry::shared_ptr( new geo::Mesh( cubeParameters ) ), unify::V3< float >( -5, 0, 0 ) ) );
	const unify::BBox< float > & bboxA = m_meshFromShape->GetGeometry()->ComputeBounds();
	float scaleA = 3 / m_meshFromShape->GetGeometry()->ComputeBounds().Size().Length();
    m_meshFromShape->SetGeometryMatrix( unify::Matrix::MatrixScale( scaleA ) );

	// From an XML file...
	geo::Mesh::shared_ptr meshXML( new geo::Mesh( "media/cube.xml", 0, GetManagers() ) );
	m_meshFromXML.reset( new scene::Object( meshXML, unify::V3< float >( -2.5, 0, 0 ) ) );
	const unify::BBox< float > & bboxB = m_meshFromXML->GetGeometry()->ComputeBounds();
	float scaleB = 3 / m_meshFromXML->GetGeometry()->ComputeBounds().Size().Length();
    m_meshFromXML->SetGeometryMatrix( unify::Matrix::MatrixScale( scaleB ) );
	
	/*
	m_meshFromX.reset( new scene::Object( geo::Geometry::shared_ptr( loader::LoadMesh( "media/tiny2.x", GetManagers() ) ), unify::V3< float >( 0, 0, 0 ) ) );
	float scaleC = m_meshFromX->GetGeometry()->ComputeBounds().Size().Length();
    m_meshFromX->SetGeometryMatrix( unify::Matrix::MatrixScale( 0.005f ) );
	*/
	
	/*
	m_meshFromASE.reset( new scene::Object( geo::Geometry::shared_ptr( loader::LoadMesh( "media/ASE_SwordTextured.ASE", GetManagers() ) ), unify::V3< float >( 2.5 * 0.5f, 0, 0 ) ) );
	float scaleD = m_meshFromASE->GetGeometry()->ComputeBounds().Size().Length();
	m_meshFromASE->SetGeometryMatrix( m_meshFromASE->GetGeometryMatrix() * unify::Matrix::MatrixScale( 0.05f ) * unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 1, 0, 0 ), unify::Angle::AnglePIHalf() ));
	*/

	geo::Mesh::shared_ptr meshDAE( new geo::Mesh( "media/cube.dae", new MyEffectSolver( *this ), GetManagers() ) );
	m_meshFromDAE.reset( new scene::Object( meshDAE, unify::V3< float >( 5, 0, 0 ) ) );
	const unify::BBox< float > & bboxD = m_meshFromShape->GetGeometry()->ComputeBounds();
	float scaleE = 3 / m_meshFromDAE->GetGeometry()->ComputeBounds().Size().Length();
	m_meshFromDAE->SetGeometryMatrix( unify::Matrix::MatrixScale( scaleE ) );
	
	const PrimitiveList & plA = ((geo::Mesh*) m_meshFromShape->GetGeometry().get() )->GetPrimitiveList();
	const PrimitiveList & plB = ((geo::Mesh*) m_meshFromDAE->GetGeometry().get() )->GetPrimitiveList();
	int x(0);x;
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	m_camera->GetObject()->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) );
	m_camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	if ( m_meshFromShape ) m_meshFromShape->Update( elapsed, input );
	if ( m_meshFromXML ) m_meshFromXML->Update( elapsed, input );
	if ( m_meshFromX ) m_meshFromX->Update( elapsed, input );
	if ( m_meshFromASE ) m_meshFromASE->Update( elapsed, input );
	if ( m_meshFromDAE ) m_meshFromDAE->Update( elapsed, input );

	return Game::Update( elapsed, input );
}

void MyGame::Render()
{
	RenderInfo renderInfo;
	renderInfo.SetFinalMatrix( m_camera->GetMatrix() );

	if ( m_meshFromShape ) m_meshFromShape->Render( renderInfo );
	if ( m_meshFromXML ) m_meshFromXML->Render( renderInfo );
	if ( m_meshFromX ) m_meshFromX->Render( renderInfo );
	if ( m_meshFromASE ) m_meshFromASE->Render( renderInfo );
	if ( m_meshFromDAE ) m_meshFromDAE->Render( renderInfo );
}

void MyGame::Shutdown()
{
	m_meshFromASE.reset();
	m_meshFromX.reset();
	m_meshFromXML.reset();
	m_meshFromShape.reset();

	Game::Shutdown();
}
