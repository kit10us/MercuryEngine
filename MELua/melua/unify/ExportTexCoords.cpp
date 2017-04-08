// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/unify/ExportTexCoords.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;

int PushTexCoords(lua_State * state, unify::TexCoords texcoords)
{
	TexCoordsProxy ** childProxy = (TexCoordsProxy**)( lua_newuserdata(state, sizeof(TexCoordsProxy*)) );
	*childProxy = new TexCoordsProxy{ texcoords };
	luaL_setmetatable(state, "TexCoords");
	return 1;
}

TexCoordsProxy* CheckTexCoords(lua_State * state, int index)
{

	TexCoordsProxy* ud = *(TexCoordsProxy**)luaL_checkudata(state, index, "TexCoords");
	return ud;
}

int TexCoords_ToString(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::TexCoords texcoords(CheckTexCoords(state, 1)->texcoords);

	lua_pushstring(state, texcoords.ToString().c_str());
	return 1;
}

int TexCoords_Add(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::TexCoords l(CheckTexCoords(state, 1)->texcoords);
	unify::TexCoords r(CheckTexCoords(state, 2)->texcoords);
	unify::TexCoords result(l + r);

	PushTexCoords(state, result);
	return 1;
}

int TexCoords_Sub(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::TexCoords l(CheckTexCoords(state, 1)->texcoords);
	unify::TexCoords r(CheckTexCoords(state, 2)->texcoords);
	unify::TexCoords result(l - r);

	PushTexCoords(state, result);
	return 1;
}

int TexCoords_Mul(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::TexCoords l(CheckTexCoords(state, 1)->texcoords);
	unify::TexCoords r(CheckTexCoords(state, 2)->texcoords);
	unify::TexCoords result(l * r);

	PushTexCoords(state, result);
	return 1;
}

int TexCoords_Div(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::TexCoords l(CheckTexCoords(state, 1)->texcoords);
	unify::TexCoords r(CheckTexCoords(state, 2)->texcoords);
	unify::TexCoords result(l / r);

	PushTexCoords(state, result);
	return 1;
}

int TexCoords_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	int args = lua_gettop(state);

	if (args == 0)
	{
		return PushTexCoords(state, unify::TexCoords( 0, 0 ));
	}
	else if (args == 1)
	{
		auto in = CheckTexCoords(state, 1);
		return PushTexCoords(state, in->texcoords);
	}
	else if (args == 2)
	{
		float u = (float)luaL_checknumber(state, 1);
		float v = (float)luaL_checknumber(state, 2);
		return PushTexCoords(state, unify::TexCoords(u, v));
	}
	else
	{
		lua_pushnil(state);
		return 1;
	}
}

int TexCoords_Destructor(lua_State * state)
{
	auto texcoords = CheckTexCoords(state, 1);
	delete texcoords;
	return 0;
}

int TexCoords_u(lua_State * state)
{
	int top = lua_gettop(state);

	TexCoordsProxy * texcoordsProxy = CheckTexCoords(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		texcoordsProxy->texcoords.u = value;
	}

	lua_pushnumber(state, texcoordsProxy->texcoords.v);
	return 1;
}

int TexCoords_v(lua_State * state)
{
	int top = lua_gettop(state);

	TexCoordsProxy * texcoordsProxy = CheckTexCoords(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		texcoordsProxy->texcoords.v = value;
	}

	lua_pushnumber(state, texcoordsProxy->texcoords.v);
	return 1;
}

static const luaL_Reg TexCoordsFunctions[] =
{
	{ "ToString", TexCoords_ToString },

	{ "u", TexCoords_u },
	{ "v", TexCoords_v },

	{ nullptr, nullptr }
};

void RegisterTexCoords(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "TexCoords", TexCoordsFunctions, sizeof(TexCoordsFunctions) / sizeof(luaL_Reg), TexCoords_Constructor, TexCoords_Destructor };
	type.add = TexCoords_Add;
	type.sub = TexCoords_Sub;
	type.mul = TexCoords_Mul;
	type.div = TexCoords_Div;
	se->AddType(type);
}
