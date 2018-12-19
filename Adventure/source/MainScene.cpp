// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>

#include <me/render/Mesh.h>
#include <me/render/Terra.h>
#include <me/object/component/CameraComponent.h>
#include <me/object/component/GeometryComponent.h>
#include <me/factory/EffectFactories.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>
#include <me/canvas/VList.h>

#include <me/object/component/ObjectActionComponent.h>
#include <me/object/action/OA_SetPosition.h>

#include <me/dyna/position/DP_Absolute.h>
#include <me/dyna/position/DP_Add.h>
#include <me/dyna/position/DP_Object.h>
#include <me/dyna/position/DP_Dampen.h>

#include <PlayerMovementStick.h>
#include <me/input/StickCondition.h>

#include <PlayerMovementButtons.h>
#include <me/input/ButtonCondition.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

class MainSceneCommandListener : public ICommandListener
{
	MainScene * m_mainScene;
public:
	MainSceneCommandListener( MainScene * mainScene )
		: m_mainScene{ mainScene }
	{

	}

	std::string SendCommand( size_t id, std::string extra ) override
	{	
		auto Is = [&]( std::string command )
		{
			return m_mainScene->GetGame()->Command( command ) == id;
		};

		if( Is( "DrawOnMap" ) )
		{
			if( !m_mainScene->m_map )
			{
				return "false";
			}

			std::vector< std::string > args( unify::string::Split< std::string >( extra, ',' ) );

			unify::V2< int > pos( unify::Cast< int >( args[0] ), unify::Cast< int >( args[1] ) );
			std::string type = unify::string::Trim( args[2] );

			m_mainScene->m_map->DrawOnMap( pos, type );

			return "true";
		}
		else if( Is( "GetMapCreated" ) )
		{
			if( !m_mainScene->m_map )
			{
				return "false";
			}
			else
			{
				return "true";
			}
		}
		else if( Is( "BuildMapGround" ) )
		{
			using namespace terrain;

			std::string name = extra;
			Ground::ptr ground( new Ground{} );
			ground->SetEdge( Direction::UL, m_mainScene->GetManager< Geometry >()->Find( name + "_ul" ) );
			ground->SetEdge( Direction::U, m_mainScene->GetManager< Geometry >()->Find( name + "_u" ) );
			ground->SetEdge( Direction::UR, m_mainScene->GetManager< Geometry >()->Find( name + "_ur" ) );
			ground->SetEdge( Direction::L, m_mainScene->GetManager< Geometry >()->Find( name + "_l" ) );
			ground->SetEdge( Direction::C, m_mainScene->GetManager< Geometry >()->Find( name + "_center" ) );
			ground->SetEdge( Direction::R, m_mainScene->GetManager< Geometry >()->Find( name + "_r" ) );
			ground->SetEdge( Direction::DL, m_mainScene->GetManager< Geometry >()->Find( name + "_dl" ) );
			ground->SetEdge( Direction::D, m_mainScene->GetManager< Geometry >()->Find( name + "_d" ) );
			ground->SetEdge( Direction::DR, m_mainScene->GetManager< Geometry >()->Find( name + "_dr" ) );

			ground->SetEdge( Direction::Surround, m_mainScene->GetManager< Geometry >()->Find( name + "_surround" ) );

			ground->SetEdge( Direction::UDL, m_mainScene->GetManager< Geometry >()->Find( name + "_udL" ) );
			ground->SetEdge( Direction::UDR, m_mainScene->GetManager< Geometry >()->Find( name + "_udr" ) );
			ground->SetEdge( Direction::ULR, m_mainScene->GetManager< Geometry >()->Find( name + "_ulr" ) );
			ground->SetEdge( Direction::DLR, m_mainScene->GetManager< Geometry >()->Find( name + "_dlr" ) );

			ground->SetEdge( Direction::UD, m_mainScene->GetManager< Geometry >()->Find( name + "_ud" ) );
			ground->SetEdge( Direction::LR, m_mainScene->GetManager< Geometry >()->Find( name + "_lr" ) );

			m_mainScene->m_map->AddGround( name, ground );
		}
		else if( Is( "AddMapFriend" ) )
		{
			using namespace terrain;

			auto parts = unify::string::Split< std::string >( extra, ',' );
			std::string first = unify::string::Trim( parts[0] );
			std::string second = unify::string::Trim( parts[1] );

			m_mainScene->m_map->AddFriend( first, second );
		}
		else if( Is( "GetMapWidth" ) )
		{
			return unify::Cast< std::string >( m_mainScene->m_mapSize.width );
		}
		else if( Is( "GetMapHeight" ) )
		{
			return unify::Cast< std::string >( m_mainScene->m_mapSize.height );
		}
		else if( Is( "GetMapTerraWidth" ) )
		{
			return unify::Cast< std::string >( m_mainScene->m_terraSize.width );
		}
		else if( Is( "GetMapTerraHeight" ) )
		{
			return unify::Cast< std::string >( m_mainScene->m_terraSize.height );
		}
		else if( Is( "SetMapWidth" ) )
		{
			m_mainScene->m_mapSize.width = unify::Cast< int >( extra );
		}
		else if( Is( "SetMapHeight" ) )
		{
			m_mainScene->m_mapSize.height = unify::Cast< int >( extra );
		}
		else if( Is( "SetMapTerraWidth" ) )
		{
			m_mainScene->m_terraSize.width = unify::Cast< float >( extra );
		}
		else if( Is( "SetMapTerraHeight" ) )
		{
			m_mainScene->m_terraSize.height = unify::Cast< float >( extra );
		}
		else if( Is( "MakeMap" ) )
		{
			using namespace terrain;
			m_mainScene->m_map = new Map( m_mainScene->m_mapSize, m_mainScene->m_terraSize );
			auto land = m_mainScene->GetObjectAllocator()->NewObject( "land" );
			land->AddComponent( object::component::IObjectComponent::ptr( m_mainScene->m_map ) );
		}
		else if( Is( "SaveGame" ) )
		{
		}
		else if( Is( "LoadGame" ) )
		{
		}
		else if( Is( "ToggleMenu" ) )
		{
			auto * canvasCompoinent( m_mainScene->GetComponent( "CanvasComponent" ) );
			auto * canvas = dynamic_cast<canvas::CanvasComponent *>( canvasCompoinent );

			if( !canvas )
			{
				return "";
			}

			me::canvas::IElement::ptr menu = canvas->GetLayer()->FindElement( "menu" );
			if( !menu )
			{
				return "";
			}

			menu->SetEnabled( !menu->IsEnabled() );
		}
		return "";
	}
};


MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
	, m_mapSize{ 30, 30 }
	, m_terraSize{ 8, 8 }
	, m_newMove{ false }
	, m_move{ unify::V3< float >::V3Zero() }
{	
	// Add MainScene command listener (catch -most-)...
	GetGame()->AddCommandListener( GetOwnership(), "DrawOnMap", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "GetMapCreated", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "BuildMapGround", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "AddMapFriend", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "GetMapWidth", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "GetMapHeight", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "GetMapTerraWidth", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "GetMapTerraHeight", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "SetMapWidth", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "SetMapHeight", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "SetMapTerraWidth", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "SetMapTerraHeight", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "MakeMap", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "SaveGame", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "LoadGame", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
	GetGame()->AddCommandListener( GetOwnership(), "ToggleMenu", me::ICommandListener::ptr{ new MainSceneCommandListener( this ) } );
}

void MainScene::OnStart()
{
	using namespace terrain;

	{
		m_map->SetInvalid( GetManager< Geometry >()->Find( "invalid" ) );
	}

	// Add a grass ground...
	{
		Base::ptr ground( new Base{} );
		ground->AddGeneric( GetManager< Geometry >()->Find( "grass" ) );
		m_map->SetBase( ground );
	}

	Object * target = FindObject( "player" );

	// Get the gamepad Input Device...
	auto gamepad = GetGame()->GetInputManager()->FindSource( "gamepad" );
	if( gamepad )
	{
		unify::V3< unify::Range< float > > low{
			unify::Range< float >( -1.0f, -1.0f * 0.3f ),
			unify::Range< float >( -1.0f, -1.0f * 0.3f ),
			unify::Range< float >( -1.0f, -1.0f * 0.3f )
		};
		unify::V3< unify::Range< float > > high{
			unify::Range< float >( 1.0f * 0.3f, 1.0f ),
			unify::Range< float >( 1.0f * 0.3f, 1.0f ),
			unify::Range< float >( 1.0f * 0.3f, 1.0f )
		};

		auto condition = input::IInputCondition::ptr( new input::StickCondition( 0, "LeftStick", low, high ) );
		gamepad->AddEvent( GetOwnership(), condition, input::IInputAction::ptr( new PlayerMovementStick( *this ) ) );
	}

	// Get the gamepad Input Device...
	auto keyboard = GetGame()->GetInputManager()->FindSource( "keyboard" );
	if( keyboard )
	{
		auto conditionUp = input::IInputCondition::ptr( new input::ButtonCondition( 0, "UP", true ) );
		auto conditionDown = input::IInputCondition::ptr( new input::ButtonCondition( 0, "Down", true ) );
		auto conditionLeft = input::IInputCondition::ptr( new input::ButtonCondition( 0, "Left", true ) );
		auto conditionRight = input::IInputCondition::ptr( new input::ButtonCondition( 0, "Right", true ) );
		auto buttonAction = input::IInputAction::ptr( new PlayerMovementButtons( *this ) );
		keyboard->AddEvent( GetOwnership(), conditionUp, buttonAction );
		keyboard->AddEvent( GetOwnership(), conditionDown, buttonAction );
		keyboard->AddEvent( GetOwnership(), conditionLeft, buttonAction );
		keyboard->AddEvent( GetOwnership(), conditionRight, buttonAction );
	}

	// Find the camera created from the scripts.
	Object * camera = FindObject( "camera" );
	if (camera)
	{
		using namespace dyna;

		// Create follow object action, so that every frame our camera follows our target character object.
		auto follow = new object::component::ObjectActionComponent( object::action::IObjectAction::ptr(
			// Create a set position dyna.
			new object::action::SetPosition( IDynaPosition::ptr(
				// Use target object PLUS an absolute offset as position
				new position::Add( 
					IDynaPosition::ptr( new position::Object( target ) ), IDynaPosition::ptr(new position::Absolute({ 0, 17, -12 }))
				)
			) ) ) );
		camera->AddComponent( component::IObjectComponent::ptr( follow ) );
	}

	// Add Canvas component...
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	// Load font...
	Effect::ptr font2( GetManager< Effect >()->Find( "font2" ) );

	// Add FPS...
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( GetGame(), font2, canvas::Anchor::TopRight ) ) );
	
	// Add simple menu...
	{
		using namespace canvas;
		auto menu = new canvas::VList( GetGame(), 10, { 100, 1000 }, canvas::Anchor::TopRight );
		canvas->GetLayer()->AddElement( canvas::IElement::ptr( menu ), "Menu" );
		menu->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "> First Item", Anchor::TopLeft ) ) );
		menu->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "  Second Item", Anchor::TopLeft ) ) );
		menu->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "  Third Item", Anchor::TopLeft ) ) );
		menu->SetEnabled( false );
	}
}

void MainScene::OnUpdate( const me::UpdateParams & params )
{
	if( m_newMove != 0 )
	{
		Object * target = FindObject( "player" );
		if( target )
		{
			// Determine what direction we are facing
			unify::Angle direction = m_move.DotAngle( { 0.0f, 0.0f, 1.0f } );

			// Account for the left (-x) inversing the direction
			if( m_move.x < 0.0f )
			{
				direction *= -1.0f;
			}

			// Accumulate our movement speed...
			float speed = 6.0f;
			float factor = params.GetDelta().GetSeconds() * speed;
			m_move *= factor;

			// Reset our rotation to identity (facing up the z-axis).
			target->GetFrame().SetRotation( unify::QuaternionIdentity() );

			// Move our position...
			unify::V3< float > position = target->GetFrame().GetPosition();
			position += m_move;

			target->GetFrame().SetPosition( position );

			// Face the correct direction.
			target->GetFrame().SetRotation( unify::MatrixRotationY( direction ) );

			// Reset move variables...
			m_newMove = false;
			m_move = unify::V3< float >::V3Zero();
		}
	}
	else
	{
	}
}

std::string MainScene::SendCommand( size_t id, std::string extra )
{
	if( GetGame()->Command( "PlayerUse" ) == id )
	{
 		int x( 0 ); x;
	}

	return std::string();
}