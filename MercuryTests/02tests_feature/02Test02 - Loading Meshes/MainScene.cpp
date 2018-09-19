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
	using namespace unify;

	// Add common effects...
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", Path( "EffectColor.effect" ) );
	Effect::ptr texture3DEffect = GetManager< Effect >()->Add( "texture3d", Path( "EffectTexture.effect") );

	// Load shaders.
	//IPixelShader::ptr ps = GetManager< IPixelShader >()->Add( "texture3d", Path( "shaders/texture3d.xml" ) );
	//IVertexShader::ptr vs = GetManager< IVertexShader >()->Add( "texture3d", Path( "shaders/texture3d.xml" ) );
	IPixelShader::ptr ps = texture3DEffect->GetPixelShader();
	IVertexShader::ptr vs = texture3DEffect->GetVertexShader();

	// Add a texture.
	GetManager< ITexture >()->Add( "borgcube", Path( "borgcube.bmp" ) );

	// Create an effect.
	Effect::ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< ITexture >()->Find( "borgcube" ) );


	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( component::IObjectComponent::ptr( new component::CameraComponent() ) );
	auto * cameraComponent = polymorphic_downcast< component::CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( { 0, 5, -17 } );
	camera->GetFrame().LookAt( { 0, 0, 0 } );

	// From dynamically generated geometry (shape creator)...
	{
		sg::CubeParameters cubeParameters;
		cubeParameters.SetEffect( color3DEffect );
		cubeParameters.SetSize( Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( ColorRed(), ColorGreen(), ColorBlue(), ColorYellow(), ColorCyan(), ColorMagenta() );
		Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
		PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "cubeDyna" );
		AddGeometryComponent( object, meshProg );
		object->GetFrame().SetPosition( { 0 - 0.0f, 0, 0 } );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	}

	// From an XML file...
	{
		Geometry::ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", Path( "cube.xml" ) ) );
		PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "XMLObject" );
	
		Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.10f );
		modelMatrix.Translate( { 0 - 2.5f, 0, 0 } );
	
		AddGeometryComponent( object, meshXML, modelMatrix );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	}

	// From an ASE file...
	{
		Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", Path( "ASE_SwordTextured.ASE" ) ) );
		PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "swordASE" );

		Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.090f );
		modelMatrix.RotateAboutAxis( { -1.0f, 0.0f, 0.0f }, AngleInDegrees( 90 ) );
		modelMatrix.Translate( { 0, 1.0f, 0.0f } );

		AddGeometryComponent( object, meshASE, modelMatrix  );
		object->GetFrame().SetPosition( { 0 + 2.5f, 0, 0 } );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	}

	{ 
		auto object = GetObjectAllocator()->NewObject( "daeModel" );

		// DISABLED: Various other dae models we can/should test.
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "USSVoyager.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "models/Death Star II/models/Death Star II.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "enterprise.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "cube.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "borgcube.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "torus.dae" ) ) );

		// Rigged...
		Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", Path( "Mickey_Mouse/Mickey_Mouse.dae" ) ) );
		//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "SuperMarioGalaxy_Mario/mario.dae" ) );

		Matrix modelMatrix = MatrixIdentity();
		unify::V3< float > sizeV3{ meshDAE->GetBBox().Size().width, meshDAE->GetBBox().Size().height, meshDAE->GetBBox().Size().depth };
		modelMatrix.Scale( sizeV3 / 4.0f );
		modelMatrix.RotateAboutAxis( { 1.0f, 0, 0 }, AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( { 0, 1.0f, 0 }, AngleInDegrees( -90.0f ) );
		AddGeometryComponent( object, meshDAE, modelMatrix );
		object->GetFrame().SetPosition( { 0 - 5.0f, 0, 0 } );
		object->AddComponent( component::IObjectComponent::ptr( new object::component::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	using namespace unify;
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( { 0, 0, 0 }, { 1, 0 }, unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	camera->GetFrame().LookAt( { 0, 0, 0 }, { 0, 1, 0 } );
}
