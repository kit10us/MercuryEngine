// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <lua.hpp>
#include <dxi/Texture.h>

struct TextureProxy
{
	dxi::Texture::ptr texture;
};

DXILUADLL_API TextureProxy* CheckTexture( lua_State* state, int index );
DXILUADLL_API int PushTexture( lua_State * state, dxi::Texture::ptr effect );

void RegisterTexture( lua_State * state );

