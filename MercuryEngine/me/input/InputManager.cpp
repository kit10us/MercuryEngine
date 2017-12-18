// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/input/InputManager.h>
#include <me/game/IGame.h>
#include <me/input/ButtonCondition.h>
#include <me/input/ButtonPressedCondition.h>
#include <me/input/TriggerCondition.h>
#include <me/input/StickCondition.h>
#include <me/action/IAction.h>
#include <me/input/action/IA_Action.h>
#include <unify/String.h>

using namespace me;
using namespace input;

InputManager::InputManager( game::IGame * gameInstance )
	: m_game{ gameInstance }
{
}

InputManager::~InputManager()
{
	Clear();
}

void InputManager::AddInputDevice( IInputDevice::ptr source )
{
	m_sourceList.push_back( source );
	m_sourceMap[ source->GetName() ] = source;
}

size_t InputManager::GetSourceCount() const
{
	return m_sourceList.size();
}

IInputDevice::ptr InputManager::GetSource( size_t index ) const
{
	return m_sourceList[ index ];
}

IInputDevice::ptr InputManager::FindSource( std::string name )
{
	auto itr = m_sourceMap.find( name );
	if ( itr == m_sourceMap.end() )
	{
		return nullptr;
	}
	else
	{
		return itr->second;
	}
	return nullptr;
}

IInputCondition::ptr InputManager::MakeCondition(const qxml::Element * element)
{
	if (!element)
	{
		return IInputCondition::ptr();
	}

	std::string conditionName = element->GetAttributeElse< std::string >("condition", "" );
	size_t subSource = element->GetAttributeElse< size_t >("subsource", 0);
	std::string name = element->GetAttributeElse< std::string >("name", "" );
	me::input::IInputCondition::ptr condition;
	if (unify::StringIs(conditionName, "ButtonDown"))
	{
		condition.reset( new ButtonCondition( subSource, name, true ) );
	}
	else if (unify::StringIs(conditionName, "ButtonUp"))
	{
		condition.reset( new ButtonCondition( subSource, name, false ) );
	}
	else if (unify::StringIs(conditionName, "ButtonPressed"))
	{
		condition.reset( new ButtonPressedCondition( subSource, name ) );
	}
	else if (unify::StringIs(conditionName, "Trigger"))
	{
		float threshold = element->GetAttributeElse< float >( "threshold", 0.0f );
		float cap = element->GetAttributeElse< float >( "cap", 1.0f );
		condition.reset( new TriggerCondition( subSource, name, threshold, cap) );
	}
	else if (unify::StringIs(conditionName, "Stick"))
	{
		//     low                high
		//  cap   threshold threshold   cap
		//   |<       >|   +   |<        >|
		unify::V3< float > low_cap;
		unify::V3< float > low_threshold;
		unify::V3< float > high_cap;
		unify::V3< float > high_threshold;

		if( element->HasAttributes( "low_cap" ) )
		{
			low_cap = unify::V3< float >( element->GetAttribute< float >( "low_cap" ) );
		}
		else
		{
			low_cap.x = element->GetAttributeElse< float >( "low_cap_x", -1.0f );
			low_cap.y = element->GetAttributeElse< float >( "low_cap_y", -1.0f );
			low_cap.z = element->GetAttributeElse< float >( "low_cap_z", -1.0f );
		}

		if( element->HasAttributes( "low_threshold" ) )
		{
			low_threshold = unify::V3< float >( element->GetAttribute< float >( "low_threshold" ) );
		}
		else
		{
			low_threshold.x = element->GetAttributeElse< float >( "low_threshold_x", -1.0f );
			low_threshold.y = element->GetAttributeElse< float >( "low_threshold_y", -1.0f );
			low_threshold.z = element->GetAttributeElse< float >( "low_threshold_z", -1.0f );
		}

		if( element->HasAttributes( "high_cap" ) )
		{
			high_cap = unify::V3< float >( element->GetAttribute< float >( "high_cap" ) );
		}
		else
		{
			high_cap.x = element->GetAttributeElse< float >( "high_cap_x", -1.0f );
			high_cap.y = element->GetAttributeElse< float >( "high_cap_y", -1.0f );
			high_cap.z = element->GetAttributeElse< float >( "high_cap_z", -1.0f );
		}

		if( element->HasAttributes( "high_threshold" ) )
		{
			high_threshold = unify::V3< float >( element->GetAttribute< float >( "high_threshold" ) );
		}
		else
		{
			high_threshold.x = element->GetAttributeElse< float >( "high_threshold_x", -1.0f );
			high_threshold.y = element->GetAttributeElse< float >( "high_threshold_y", -1.0f );
			high_threshold.z = element->GetAttributeElse< float >( "high_threshold_z", -1.0f );
		}

		unify::V3< unify::Range< float > > low(
			unify::Range< float >( low_cap.x, low_threshold.x ),
			unify::Range< float >( low_cap.y, low_threshold.y ),
			unify::Range< float >( low_cap.z, low_threshold.z )
		);
		unify::V3< unify::Range< float > > high(
			unify::Range< float >( high_threshold.x, high_cap.x ),
			unify::Range< float >( high_threshold.y, high_cap.y ),
			unify::Range< float >( high_threshold.z, high_cap.z )
		);

		condition.reset( new StickCondition( subSource, name, low, high ) );
	}
	else
	{
		return IInputCondition::ptr();
	}

	return condition;
}

size_t InputManager::AddInputActions(unify::Owner::ptr owner, const qxml::Element * parentNode, bool continueOnFail )
{
	size_t failures = 0;
	for (auto inputActionNode : parentNode->Children("inputaction"))
	{
		bool result = AddSingleInputAction(owner, &inputActionNode);
		if (!result )
		{
			if (!continueOnFail)
			{
				return false;
			}
			failures++;
		}
	}

	return failures;
}

bool InputManager::AddSingleInputAction(unify::Owner::ptr owner, const qxml::Element * element)
{
	std::string inputName = element->GetAttribute< std::string >( "input" );
	auto source = FindSource( inputName );
	if( !source )
	{
		return false;
	}

	auto condition = MakeCondition( element );
	if (!condition)
	{
		return false;
	}

	int actionsAdded = 0;
	for( const auto actionNode : element->Children() )
	{
		auto action = m_game->CreateAction( &actionNode );
		if( !action )
		{
			return false;
		}
		source->AddEvent( owner, condition, IInputAction::ptr( new action::Action( action ) ) );
		actionsAdded++;
	}

	return true;
}

void InputManager::Update( const UpdateParams & params )
{
	if( !m_game->GetOS()->GetHasFocus() )
	{
		return;
	}

	for( auto & source : m_sourceList )
	{
		source->Update();
	}

	for (auto & source : m_sourceList)
	{
		source->HandleEvents( params.GetDelta() );
	}
}

void InputManager::Clear()
{
	m_sourceList.clear();
	m_sourceMap.clear();
}