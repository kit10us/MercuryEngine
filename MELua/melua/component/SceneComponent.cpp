// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/SceneComponent.h>
#include <melua/exports/ExportGame.h>
#include <melua/CreateState.h>
#include <melua/exports/ExportScene.h>
#include <me/interop/MyThing.h>

using namespace melua;
using namespace component;
using namespace me;

SceneComponent::SceneComponent( me::game::IGame * gameInstance, Script * script )
	: me::scene::component::SceneComponent( gameInstance->GetOS(), "LuaScene" )
	, m_game{ gameInstance }
	, m_script( script )
{
	{
		me::interop::Getter< const me::scene::component::ISceneComponent * > scriptNameGetter =
			[&]( const me::scene::component::ISceneComponent * component ) -> std::string {
			return dynamic_cast< const SceneComponent * >( component )->GetScript()->GetName();
		};

		GetLookup()->Add( "luaName", interop::IValue::ptr{
			new interop::MyThing< scene::component::ISceneComponent * >( this, scriptNameGetter )
		} );
	}

	{
		me::interop::Getter< const me::scene::component::ISceneComponent * > scriptSourceGetter =
			[&]( const scene::component::ISceneComponent * component ) -> std::string {
			return dynamic_cast< const SceneComponent * >( component )->GetScript()->GetSource();
		};

		GetLookup()->Add( "luaSource",
			interop::IValue::ptr{ new interop::MyThing< SceneComponent * >( this, scriptSourceGetter )
		} );
	}
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

const Script * SceneComponent::GetScript() const
{
	return m_script;
}

void SceneComponent::OnAttach( me::scene::IScene * scene )
{
	me::scene::component::SceneComponent::OnAttach( scene );

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
	scene::component::SceneComponent::OnBeforeStart();
	CallMember( "OnBeforeStart" );
}

void SceneComponent::OnAfterStart()
{
	CallMember( "OnAfterStart" );
	scene::component::SceneComponent::OnAfterStart();
}

void SceneComponent::OnUpdate( const UpdateParams & params ) 
{
	scene::component::SceneComponent::OnUpdate( params );
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
	scene::component::SceneComponent::OnEnd();
}

std::string SceneComponent::GetWhat() const
{
	return std::string();
}