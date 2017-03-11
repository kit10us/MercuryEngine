// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/Mesh.h>
#include <me/scene/SceneManager.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <MEWinMain.h>

#include <me/scene/BBoxRendererComponent.h>
#include <me/scene/CameraComponent.h>

using namespace me;

class MyGame : public Game
{
protected:
public:
	MyGame() : Game( "setup_models.xml" ) {}
	void Startup() override;
	void Update( UpdateParams params ) override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	// Add common effects...
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color_ambient", "Color_Ambient.effect" );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "textured_ambient", "Textured_Ambient.effect" );

	// Load shaders.
	IPixelShader::ptr ps = GetManager< IPixelShader >()->Add( "textured_ambient", "shaders/textured_ambient.xml" );
	IVertexShader::ptr vs = GetManager< IVertexShader >()->Add( "textured_ambient", "shaders/textured_ambient.xml" );

	// Add a texture.
	GetManager< ITexture >()->Add( "borgcube", "borgcube.bmp" );

	// Create an effect.
	Effect::ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< ITexture >()->Find( "borgcube" ) );

	// Add a scene.
	Scene::ptr scene = sceneManager->AddScene( "scene" );

	// Add a camera...
	Object * camera = scene->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );	 
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
	auto progObject = scene->NewObject( "cubeDyna" );
	AddGeometryComponent( progObject, meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	/*
	// From an XML file...
	Geometry::ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", "cube.xml" ) );
	PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
	auto xmlObject = scene->NewObject( "XMLObject" );
	auto gc = AddGeometryComponent( xmlObject, GetOS(), meshXML );
	xmlObject->GetFrame().SetPosition( unify::V3< float >( 0 - 2.5f, 0, 0 ) );
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.10f );
		xmlObject->GetFrame().SetModelMatrix( modelMatrix );
	}
	xmlObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	*/

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", "ASE_SwordTextured.ASE" ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	auto aseObject = scene->NewObject( "swordASE" );
	AddGeometryComponent( aseObject, meshASE );
	aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.090f );
		modelMatrix.RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
		modelMatrix.Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
		aseObject->GetFrame().SetModelMatrix( modelMatrix );
	}
	aseObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	/*
	// Non-rigged
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "USSVoyager.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "models/Death Star II/models/Death Star II.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "enterprise.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "cube.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "borgcube.dae" ) );
	Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "torus.dae" ) );	 	
	// Rigged...
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "Mickey_Mouse/Mickey_Mouse.dae" ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "SuperMarioGalaxy_Mario/mario.dae" ) );

	auto daeModel = scene->NewObject( "daeModel" );
	AddGeometryComponent( daeModel, GetOS(), meshDAE );
	daeModel->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
	daeModel->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 4.0f / meshDAE->GetBBox().Size().Length() );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );
		daeModel->GetFrame().SetModelMatrix( modelMatrix );
	}
	*/
}

void MyGame::Update( UpdateParams params )
{
	using namespace scene;
	
	SceneManager * sceneManager = dynamic_cast< SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	// Use of camera controls to simplify camera movement...
	Object * camera = sceneManager->FindScene( "scene" )->FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
