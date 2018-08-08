// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>
#include <me/scene/SceneManager.h>
#include <me/object/Object.h>
#include <me/render/Mesh.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/object/component/CameraComponent.h>

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
	// Add common effects...
	IVertexShader::ptr vs_color3D( GetManager< IVertexShader >()->Add( "color3d", unify::Path( "color3d.xml" ) ) );
	IPixelShader::ptr ps_color3D( GetManager< IPixelShader >()->Add( "color3d", unify::Path( "color3d.xml" ) ) );
	Effect::ptr color3DEffect( new Effect(  vs_color3D, ps_color3D ) );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( component::IObjectComponent::ptr( new component::CameraComponent() ) );
	auto * cameraComponent = unify::polymorphic_downcast< component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	auto object = GetObjectAllocator()->NewObject( "object" );

	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "Mickey_Mouse/Mickey_Mouse.dae" ) ) );
	Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "windmill1_01.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "CubeSphereCylinder.dae" ) ) );
	
	
	Mesh* mesh = dynamic_cast<Mesh *>( meshDAE.get() );
	{
		// Add a skeleton effect, so we can see the skeleton.
		mesh->SetSkeletonEffect( color3DEffect );

		using namespace unify;
		Matrix modelMatrix = MatrixIdentity();

		// Fix mesh orientation and scale...
		modelMatrix.Scale( 4.0f / meshDAE->GetBBox().Size().Length() );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );

		//modelMatrix.Scale( 8.0f / meshDAE->GetBBox().Size().Length() );


		AddGeometryComponent( object, meshDAE, modelMatrix );

		// TODO: While debugging the skeleton animation.
		//mesh->SetRenderMesh( false );
	}

	object->GetFrame().SetPosition( unify::V3< float >( 0, 0, 0 ) );
	object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	assert( camera );

	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );


	// object...
	static float x = 0.0f;
	auto object = GetObjectAllocator()->FindObject( "object" );
	object->GetFrame().SetPosition( unify::V3< float >( x < 10.0f ? (-5.0f + x) : (15.0f - x) , 0, 0 ) );
	x += params.GetDelta();
	if( x >= 20.0f ) x = 0.0f;
}
