// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Win32Game.h>


#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")



using namespace dxi;
using namespace core;

class MyGame : public Win32Game
{
protected:
public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;


LPCDIDEVICEINSTANCEA g_deviceInstance( 0 );

//typedef BOOL (FAR PASCAL * LPDIENUMDEVICESCALLBACKA)(LPCDIDEVICEINSTANCEA, LPVOID);
//BOOL DIEnumDevicesCallback( LPCDIDEVICEINSTANCE lpddi,LPVOID pvRef )
bool FAR PASCAL DIEnumDevicesCallback( LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef )
{
    if ( lpddi->dwDevType == 65553 )
    {
        g_deviceInstance = lpddi;
    }
    //std::cout << "Device: " < std::endl;
    return DIENUM_CONTINUE;
}

void MyGame::Startup()
{
	Win32Game::Startup();

    GetDxDevice();

    IDirectInput8A * di( 0 );
    IDirectInputDevice8A * diDevice( 0 );

    DirectInput8Create( GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, 0 );

    HRESULT hr = S_OK;
    unsigned int ref = 13;

    LPDIENUMDEVICESCALLBACKA cb = (LPDIENUMDEVICESCALLBACKA)&DIEnumDevicesCallback;
    hr = di->EnumDevices( DI8DEVCLASS_ALL, cb, &ref, DIEDFL_ALLDEVICES );
    assert( hr );

    DIDEVTYPE_HID;
    GUID_SysKeyboard;


    hr = di->CreateDevice( g_deviceInstance->guidProduct, &diDevice, 0 );
    assert( hr );

    di->Release();
    di = 0;

    diDevice->Release();
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	return Win32Game::Update( elapsed, input );
}

void MyGame::Render()
{
}

void MyGame::Shutdown()
{
	Win32Game::Shutdown();
}
