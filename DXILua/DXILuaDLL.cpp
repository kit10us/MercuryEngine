// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DXILuaDLL.h"
#include <dxilua/DXILua.h>


// This is an example of an exported variable
DXILUADLL_API int nDXILuaDLL=0;

// This is an example of an exported function.
DXILUADLL_API int fnDXILuaDLL(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see DXILuaDLL.h for the class definition
CDXILuaDLL::CDXILuaDLL()
{
    return;
}

DXILUADLL_API bool DXILoader( dxi::core::Game & game )
{
	game.AddScriptEngine( "lua", new dxilua::ScriptEngine( game ) );
	return true;
}