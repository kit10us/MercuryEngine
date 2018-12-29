// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/game/Game.h>
#include <me/object/component/TagsComponent.h>

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportObject.h>
#include <melua/exports/ExportTransform.h>
#include <melua/exports/ExportScene.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/exports/ExportComponent.h>
#include <melua/exports/ExportObjectComponent.h>
#include <melua/exports/ExportCameraComponent.h>
#include <melua/exports/ExportGeometry.h>
#include <melua/exports/ExportTerra.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/Util.h>

#include <me/object/component/GeometryComponent.h>

using namespace me;
using namespace scene;
using namespace render;

namespace melua
{

	char* ObjectProxy::Name()
	{
		return "Object";
	}

	int Object_AddScript( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 4 );

		auto objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string name = lua_tostring( state, 2 );
		std::string type = lua_tostring( state, 3 );
		unify::Path source( lua_tostring( state, 4 ) );

		auto component = se->LoadObjectScript( source );

		objectProxy->object->AddComponent( object::component::IObjectComponent::ptr( component ) );

		return 0;
	}

	int Object_GetName( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		lua_pushstring( state, objectProxy->object->GetName().c_str() );

		return 1;
	}

	int Object_SetEnabled( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		bool enabled = lua_toboolean( state, 2 ) ? true : false;

		objectProxy->object->SetEnabled( enabled );
		return 0;
	}

	int Object_IsEnabled( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		lua_pushboolean( state, objectProxy->object->IsEnabled() );

		return 1;
	}

	int Object_AddGeometry( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		int args = lua_gettop( state );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		auto gameInstance = se->GetGame();

		Geometry::ptr geometry;

		// If string, pull an existing resource...
		if( lua_type( state, 2 ) == LUA_TSTRING )
		{
			std::string geometryName = lua_tostring( state, 2 );
			geometry = gameInstance->GetManager< Geometry >()->Find( geometryName );
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

	int Object_ClearGeometry( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		int args = lua_gettop( state );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		auto gameInstance = se->GetGame();

		object::component::IObjectComponent::ptr geometryComponent = objectProxy->object->GetComponent( "Geometry" );
		while( geometryComponent )
		{
			objectProxy->object->RemoveComponent( geometryComponent );
			geometryComponent = objectProxy->object->GetComponent( "Geometry" );
		}

		return 0;
	}

	int Object_GetSize( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		// TODO:
		lua_pushnumber( state, /*objectProxy->object->GetBBox().Size().Length()*/ 1.0f );

		return 1;
	}

	int Object_Transform( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );

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
		ScriptEngine * se = ScriptEngine::GetInstance();

		int args = lua_gettop( state );

		std::string value;
		if( args == 3 )
		{
			value = lua_tostring( state, 3 );
		}

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string tag( lua_tostring( state, 2 ) );

		object::component::ObjectComponent::ptr component = objectProxy->object->GetComponent( "Tags" );
		if( component )
		{
			auto * tagsComponent = dynamic_cast< object::component::TagsComponent * >( component.get() );

			tagsComponent->AddTag( tag, value );
		}

		return 0;
	}

	int Object_HasTag( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string tag( lua_tostring( state, 2 ) );

		auto component = objectProxy->object->GetComponent( "Tags" );
		if( component )
		{
			auto * tagsComponent = dynamic_cast< object::component::TagsComponent * >( component.get() );

			lua_pushboolean( state, tagsComponent->HasTag( tag ) ? 1 : 0 );
			return 1;
		}

		lua_pushboolean( state, 0 );
		return 1;
	}

	int Object_GetTagValue( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string tag( lua_tostring( state, 2 ) );

		auto component = objectProxy->object->GetComponent( "Tags" );
		if( component )
		{
			auto * tagsComponent = dynamic_cast< object::component::TagsComponent * >( component.get() );

			lua_pushstring( state, tagsComponent->GetTagValue( tag ).c_str() );
		}

		return 1;
	}

	int Object_GetComponentCount( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		lua_pushnumber( state, objectProxy->object->GetComponentCount() );
		return 1;
	}

	int Object_GetComponent( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string name = lua_tostring( state, 2 );

		auto component = objectProxy->object->GetComponent( name );

		if( !component )
		{
			lua_pushnil( state );
			return 1;
		}

		if( unify::string::StringIs( component->GetTypeName(), "Camera" ) )
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
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );
		std::string name = lua_tostring( state, 2 );

		auto component = objectProxy->object->GetComponent( name );

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
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 2 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		std::string type = GetTypename( state, 2 );

		if( unify::string::StringIs( type, "CameraComponent" ) )
		{
			CameraComponentProxy * cameraComponentProxy = CheckCameraComponent( state, 2 );
			objectProxy->object->AddComponent( cameraComponentProxy->component );
		}
		else if( unify::string::StringIs( type, "ObjectComponent" ) )
		{
			ObjectComponentProxy * cameraComponentProxy = CheckUserType< ObjectComponentProxy >( state, 2 );
			objectProxy->object->AddComponent( cameraComponentProxy->component );
		}
		else
		{
			ScriptEngine * se = ScriptEngine::GetInstance();
			auto gameInstance = se->GetGame();
			Error( state, "Attempted to attach am unsupported type, \"" + type + "\", to an object!" );
		}
		return 0;
	}

	int Object_GetPosition( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		int args = lua_gettop( state );

		if( !se->Assert( args == 1, "Object:GetPosition, invalid number of parameters! Expected 1, had " + unify::Cast< std::string >( args ) + "!" ) )
		{
			PushNil( state );
			return 1;
		}

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		PushV3( state, objectProxy->object->GetFrame().GetPosition() );
		return 1;
	}

	int Object_GetForward( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );
		
		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		PushV3( state, objectProxy->object->GetFrame().GetForward() );
		return 1;
	}

	int Object_GetUp( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		PushV3( state, objectProxy->object->GetFrame().GetUp() );
		return 1;
	}

	int Object_GetLeft( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();

		se->AssertTop( 1 );

		ObjectProxy * objectProxy = CheckUserType< ObjectProxy >( state, 1 );

		PushV3( state, objectProxy->object->GetFrame().GetLeft() );
		return 1;
	}

	int Object_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
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
			{ "ClearGeometry", Object_ClearGeometry },
			{ "GetSize", Object_GetSize },
			{ "Transform", Object_Transform },
			{ "AddComponent", Object_AddComponent },
			{ "GetComponentCount", Object_GetComponentCount },
			{ "GetComponent", Object_GetComponent },
			{ "GetComponentName", Object_GetComponentTypeName },
			{ "AddTag", Object_AddTag },
			{ "HasTag", Object_HasTag },
			{ "GetTagValue", Object_GetTagValue },
			{ "GetPosition", Object_GetPosition },
			{ "GetForward", Object_GetForward },
			{ "GetUp", Object_GetUp },
			{ "GetLeft", Object_GetLeft },
			{ nullptr, nullptr }
		};

		Type type = { "Object", ObjectFunctions, Object_Constructor, Object_Destructor };
		se->AddType( type );
	}
}