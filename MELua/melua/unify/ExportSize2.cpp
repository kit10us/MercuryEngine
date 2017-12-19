// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/unify/ExportSize2.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;

int PushSize2(lua_State * state, unify::Size< float > size2)
{
	Size2Proxy ** childProxy = (Size2Proxy**)( lua_newuserdata(state, sizeof(Size2Proxy*)) );
	*childProxy = new Size2Proxy{ size2 };
	luaL_setmetatable(state, "Size2");
	return 1;
}

Size2Proxy* CheckSize2(lua_State * state, int index)
{
	Size2Proxy* ud = *(Size2Proxy**)luaL_checkudata(state, index, "Size2");
	return ud;
}

int Size2_ToString(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::Size< float > size2(CheckSize2(state, 1)->size2);

	lua_pushstring(state, size2.ToString().c_str());
	return 1;
}

int Size2_Add(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size< float > l(CheckSize2(state, 1)->size2);
	unify::Size< float > r(CheckSize2(state, 2)->size2);
	unify::Size< float > result(l + r);

	PushSize2(state, result);
	return 1;
}

int Size2_Sub(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size< float > l(CheckSize2(state, 1)->size2);
	unify::Size< float > r(CheckSize2(state, 2)->size2);
	unify::Size< float > result(l - r);

	PushSize2(state, result);
	return 1;
}

int Size2_Mul(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size< float > l(CheckSize2(state, 1)->size2);
	unify::Size< float > r(CheckSize2(state, 2)->size2);
	unify::Size< float > result(l * r);

	PushSize2(state, result);
	return 1;
}

int Size2_Div(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Size< float > l(CheckSize2(state, 1)->size2);
	unify::Size< float > r(CheckSize2(state, 2)->size2);
	unify::Size< float > result(l / r);

	PushSize2(state, result);
	return 1;
}

int Size2_Length(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);
	unify::Size< float > l(CheckSize2(state, 1)->size2);
	unify::Size< float > r(CheckSize2(state, 2)->size2);
	lua_pushnumber(state, l.Units());
	return 1;
}

int Size2_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop(state);

	if (args == 0)
	{
		return PushSize2(state, unify::Size< float >( 0, 0 ));
	}
	else if (args == 1)
	{
		auto in = CheckSize2(state, 1);
		return PushSize2(state, in->size2);
	}
	else if (args == 2)
	{
		float width = (float)luaL_checknumber(state, 1);
		float height = (float)luaL_checknumber(state, 2);
		return PushSize2(state, unify::Size< float >(width, height));
	}
	else
	{
		lua_pushnil(state);
		return 1;
	}
}

int Size2_Destructor(lua_State * state)
{
	auto size2 = CheckSize2(state, 1);
	delete size2;
	return 0;
}

int Size2_width(lua_State * state)
{
	int top = lua_gettop(state);

	Size2Proxy * size2Proxy = CheckSize2(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		size2Proxy->size2.width = value;
	}

	lua_pushnumber(state, size2Proxy->size2.width);
	return 1;
}

int Size2_height(lua_State * state)
{
	int top = lua_gettop(state);

	Size2Proxy * size2Proxy = CheckSize2(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		size2Proxy->size2.height = value;
	}

	lua_pushnumber(state, size2Proxy->size2.height);
	return 1;
}

static const luaL_Reg Size2Functions[] =
{
	{ "ToString", Size2_ToString },

	{ "width", Size2_width },
	{ "height", Size2_height },

	{ nullptr, nullptr }
};

void RegisterSize2(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "Size2", Size2Functions, Size2_Constructor, Size2_Destructor };
	type.add = Size2_Add;
	type.sub = Size2_Sub;
	type.mul = Size2_Mul;
	type.div = Size2_Div;
	type.len = Size2_Length;
	se->AddType(type);
}
