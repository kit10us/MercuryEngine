// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>

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

