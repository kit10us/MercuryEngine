// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/unify/ExportV2.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;

int PushV2(lua_State * state, unify::V2< float > v2)
{
	V2Proxy ** childProxy = (V2Proxy**)( lua_newuserdata(state, sizeof(V2Proxy*)) );
	*childProxy = new V2Proxy{ v2 };
	luaL_setmetatable(state, "V2");
	return 1;
}

V2Proxy* CheckV2(lua_State * state, int index)
{

	V2Proxy* ud = *(V2Proxy**)luaL_checkudata(state, index, "V2");
	return ud;
}

int V2_ToString(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::V2< float > v2(CheckV2(state, 1)->v2);

	lua_pushstring(state, v2.ToString().c_str());
	return 1;
}

int V2_Add(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);
	unify::V2< float > result(l + r);

	PushV2(state, result);
	return 1;
}

int V2_Sub(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);
	unify::V2< float > result(l - r);

	PushV2(state, result);
	return 1;
}

int V2_Mul(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);
	unify::V2< float > result(l * r);

	PushV2(state, result);
	return 1;
}

int V2_Div(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);
	unify::V2< float > result(l / r);

	PushV2(state, result);
	return 1;
}

int V2_Length(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);
	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);
	lua_pushnumber(state, l.Length());
	return 1;
}

int V2_Normalize(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	auto v2(CheckV2(state, 1));
	v2->v2.Normalize();
	return 0;
}

int V2_DistanceTo(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);

	lua_pushnumber(state, l.DistanceTo(r));
	return 1;
}

int V2_Dot(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);

	lua_pushnumber(state, l.Dot(r));
	return 1;
}

int V2_Angle(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);

	lua_pushnumber(state, l.Angle(r).ToDegrees());
	return 1;
}

int V2_Inverse(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::V2< float > v2(CheckV2(state, 1)->v2);

	PushV2(state, unify::V2< float >::V2Inverse(v2));
	return 1;
}

int V2_Lerp(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 3);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);
	float delta = (float)lua_tonumber(state, 3);
	unify::V2< float > result(unify::V2< float >::V2Lerp(l, r, delta));

	PushV2(state, result);
	return 1;
}

int V2_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop(state);

	if (args == 0)
	{
		return PushV2(state, unify::V2< float >::V2Zero());
	}
	else if (args == 1)
	{
		auto in = CheckV2(state, 1);
		return PushV2(state, in->v2);
	}
	else if (args == 2)
	{
		float x = (float)luaL_checknumber(state, 1);
		float y = (float)luaL_checknumber(state, 2);
		return PushV2(state, unify::V2< float >(x, y));
	}
	else
	{
		lua_pushnil(state);
		return 1;
	}
}

int V2_V2Lerp(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 3);

	unify::V2< float > l(CheckV2(state, 1)->v2);
	unify::V2< float > r(CheckV2(state, 2)->v2);
	float delta = (float)lua_tonumber(state, 3);
	unify::V2< float > result(unify::V2< float >::V2Lerp(l, r, delta));

	PushV2(state, result);
	return 1;
}

int V2_Destructor(lua_State * state)
{
	auto v2 = CheckV2(state, 1);
	delete v2;
	return 0;
}

int V2_x(lua_State * state)
{
	int top = lua_gettop(state);

	V2Proxy * v2Proxy = CheckV2(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		v2Proxy->v2.x = value;
	}

	lua_pushnumber(state, v2Proxy->v2.x);
	return 1;
}

int V2_y(lua_State * state)
{
	int top = lua_gettop(state);

	V2Proxy * v2Proxy = CheckV2(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		v2Proxy->v2.y = value;
	}

	lua_pushnumber(state, v2Proxy->v2.y);
	return 1;
}

static const luaL_Reg V2Functions[] =
{
	{ "Normalize", V2_Normalize },
	{ "DistanceTo", V2_DistanceTo },
	{ "Dot", V2_Dot },
	{ "Angle", V2_Angle },
	{ "Inverse", V2_Inverse },
	{ "Lerp", V2_Lerp },
	{ "ToString", V2_ToString },

	{ "x", V2_x },
	{ "y", V2_y },

	{ nullptr, nullptr }
};

void RegisterV2(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "V2", V2Functions, sizeof(V2Functions) / sizeof(luaL_Reg), V2_Constructor, V2_Destructor };
	type.add = V2_Add;
	type.sub = V2_Sub;
	type.mul = V2_Mul;
	type.div = V2_Div;
	type.len = V2_Length;
	type.named_constructors.push_back({ "V2Lerp", V2_V2Lerp });
	se->AddType(type);
}
