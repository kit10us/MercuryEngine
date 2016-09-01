// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/shapes/ShapeCreators.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/ShapeFactory.h>

#include <dxi/GeometryASEFactory.h>
#include <dae/DAE.h>

#include <chrono>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();

} game;

#include <regex>

void Take( std::vector< char > things )
{
}

void MyGame::Startup()
{
	Game::Startup();

	// Add common effects...
	Effect::shared_ptr color3DEffect = GetManager< Effect >()->Add( "color3d", "media/EffectColor.xml" );
	Effect::shared_ptr textured3DEffect = GetManager< Effect >()->Add( "textured3d", "media/EffectTextured.xml" );

	// Load shaders.
	PixelShader::shared_ptr ps = GetManager< PixelShader >()->Add( "textured3d", "media/shaders/textured3d.xml" );
	VertexShader::shared_ptr vs = GetManager< VertexShader >()->Add( "textured3d", "media/shaders/textured3d.xml" );

	// Add a texture.
	GetManager< Texture >()->Add( "borgcube", "media/borgcube.bmp" );

	// Create an effect.
	Effect::shared_ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< Texture >()->Find( "borgcube" ) );

	// Setup ASE factories.
	GeometryASEFactory * aseFactory = new GeometryASEFactory;
	aseFactory->SetVertexShader( vs );
	aseFactory->SetPixelShader( ps );
	GetManager< Geometry >()->AddFactory( aseFactory );

	// Setup DAE factory.
	class MyEffectSolver : public dae::util::IEffectSolver
	{
		Effect::shared_ptr m_color;
		Effect::shared_ptr m_textured;
	public:

		MyEffectSolver( Game & game )
		{
			m_color = game.GetManager< Effect >()->Add( "color", "media/EffectColor.xml" );
			m_textured = game.GetManager< Effect >()->Add( "textured", "media/EffectTextured.xml" );
		}

		dxi::Effect::shared_ptr GetEffect( const dae::Shading & shading ) const
		{
			if( shading.GetDiffuse().GetType() == dae::Shading::Property::ColorType )
			{
				return m_color;
			}
			else
			{
				return m_textured;
			}
		}
	};
	dae::GeometrySourceFactory * daeFactory = new dae::GeometrySourceFactory( new MyEffectSolver( *this ) );
	GetManager< Geometry >()->AddFactory( daeFactory );

	// Add a scene.
	scene::Scene::shared_ptr scene = GetSceneManager()->Add( "scene", new scene::Scene );

	// Add a camera...
	scene::Object::shared_ptr cameraObject = scene->Add( "camera", new scene::Object );
	scene::Camera::shared_ptr camera( new scene::Camera( cameraObject ) );
	scene->SetCamera( "camera" );
	scene->GetCamera().SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );
	camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	shapes::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	Geometry::shared_ptr meshProg( shapes::CreateShape( cubeParameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = scene->Add( "cubeDyna", new scene::Object( meshProg, unify::V3< float >( 0 - 0.0f, 0, 0 ) ) );
	progObject->SetVisible( false );
				  
	// From an XML file...
	Mesh::shared_ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", "media/cube.xml" ) );
	PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
	auto xmlObject = scene->Add( "XMLObject", new scene::Object( meshXML, unify::V3< float >( 0 - 2.5f, 0, 0 ) ) );
	xmlObject->GetGeometryMatrix().Scale( 0.10f );
	xmlObject->SetVisible( false );
	
	// From an ASE file...
	Mesh::shared_ptr meshASE( GetManager< Geometry >()->Add( "swordASE", "media/ASE_SwordTextured.ASE" ) );
	//Mesh::shared_ptr meshASE( GetManager< Geometry >()->Add( "cubeASE", "media/borgcube.ase" ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	auto aseObject = scene->Add( "swordASE", new scene::Object( meshASE, unify::V3< float >( 0 + 2.5f, 0, 0 ) ) );
	aseObject->GetGeometryMatrix().Scale( 0.090f );
	aseObject->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::Angle::AngleInDegrees( 90 ) );
	aseObject->GetGeometryMatrix().Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
	aseObject->SetVisible( false );



	std::chrono::time_point< std::chrono::system_clock > start, end;
	start = std::chrono::system_clock::now();
	
	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/borgcube.dae" ) );
	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/IcoSphereTest.dae" ) );
	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/CubeSphereCylinder.dae" ) );
	Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/models/USS Voyager/models/USS Voyager.dae" ) );
	PrimitiveList & plDAE = ((Mesh*)meshDAE.get())->GetPrimitiveList();

	end = std::chrono::system_clock::now();
	std::chrono::duration< double > elapsed = end - start;
	double elapsed_count = elapsed.count();
			
	scene->Add( "cubeDAE", new scene::Object( meshDAE, unify::V3< float >( 0 /*- 5.0f*/, 0, 0 ) ) );
	auto daeObject = scene->FindObject( "cubeDAE" );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();

	float size = meshDAE->GetBBox().Size().Length();
	float scaleE = 12.0f / meshDAE->GetBBox().Size().Length();
	daeObject->GetGeometryMatrix().Scale( scaleE );
	daeObject->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::Angle::AngleInDegrees( 270.0f ) );


	auto progBB = meshProg->GetBBox();
	auto xmlBB = meshXML->GetBBox();
	auto aseBB = meshASE->GetBBox();
	auto daeBB = meshDAE->GetBBox();

	size_t progBufferSetCount = plProg.GetBufferSetCount();
	size_t xmlBufferSetCount = plXML.GetBufferSetCount();
	size_t aseBufferSetCount = plASE.GetBufferSetCount();
	size_t daeBufferSetCount = plDAE.GetBufferSetCount();


	size_t daeVertexCount {};
	for( size_t i = 0; i < plDAE.GetBufferSetCount(); ++i )
	{
		BufferSet & bs = plDAE.GetBufferSet( i );
		daeVertexCount += plDAE.GetBufferSet( i ).GetVertexBuffer().GetSize();
		//bs.SetEnabled( false );
	}

	/*
	m_meshFromX.reset( new scene::Object( geo::Geometry::shared_ptr( loader::LoadMesh( "media/tiny2.x", GetManagers() ) ), unify::V3< float >( 0, 0, 0 ) ) );
	float scaleC = m_meshFromX->GetGeometry()->ComputeBounds().Size().Length();
	m_meshFromX->SetGeometryMatrix( unify::Matrix::MatrixScale( 0.005f ) );
	*/

	//int x( 0 ); x;
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	static int state = -1;
	if ( input.KeyPressed( Key::RightArrow ) || input.KeyPressed( Key::LeftArrow ) )
	{
		auto scene = GetSceneManager()->Find( "scene" );
		auto daeObject = scene->FindObject( "cubeDAE" );
		PrimitiveList & plDAE = ((Mesh*)daeObject->GetGeometry().get())->GetPrimitiveList();

		if( input.KeyPressed( Key::RightArrow ) )
		{
			state++;
			if( state >= (int)plDAE.GetBufferSetCount() )
			{
				state = -1;
			}
		}
		if( input.KeyPressed( Key::LeftArrow ) )
		{
			state--;
			if( state < -1 )
			{
				state = plDAE.GetBufferSetCount() - 1;
			}
		}				  
			
		for( size_t i = 0; i < plDAE.GetBufferSetCount(); ++i )
		{
			BufferSet & bs = plDAE.GetBufferSet( i );
			bs.SetEnabled( ( state == -1 || state == i ) ? true : false );
		}
	}
	

	// Use of camera controls to simplify camera movement...
	scene::Object::shared_ptr cameraObject = GetSceneManager()->Find( "scene" )->GetCamera().GetObject();
	cameraObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) );
	cameraObject->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	return Game::Update( elapsed, input );

	/*
	m_camera->GetObject()->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) );
	m_camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	if ( m_meshFromShape ) m_meshFromShape->Update( elapsed, input );
	if ( m_meshFromXML ) m_meshFromXML->Update( elapsed, input );
	if ( m_meshFromX ) m_meshFromX->Update( elapsed, input );
	if ( m_meshFromASE ) m_meshFromASE->Update( elapsed, input );
	if ( m_meshFromDAE ) m_meshFromDAE->Update( elapsed, input );

	return Game::Update( elapsed, input );
	*/
}

void MyGame::Render()
{
	/*
	RenderInfo renderInfo;
	renderInfo.SetFinalMatrix( m_camera->GetMatrix() );

	if ( m_meshFromShape ) m_meshFromShape->Render( renderInfo );
	if ( m_meshFromXML ) m_meshFromXML->Render( renderInfo );
	if ( m_meshFromX ) m_meshFromX->Render( renderInfo );
	if ( m_meshFromASE ) m_meshFromASE->Render( renderInfo );
	if ( m_meshFromDAE ) m_meshFromDAE->Render( renderInfo );
	*/

	Game::Render();
}

void MyGame::Shutdown()
{
	/*
	m_meshFromASE.reset();
	m_meshFromX.reset();
	m_meshFromXML.reset();
	m_meshFromShape.reset();
	*/

	Game::Shutdown();
}
