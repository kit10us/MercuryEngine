// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/object/BBoxRendererComponent.h>
#include <me/object/CameraComponent.h>
#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/scene/SceneManager.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <sg/ShapeCreators.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
{	
}

void MainScene::OnStart()
{
	//Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", "EffectColor.effect" );

	//Effect::ptr color3DEffect = GetManager< Effect >()->Add( "ColorInstanced3D", "EffectColorInstanced3D_SC.effect" );

	//Effect::ptr color3DEffect = GetManager< Effect >()->Add( "ColorInstanced3D", "EffectColorInstanced3D.effect" );

	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "ColorInstanced_ambient", unify::Path( "ColorInstanced_ambient.effect" ) );

	/*
	AddComponent( scene::SceneComponent::ptr( new scene::AutoBBoxSceneComponent( GetOS(), color3DEffect ) ) );
	*/



	// Add a camera...
	m_camera = GetObjectAllocator()->NewObject( "camera" );
	m_camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( m_camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().AspectRatioWH(), 1.0f, 1000.0f ) );

	float shapesize = 1.0f;

	// Geo1
	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
	cubeParameters.SetSize( unify::Size3< float >( shapesize, shapesize, shapesize ) );
	cubeParameters.SetDiffuse( unify::Color::ColorBlue() );
	Geometry::ptr geo1( sg::CreateShape( GetOS()->GetRenderer( 0 ), cubeParameters ) );

	// Geo2
	sg::SphereParameters sphereParameters;
	sphereParameters.SetEffect( color3DEffect );
	sphereParameters.SetRadius( shapesize );
	sphereParameters.SetDiffuse( unify::Color::ColorRed() );
	Geometry::ptr geo2( sg::CreateShape( GetOS()->GetRenderer( 0 ), sphereParameters ) );

	// Geo3
	sg::ConeParameters coneParameters;
	coneParameters.SetEffect( color3DEffect );
	coneParameters.SetHeight( shapesize );
	coneParameters.SetRadius( shapesize );
	coneParameters.SetDiffuse( unify::Color::ColorGreen() );
	Geometry::ptr geo3( sg::CreateShape( GetOS()->GetRenderer( 0 ), coneParameters ) );


	// Block object counts...
	int objectCountX = 10;
	int objectCountY = 10;
	int objectCountZ = 10;
	float spacing = 2.0f;

	// Block size...
	auto sectorSpacing = 20.0f;
	auto sectorSizeX = (shapesize * objectCountX) + (spacing * (objectCountX - 1));
	auto sectorSizeY = (shapesize * objectCountY) + (spacing * (objectCountY - 1));
	auto sectorSizeZ = (shapesize * objectCountZ) + (spacing * (objectCountZ - 1));

	// Sets of blocks counts...
	int sectorsCountX = 3;
	int sectorsCountY = 1;
	int sectorsCountZ = 3;
	auto totalSectorsSizeX = (sectorSizeX * sectorsCountX) + (sectorSpacing * sectorsCountX);
	auto totalSectorsSizeY = (sectorSizeY * sectorsCountY) + (sectorSpacing * sectorsCountY);
	auto totalSectorsSizeZ = (sectorSizeZ * sectorsCountZ) + (sectorSpacing * sectorsCountZ);

	// Surround with geometry...
	for ( int sectorZ = 0; sectorZ < sectorsCountZ; sectorZ++ )
	{
		for ( int sectorY = 0; sectorY < sectorsCountY; sectorY++ )
		{
			for ( int sectorX = 0; sectorX < sectorsCountX; sectorX++ )
			{
				// Skip center block (camera's area)...
				if ( sectorX == (sectorsCountX / 2) && sectorY == (sectorsCountY / 2) && sectorZ == (sectorsCountZ / 2) )
				{
					continue;
				}

				float centerX = (totalSectorsSizeX * -0.5f) + (sectorX * ( sectorSpacing + sectorSizeX)) + ((sectorSpacing + sectorSizeX) * 0.5f);
				float centerY = (totalSectorsSizeY * -0.5f) + (sectorY * ( sectorSpacing + sectorSizeY)) + ((sectorSpacing + sectorSizeY) * 0.5f);
				float centerZ = (totalSectorsSizeZ * -0.5f) + (sectorZ * ( sectorSpacing + sectorSizeZ)) + ((sectorSpacing + sectorSizeZ) * 0.5f);

				// Create a block of geometry...
				for ( auto oz = 0; oz < objectCountZ; oz++ )
				{
					for ( auto oy = 0; oy < objectCountY; oy++ )
					{
						for ( auto ox = 0; ox < objectCountX; ox++ )
						{
							size_t shape = (ox + oy + oz) % 3;
							auto object = GetObjectAllocator()->NewObject( "geo" );
							AddGeometryComponent( object, (shape == 0) ? geo1 : (shape == 1) ? geo2 : geo3 );

							float x = ((shapesize + spacing) * objectCountX * -0.5f) + ox * (shapesize + spacing);
							float y = ((shapesize + spacing) * objectCountY * -0.5f) + oy * (shapesize + spacing);
							float z = ((shapesize + spacing) * objectCountZ * -0.5f) + oz * (shapesize + spacing);

							object->GetFrame().SetPosition( { centerX + x, centerY + y, centerZ + z } );
						}
					}
				}
			}
		}
	}

	/*
	{
		// One object test...
		auto object = GetObjectAllocator()->NewObject( "geo" );
		AddGeometryComponent( object, geo2 );

		object->GetFrame().SetPosition( { 0.0f, 0.0f, 10.0f } );
	}
	*/

	m_camera->GetFrame().SetPosition( { 0, 0, 0 } );
	m_camera->GetFrame().LookAt( { 0, 0, 10 } );
																	
	{
		using namespace canvas;
		CanvasComponent::ptr canvas( new CanvasComponent( GetGame() ) );
		AddComponent( canvas );

		Effect::ptr font2 = GetManager< Effect>()->Add( "font2", unify::Path( "font2.effect" ) );
		canvas->GetLayer()->AddElement( IElement::ptr( new FPS( GetGame(), font2 ) ) );

		m_text = new TextElement( GetGame(), font2, "Objects = N", Anchor::BottomLeft );
		canvas->GetLayer()->AddElement( IElement::ptr( m_text ) );
	}
}

void MainScene::OnUpdate( UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	m_camera->GetFrame().RotateAboutAxis( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );

	static float elapsed = 0.0f;
	if ( elapsed < 0.0f )
	{
		auto sceneManager = GetGame()->GetComponentT< SceneManager >();
		m_text->SetText( "Objects = " + unify::Cast< std::string >( sceneManager->GetRenderCount() ) );
		elapsed = 1.0f;
	}
	elapsed -= params.GetDelta();
}
