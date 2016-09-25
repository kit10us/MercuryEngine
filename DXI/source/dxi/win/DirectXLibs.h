// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved
#pragma once

// A common header to include Direct-X and Windows in a clean manor.


#ifdef _DEBUG
#define D3D_DEBUG_INFO
#pragma comment( lib, "d3d9" )
#pragma comment( lib, "d3dx9d" )
#else // NDEBUG
#pragma comment( lib, "d3d9" )
#pragma comment( lib, "d3dx9" )
#endif

#pragma comment( lib, "winmm" ) // TOOD: Might not need this now that I'm using chrono

#pragma warning ( disable: 4221 ) // unused lib
#pragma warning ( disable: 4006 ) // duplicate definition

#pragma comment(lib,"dxguid")
#pragma comment(lib,"dinput8")


