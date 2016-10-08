// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <unify/Matrix.h>

int ExportMatrix( lua_State * state );

unify::Matrix CheckMatrix( lua_State* state, int index );
