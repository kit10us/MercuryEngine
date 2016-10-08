// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/shapes/ShapeCreators.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/factory/ShapeFactory.h>
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
public:
	MyGame() : Game( "setup_models.xml" ) {}
	void Startup();
	bool Update( RenderInfo & renderInfo, IInput & input );
} game;

RegisterGame( game );

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

	// Add a scene.
	scene::Scene::shared_ptr scene = GetSceneManager()->Add( "scene" );

	// Add a camera...
	scene::Object::ptr camera = scene->GetRoot()->AddChild( "camera" );
	camera->AddComponent( scene::IComponent::ptr( new scene::Camera() ) );

	scene::Camera * cameraComponent = unify::polymorphic_downcast< scene::Camera * >( camera->GetComponent( "camera" ).get() );

	cameraComponent->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

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

	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/models/USS Voyager/models/USS Voyager.dae" ) );
	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/models/Death Star II/models/Death Star II.dae" ) );
	Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/enterprise.dae" ) );
	//Mesh::shared_ptr meshDAE( GetManager< Geometry >()->Add( "cubeDAE", "media/cube.dae" ) );
	
	auto daeModel = scene->GetRoot()->AddChild( "daeModel" );
	daeModel->SetGeometry( meshDAE );
	daeModel->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
	auto daeObject = scene->FindObject( "daeModel" );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();

	float size = meshDAE->GetBBox().Size().Length();
	float scaleE = 8.0f / meshDAE->GetBBox().Size().Length();
	daeObject->GetGeometryMatrix().Scale( scaleE * 0.25f );
	daeObject->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::Angle::AngleInDegrees( 270.0f ) );
}

bool MyGame::Update( RenderInfo & renderInfo, IInput & input )
{
	// Use of camera controls to simplify camera movement...
	scene::Object::ptr camera = GetSceneManager()->Find( "scene" )->GetRoot()->FindObject( "camera" );
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( renderInfo.GetDelta() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	return Game::Update( renderInfo, input );
}
