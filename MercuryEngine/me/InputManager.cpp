// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/InputManager.h>
#include <unify/String.h>

using namespace me;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::AddInputSource( IInputSource::ptr source )
{
	m_sources.push_back( source );
}

IInputSource::ptr InputManager::Find( std::string name )
{
	for( auto & source : m_sources )
	{
		if ( unify::StringIs( source->Name(), name ) )
		{
			return source;
		}
	}
	return nullptr;
}

void InputManager::Update()
{
	for( auto & source : m_sources )
	{
		source->Update();
	}
}

void InputManager::Clear()
{
	m_sources.clear();
}