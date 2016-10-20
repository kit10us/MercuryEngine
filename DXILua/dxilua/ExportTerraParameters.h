// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <dxi/Terra.h>

struct TerraParameters
{
	unify::Parameters parameters;
};

int PushTerraParameters( lua_State * state, TerraParameters & parameters );						  
TerraParameters * CheckTerraParameters( lua_State* state, int index );

void RegisterTerraParameters( lua_State * state );
