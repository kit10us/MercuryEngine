// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>

#include <me/Mesh.h>
#include <me/Terra.h>
#include <me/object/CameraComponent.h>
#include <me/object/GeometryComponent.h>
#include <me/factory/EffectFactories.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>
#include <mephy/GameComponent.h>

#include <CameraMotivator.h>


#include <me/object/ObjectActionComponent.h>
#include <me/object/action/OA_SetPosition.h>

#include <me/dyna/position/DP_Absolute.h>
#include <me/dyna/position/DP_Add.h>
#include <me/dyna/position/DP_Object.h>
#include <me/dyna/position/DP_Dampen.h>


using namespace me;
using namespace scene;
using namespace object;

MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
{	
}

void MainScene::OnStart()
{
	// Create map...
	TerrainMap * map = new TerrainMap( { 30, 30 }, { 2, 2 } );
	m_map.reset( map );
	auto land = GetObjectAllocator()->NewObject( "land" );
	land->AddComponent( m_map );
	land->GetFrame().MoveBy( { -20 * 2 * 0.5f, 0, -20 * 2 * 0.5f } );

	/*
	TerrainSceneComponent * map = new TerrainSceneComponent( GetOS(), { 30, 30 }, { 2, 2 } );
	m_mapSC.reset( map );
	AddComponent( m_mapSC );
	*/


	// Reusable...
	Terra::Parameters parameters;
	parameters.SetSize( unify::Size< float >( 2, 2 ) );
	parameters.SetPoints( unify::RowColumn< unsigned int >( 10, 10 ) );
	parameters.SetConstant( 0.0f );

	// 	scene->AddComponent( scene::SceneComponent::ptr( new scene::AutoBBoxSceneComponent( GetOS(), color3DEffect ) ) );

	// Load an effect, then modify it to fit our needs.
	{
		map->SetInvalidGeometry( GetManager< Geometry >()->Find( "invallid" ) );
	}

	// Add a grass ground...
	{
		GroundDesc::ptr ground( new GroundDesc{} );
		ground->AddDefault( GetManager< Geometry >()->Find( "grass" ) );
		map->AddGround( "grass", ground );
	}

	// Add a sand ground...
	{
		GroundDesc::ptr ground( new GroundDesc{} );
		ground->AddDefault( GetManager< Geometry >()->Find( "sand" ) );
		map->AddGround( "sand", ground );
	}

	for ( int i = 0; i < map->GetSize().height && i < map->GetSize().width; i++ )
	{
		int xp = i;
		int xn = map->GetSize().width - i - 1;
		int y = i;
		map->DrawOnMap( { xp, y }, "sand" );
		map->DrawOnMap( { xn, y }, "sand" );
	}
															   
	// Add a camera...
	Object * target = FindObject( "player" );
	auto cameraMotivator = new CameraMotivator();
	target->AddComponent( IObjectComponent::ptr( cameraMotivator ) );

	Object * camera = FindObject( "camera" );
	if (camera)
	{
		using namespace dyna;
		typedef IDynaPosition::ptr DP;

		auto follow = new object::ObjectActionComponent( object::action::IObjectAction::ptr( 
			new object::action::SetPosition( DP(
				new position::Add( 
						DP( new position::Object( target ) ), DP(new position::Absolute({ 0, 17, -12 }))
				)
			) ) ) );
		camera->AddComponent( IObjectComponent::ptr( follow ) );
	}


	mephy::GameComponent * physics = {};
	{												 
		auto component = GetComponentT< mephy::GameComponent >( "MEPhysics" );
	}

	Object * warpObject = FindObject( "cube" );
	//auto warp = physics->CreateBoxCollider( unify::BBox< float >( 1.0f ) );
	//warpObject->AddComponent( warp );

	// Add Canvas component...
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );
	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( GetGame(), font2, canvas::Anchor::TopRight ) ) );
}
