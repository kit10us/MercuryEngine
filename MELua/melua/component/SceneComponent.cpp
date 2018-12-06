// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/SceneComponent.h>
#include <melua/exports/ExportGame.h>
#include <melua/CreateState.h>
#include <melua/exports/ExportScene.h>

using namespace melua;
using namespace component;
using namespace me;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		// Name, Index into value.
		{ "luaName", 0 },
		{ "path", 1 },
	};

	std::vector< std::string > g_ValuesList
	{
		{ "luaName" },
		{ "path" },
	};
}

SceneComponent::SceneComponent( me::game::IGame * gameInstance, Script * script )
	: me::scene::SceneComponent( gameInstance->GetOS(), "LuaScene" )
	, m_game{ gameInstance }
	, m_script( script )
{
}

SceneComponent::~SceneComponent()
{
}

void SceneComponent::CallMember( std::string function )
{
	// Get our _ENV...
	if ( ! m_script->GetField( LUA_REGISTRYINDEX, m_script->GetName() ) )						   
	{
		Error( m_script->GetState(), "SceneComponent not found! (" + m_script->GetName() + ")" );
	}

	int r2 = m_script->GetField( -1, function );

	if ( r2 != 0 )
	{
		try
		{
			if ( m_script->PCall( 0, 0, 0 ) != 0 )
			{
				std::string error = m_script->ToString( -1 );
				m_game->Debug()->ReportError( me::ErrorLevel::Failure, "LUA", "Failed in script \"" + m_script->GetSource() +"\" in function " + function + ": " + error );
			}
		}
		catch ( std::exception ex )
		{
			m_game->Debug()->ReportError( me::ErrorLevel::Failure, "LUA", "Exception in script \"" + m_script->GetSource() + "\" in function " + function + ":\n " + ex.what() );
		}
	}
	else
	{
		m_script->Pop( 1 );
	}

	// Pop our _ENV.
	m_script->Pop( 1 );
}

void SceneComponent::OnAttach( me::scene::IScene * scene )
{
	me::scene::SceneComponent::OnAttach( scene );

	m_script->Reload();
	
	m_script->GetGlobal( scene->GetName() + "_env" );
	m_script->SetUpValue( -2, 1 );

	// Create table for modules _ENV table.
	m_script->NewTable(); // 2 - table

	// Add field name:scene->GetName()
	m_script->PushString( scene->GetName() ); // 2 - scene->GetName()
	m_script->SetField( 2, "name" ); // 2 - table
	
	// Add variable this:scene
	{
		PushScene( m_script->GetState(), scene ); // 3 - Scene
		m_script->SetField( 2, "this" ); // 2 - table
	}

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

void SceneComponent::OnDetach( me::scene::IScene * scene )
{
}

void SceneComponent::OnBeforeStart()
{
	scene::SceneComponent::OnBeforeStart();
	CallMember( "OnBeforeStart" );
}

void SceneComponent::OnAfterStart()
{
	CallMember( "OnAfterStart" );
	scene::SceneComponent::OnAfterStart();
}

void SceneComponent::OnUpdate( const UpdateParams & params ) 
{
	scene::SceneComponent::OnUpdate( params );
	CallMember( "OnUpdate" );
}

void SceneComponent::OnSuspend()
{
	CallMember( "OnSuspend" );
}

void SceneComponent::OnResume()
{
	CallMember( "OnResume" );
}

void SceneComponent::OnEnd()
{
	CallMember("OnEnd");
	scene::SceneComponent::OnEnd();
}

std::string SceneComponent::GetWhat() const
{
	return std::string();
}

int SceneComponent::GetValueCount() const
{
	return me::scene::SceneComponent::GetValueCount() + (int)g_ValuesList.size();
}

bool SceneComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return true;
	}
	return me::scene::SceneComponent::ValueExists( name );
}

std::string SceneComponent::GetValueName( int index ) const
{
	if( index < me::scene::SceneComponent::GetValueCount() )
	{
		return me::scene::SceneComponent::GetValueName( index );
	}

	int localIndex = index - me::scene::SceneComponent::GetValueCount();
	if( localIndex < (int)g_ValuesList.size() )
	{
		return g_ValuesList[localIndex];
	}

	return std::string();
}

int SceneComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}

	return me::scene::SceneComponent::FindValueIndex( name );
}

bool SceneComponent::SetValue( int index, std::string value )
{
	if( index < me::scene::SceneComponent::GetValueCount() )
	{
		return me::scene::SceneComponent::SetValue( index, value );
	}

	int localIndex = index - me::scene::SceneComponent::GetValueCount();
	switch( localIndex )
	{
	case 0:
		return false;

	case 1:
		return false;

	default:
		return false;
	}
}

std::string SceneComponent::GetValue( int index ) const
{
	if( index < me::scene::SceneComponent::GetValueCount() )
	{
		return me::scene::SceneComponent::GetValue( index );
	}

	int localIndex = index - me::scene::SceneComponent::GetValueCount();
	switch( localIndex )
	{
	case 0:
		return m_script->GetName();

	case 1:
		return m_script->GetSource();

	default:
		return std::string();
	}
}
