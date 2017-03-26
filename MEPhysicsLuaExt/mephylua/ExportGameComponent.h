// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <melua/ScriptEngine.h>
#include <mephy/GameComponent.h>

namespace mephylua
{
	void RegisterGameComponent( melua::ScriptEngine * luaSE );

	struct GameComponentProxy
	{
		static char* Name();
		mephy::GameComponent * component;
	};
}