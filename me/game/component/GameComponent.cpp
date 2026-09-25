// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/game/component/GameComponent.h>
#include <me/interop/ReferenceCast.h>
#include <me/game/IGame.h>

using namespace me;
using namespace game;
using namespace component;

GameComponent::GameComponent( std::string typeName )
	: m_typeName{ typeName }
	, m_enabled{ true }
	, m_game{ nullptr }
{
	AddInterface( "IComponent", this );
	AddInterface( "IGameComponent", this );
	AddInterface( "GameComponent", this );

	GetLookup()->Add( "typename", interop::IValue::ptr{ new interop::ReferenceCast< std::string >( m_typeName, false, true ) } );
	GetLookup()->Add( "enabled", interop::IValue::ptr{ new interop::ReferenceCast< bool >( m_enabled ) } );
}

GameComponent::~GameComponent()
{
}

kit::debug::IBlock* GameComponent::GetBlock()
{
	return m_block.get();
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

unify::Result<> GameComponent::OnAttach( game::IGame * gameInstance )
{
	m_game = gameInstance;
	m_block = gameInstance->Debug()->GetLogger()->CreateBlock( "GameComponent \"" + GetTypeName() + "\"" );
	return {};
}

unify::Result<> GameComponent::OnDetach( game::IGame * gameInstance ) 
{
	m_game = nullptr;
	return {};
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

unify::Result<> GameComponent::BeforeOnStartup()
{
	return {};
}

unify::Result<> GameComponent::AfterOnStartup() 
{
	return {};
}

unify::Result<> GameComponent::EarlyOnUpdate( const UpdateParams & params )
{
	return {};
}

unify::Result<> GameComponent::OnUpdate( const UpdateParams & params ) 
{
	return {};
}

unify::Result<> GameComponent::LateOnUpdate( const UpdateParams & params )
{
	return {};
}

unify::Result<> GameComponent::OnRender( const render::Params & params )
{
	return {};
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

interop::Interop * GameComponent::GetLookup()
{
	return &m_values;
}

const interop::Interop * GameComponent::GetLookup() const
{
	return &m_values;
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
