// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>

#include <me/render/Mesh.h>
#include <me/render/Terra.h>
#include <me/object/CameraComponent.h>
#include <me/object/GeometryComponent.h>
#include <me/factory/EffectFactories.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>
#include <me/canvas/VList.h>

#include <me/input/StickCondition.h>

#include <me/object/ObjectActionComponent.h>
#include <me/object/action/OA_SetPosition.h>

#include <me/dyna/position/DP_Absolute.h>
#include <me/dyna/position/DP_Add.h>
#include <me/dyna/position/DP_Object.h>
#include <me/dyna/position/DP_Dampen.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

class PlayerMovementStick : public input::IInputAction
{
	Object * m_object;
public:
	PlayerMovementStick( Object * object )
		: m_object{ object }
	{
	}

	bool Perform( input::IInputDevice * device, input::IInputCondition * condition, float delta )
	{
		float speed = 6.0f;

		unify::V3< float > move = condition->GetValue( device ).xzy();

		// Normalize, so if we move in an angle, we aren't moving twice as fast.
		move.Normalize();

		// Determine what direction we are facing
		unify::Angle direction = move.DotAngle( { 0.0f, 0.0f, 1.0f } );

		// Account for the left (-x) inversing the direction
		if( move.x < 0.0f )
		{
			direction *= -1.0f;
		}

		// Accumulate our movement speed...
		float factor = delta * speed;
		move *= factor;

		// Reset our rotation to identity (facing up the z-axis).
		m_object->GetFrame().SetRotation( unify::QuaternionIdentity() );

		// Move our position...
		unify::V3< float > position = m_object->GetFrame().GetPosition();
		position += move;

		m_object->GetFrame().SetPosition( position );

		// Face the correct direction.
		m_object->GetFrame().SetRotation( unify::MatrixRotationY( direction ) );
		
		return true;
	}
};


MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
	, m_mapSize{ 30, 30 }
	, m_terraSize{ 8, 8 }
{	
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
		gamepad->AddEvent( GetOwnership(), condition, input::IInputAction::ptr( new PlayerMovementStick( target ) ) );
	}

	// Find the camera created from the scripts.
	Object * camera = FindObject( "camera" );
	if (camera)
	{
		using namespace dyna;

		// Create follow object action, so that every frame our camera follows our target character object.
		auto follow = new object::ObjectActionComponent( object::action::IObjectAction::ptr( 
			// Create a set position dyna.
			new object::action::SetPosition( IDynaPosition::ptr(
				// Use target object PLUS an absolute offset as position
				new position::Add( 
					IDynaPosition::ptr( new position::Object( target ) ), IDynaPosition::ptr(new position::Absolute({ 0, 17, -12 }))
				)
			) ) ) );
		camera->AddComponent( IObjectComponent::ptr( follow ) );
	}

	// Add Canvas component...
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	// Load font...
	Effect::ptr font2( new Effect( 
		GetManager< IVertexShader >()->Add( unify::Path( "texture2d_trans.xml" ) ), 
		GetManager< IPixelShader>()->Add( unify::Path( "texture2d_trans.xml" ) ),
		GetManager< ITexture >()->Add( unify::Path( "font2.png" ) )
	) );

	// Add FPS...
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( GetGame(), font2, canvas::Anchor::TopRight ) ) );
	
	// Add simple menu...
	{
		using namespace canvas;
		auto menu = new canvas::VList( GetGame(), 10, { 100, 1000 }, canvas::Anchor::TopRight );
		canvas->GetLayer()->AddElement( canvas::IElement::ptr( menu ) );
		menu->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "> First Item", Anchor::TopLeft ) ) );
		menu->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "  Second Item", Anchor::TopLeft ) ) );
		menu->AddItem( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "  Third Item", Anchor::TopLeft ) ) );
		menu->SetEnabled( false );
	}


}

std::string MainScene::SendCommand( std::string command, std::string extra )
{
	if( unify::StringIs( command, "DrawOnMap" ) )
	{
		if( !m_map )
		{
			return "false";
		}

		std::vector< std::string > args( unify::Split< std::string >( extra, ',' ) );

		unify::V2< int > pos( unify::Cast< int >( args[0] ), unify::Cast< int >( args[ 1 ] ) );
		std::string type = unify::Trim( args[2] );
		
		m_map->DrawOnMap( pos, type );
		
		return "true";
	}
	else if( unify::StringIs( command, "GetMapCreated" ) )
	{
		if( !m_map )
		{
			return "false";
		}
		else
		{
			return "true";
		}
	}
	else if( unify::StringIs( command, "BuildMapGround" ) )
	{
		using namespace terrain;

		std::string name = extra;
		Ground::ptr ground( new Ground{} );
		ground->SetEdge( Direction::UL, GetManager< Geometry >()->Find( name + "_ul" ) );
		ground->SetEdge( Direction::U, GetManager< Geometry >()->Find( name + "_u" ) );
		ground->SetEdge( Direction::UR, GetManager< Geometry >()->Find( name + "_ur" ) );
		ground->SetEdge( Direction::L, GetManager< Geometry >()->Find( name + "_l" ) );
		ground->SetEdge( Direction::C, GetManager< Geometry >()->Find( name + "_center" ) );
		ground->SetEdge( Direction::R, GetManager< Geometry >()->Find( name + "_r" ) );
		ground->SetEdge( Direction::DL, GetManager< Geometry >()->Find( name + "_dl" ) );
		ground->SetEdge( Direction::D, GetManager< Geometry >()->Find( name + "_d" ) );
		ground->SetEdge( Direction::DR, GetManager< Geometry >()->Find( name + "_dr" ) );

		ground->SetEdge( Direction::Surround, GetManager< Geometry >()->Find( name + "_surround" ) );

		ground->SetEdge( Direction::UDL, GetManager< Geometry >()->Find( name + "_udL" ) );
		ground->SetEdge( Direction::UDR, GetManager< Geometry >()->Find( name + "_udr" ) );
		ground->SetEdge( Direction::ULR, GetManager< Geometry >()->Find( name + "_ulr" ) );
		ground->SetEdge( Direction::DLR, GetManager< Geometry >()->Find( name + "_dlr" ) );

		ground->SetEdge( Direction::UD, GetManager< Geometry >()->Find( name + "_ud" ) );
		ground->SetEdge( Direction::LR, GetManager< Geometry >()->Find( name + "_lr" ) );

		m_map->AddGround( name, ground );
	}
	else if( unify::StringIs( command, "AddMapFriend" ) )
	{
		using namespace terrain;

		auto parts = unify::Split< std::string >( extra, ',' );
		std::string first = unify::Trim( parts[0] );
		std::string second = unify::Trim( parts[1] );
		
		m_map->AddFriend( first, second );
	}
	else if(unify::StringIs( command, "GetMapWidth" ) )
	{
		return unify::Cast< std::string >( m_mapSize.width );
	}
	else if( unify::StringIs( command, "GetMapHeight" ) )
	{
		return unify::Cast< std::string >( m_mapSize.height );
	}
	else if( unify::StringIs( command, "GetMapTerraWidth" ) )
	{
		return unify::Cast< std::string >( m_terraSize.width );
	}
	else if( unify::StringIs( command, "GetMapTerraHeight" ) )
	{
		return unify::Cast< std::string >( m_terraSize.height );
	}
	else if( unify::StringIs( command, "SetMapWidth" ) )
	{
		m_mapSize.width = unify::Cast< int >( extra );
	}
	else if( unify::StringIs( command, "SetMapHeight" ) )
	{
		m_mapSize.height = unify::Cast< int >( extra );
	}
	else if( unify::StringIs( command, "SetMapTerraWidth" ) )
	{
		m_terraSize.width = unify::Cast< float >( extra );
	}
	else if( unify::StringIs( command, "SetMapTerraHeight" ) )
	{
		m_terraSize.height = unify::Cast< float >( extra );
	}
	else if( unify::StringIs( command, "MakeMap" ) )
	{
		using namespace terrain;
		m_map = new Map( m_mapSize, m_terraSize );
		auto land = GetObjectAllocator()->NewObject( "land" );
		land->AddComponent( object::IObjectComponent::ptr( m_map ) );
	}	

	else if( unify::StringIs( command, "SaveGame" ) )
	{
		// TODO:
	}

	return "";
}
