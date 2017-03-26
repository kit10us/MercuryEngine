// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/game/component/GC_ActionFactory.h>
#include <me/scene/SceneManager.h>

// Actions...
#include <me/action/QuitGame.h>
#include <me/action/ObjectAction.h>

// Input actions...
#include <me/input/action/IA_Action.h>

// Object Actions...
#include <me/object/action/OA_Action.h>
#include <me/object/action/OA_IgnoreResult.h>
#include <me/object/action/OA_SetPosition.h>
#include <me/object/action/OA_List.h>

// Dynas...
#include <me/dyna/position/DP_Absolute.h>
#include <me/dyna/position/DP_Object.h>

using namespace me;
using namespace game;
using namespace component;

ActionFactory::ActionFactory()
	: GameComponent( "ActionFactory" )
{
}

ActionFactory::~ActionFactory()
{
}

action::IAction::ptr ActionFactory::CreateAction( const qxml::Element * element )
{
	if( element->IsTagName( "QuitGame" ) )
	{
		return action::IAction::ptr( new action::QuitGame( GetGame() ) );
	}
	else if( element->IsTagName( "ObjectAction" ) )
	{
		std::string objectName = element->GetAttribute< std::string >( "name" );

		auto sceneManager = GetGame()->GetComponentT< scene::SceneManager >();
		auto object = sceneManager->GetCurrentScene()->FindObject( objectName );
		auto action = CreateObjectAction( element->GetFirstChild() );
		return action::IAction::ptr( new action::ObjectAction( object, action ) );
	}

	return action::IAction::ptr();
}

object::action::IObjectAction::ptr ActionFactory::CreateObjectAction( const qxml::Element * element )
{
	std::string name = element->GetAttributeElse< std::string >( "name", "" );
	if( element->IsTagName( "Action" ) )
	{
		auto action = CreateAction( element->GetFirstChild() );
		auto objectAction = new object::action::Action( action );
		return object::action::IObjectAction::ptr( objectAction );
	}
	else if( element->IsTagName( "IgnoreResult" ) )
	{
		auto objectAction = new object::action::IgnoreResult( CreateObjectAction( element->GetFirstChild() ) );
		return object::action::IObjectAction::ptr( objectAction );
	}
	else if( element->IsTagName( "SetPosition" ) )
	{
		dyna::IDynaPosition::ptr position;

		auto type = element->GetAttributeElse< std::string >( "type", "absolute" );
		if( unify::StringIs( type, "absolute" ) )
		{
			position.reset( new dyna::position::Absolute( unify::V3< float >( element->GetText() ) ) );
		}
		else if( unify::StringIs( type, "object" ) )
		{
			auto sceneManager = GetGame()->GetComponentT< scene::SceneManager >( "SceneManager" );
			if( sceneManager )
			{
				auto object = sceneManager->GetCurrentScene()->FindObject( element->GetText() );
				if( object )
				{
					position.reset( new dyna::position::Object( object ) );
				}
			}
		}
		
		if( position )
		{
			auto objectAction = new object::action::SetPosition( position );
			return object::action::IObjectAction::ptr( objectAction );
		}
	}

	return object::action::IObjectAction::ptr();
}

input::IInputAction::ptr ActionFactory::CreateInputAction( const qxml::Element * element )
{
	return input::IInputAction::ptr();
}
