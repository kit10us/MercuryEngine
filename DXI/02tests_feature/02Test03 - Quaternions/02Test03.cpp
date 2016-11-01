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
	void Update( RenderInfo & renderInfo );
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
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );

	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -10 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	shapes::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 1, 1, 1 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	Geometry::ptr meshProg( shapes::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = scene->GetRoot()->AddChild( "cubeDyna" );
	progObject->SetGeometry( meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( scene::IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", "media/ASE_SwordTextured.ASE" ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	{
		auto aseObject = scene->GetRoot()->AddChild( "sword1" );
		aseObject->SetGeometry( meshASE );
		aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
		aseObject->GetGeometryMatrix().Scale( 0.090f );
		aseObject->GetGeometryMatrix().RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
		aseObject->GetGeometryMatrix().Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
		aseObject->AddComponent( scene::IObjectComponent::ptr( new scene::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 0 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword1" )->Duplicate( "sword2" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword2" )->Duplicate( "sword3" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword3" )->Duplicate( "sword4" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInDegrees( 90 ) ) );
	}


	{
		auto aseObject = scene->GetRoot()->FindObject( "sword1" )->Duplicate( "sword5" );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword5" )->Duplicate( "sword6" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword6" )->Duplicate( "sword7" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword7" )->Duplicate( "sword8" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 90 ) ) );
	}

	
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword1" )->Duplicate( "sword9" );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword9" )->Duplicate( "sword10" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword10" )->Duplicate( "sword11" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword11" )->Duplicate( "sword12" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 90 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword1" )->Duplicate( "sword13" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword3" )->Duplicate( "sword14" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword14" )->Duplicate( "sword15" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
	{
		auto aseObject = scene->GetRoot()->FindObject( "sword15" )->Duplicate( "sword16" );
		aseObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( 180 ) ) );
	}
}

void MyGame::Update( RenderInfo & renderInfo )
{
	using namespace unify;

	scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	scene::Scene::ptr scene = sceneManager->Find( "scene" );

	{
		auto sword = scene->FindObject( "sword1" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );	
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword2" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword3" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword4" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	{
		auto sword = scene->FindObject( "sword5" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword6" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword7" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword8" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	{
		auto sword = scene->FindObject( "sword9" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 1 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword10" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 1 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword11" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 1 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword12" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword13" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 1 ), unify::AngleInRadians( -renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword14" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 1 ), unify::AngleInRadians( -renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword15" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 0, 1 ), unify::AngleInRadians( -renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}
	{
		auto sword = scene->FindObject( "sword16" );
		sword->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 1, 1, 0 ), unify::AngleInRadians( -renderInfo.GetDelta() ) ) );
		sword->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
		sword->GetFrame().PreMul( Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInDegrees( -90 ) ) );
	}


	auto camera = scene->FindObject( "camera" );
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() * - 1.0f) ) );
	camera->GetFrame().LookAt( V3< float >( 0, 0, 0 ) );
}