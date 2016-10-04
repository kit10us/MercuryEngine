// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/DXDevice.h>
#include <cassert>

using namespace dxi;
using namespace win;


#if defined( DIRECTX9 )

IDirect3DDevice9 * s_dxDevice {};

void DX::SetDxDevice( IDirect3DDevice9 * dxDevice )
{
	s_dxDevice = dxDevice;
}

IDirect3DDevice9 * DX::GetDxDevice()
{
	assert( s_dxDevice );
	return s_dxDevice;
}

#elif defined( DIRECTX11 )

ID3D11Device * s_dxDevice{};

void DX::SetDxDevice( ID3D11Device * dxDevice )
{
	s_dxDevice = dxDevice;
}

ID3D11Device * DX::GetDxDevice()
{
	return s_dxDevice;
}					  

ID3D11DeviceContext * s_dxContext {};

void DX::SetDxContext( ID3D11DeviceContext * dxContext )
{
	s_dxContext = dxContext;
}

ID3D11DeviceContext * DX::GetDxContext()
{
	return s_dxContext;
}	

#endif