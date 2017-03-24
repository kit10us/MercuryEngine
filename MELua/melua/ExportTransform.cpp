// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportTransform.h>
#include <melua/ExportObject.h>
#include <melua/ExportScene.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportQuaternion.h>
#include <me/object/GeometryComponent.h>
#include <melua/Util.h>

using namespace melua;
using namespace me;

TransformProxy* CheckTransform( lua_State* state, int index )
{
	TransformProxy* ud = *(TransformProxy**)luaL_checkudata( state, index, "Transform" );
	return ud;
}
			   
int Transform_SetPosition( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::V3< float > position( CheckV3( state, 2 )->v3 );
	
	auto game = ScriptEngine::GetGame();
	
	proxy->transform->SetPosition( position );

	return 0;
}

int Transform_GetPosition(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	TransformProxy * proxy = CheckTransform(state, 1);

	return PushV3( state, proxy->transform->GetPosition() );
}

int Transform_LookAt( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );
	
	unify::V3< float > position( CheckV3( state, 2 )->v3 );
	
	auto game = ScriptEngine::GetGame();
	
	proxy->transform->LookAt( position );

	return 0;
}

int Transform_Orbit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::V3< float > origin( CheckV3( state, 2 )->v3 );
	
	unify::V2< float > axis( CheckV2( state, 3 )->v2 );
	
	float distance = (float)lua_tonumber( state, 4 );
	
	proxy->transform->Orbit( origin, axis, unify::AngleInRadians( distance ) );

	return 0;
}

int Transform_RotateAbout( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	TransformProxy * proxy = CheckTransform( state, 1 );
	
	unify::V3< float > axis( CheckV3( state, 2 )->v3 );
	
	float rotation = (float)lua_tonumber( state, 3 );
	
	proxy->transform->PreMul( unify::Quaternion( axis, unify::AngleInRadians( rotation ) ) );

	return 0;
}

int Transform_GetRotation( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	PushQuaternion( state, proxy->transform->GetRotation() );

	return 1;
}

int Transform_SetRotation( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	unify::Quaternion q = CheckQuaternion( state, 2 )->q;

	proxy->transform->SetRotation( q );

	return 0;
}

int Transform_GetMatrix(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	TransformProxy * proxy = CheckTransform(state, 1);
	PushMatrix(state, proxy->object->GetFrame().GetMatrix());

	return 1;
}

int Transform_PreMul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform( state, 1 );

	std::string secondType = GetTypename(state, 2);

	if (unify::StringIs(secondType, "Quaternion"))
	{
		unify::Quaternion q = CheckQuaternion(state, 2)->q;
		proxy->transform->PreMul(q);
	}
	else if (unify::StringIs(secondType, "Matrix"))
	{ 
		unify::Matrix m = CheckMatrix(state, 2)->matrix;
		proxy->transform->PreMul(m);
	}
	else
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		se->Error("Transform:PreMul", "Invalid parameter type \"" + secondType + "\"!");
	}
	return 0;
}

int Transform_PostMul( lua_State * state )
{
	int args = lua_gettop(state);
	assert(args == 2);

	TransformProxy * proxy = CheckTransform(state, 1);

	std::string secondType = GetTypename(state, 2);

	if (unify::StringIs(secondType, "Quaternion"))
	{
		unify::Quaternion q = CheckQuaternion(state, 2)->q;
		proxy->transform->PostMul(q);
	}
	else if (unify::StringIs(secondType, "Matrix"))
	{
		unify::Matrix m = CheckMatrix(state, 2)->matrix;
		proxy->transform->PostMul(m);
	}
	else
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		se->Error("Transform:PostMul", "Invalid parameter type \"" + secondType + "\"!");
	}
	return 0;
}
 
int Transform_SetModelMatrix( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TransformProxy * proxy = CheckTransform(state, 1);

	unify::Matrix matrix{ CheckMatrix( state, 2 )->matrix };
	proxy->object->GetFrame().SetModelMatrix( matrix );

	return 0;
}

int Transform_GetModelMatrix( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	TransformProxy * proxy = CheckTransform(state, 1);
	PushMatrix( state, proxy->object->GetFrame().GetModelMatrix() );

	return 1;
}

static const luaL_Reg TransformFunctions[] =
{
	{ "SetPosition", Transform_SetPosition },
	{ "GetPosition", Transform_GetPosition },
	{ "LookAt", Transform_LookAt },
	{ "Orbit", Transform_Orbit },
	{ "RotateAbout", Transform_RotateAbout },
	{ "GetRotation", Transform_GetRotation },
	{ "SetRotation", Transform_SetRotation },
	{ "GetMatrix", Transform_GetMatrix },
	{ "PreMul", Transform_PreMul },
	{ "PostMul", Transform_PostMul },
	{ "SetModelMatrix", Transform_SetModelMatrix },
	{ "GetModelMatrix", Transform_GetModelMatrix },
	{ nullptr, nullptr }
};

int Transform_Constructor( lua_State * state )
{																	 
	assert( 0 );  // TODO: Can support this, once I clean up the Transform Proxy to support shared_ptr as as well as straight ptrs.
	return 0;
}

int Transform_Destructor( lua_State * state )
{
	TransformProxy * proxy = CheckTransform( state, 1 );
	delete proxy;
	return 0;
}

void RegisterTransform( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( { "Transform", TransformFunctions, sizeof( TransformFunctions ) / sizeof( luaL_Reg ), Transform_Constructor, Transform_Destructor } );
}
