// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/input/InputManager.h>
#include <unify/String.h>

using namespace me;
using namespace input;

InputManager::InputManager()
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

void InputManager::Update()
{
	for( auto & source : m_sourceList )
	{
		source->Update();
	}
}

void InputManager::Clear()
{
	m_sourceList.clear();
	m_sourceMap.clear();
}