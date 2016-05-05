// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/DXDevice.h>
#include <cassert>

using namespace dxi;
using namespace win;


IDirect3DDevice9 * s_dxDevice = 0;

void DX::SetDxDevice( IDirect3DDevice9 * dxDevice )
{
	s_dxDevice = dxDevice;
}

IDirect3DDevice9 * DX::GetDxDevice()
{
	assert( s_dxDevice );
	return s_dxDevice;
}
