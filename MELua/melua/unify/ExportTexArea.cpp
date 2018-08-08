// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/unify/ExportTexArea.h>
#include <melua/unify/ExportTexCoords.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;

int PushTexArea(lua_State * state, unify::TexArea texarea)
{
	TexAreaProxy ** childProxy = (TexAreaProxy**)( lua_newuserdata(state, sizeof(TexAreaProxy*)) );
	*childProxy = new TexAreaProxy{ texarea };
	luaL_setmetatable(state, "TexArea");
	return 1;
}

TexAreaProxy* CheckTexArea(lua_State * state, int index)
{

	TexAreaProxy* ud = *(TexAreaProxy**)luaL_checkudata(state, index, "TexArea");
	return ud;
}

int TexArea_ToString(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::TexArea texarea(CheckTexArea(state, 1)->texarea);

	lua_pushstring(state, texarea.ToString().c_str());
	return 1;
}

int TexArea_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop(state);

	if (args == 0)
	{
		return PushTexArea(state, unify::TexAreaFull());
	}
	else if (args == 1)
	{
		auto in = CheckTexArea(state, 1);
		return PushTexArea(state, in->texarea);
	}
	else if (args == 2)
	{
		auto inUL = CheckTexCoords(state, 1);
		auto inDR = CheckTexCoords(state, 2);
		return PushTexArea(state, unify::TexArea( inUL->texcoords, inDR->texcoords ) );
	}
	else if (args == 4)
	{
		float ulu = (float)luaL_checknumber(state, 1);
		float ulv = (float)luaL_checknumber(state, 2);
		float dru = (float)luaL_checknumber(state, 3);
		float drv = (float)luaL_checknumber(state, 4);
		return PushTexArea(state, unify::TexArea(ulu, ulv, dru, drv));
	}
	else
	{
		lua_pushnil(state);
		return 1;
	}
}

int TexArea_Destructor(lua_State * state)
{
	auto texarea = CheckTexArea(state, 1);
	delete texarea;
	return 0;
}

int TexArea_ulu(lua_State * state)
{
	int top = lua_gettop(state);

	TexAreaProxy * texareaProxy = CheckTexArea(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		texareaProxy->texarea.ul.u = value;
	}

	lua_pushnumber(state, texareaProxy->texarea.ul.v);
	return 1;
}

int TexArea_ulv(lua_State * state)
{
	int top = lua_gettop(state);

	TexAreaProxy * texareaProxy = CheckTexArea(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		texareaProxy->texarea.ul.v = value;
	}

	lua_pushnumber(state, texareaProxy->texarea.ul.v);
	return 1;
}

int TexArea_dru(lua_State * state)
{
	int top = lua_gettop(state);

	TexAreaProxy * texareaProxy = CheckTexArea(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		texareaProxy->texarea.dr.u = value;
	}

	lua_pushnumber(state, texareaProxy->texarea.dr.v);
	return 1;
}

int TexArea_drv(lua_State * state)
{
	int top = lua_gettop(state);

	TexAreaProxy * texareaProxy = CheckTexArea(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		texareaProxy->texarea.dr.v = value;
	}

	lua_pushnumber(state, texareaProxy->texarea.dr.v);
	return 1;
}

int TexArea_ul(lua_State * state)
{
	int top = lua_gettop(state);

	TexAreaProxy * texareaProxy = CheckTexArea(state, 1);

	if (top == 2) // If this is an assignment.
	{
		TexCoordsProxy* texcoordsProxy = CheckTexCoords(state, 2);
		texareaProxy->texarea.dr = texcoordsProxy->texcoords;
	}
	else if (top == 3)
	{
		float ulu = (float)lua_tonumber(state, 2);
		float ulv = (float)lua_tonumber(state, 3);
		texareaProxy->texarea.ul.u = ulu;
		texareaProxy->texarea.ul.v = ulv;
	}

	PushTexCoords(state, texareaProxy->texarea.ul );
	return 1;
}

int TexArea_dr(lua_State * state)
{
	int top = lua_gettop(state);

	TexAreaProxy * texareaProxy = CheckTexArea(state, 1);

	if (top == 2) // If this is an assignment.
	{
		TexCoordsProxy* texcoordsProxy = CheckTexCoords(state, 2);
		texareaProxy->texarea.dr = texcoordsProxy->texcoords;
	}
	else if (top == 3)
	{
		float dru = (float)lua_tonumber(state, 2);
		float drv = (float)lua_tonumber(state, 3);
		texareaProxy->texarea.dr.u = dru;
		texareaProxy->texarea.dr.v = drv;
	}

	PushTexCoords(state, texareaProxy->texarea.dr );
	return 1;
}

static const luaL_Reg TexAreaFunctions[] =
{
	{ "ToString", TexArea_ToString },

	{ "ulu", TexArea_ulu },
	{ "ulv", TexArea_ulv },
	{ "dru", TexArea_dru },
	{ "drv", TexArea_drv },
	{ "ul", TexArea_ul },
	{ "dr", TexArea_dr },

	{ nullptr, nullptr }
};

void RegisterTexArea(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "TexArea", TexAreaFunctions, TexArea_Constructor, TexArea_Destructor };
	se->AddType(type);
}
