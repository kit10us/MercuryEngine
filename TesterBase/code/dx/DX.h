#pragma once

#ifdef _DEBUG
#define DXDEBUG
#endif

#include <d3d9.h>
#include <d3dx9.h>

#include <atlbase.h>

#include <dx/Format.h>

inline
void OnFailThrow( HRESULT result, const std::string & what )
{
	if( FAILED( result ) )
	{
		throw std::exception( what.c_str() );
	}
}