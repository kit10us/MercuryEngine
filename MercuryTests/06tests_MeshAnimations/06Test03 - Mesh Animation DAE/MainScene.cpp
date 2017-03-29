// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>
#include <me/RenderMethod.h>
#include <me/VertexUtil.h>
#include <me/scene/SceneManager.h>
#include <me/object/Object.h>
#include <me/Mesh.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/BBoxRendererComponent.h>
#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/object/CameraComponent.h>

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
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", unify::Path( "EffectColor.effect" ) );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	auto object = GetObjectAllocator()->NewObject( "object" );

	Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "Mickey_Mouse/Mickey_Mouse.dae" ) ) );
	Mesh* mesh = dynamic_cast<Mesh *>( meshDAE.get() );
	{
		mesh->SetSkeletonEffect( color3DEffect );
		using namespace unify;
		Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 4.0f / meshDAE->GetBBox().Size().Length() );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );
		AddGeometryComponent( object, meshDAE, modelMatrix );
	}

	object->GetFrame().SetPosition( unify::V3< float >( 0, 0, 0 ) );
	object->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();
	{
		/*
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 4.0f / meshDAE->GetBBox().Size().Length() );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );
		object->GetFrame().SetModelMatrix( modelMatrix );
		*/
	}
}

void MainScene::OnUpdate( UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );

	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );

	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );


	// object...
	static float x = 0.0f;
	auto object = GetObjectAllocator()->FindObject( "object" );
	object->GetFrame().SetPosition( unify::V3< float >( x < 10.0f ? (-5.0f + x) : (15.0f - x) , 0, 0 ) );
	x += params.GetDelta();
	if( x >= 20.0f ) x = 0.0f;
}
