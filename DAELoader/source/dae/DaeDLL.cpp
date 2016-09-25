// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include "DaeDLL.h"
#include <memory.h>
#include <dxi/win/DXILib.h>

void Deleter( dxi::scripting::IScriptEngine * se )
{
	delete se;
}

__declspec(dllexport) bool __cdecl DXILoader( dxi::core::Game * game )
{
	/*
	game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	*/
	return true;
}


