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
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "texture3d", unify::Path( "EffectTextured.effect") );

	// Load shaders.
	IPixelShader::ptr ps = GetManager< IPixelShader >()->Add( "texture3d", unify::Path( "shaders/textured3d.xml" ) );
	IVertexShader::ptr vs = GetManager< IVertexShader >()->Add( "texture3d", unify::Path( "shaders/textured3d.xml" ) );
																									  
	// Add a texture.
	GetManager< ITexture >()->Add( "borgcube", unify::Path( "borgcube.bmp" ) );

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
	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
	auto progObject = GetObjectAllocator()->NewObject( "cubeDyna" );
	AddGeometryComponent( progObject, meshProg );
	progObject->GetFrame().SetPosition( unify::V3< float >( 0 - 0.0f, 0, 0 ) );
	progObject->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an XML file...
	Geometry::ptr meshXML( GetManager< Geometry >()->Add( "cubeXML", unify::Path( "cube.xml" ) ) );
	PrimitiveList & plXML = ((Mesh*)meshXML.get())->GetPrimitiveList();
	auto xmlObject = GetObjectAllocator()->NewObject( "XMLObject" );
	AddGeometryComponent( xmlObject, meshXML );
	xmlObject->GetFrame().SetPosition( unify::V3< float >( 0 - 2.5f, 0, 0 ) );
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.10f );
		// TODO: xmlObject->GetFrame().SetModelMatrix( modelMatrix );
	}
	xmlObject->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	// From an ASE file...
	Geometry::ptr meshASE( GetManager< Geometry >()->Add( "swordASE", unify::Path( "ASE_SwordTextured.ASE" ) ) );
	PrimitiveList & plASE = ((Mesh*)meshASE.get())->GetPrimitiveList();
	auto aseObject = GetObjectAllocator()->NewObject( "swordASE" );
	AddGeometryComponent( aseObject, meshASE );
	aseObject->GetFrame().SetPosition( unify::V3< float >( 0 + 2.5f, 0, 0 ) );
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 0.090f );
		modelMatrix.RotateAboutAxis( unify::V3< float >( -1.0f, 0.0f, 0.0f ), unify::AngleInDegrees( 90 ) );
		modelMatrix.Translate( unify::V3< float >( 0, 1.0f, 0.0f ) );
		// TODO: aseObject->GetFrame().SetModelMatrix( modelMatrix );
	}
	aseObject->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );

	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "USSVoyager.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "models/Death Star II/models/Death Star II.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "enterprise.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "cube.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "borgcube.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "torus.dae" ) ) );
	
	// Rigged...
	Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", unify::Path( "Mickey_Mouse/Mickey_Mouse.dae" ) ) );
	//Geometry::ptr meshDAE( GetManager< Geometry >()->Add( "daeModel", "SuperMarioGalaxy_Mario/mario.dae" ) );

	auto daeModel = GetObjectAllocator()->NewObject( "daeModel" );
	const unify::BBox< float > & bboxD = meshDAE->GetBBox();
	{ 
		using namespace unify;
		unify::Matrix modelMatrix = MatrixIdentity();
		modelMatrix.Scale( 4.0f / meshDAE->GetBBox().Size().Length() );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 1.0f, 0, 0 ), unify::AngleInDegrees( 270.0f ) );
		modelMatrix.RotateAboutAxis( unify::V3< float >( 0, 1.0f, 0 ), unify::AngleInDegrees( -90.0f ) );
		AddGeometryComponent( daeModel, meshDAE, modelMatrix );
	}
	daeModel->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
	daeModel->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
}

void MainScene::OnUpdate( UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
