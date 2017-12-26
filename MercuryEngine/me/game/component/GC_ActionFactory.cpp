// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/game/component/GC_ActionFactory.h>
#include <me/scene/SceneManager.h>

// Actions...
#include <me/action/QuitGame.h>
#include <me/action/ObjectAction.h>
#include <me/action/RestartScene.h>
#include <me/action/SendCommand.h>

// Input actions...
#include <me/input/action/IA_Action.h>

// Object Actions...
#include <me/object/action/OA_Action.h>
#include <me/object/action/OA_IgnoreResult.h>
#include <me/object/action/OA_SetPosition.h>
#include <me/object/action/OA_SetComponentValue.h>
#include <me/object/action/OA_List.h>

// Dynas...
#include <me/dyna/position/DP_Absolute.h>
#include <me/dyna/position/DP_Object.h>

using namespace me;
using namespace game;
using namespace component;

char* ActionFactory::Name()
{
	return "ActionFactory";
}

ActionFactory::ActionFactory()
	: GameComponent( Name() )
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
	else if( element->IsTagName( "RestartScene" ) )
	{
		return action::IAction::ptr( new action::RestartScene( GetGame() ) );
	}
	else if( element->IsTagName( "SendCommand" ) )
	{
		return action::IAction::ptr( new action::SendCommand( GetGame(), GetGame()->Command( element->GetAttribute( "command" )->GetString() ), element->GetText() ) );
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
			auto sceneManager = GetGame()->GetComponentT< scene::SceneManager >();
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
	else if( element->IsTagName( "SetComponentValue" ) )
	{
		std::string typeName = element->GetAttribute< std::string >( "type" );
		std::string alias = element->GetAttributeElse< std::string >( "alias", std::string() );
		std::string valueName = element->GetAttribute< std::string >( "valueName" );
		std::string value = element->GetAttribute< std::string >( "value" );

		auto objectAction = new object::action::SetComponentValue( typeName, alias, valueName, value );
		return object::action::IObjectAction::ptr( objectAction );
	}

	return object::action::IObjectAction::ptr();
}

input::IInputAction::ptr ActionFactory::CreateInputAction( const qxml::Element * element )
{
	return input::IInputAction::ptr();
}
