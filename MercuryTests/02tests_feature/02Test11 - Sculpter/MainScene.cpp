// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>
#include <me/scene/SceneManager.h>
#include <me/object/Object.h>
#include <me/Mesh.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/BBoxRendererComponent.h>
#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/object/CameraComponent.h>

#include <sculpter/Shape.h>

using namespace me;
using namespace scene;
using namespace object;

MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
{	
}

void MainScene::OnStart()
{
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

	//scene->AddComponent( scene::SceneComponent::ptr( new scene::AutoBBoxSceneComponent( GetOS(), color3DEffect ) ) );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	{
		sg::CubeParameters cubeParameters;
		cubeParameters.SetEffect( color3DEffect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer( 0 ), cubeParameters ) );
		PrimitiveList & plProg = ( (Mesh*)meshProg.get() )->GetPrimitiveList();
		auto progObject = GetObjectAllocator()->NewObject( "cubeDyna" );
		AddGeometryComponent( progObject, meshProg );
		progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
		progObject->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	}


	// Sculpter...
	{
		using namespace sculpter;

		Polygon::ptr sheet( new Polygon( Point( unify::MatrixIdentity() ) ) );

		auto  shape( sculpter::Shape::ptr( new sculpter::Shape() ) );


	}
	
}

void MainScene::OnUpdate( UpdateParams params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
