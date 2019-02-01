// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <melua/unify/ExportV3.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;
 
int PushV3( lua_State * state, unify::V3< float > v3 )
{
	V3Proxy ** childProxy = (V3Proxy**)(lua_newuserdata( state, sizeof( V3Proxy* ) ));
	*childProxy = new V3Proxy{ v3 };
	luaL_setmetatable( state, "V3" );
	return 1;
}

V3Proxy* CheckV3( lua_State * state, int index )
{
	V3Proxy* ud = *(V3Proxy**)luaL_checkudata( state, index, "V3" );
	return ud;
}

int V3_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( CheckV3( state, 1 )->v3 );

	lua_pushstring( state, v3.ToString().c_str() );
	return 1;
}

int V3_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 )->v3 );
	unify::V3< float > r( CheckV3( state, 2 )->v3 );
	unify::V3< float > result( l + r );

	PushV3( state, result );
	return 1;
}

int V3_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 )->v3 );
	unify::V3< float > r( CheckV3( state, 2 )->v3 );
	unify::V3< float > result( l - r );

	PushV3( state, result );
	return 1;
}

int V3_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );


	std::string typeL = GetTypename( state, 1 );
	std::string typeR = GetTypename( state, 2 );

	unify::V3< float > l;
	unify::V3< float > r;

	if( unify::string::StringIs( typeL, "V3" ) )
	{
		l = CheckV3( state, 1 )->v3;
	}
	else if( unify::string::StringIs( typeL, "Number" ) )
	{
		l = unify::V3< float >( (float)lua_tonumber( state, 1 ) );
	}

	if( unify::string::StringIs( typeR, "V3" ) )
	{
		r = CheckV3( state, 2 )->v3;
	}
	else if( unify::string::StringIs( typeR, "Number" ) )
	{
		r = unify::V3< float >( (float)lua_tonumber( state, 2 ) );
	}

	unify::V3< float > result( l * r );

	PushV3( state, result );
	return 1;
}

int V3_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 )->v3 );
	unify::V3< float > r( CheckV3( state, 2 )->v3 );
	unify::V3< float > result( l / r );

	PushV3( state, result );
	return 1;
}
 
int V3_Length( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );
	unify::V3< float > l( CheckV3( state, 1 )->v3 );
	unify::V3< float > r( CheckV3( state, 2 )->v3 );
	lua_pushnumber( state, l.Length() );
	return 1;
}

int V3_Normalize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	auto v3( CheckV3( state, 1 ) );
	float result = v3->v3.Normalize();
	lua_pushnumber(state, result);
	return 1;
}			   

int V3_DistanceTo( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 )->v3 );
	unify::V3< float > r( CheckV3( state, 2 )->v3 );

	lua_pushnumber( state, l.DistanceTo( r ) );
	return 1;
}

int V3_Dot( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > l( CheckV3( state, 1 )->v3 );
	unify::V3< float > r( CheckV3( state, 2 )->v3 );

	lua_pushnumber( state, l.Dot( r ) );
	return 1;
}

int V3_Angle(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V3< float > l(CheckV3(state, 1)->v3);
	unify::V3< float > r(CheckV3(state, 2)->v3);

	lua_pushnumber(state, l.DotAngle(r).ToDegrees());
	return 1;
}

int V3_Inverse( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > v3( CheckV3( state, 1 )->v3 );

	PushV3( state, unify::V3< float >::V3Inverse( v3 ) );
	return 1;
}

int V3_Lerp( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	unify::V3< float > l( CheckV3( state, 1 )->v3 );
	unify::V3< float > r( CheckV3( state, 2 )->v3 );
	float delta = (float)lua_tonumber( state, 3 );
	unify::V3< float > result( unify::V3< float >::V3Lerp( l, r, delta ) );

	PushV3( state, result );
	return 1;
}


int V3_Constructor( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop( state );

	if ( args == 0 )
	{
		return PushV3( state, unify::V3< float >::V3Zero() );
	}
	else if (args == 1)
	{
		std::string type = GetTypename( state, 1 );
		if( unify::string::StringIs( type, "Number" ) )
		{
			float value = (float)lua_tonumber( state, 1 );
			return PushV3( state, unify::V3< float >( value ) );
		}
		else
		{
			auto in = CheckV3( state, 1 );
			return PushV3( state, in->v3 );
		}
	}
	else if ( args == 3 )
	{
		float x = (float)luaL_checknumber( state, 1 );
		float y = (float)luaL_checknumber( state, 2 );
		float z = (float)luaL_checknumber( state, 3 );
		return PushV3( state, unify::V3< float >( x, y, z ) );
	}
	else
	{
		lua_pushnil( state );
		return 1;
	}
}

int V3_V3Cross(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V3< float > l(CheckV3(state, 1)->v3);
	unify::V3< float > r(CheckV3(state, 2)->v3);
	unify::V3< float > result(unify::V3< float >::V3Cross(l, r));

	PushV3(state, result);
	return 1;
}

int V3_V3Lerp(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 3);

	unify::V3< float > l(CheckV3(state, 1)->v3);
	unify::V3< float > r(CheckV3(state, 2)->v3);
	float delta = (float)lua_tonumber(state, 3);
	unify::V3< float > result( unify::V3< float >::V3Lerp( l, r, delta ) );

	PushV3(state, result);
	return 1;
}

int V3_Destructor( lua_State * state )
{
	auto v3 = CheckV3(state, 1);
	delete v3;
	return 0;
}

int V3_x( lua_State * state )
{
	int top = lua_gettop( state );

	V3Proxy * v3Proxy = CheckV3( state, 1 );

	if ( top == 2 ) // If this is an assignment.
	{
		float value = (float)lua_tonumber( state, 2 );
		v3Proxy->v3.x = value;
	}

	lua_pushnumber( state, v3Proxy->v3.x );
	return 1;
}

int V3_y( lua_State * state )
{
	int top = lua_gettop( state );

	V3Proxy * v3Proxy = CheckV3( state, 1 );

	if ( top == 2 ) // If this is an assignment.
	{
		float value = (float)lua_tonumber( state, 2 );
		v3Proxy->v3.y = value;
	}

	lua_pushnumber( state, v3Proxy->v3.y );
	return 1;
}

int V3_z( lua_State * state )
{
	int top = lua_gettop( state );

	V3Proxy * v3Proxy = CheckV3( state, 1 );

	if ( top == 2 ) // If this is an assignment.
	{
		float value = (float)lua_tonumber( state, 2 );
		v3Proxy->v3.z = value;
	}

	lua_pushnumber( state, v3Proxy->v3.z );
	return 1;
}


static const luaL_Reg V3Functions[] =
{
	{ "Normalize", V3_Normalize },
	{ "DistanceTo", V3_DistanceTo },
	{ "Dot", V3_Dot },
	{ "Angle", V3_Angle },
	{ "Inverse", V3_Inverse },
	{ "Lerp", V3_Lerp },
	{ "ToString", V3_ToString },

	{ "x", V3_x },
	{ "y", V3_y },
	{ "z", V3_z },

	{ nullptr, nullptr }
};

void RegisterV3( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "V3", V3Functions, V3_Constructor, V3_Destructor };
	type.add = V3_Add;
	type.sub = V3_Sub;
	type.mul = V3_Mul;
	type.div = V3_Div;
	type.len = V3_Length;
	type.named_constructors.push_back({ "V3Cross", V3_V3Cross });
	type.named_constructors.push_back({ "V3Lerp", V3_V3Lerp });
	se->AddType( type );
}
