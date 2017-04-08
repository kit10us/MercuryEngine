// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>

#include <melua/ScriptEngine.h>
#include <melua/ExportObject.h>
#include <melua/ExportTransform.h>
#include <melua/ExportScene.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/ExportComponent.h>
#include <melua/ExportObjectComponent.h>
#include <melua/ExportCameraComponent.h>
#include <melua/ExportGeometry.h>
#include <melua/ExportTerra.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/Util.h>

#include <me/object/GeometryComponent.h>

using namespace me;
using namespace scene;
using namespace object;
using namespace render;

namespace melua
{

	char* ObjectProxy::Name()
	{
		return "Object";
	}

	int Object_AddScript( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 4 );

		auto objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string name = lua_tostring( state, 2 );
		std::string type = lua_tostring( state, 3 );
		unify::Path source( lua_tostring( state, 4 ) );

		ScriptEngine * se = ScriptEngine::GetInstance();

		auto component = se->LoadObjectScript( source );

		objectProxy->object->AddComponent( IObjectComponent::ptr( component ) );

		return 0;
	}

	int Object_GetName( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		lua_pushstring( state, objectProxy->object->GetName().c_str() );

		return 1;
	}

	int Object_SetEnabled( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		bool enabled = lua_toboolean( state, 2 ) ? true : false;

		objectProxy->object->SetEnabled( enabled );
		return 0;
	}

	int Object_IsEnabled( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		lua_pushboolean( state, objectProxy->object->IsEnabled() );

		return 1;
	}

	int Object_AddGeometry( lua_State * state )
	{
		int args = lua_gettop( state );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		ScriptEngine * se = ScriptEngine::GetInstance();
		auto game = se->GetGame();

		Geometry::ptr geometry;

		// If string, pull an existing resource...
		if( lua_type( state, 2 ) == LUA_TSTRING )
		{
			std::string geometryName = lua_tostring( state, 2 );
			geometry = game->GetManager< Geometry >()->Find( geometryName );
		}
		else
		{
			GeometryProxy * geometryProxy = CheckUserType< GeometryProxy >( state, 2 );
			if( geometryProxy )
			{
				geometry = geometryProxy->geometry;
			}
			else
			{
				TerraProxy * terraProxy = CheckTerra( state, 2 );
				if( !terraProxy )
				{
					Error( state, "AddGeometry has invalid type!" );
				}
				geometry = terraProxy->geometry;
			}
		}

		if( lua_gettop( state ) == 3 )
		{
			AddGeometryComponent( objectProxy->object, geometry, CheckUserType< MatrixProxy >( state, 3 )->matrix );
		}
		else 
		{
			AddGeometryComponent( objectProxy->object, geometry );
		}

		return 0;
	}

	int Object_GetSize( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		// TODO:
		lua_pushnumber( state, /*objectProxy->object->GetBBox().Size().Length()*/ 1.0f );

		return 1;
	}

	int Object_Transform( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		TransformProxy ** childProxy = (TransformProxy**)( lua_newuserdata( state, sizeof( TransformProxy* ) ) );
		*childProxy = new TransformProxy;
		luaL_setmetatable( state, "Transform" );
		( *childProxy )->object = objectProxy->object;
		( *childProxy )->transform = &objectProxy->object->GetFrame();
		return 1;
	}

	int Object_AddTag( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string tag( lua_tostring( state, 2 ) );

		objectProxy->object->AddTag( tag );

		return 1;
	}

	int Object_HasTag( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string tag( lua_tostring( state, 2 ) );

		lua_pushboolean( state, objectProxy->object->HasTag( tag ) ? 1 : 0 );

		return 1;
	}

	int Object_GetComponentCount( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		lua_pushnumber( state, objectProxy->object->GetComponentCount() );
		return 1;
	}

	int Object_GetComponent( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string name = lua_tostring( state, 2 );

		auto component = objectProxy->object->GetComponent( name );

		if( !component )
		{
			lua_pushnil( state );
			return 1;
		}

		if( unify::StringIs( component->GetTypeName(), "Camera" ) )
		{
			return PushCameraComponent( state, component );
		}
		else
		{
			return PushUserType< ObjectComponentProxy >( state, { component } );
		}
	}

	int Object_GetComponentTypeName( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string name = lua_tostring( state, 2 );

		IObjectComponent::ptr component = objectProxy->object->GetComponent( name );

		if( !component )
		{
			lua_pushstring( state, component->GetTypeName().c_str() );
			return 1;
		}

		PushUserType< ObjectComponentProxy >( state, { component } );
		return 1;
	}

	int Object_AddComponent( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		std::string type = GetTypename( state, 2 );

		if( unify::StringIs( type, "CameraComponent" ) )
		{
			CameraComponentProxy * cameraComponentProxy = CheckCameraComponent( state, 2 );
			objectProxy->object->AddComponent( cameraComponentProxy->component );
		}
		else if( unify::StringIs( type, "ObjectComponent" ) )
		{
			ObjectComponentProxy * cameraComponentProxy = CheckUserType< ObjectComponentProxy >( state, 2 );
			objectProxy->object->AddComponent( cameraComponentProxy->component );
		}
		else
		{
			ScriptEngine * se = ScriptEngine::GetInstance();
			auto game = se->GetGame();
			Error( state, "Attempted to attach am unsupported type, \"" + type + "\", to an object!" );
		}
		return 0;
	}

	int Object_Constructor( lua_State * state )
	{

		return 1;
	}

	int Object_Destructor( lua_State * state )
	{
		ObjectProxy * proxy = CheckUserType< ObjectProxy >( state, 1 );
		delete proxy;
		return 0;
	}

	void RegisterObject( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		static const luaL_Reg ObjectFunctions[] =
		{
			{ "AddScript", Object_AddScript },
			{ "GetName", Object_GetName },
			{ "SetEnabled", Object_SetEnabled },
			{ "GetEnabled", Object_IsEnabled },
			{ "AddGeometry", Object_AddGeometry },
			{ "GetSize", Object_GetSize },
			{ "Transform", Object_Transform },
			{ "AddComponent", Object_AddComponent },
			{ "GetComponentCount", Object_GetComponentCount },
			{ "GetComponent", Object_GetComponent },
			{ "GetComponentName", Object_GetComponentTypeName },
			{ "AddTag", Object_AddTag },
			{ "HasTag", Object_HasTag },
			{ nullptr, nullptr }
		};
		se->AddType( { "Object", ObjectFunctions, sizeof( ObjectFunctions ) / sizeof( luaL_Reg ), Object_Constructor, Object_Destructor } );
	}
}