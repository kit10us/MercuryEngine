// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/InputMotivator.h>

using namespace me;
using namespace motivator;
using namespace input;

InputMotivator::InputMotivator()
{
}

InputMotivator::~InputMotivator() 
{
}

void InputMotivator::AddMotivation( std::string motivation, IInputCondition::ptr condition )
{
	auto itr = m_motivationsMap.find( motivation );
	if ( itr != m_motivationsMap.end() )
	{
		m_motivationConditions[ itr->second ] = condition;
		m_motivationNames[ itr->second ] = motivation;
	}
	else
	{
		m_motivationsMap[ motivation ] = m_motivationConditions.size();
		m_motivationConditions.push_back( condition );
		m_motivationNames.push_back( motivation );
	}
}

size_t InputMotivator::GetMotivationCount() const
{
	return m_motivationNames.size();
}

std::string InputMotivator::GetMotivationName( size_t index ) const
{
	return m_motivationNames[ index ];
}

input::IInputCondition::ptr InputMotivator::GetCondition( size_t index ) const
{
	return m_motivationConditions[ index ];
}

input::IInputCondition::ptr InputMotivator::GetCondition( std::string name ) const
{
	auto itr = m_motivationsMap.find( name );
	if ( itr != m_motivationsMap.end() )
	{
		return m_motivationConditions[ itr->second ];
	}
	else
	{
		return input::IInputCondition::ptr();
	}
}
