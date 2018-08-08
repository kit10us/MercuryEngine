// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/unify/ExportSize3.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;

int PushSize3(lua_State * state, unify::Size3< float > size3)
{
	Size3Proxy ** childProxy = (Size3Proxy**)( lua_newuserdata(state, sizeof(Size3Proxy*)) );
	*childProxy = new Size3Proxy{ size3 };
	luaL_setmetatable(state, "Size3");
	return 1;
}

Size3Proxy* CheckSize3(lua_State * state, int index)
{
	Size3Proxy* ud = *(Size3Proxy**)luaL_checkudata(state, index, "Size3");
	return ud;
}

int Size3_ToString(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::Size3< float > size3(CheckSize3(state, 1)->size3);

	lua_pushstring(state, size3.ToString().c_str());
	return 1;
}

int Size3_Add(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size3< float > l(CheckSize3(state, 1)->size3);
	unify::Size3< float > r(CheckSize3(state, 2)->size3);
	unify::Size3< float > result(l + r);

	PushSize3(state, result);
	return 1;
}

int Size3_Sub(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size3< float > l(CheckSize3(state, 1)->size3);
	unify::Size3< float > r(CheckSize3(state, 2)->size3);
	unify::Size3< float > result(l - r);

	PushSize3(state, result);
	return 1;
}

int Size3_Mul(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size3< float > l(CheckSize3(state, 1)->size3);
	unify::Size3< float > r(CheckSize3(state, 2)->size3);
	unify::Size3< float > result(l * r);

	PushSize3(state, result);
	return 1;
}

int Size3_Div(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size3< float > l(CheckSize3(state, 1)->size3);
	unify::Size3< float > r(CheckSize3(state, 2)->size3);
	unify::Size3< float > result(l / r);

	PushSize3(state, result);
	return 1;
}

int Size3_Length(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);
	unify::Size3< float > l(CheckSize3(state, 1)->size3);
	unify::Size3< float > r(CheckSize3(state, 2)->size3);
	lua_pushnumber(state, l.Units());
	return 1;
}

int Size3_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop(state);

	if (args == 0)
	{
		return PushSize3(state, unify::Size3< float >::Size3(0, 0, 0));
	}
	else if (args == 1)
	{
		auto in = CheckSize3(state, 1);
		return PushSize3(state, in->size3);
	}
	else if (args == 3)
	{
		float width = (float)luaL_checknumber(state, 1);
		float height = (float)luaL_checknumber(state, 2);
		float depth = (float)luaL_checknumber(state, 3);
		return PushSize3(state, unify::Size3< float >(width, height, depth));
	}
	else
	{
		lua_pushnil(state);
		return 1;
	}
}

int Size3_Destructor(lua_State * state)
{
	auto size3 = CheckSize3(state, 1);
	delete size3;
	return 0;
}

int Size3_width(lua_State * state)
{
	int top = lua_gettop(state);

	Size3Proxy * size3Proxy = CheckSize3(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		size3Proxy->size3.width = value;
	}

	lua_pushnumber(state, size3Proxy->size3.width);
	return 1;
}

int Size3_height(lua_State * state)
{
	int top = lua_gettop(state);

	Size3Proxy * size3Proxy = CheckSize3(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		size3Proxy->size3.height = value;
	}

	lua_pushnumber(state, size3Proxy->size3.height);
	return 1;
}

int Size3_depth(lua_State * state)
{
	int top = lua_gettop(state);

	Size3Proxy * size3Proxy = CheckSize3(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		size3Proxy->size3.depth = value;
	}

	lua_pushnumber(state, size3Proxy->size3.depth);
	return 1;
}


static const luaL_Reg Size3Functions[] =
{
	{ "ToString", Size3_ToString },

	{ "width", Size3_width },
	{ "height", Size3_height },
	{ "depth", Size3_depth },

	{ nullptr, nullptr }
};

void RegisterSize3(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "Size3", Size3Functions, Size3_Constructor, Size3_Destructor };
	type.add = Size3_Add;
	type.sub = Size3_Sub;
	type.mul = Size3_Mul;
	type.div = Size3_Div;
	type.len = Size3_Length;
	se->AddType(type);
}
