// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/input/InputManager.h>
#include <me/IGame.h>
#include <me/input/ButtonCondition.h>
#include <me/input/ButtonPressedCondition.h>
#include <me/input/TriggerCondition.h>
#include <me/input/StickCondition.h>
#include <me/action/IAction.h>
#include <me/input/InputAction.h>
#include <unify/String.h>

using namespace me;
using namespace input;

InputManager::InputManager( IGame * game )
	: m_game{ game }
{
}

InputManager::~InputManager()
{
	Clear();
}

void InputManager::AddInputSource( IInputSource::ptr source )
{
	m_sourceList.push_back( source );
	m_sourceMap[ source->Name() ] = source;
}

size_t InputManager::GetSourceCount() const
{
	return m_sourceList.size();
}

IInputSource::ptr InputManager::GetSource( size_t index ) const
{
	return m_sourceList[ index ];
}

IInputSource::ptr InputManager::FindSource( std::string name )
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

	std::string inputName = element->GetAttribute< std::string >("input");
	auto source = FindSource(inputName);
	if (!source)
	{
		return IInputCondition::ptr();
	}

	std::string conditionName = element->GetAttributeElse< std::string >("condition", "" );
	size_t subSource = element->GetAttributeElse< size_t >("subsource", 0);
	std::string name = element->GetAttributeElse< std::string >("name", "" );
	me::input::IInputCondition::ptr condition;
	if (unify::StringIs(conditionName, "ButtonDown"))
	{
		condition.reset( new ButtonCondition(source, subSource, name, true ) );
	}
	else if (unify::StringIs(conditionName, "ButtonUp"))
	{
		condition.reset( new ButtonCondition(source, subSource, name, false ) );
	}
	else if (unify::StringIs(conditionName, "ButtonPressed"))
	{
		condition.reset( new ButtonPressedCondition( source, subSource, name ) );
	}
	else if (unify::StringIs(conditionName, "Trigger"))
	{
		float threshold = element->GetAttributeElse< float >( "threshold", 0.0f );
		float cap = element->GetAttributeElse< float >( "cap", 1.0f );
		condition.reset( new TriggerCondition( source, subSource, name, threshold, cap) );
	}
	else if (unify::StringIs(conditionName, "Stick"))
	{
		StickAxis axis = StickAxisFromString(element->GetAttribute< std::string >("axis"));
		float cap_low = element->GetAttributeElse< float >("cap_low", -1.0f);
		float threshold_low = element->GetAttributeElse< float >("threshold_low", 0.0f);
		float threshold_high = element->GetAttributeElse< float >("threshold_high", 0.0f);
		float cap_high = element->GetAttributeElse< float >("cap_high", 1.0f);
		condition.reset( new StickCondition( source, subSource, name, axis, cap_low, threshold_low, threshold_high, cap_high) );
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
	auto actionNode = element->FindFirstElement("action");
	if (!actionNode)
	{
		return false;
	}

	auto condition = MakeCondition( element );
	if (!condition)
	{
		return false;
	}

	auto action = m_game->CreateAction(actionNode);
	if (!action)
	{
		return false;
	}

	condition->GetSource()->AddEvent(owner, condition, IInputAction::ptr( new InputAction( action ) ) );
	return true;
}

void InputManager::Update()
{
	for( auto & source : m_sourceList )
	{
		source->Update();
	}

	for (auto & source : m_sourceList)
	{
		source->HandleEvents();
	}
}

void InputManager::Clear()
{
	m_sourceList.clear();
	m_sourceMap.clear();
}