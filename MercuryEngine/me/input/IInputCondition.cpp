// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>

using namespace me;
using namespace input;

InputCondition::InputCondition( IInputSource::ptr source, size_t subSource, size_t index )
	: m_source{ source }
	, m_subSource{ subSource }
	, m_index{ index }
{
}

InputCondition::~InputCondition()
{
}

IInputSource::ptr InputCondition::GetSource() const
{
	return m_source;
}

size_t InputCondition::GetIndex() const
{
	return m_index;
}

size_t InputCondition::GetSubSource() const
{
	return m_subSource;
}


ButtonCondition::ButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, std::string condition )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
	, m_condition{ source->InputConditionIndex( subSource, GetIndex(), condition ) }
{
}

ButtonCondition::~ButtonCondition() 
{
}
										
size_t ButtonCondition::GetCondition() const
{
	return m_condition;
}

bool ButtonCondition::operator()() const
{
	return GetSource()->GetState( GetSubSource(), GetIndex(), GetCondition() ) == State::True;
}


IInputCondition::ptr me::input::MakeButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, std::string condition )
{
	return IInputCondition::ptr( new ButtonCondition( source, subSource, name, condition ) );
}
