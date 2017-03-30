// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/ITexture.h>

struct TextureProxy
{
	MELUADLL_API static char* Name();
	me::ITexture::ptr texture;
};

void RegisterTexture( lua_State * state );

