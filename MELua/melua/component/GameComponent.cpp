// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/GameComponent.h>
#include <melua/exports/ExportGame.h>
#include <melua/CreateState.h>
#include <me/interop/MyThing.h>

using namespace me;
using namespace melua;
using namespace component;

char* GameComponent::Name()
{
	return "LuaGameScript";
}

GameComponent::GameComponent( Script * script )
	: me::game::GameComponent( Name() )
	, m_script{ script }
{
	using namespace me;
	using namespace interop;

	GetLookup()->Add( "luaName",
		me::interop::IValue::ptr{ new interop::MyThing< GameComponent * >(
			this,
			[&]( const GameComponent * component ) -> std::string { return component->GetScript()->GetName(); }
			)
	} );

	GetLookup()->Add( "path",
		me::interop::IValue::ptr{ new interop::MyThing< GameComponent * >(
			this,
			[&]( const GameComponent * component ) -> std::string { return component->GetScript()->GetSource(); }
			)
	} );
}

GameComponent::~GameComponent()
{
}

const Script * GameComponent::GetScript() const
{
	return m_script;
}

void GameComponent::CallMember( std::string function )
{
	// Get our _ENV...
	if ( ! m_script->GetField( LUA_REGISTRYINDEX, m_script->GetName() ) )						   
	{
		Error( m_script->GetState(), "GameComponent not found! (" + m_script->GetName() + ")" );
	}

	int r2 = m_script->GetField( -1, function );

	if ( r2 != 0 )
	{
		try
		{
			if (m_script->PCall( 0, 0, 0 ) != 0 )
			{
				std::string error = m_script->ToString( -1 );
				Error( m_script->GetState(), "Failed in script \"" + m_script->GetSource() +"\" in function " + function + ": " + error );
			}
		}
		catch ( std::exception ex )
		{
			Error( m_script->GetState(), "Exception in script \"" + m_script->GetSource() +"\" in function " + function + ":\n " + ex.what() );
		}
	}
	else
	{
		m_script->Pop( 1 );
	}

	// Pop our _ENV.
	m_script->Pop( 1 );
}

void GameComponent::OnAttach( me::game::IGame * gameInstance )
{
	me::game::GameComponent::OnAttach( gameInstance );

	m_script->Reload();

	// Create table for modules _ENV table.
	m_script->NewTable();

	// Add member variables.
	m_script->PushString( m_script->GetName() );
	m_script->SetField( -2, "_name" );

	// Create new metatable for __index to be _G (so missed functions, non-member functions, look in _G).
	m_script->NewTable();
	m_script->GetGlobal( "_G" );
	m_script->SetField( -2, "__index" );
	m_script->SetMetatable( -2 ); // Set global as the metatable

	// Push to registery with a unique name.
	m_script->SetField( LUA_REGISTRYINDEX, m_script->GetName() );

	// Retrieve registry.
	int i = m_script->GetField( LUA_REGISTRYINDEX, m_script->GetName() );

	// Set the upvalue (_ENV)
	std::string uv = m_script->SetUpValue( -2, 1 );

	int result = m_script->PCall( 0, LUA_MULTRET, 0 );
	if ( result != LUA_OK )
	{
		std::string error = m_script->ToString( -1 );
		Error( m_script->GetState(), "Failed with script initial call: " + error );
	}

	CallMember( "OnInit" );
}
		 
void GameComponent::OnBeforeStartup()
{
	CallMember( "OnBeforeStartup" );
}

void GameComponent::OnAfterStartup()
{
	CallMember( "OnAfterStartup" );
}

void GameComponent::OnUpdate( const me::UpdateParams & params )
{
	CallMember( "OnUpdate" );
}

void GameComponent::OnRender( const me::render::Params & params )
{
	CallMember( "OnRender" );
}

void GameComponent::OnDetach( me::game::IGame * gameInstance )
{
	me::game::GameComponent::OnDetach( gameInstance );
}

std::string GameComponent::GetWhat() const
{
	return std::string();
}