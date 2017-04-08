// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/unify/ExportColor.h>
#include <melua/ScriptEngine.h>

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>

using namespace melua;
using namespace me;

int PushColor(lua_State * state, unify::ColorUnit color )
{
	ColorProxy ** childProxy = (ColorProxy**)( lua_newuserdata(state, sizeof(ColorProxy*)) );
	*childProxy = new ColorProxy{ color };
	luaL_setmetatable(state, "Color");
	return 1;
}

ColorProxy* CheckColor(lua_State * state, int index)
{
	ColorProxy* ud = *(ColorProxy**)luaL_checkudata(state, index, "Color");
	return ud;
}

int Color_RGBA( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float r = (float)lua_tonumber( state, 1 );
	float g = (float)lua_tonumber( state, 2 );
	float b = (float)lua_tonumber( state, 3 );
	float a = (float)lua_tonumber( state, 4 );

	PushColor( state, unify::ColorUnit::ColorUnitRGBA( r, g, b, a ) );

	return 1;
}

int Color_ARGB( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float a = (float)lua_tonumber( state, 1 );
	float r = (float)lua_tonumber( state, 2 );
	float g = (float)lua_tonumber( state, 3 );
	float b = (float)lua_tonumber( state, 4 );

	PushColor( state, unify::ColorUnit::ColorUnitARGB( a, r, g, b ) );

	return 1;
}

int Color_RGB( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	float r = (float)lua_tonumber( state, 1 );
	float g = (float)lua_tonumber( state, 2 );
	float b = (float)lua_tonumber( state, 3 );

	PushColor( state, unify::ColorUnit::ColorUnitRGB( r, g, b ) );

	return 1;
}

int Color_White( lua_State * state )
{
	int args = lua_gettop( state );
	float a = 1.0f;
	if ( args == 1 )
	{
		a = (float)lua_tonumber( state, 1 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitWhite( a ) );

	return 1;
}

int Color_Red( lua_State * state )
{
	int args = lua_gettop( state );
	float r = 1.0f;
	float a = 1.0f;
	if ( args >= 1 )
	{
		r = (float)lua_tonumber( state, 1 );
	}
	if ( args >= 2 )
	{
		a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitRGBA( r, 0.0f, 0.0f, a ) );

	return 1;
}

int Color_Green( lua_State * state )
{
	int args = lua_gettop( state );
	float g = 1.0f;
	float a = 1.0f;
	if ( args >= 1 )
	{
		g = (float)lua_tonumber( state, 1 );
	}
	if ( args >= 2 )
	{
		a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitGreen( g, a ) );

	return 1;
}

int Color_Blue( lua_State * state )
{
	int args = lua_gettop( state );
	float b = 1.0f;
	float a = 1.0f;
	if ( args >= 1 )
	{
		b = (float)lua_tonumber( state, 1 );
	}
	if ( args >= 2 )
	{
		a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitRGBA( 0.0f, 0.0f, b, a ) );

	return 1;
}

int Color_Grey( lua_State * state )
{
	int args = lua_gettop( state );
	float grey = 1.0f;
	float a = 1.0f;
	if ( args >= 1 )
	{
		grey = (float)lua_tonumber( state, 1 );
	}
	if ( args >= 2 )
	{
		a = (float)lua_tonumber( state, 2 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitGrey( grey, a ) );

	return 1;
}

int Color_Black( lua_State * state )
{
	int args = lua_gettop( state );
	float a = 1.0f;
	if ( args >= 1 )
	{
		a = (float)lua_tonumber( state, 1 );
	}

	PushColor( state, unify::ColorUnit::ColorUnitBlack( a ) );

	return 1;
}


int Color_Lerp(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 3);

	unify::ColorUnit l(CheckColor(state, 1)->color);
	unify::ColorUnit r(CheckColor(state, 2)->color);
	float delta = (float)lua_tonumber(state, 3);
	unify::ColorUnit result(unify::ColorUnit::ColorUnitLerp(l, r, delta));

	PushColor(state, result);
	return 1;
}

int Color_ToString( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::ColorUnit color( CheckColor( state, 1 )->color );

	lua_pushstring( state, color.ToString().c_str() );
	return 1;
}

int Color_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 )->color );
	unify::ColorUnit r( CheckColor( state, 2 )->color );
	unify::ColorUnit result( l + r );

	PushColor( state, result );
	return 1;
}

int Color_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 )->color );
	unify::ColorUnit r( CheckColor( state, 2 )->color );
	unify::ColorUnit result( l - r );

	PushColor( state, result );
	return 1;
}

int Color_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 )->color );
	unify::ColorUnit r( CheckColor( state, 2 )->color );
	unify::ColorUnit result( l * r );

	PushColor( state, result );
	return 1;
}

int Color_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::ColorUnit l( CheckColor( state, 1 )->color );
	unify::ColorUnit r( CheckColor( state, 2 )->color );
	unify::ColorUnit result( l / r );

	PushColor( state, result );
	return 1;
}

int Color_Destructor(lua_State * state)
{
	auto color = CheckColor(state, 1);
	delete color;
	return 0;
}

int Color_r(lua_State * state)
{
	int top = lua_gettop(state);

	ColorProxy * colorProxy = CheckColor(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		colorProxy->color.component.r = value;
	}

	lua_pushnumber(state, colorProxy->color.component.r);
	return 1;
}

int Color_g(lua_State * state)
{
	int top = lua_gettop(state);

	ColorProxy * colorProxy = CheckColor(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		colorProxy->color.component.g = value;
	}

	lua_pushnumber(state, colorProxy->color.component.g);
	return 1;
}

int Color_b(lua_State * state)
{
	int top = lua_gettop(state);

	ColorProxy * colorProxy = CheckColor(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		colorProxy->color.component.b = value;
	}

	lua_pushnumber(state, colorProxy->color.component.b);
	return 1;
}

int Color_a(lua_State * state)
{
	int top = lua_gettop(state);

	ColorProxy * colorProxy = CheckColor(state, 1);

	if (top == 2) // If this is an assignment.
	{
		float value = (float)lua_tonumber(state, 2);
		colorProxy->color.component.a = value;
	}

	lua_pushnumber(state, colorProxy->color.component.a);
	return 1;
}

static const luaL_Reg ColorFunctions[] =
{
	{ "ToString", Color_ToString },
	
	{ "r", Color_r },
	{ "g", Color_g },
	{ "b", Color_b },
	{ "a", Color_a },

	{ nullptr, nullptr }
};

void RegisterColor( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "Color", ColorFunctions, sizeof(ColorFunctions) / sizeof(luaL_Reg), nullptr, Color_Destructor };
	type.add = Color_Add;
	type.sub = Color_Sub;
	type.mul = Color_Mul;
	type.div = Color_Div;

	type.named_constructors.push_back({ "RGBA", Color_RGBA });
	type.named_constructors.push_back({ "ARGB", Color_ARGB });
	type.named_constructors.push_back({ "RGB", Color_RGB });
	type.named_constructors.push_back({ "White", Color_White });
	type.named_constructors.push_back({ "Red", Color_Red });
	type.named_constructors.push_back({ "Green", Color_Green });
	type.named_constructors.push_back({ "Blue", Color_Blue });
	type.named_constructors.push_back({ "Grey", Color_Grey });
	type.named_constructors.push_back({ "Black", Color_Black });
	type.named_constructors.push_back({ "ColorLerp", Color_Lerp });
	se->AddType(type);
}
