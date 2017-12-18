// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/game/GameComponent.h>

using namespace me;
using namespace game;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "typename", 0 },
		{ "enabled", 1 }
	};

	std::vector< std::string > g_ValuesList
	{
		{ "typename" },
		{ "enabled" }
	};
}

Lookup * GameComponent::GetLookup()
{
	return &m_values;
}

const Lookup * GameComponent::GetLookup() const
{
	return &m_values;
}

GameComponent::GameComponent( std::string typeName )
	: m_typeName{ typeName }
	, m_enabled{ true }
	, m_game{ nullptr }
{
	AddInterface( "IComponent", this );
	AddInterface( "IGameComponent", this );
	AddInterface( "GameComponent", this );
	AddInterface( typeName, this );
}

GameComponent::~GameComponent()
{
}

IGame * GameComponent::GetGame()
{
	return m_game;
}

const IGame * GameComponent::GetGame() const
{
	return m_game;
}

void GameComponent::AddInterface( std::string name, me::IThing* ptr )
{
	m_interfaceMap[ name ] = ptr;
}

void GameComponent::OnAttach( game::IGame * gameInstance )
{
	m_game = gameInstance;
}

void GameComponent::OnDetach( game::IGame * gameInstance ) 
{
	m_game = 0;
}

action::IAction::ptr GameComponent::CreateAction(const qxml::Element * element)
{
	return action::IAction::ptr();
}

object::action::IObjectAction::ptr GameComponent::CreateObjectAction( const qxml::Element * element )
{
	return object::action::IObjectAction::ptr();
}

input::IInputAction::ptr GameComponent::CreateInputAction( const qxml::Element * element )
{
	return input::IInputAction::ptr();
}

input::IInputAction::ptr CreateInputAction( const qxml::Element * element )
{
	return input::IInputAction::ptr();
}

void GameComponent::OnBeforeStartup()
{
}

void GameComponent::OnAfterStartup() 
{
}

void GameComponent::OnEarlyUpdate( const UpdateParams & params )
{
}

void GameComponent::OnUpdate( const UpdateParams & params ) 
{
}

void GameComponent::OnLateUpdate( const UpdateParams & params )
{
}

void GameComponent::OnRender( render::Params params )
{
}

std::string GameComponent::SendCommand( size_t id, std::string extra )
{
	// Stub.
	return std::string();
}

bool GameComponent::IsEnabled() const
{
	return m_enabled;
}

void GameComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

int GameComponent::GetValueCount() const
{
	return (int)g_ValuesList.size() + m_values.Count();
}

bool GameComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr != g_ValuesMap.end() )
	{
		return true;
	}
	return m_values.Exists( name );
}

std::string GameComponent::GetValueName( int index ) const
{
	if ( index < (int)g_ValuesList.size() )
	{
		return g_ValuesList[ index ];
	}
	return m_values.GetName( index - (int)g_ValuesList.size() );
}

int GameComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}
	return m_values.Find( name ) + g_ValuesMap.size();
}

bool GameComponent::SetValue( int index, std::string value )
{
	switch ( index )
	{
	case 0:
		return false;
	case 1:
		m_enabled = unify::Cast< bool >( value );
		return true;
	default:
		return m_values.SetValue( index - g_ValuesMap.size(), value );
	}
}

bool GameComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string GameComponent::GetValue( int index ) const
{
	switch ( index )
	{
	case 0:
		return GetTypeName();
	case 1:
		return unify::Cast< std::string >( m_enabled );
	default:
		return m_values.GetValue( index - g_ValuesMap.size() );
	}
}
 
std::string GameComponent::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}

me::IThing* GameComponent::QueryInterface( std::string name )
{
	auto itr = m_interfaceMap.find( name );
	if ( itr == m_interfaceMap.end() )
	{
		return nullptr;
	}

	return itr->second;
}

std::string GameComponent::GetTypeName() const
{
	return m_typeName;
}

std::string GameComponent::GetWhat() const
{
	return std::string();
}