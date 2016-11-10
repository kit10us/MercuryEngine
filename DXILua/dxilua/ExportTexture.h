// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <me/ITexture.h>

struct TextureProxy
{
	me::ITexture::ptr texture;
};

DXILUADLL_API TextureProxy* CheckTexture( lua_State* state, int index );
DXILUADLL_API int PushTexture( lua_State * state, me::ITexture::ptr effect );

void RegisterTexture( lua_State * state );

