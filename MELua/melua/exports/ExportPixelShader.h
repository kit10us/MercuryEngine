// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/render/Effect.h>

struct PixelShaderProxy
{
	MELUADLL_API static char* Name();
	me::render::IPixelShader::ptr shader;
};

void RegisterPixelShader( lua_State * state );

