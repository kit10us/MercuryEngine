// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/object/BBoxRendererComponent.h>
#include <me/object/CameraComponent.h>
#include <me/object/CameraComponent.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>
#include <sg/ShapeCreators.h>

using namespace me;
using namespace scene;
using namespace object;
using namespace bw;
using namespace render;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	Effect::ptr testEffect = GetManager< Effect >()->Add( "Test", unify::Path( "GrassBlock00.effect" ) );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, GetOS()->GetRenderer(0)->GetDisplay().GetSize().AspectRatioWH(), 1.0f, 1000.0f ) );
													  
	m_world.reset( new World( 100 ) );

	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( testEffect );
    cubeParameters.SetSize( unify::Size3< float >( 10, 10, 10 ) );
	cubeParameters.SetDiffuse( unify::Color::ColorBlue() );
	cubeParameters.SetFaceUV( 
	{ 
		// Top
		{ unify::TexCoords( 0.0f, 0.0f ), unify::TexCoords( 0.25f, 0.25f ) },

		// Bottom
		{ unify::TexCoords( 0.0f, 0.25f ), unify::TexCoords( 0.25f, 0.5f ) },

		// Front
		{ unify::TexCoords( 0.25f, 0.0f ), unify::TexCoords( 0.5f, 0.25f ) }, 

		// Back
		{ unify::TexCoords( 0.25f, 0.25f ), unify::TexCoords( 0.5f, 0.5f ) }, 

		// Left
		{ unify::TexCoords( 0.5f, 0.0f ), unify::TexCoords( 0.75f, 0.25f ) }, 
		
		// Right
		{ unify::TexCoords( 0.5f, 0.25f ), unify::TexCoords( 0.75f, 0.5f ) } 
	} 
	);
	Geometry::ptr geo1( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	m_world->GetBlockLibrary().Add( BlockDescription::ptr( new BlockDescription{ "grass", geo1 } ) );

	// Test...
	auto object = GetObjectAllocator()->NewObject( "geo" );
	AddGeometryComponent( object, geo1 );

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	object->GetFrame().SetPosition( unify::V3< float >( x, y, z ) );

	// Camera is based on number of objects
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 10.0f, 0 - 10.0f * 2.0f ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
																 
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", unify::Path( "font2.effect" ) );
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( GetGame(), font2 ) ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.renderInfo.GetDelta() ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
