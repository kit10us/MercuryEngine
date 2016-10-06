// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/shapes/ShapeCreators.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/factory/ShapeFactory.h>
#include <dxi/factory/GeometryASEFactory.h>
#include <dae/DAELib.h>
#include <dxi/win/DXILib.h>

#include <chrono>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
public:
	//MyGame() : Game( "setup_dae.xml" ) {}
	void Startup();
	bool Update( RenderInfo & renderInfo, IInput & input );
	void Render( const RenderInfo & renderInfo );
	void Shutdown();

} game;


void MyGame::Startup()
{
	// Add common effects...
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", "media/EffectColor.effect" );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "texture3d", "media/EffectTextured.effect" );

	// Load shaders.
	PixelShader::ptr ps = GetManager< PixelShader >()->Add( "texture3d", "media/shaders/texture3d.xml" );
	VertexShader::ptr vs = GetManager< VertexShader >()->Add( "texture3d", "media/shaders/texture3d.xml" );

	// Add a texture.
	GetManager< Texture >()->Add( "borgcube", "media/borgcube.bmp" );

	// Create an effect.
	Effect::ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< Texture >()->Find( "borgcube" ) );

	// Setup ASE factories.
	GeometryASEFactory * aseFactory = new GeometryASEFactory;
	aseFactory->SetVertexShader( vs );
	aseFactory->SetPixelShader( ps );
	GetManager< Geometry >()->AddFactory( "ase", aseFactory );

	// Setup DAE factory.
	class MyEffectSolver : public dae::util::IEffectSolver
	{
		Effect::ptr m_color;
		Effect::ptr m_textured;
	public:

		MyEffectSolver( Game & game )
		{
			m_color = game.GetManager< Effect >()->Add( "color", "media/EffectColor.effect" );
			m_textured = game.GetManager< Effect >()->Add( "texture", "media/EffectTextured.effect" );
		}

		dxi::Effect::ptr GetEffect( const dae::Shading & shading ) const
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
	GetManager< Geometry >()->AddFactory( "dae", daeFactory );

	// Add a scene.
	scene::Scene::shared_ptr scene = GetSceneManager()->Add( "scene" );

	// Add a camera...
	scene::Object::shared_ptr cameraObject = scene->GetRoot()->AddChild( "camera" );
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
	auto progObject = scene->GetRoot()->AddChild( "cubeDyna" );
	progObject->SetGeometry( meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
				  
	// From an XML file...
	Mesh::shared_ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", "media/cube.xml" ) );
	PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
	auto xmlObject = scene->GetRoot()->AddChild( "XMLObject" );
	xmlObject->SetGeometry( meshXML );
	xmlObject->GetFrame().SetPosition( unify::V3< float >( 0 - 2.5f, 0, 0 ) );
	xmlObject->GetGeometryMatrix().Scale( 0.10f );
	
	// From an ASE file...
	Mesh::shared_ptr meshASE( GetManager< Geometry >()->Add( "swordASE", "media/ASE_SwordTextured.ASE" ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	auto aseObject = scene->GetRoot()->AddChild( "swordASE" );
	aseObject->SetGeometry( meshASE );
	aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
	aseObject->GetGeometryMatrix().Scale( 0.090f );
	aseObject->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::Angle::AngleInDegrees( 90 ) );
	aseObject->GetGeometryMatrix().Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );

	std::chrono::time_point< std::chrono::system_clock > start, end;
	start = std::chrono::system_clock::now();
	
	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/models/USS Voyager/models/USS Voyager.dae" ) );
	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/models/Death Star II/models/Death Star II.dae" ) );
	Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/models/UssEnterprise/models/model.dae" ) );

	PrimitiveList & plDAE = ((Mesh*)meshDAE.get())->GetPrimitiveList();

	end = std::chrono::system_clock::now();
	std::chrono::duration< double > elapsed = end - start;
	double elapsed_count = elapsed.count();
			
	auto cubeDAE = scene->GetRoot()->AddChild( "cubeDAE" );
	cubeDAE->SetGeometry( meshDAE );
	cubeDAE->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
	auto daeObject = scene->FindObject( "cubeDAE" );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();

	float size = meshDAE->GetBBox().Size().Length();
	float scaleE = 8.0f / meshDAE->GetBBox().Size().Length();
	daeObject->GetGeometryMatrix().Scale( scaleE );
	daeObject->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::Angle::AngleInDegrees( 270.0f ) );
}

bool MyGame::Update( RenderInfo & renderInfo, IInput & input )
{
	// Use of camera controls to simplify camera movement...
	scene::Object::shared_ptr cameraObject = GetSceneManager()->Find( "scene" )->GetCamera().GetObject();
	cameraObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( renderInfo.GetDelta() ) );
	cameraObject->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	return Game::Update( renderInfo, input );
}

void MyGame::Render( const RenderInfo & renderInfo )
{
	Game::Render( renderInfo );
}

void MyGame::Shutdown()
{
	Game::Shutdown();
}
