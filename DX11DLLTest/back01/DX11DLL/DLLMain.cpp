#include <DLLMain.h>

class Keeper
{
public:
	Keeper( HWND hwnd )
	{
	}
	~Keeper()
	{
	}

	void Shutdown( HWND hwnd )
	{
	}

	bool IsShutdown( HWND hwnd )
	{
		return true;
	}

private:
	HWND m_hwnd;

} * g_keeper;

bool Startup( HWND hwnd )
{
	if( g_keeper != nullptr ) return false;

	g_keeper = new Keeper( hwnd );
	return true;
}

void Shutdown( HWND hwnd )
{
	if( g_keeper != nullptr ) return;

	g_keeper->Shutdown( hwnd );

	delete g_keeper;
	g_keeper = nullptr;
}

bool IsShutdown( HWND hwnd )
{
	if( !g_keeper ) return true;

	return g_keeper->IsShutdown( hwnd );
}
