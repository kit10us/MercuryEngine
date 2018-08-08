// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/object/component/ObjectComponent.h>

void RegisterObjectComponent( lua_State * state );


struct ObjectComponentProxy
{
	MELUADLL_API static char* Name();
	me::object::component::ObjectComponent::ptr component;
};

/*
ObjectComponentProxy* CheckUserType< ObjectComponentProxy >( lua_State* state, int index );
int PushObjectComponent( lua_State * state, me::object::component::ObjectComponent::ptr component );
*/