// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/Effect.h>

struct PixelShaderProxy
{
	MELUADLL_API static char* Name();
	me::IPixelShader::ptr shader;
};

void RegisterPixelShader( lua_State * state );

