#pragma once 

#include <windows.h>
#include <cstdlib>
#include <string>
#include <tchar.h>
#include <fstream>
#include <cassert>
#include <string>
#include <iostream>
#include <atlbase.h>

#include <D3D11.h>
#include <D3DCompiler.h>
#include <DirectXMath.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <DWrite.h>

#include <vector>


#pragma comment( lib, "d3d11" )
#pragma comment( lib, "d3dcompiler" )
#pragma comment( lib, "dxguid" )

#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "winmm.lib" )
//C:\Program Files ( x86 )\Windows Kits\8.0\Redist\D3D

// Undefine Microsoft pervasive definitions...

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif