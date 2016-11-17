// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/Mesh.h>
#include <me/scene/SceneManager.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <dxi/win/DXILib.h>
#include <MEWinMain.h>

#include <me/scene/BBoxRendererComponent.h>
#include <me/scene/CameraComponent.h>

using namespace me;

class MyGame : public Game
{
protected:
public:
	MyGame() : Game( "setup_models.xml" ) {}
	void Startup();
	void Update( RenderInfo & renderInfo );
} game;

RegisterGame( game );

void MyGame::Startup()
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	// Add common effects...
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", "media/EffectColor.effect" );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "texture3d", "media/EffectTextured.effect" );

	// Load shaders.
	IPixelShader::ptr ps = GetManager< IPixelShader >()->Add( "texture3d", "media/shaders/texture3d.xml" );
	IVertexShader::ptr vs = GetManager< IVertexShader >()->Add( "texture3d", "media/shaders/texture3d.xml" );

	// Add a texture.
	GetManager< ITexture >()->Add( "borgcube", "media/borgcube.bmp" );

	// Create an effect.
	Effect::ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< ITexture >()->Find( "borgcube" ) );

	// Add a scene.
	Scene::ptr scene = sceneManager->Add( "scene" );

	// Add a camera...
	Object::ptr camera = scene->GetRoot()->AddChild( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent( GetOS() ) ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = scene->GetRoot()->AddChild( "cubeDyna" );
	AddGeometryComponent( progObject.get(), GetOS(), meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an XML file...
	Geometry::ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", "media/cube.xml" ) );
	PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
	auto xmlObject = scene->GetRoot()->AddChild( "XMLObject" );
	auto gc = AddGeometryComponent( xmlObject.get(), GetOS(), meshXML );
	xmlObject->GetFrame().SetPosition( unify::V3< float >( 0 - 2.5f, 0, 0 ) );
	gc->GetMatrix().Scale( 0.10f );
	xmlObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", "media/ASE_SwordTextured.ASE" ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	auto aseObject = scene->GetRoot()->AddChild( "swordASE" );
	gc = AddGeometryComponent( aseObject.get(), GetOS(), meshASE );
	aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
	gc->GetMatrix().Scale( 0.090f );
	gc->GetMatrix().RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
	gc->GetMatrix().Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
	aseObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

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
	gc = AddGeometryComponent( daeModel.get(), GetOS(), meshDAE );
	daeModel->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
	daeModel->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();

	gc->GetMatrix().Scale( 4.0f / meshDAE->GetBBox().Size().Length() );
	gc->GetMatrix().RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
	gc->GetMatrix().RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );
}

void MyGame::Update( RenderInfo & renderInfo )
{
	using namespace scene;
	
	SceneManager * sceneManager = dynamic_cast< SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	// Use of camera controls to simplify camera movement...
	Object::ptr camera = sceneManager->Find( "scene" )->GetRoot()->FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
