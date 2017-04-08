// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/object/ObjectComponent.h>

void RegisterObjectComponent( lua_State * state );


struct ObjectComponentProxy
{
	MELUADLL_API static char* Name();
	me::object::ObjectComponent::ptr component;
};

/*
ObjectComponentProxy* CheckUserType< ObjectComponentProxy >( lua_State* state, int index );
int PushObjectComponent( lua_State * state, me::object::ObjectComponent::ptr component );
*/