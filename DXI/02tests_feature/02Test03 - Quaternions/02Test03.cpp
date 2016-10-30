// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/Mesh.h>
#include <dxi/scene/SceneManager.h>
#include <dxi/shapes/ShapeCreators.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/factory/ShapeFactory.h>
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>

#include <dxi/scene/BBoxRendererComponent.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
public:
	MyGame() : Game( "setup_models.xml" ) {}
	void Startup();
	bool Update( RenderInfo & renderInfo );
} game;

RegisterGame( game );

void MyGame::Startup()
{
	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

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
	scene::Scene::ptr scene = sceneManager->Add( "scene" );

	// Add a camera...
	scene::Object::ptr camera = scene->GetRoot()->AddChild( "camera" );
	camera->AddComponent( scene::IObjectComponent::ptr( new scene::CameraComponent( GetOS() ) ) );	 
	scene::CameraComponent * cameraComponent = unify::polymorphic_downcast< scene::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );

	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -10 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	shapes::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	Geometry::ptr meshProg( shapes::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = scene->GetRoot()->AddChild( "cubeDyna" );
	progObject->SetGeometry( meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( scene::IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an XML file...
	Geometry::ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", "media/cube.xml" ) );
	PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
	auto xmlObject = scene->GetRoot()->AddChild( "XMLObject" );
	xmlObject->SetGeometry( meshXML );
	xmlObject->GetFrame().SetPosition( unify::V3< float >( 0 - 2.5f, 0, 0 ) );
	xmlObject->GetGeometryMatrix().Scale( 0.10f );
	xmlObject->AddComponent( scene::IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", "media/ASE_SwordTextured.ASE" ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	auto aseObject = scene->GetRoot()->AddChild( "swordASE" );
	aseObject->SetGeometry( meshASE );
	aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
	aseObject->GetGeometryMatrix().Scale( 0.090f );
	aseObject->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
	aseObject->GetGeometryMatrix().Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
	aseObject->AddComponent( scene::IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/USSVoyager.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/models/Death Star II/models/Death Star II.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/enterprise.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/cube.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/borgcube.dae" ) );
	Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/torus.dae" ) );
	
	// Rigged...
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/Mickey_Mouse/Mickey_Mouse.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "media/SuperMarioGalaxy_Mario/mario.dae" ) );

	auto daeModel = scene->GetRoot()->AddChild( "daeModel" );
	daeModel->SetGeometry( meshDAE );
	daeModel->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
	daeModel->AddComponent( scene::IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();

	daeModel->GetGeometryMatrix().Scale( 4.0f / meshDAE->GetBBox().Size().Length() );
	daeModel->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
	daeModel->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );
}

bool MyGame::Update( RenderInfo & renderInfo )
{
	using namespace unify;

	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	scene::Scene::ptr scene = sceneManager->Find( "scene" );

	// Use of camera controls to simplify camera movement...
	scene::Object::ptr camera = sceneManager->Find( "scene" )->GetRoot()->FindObject( "camera" );

	auto swordASE = scene->FindObject( "swordASE" );

	swordASE->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	swordASE->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
	swordASE->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );

	return true;
}
