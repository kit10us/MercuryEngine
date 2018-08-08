// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <melua/ScriptEngine.h>
#include <mephy/phy3d/GameComponent3D.h>

namespace mephylua
{
	void RegisterGameComponent( melua::ScriptEngine * luaSE );

	struct GameComponentProxy
	{
		static char* Name();
		mephy::phy3d::GameComponent * component;
	};
}