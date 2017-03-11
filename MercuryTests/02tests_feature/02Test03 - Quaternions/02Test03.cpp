// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/Mesh.h>
#include <me/scene/SceneManager.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <sg/ShapeFactory.h>
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
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", "EffectColor.effect" );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "texture3d", "EffectTextured.effect" );

	// Load shaders.
	IPixelShader::ptr ps = GetManager< IPixelShader >()->Add( "texture3d", "shaders/textured3d.xml" );
	IVertexShader::ptr vs = GetManager< IVertexShader >()->Add( "texture3d", "shaders/textured3d.xml" );

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
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent()  ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );

	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -10 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 1, 1, 1 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = scene->NewObject( "cubeDyna" );
	auto gc = AddGeometryComponent( progObject, meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", "ASE_SwordTextured.ASE" ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	{
		auto aseObject = scene->NewObject( "sword1" );
		gc = AddGeometryComponent( aseObject, meshASE );
		aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
		aseObject->GetFrame().GetModelMatrix().Scale( 0.090f );
		aseObject->GetFrame().GetModelMatrix().RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
		aseObject->GetFrame().GetModelMatrix().Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
		aseObject->AddComponent( IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 0 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword1" ), "sword2" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword2" ), "sword3" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword3" ), "sword4" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}


	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword1" ), "sword5" );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword5" ), "sword6" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword6" ), "sword7" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword7" ), "sword8" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}

	
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword1" ), "sword9" );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword9" ), "sword10" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword10" ), "sword11" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword11" ), "sword12" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword12" ), "sword13" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword13" ), "sword14" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword14" ), "sword15" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = scene->CopyObject( scene->FindObject( "sword15" ), "sword16" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
}

void MyGame::Update( UpdateParams params )
{
	using namespace unify;
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	Scene::ptr scene = sceneManager->FindScene( "scene" );

	{
		auto sword = scene->FindObject( "sword1" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta() ) ) );	
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword2" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword3" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword4" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	{
		auto sword = scene->FindObject( "sword5" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword6" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword7" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword8" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	{
		auto sword = scene->FindObject( "sword9" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword10" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword11" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword12" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 0 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword13" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 1 ), unify::AngleInRadians( -params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword14" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 1 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword15" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 1 ), unify::AngleInRadians( -params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword16" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 0 ), unify::AngleInRadians( params.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	auto camera = scene->FindObject( "camera" );
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta() * - 1.0f) ) );
	camera->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
}
