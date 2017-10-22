// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <me/render/Terra.h>

struct TerraParameters
{
	unify::Parameters parameters;
};

int PushTerraParameters( lua_State * state, TerraParameters & parameters );						  
TerraParameters * CheckTerraParameters( lua_State* state, int index );

void RegisterTerraParameters( lua_State * state );