// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>

using namespace me;
using namespace input;

InputCondition::InputCondition( size_t subSource, std::string name )
	: m_subSource{ subSource }
	, m_name{ name }
{
}

InputCondition::~InputCondition()
{
}

std::string InputCondition::GetName() const
{
	return m_name;
}

size_t InputCondition::GetSubSource() const
{
	return m_subSource;
}

