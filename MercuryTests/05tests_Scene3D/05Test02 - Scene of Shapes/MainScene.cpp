// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <sg/ShapeCreators.h>

#include <me/render/Billboard.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", unify::Path( "EffectColor.effect" ) );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "color3d", unify::Path( "EffectTextured.effect" ) );

	// Add an object to act as a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// Add a camera component to the camera object
	auto * cameraComponent = new component::CameraComponent();
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->AddComponent( component::IObjectComponent::ptr( cameraComponent ) );

	auto createObject = [&]( float x, float y )->me::object::Object*
	{
		static int objectIndex = 0;
		std::string objectName = "object " + unify::Cast< std::string >( objectIndex++ );
		auto object = GetObjectAllocator()->NewObject( objectName );
		object->GetFrame().SetPosition( unify::V3< float >( x, y, 0 ) );
		return object;
	};

	// Create objects...
	{
		auto object = createObject( -2.5f, 0 );

		sg::CubeParameters cubeParameters;
		cubeParameters.SetEffect( color3DEffect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer( 0 ), cubeParameters ) );
		PrimitiveList & plProg = ( (Mesh*)meshProg.get() )->GetPrimitiveList();
		AddGeometryComponent( object, meshProg );
	}

	{
		auto object = createObject( 2.5f, 0 );

		sg::SphereParameters sphereParameters;
		sphereParameters.SetEffect( color3DEffect );
		sphereParameters.SetRadius( 2 );
		sphereParameters.SetDiffuse( unify::Color::ColorRed() );
		Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer( 0 ), sphereParameters ) );
		PrimitiveList & plProg = ( (Mesh*)meshProg.get() )->GetPrimitiveList();
		AddGeometryComponent( object, meshProg );
	}

	{
		//treeside.bmp
			
		auto object = createObject( 2.5f, 2.5f );

		Billboard * billboard = new Billboard( GetOS()->GetRenderer( 0 ), 2.5f, textured3DEffect );

		auto component = AddGeometryComponent( object, Geometry::ptr{ billboard } );

		component->SetValue( "alias", "sphere" );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
