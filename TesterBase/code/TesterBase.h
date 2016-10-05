#pragma once
#include <dx/DX.h>
#include <Input.h>
#include <DisplayDetails.h>
#include <DisplayActual.h>
#include <POD.h>
#include <vector>
#include <list>

#ifdef CreateWindow // Remove Microsoft definition.
#undef CreateWindow
#endif

// Our system, one-stop, class.
// Note that the functions are listed in order of execution, which means more public/protected/private statements, but this makes for clearer flow.
class TesterBase
{
public:
	TesterBase();
	virtual ~TesterBase();
	
	void SetHardwareAcceleration( bool enabled );
	bool GetHardwareAcceleration() const;

    // Called from our main function (WinMain), this is our one entry point into our system.
	int Go( HINSTANCE hInstance, LPSTR cmdLine, int cmdShow );

	virtual LRESULT HandleWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

protected:

    // Called by Go, before anything substantial happens, giving us an oppertunity to directly guide code flow.
    // We call our setup functions to inform the system how we want to be created and used.
	virtual void BeforeStartup( const std::string & commandLine ) = 0;

    // Called by BeforeStartup to inform the system how we want to be created.
    void CreatePrimaryDisplay( const DisplayDetails & displayDetails );

    // Can be called after CreatePrimaryDisplay to add displays.
    void AddAdditionalDisplay( const DisplayDetails & displayDetails );

private:
    // Called before Startup to create our initial set of pending displays, and perform basic initialization as directed by BeforeStartup(...).
    void CreatePendingDisplays();

    // These are our display creation functions...

    void CreateDisplay( const DisplayDetails & details );
	void CreateWindow( unsigned int width, unsigned int height, signed int cmdShow );
	void CreateDirectX( IDirect3DDevice9 *& dxDevice, HWND hWnd, D3DPRESENT_PARAMETERS & pp );
	
    // Destroys all of our actual displays Direct-X devices.
    void DestroyDirectX();

protected:
    // Called after our first pass at creating displas, so we can load assets, act on basic game types. 
	virtual void Startup() = 0;

    // Called immediately before render, in a tight loop.
    virtual void Update( float delta, Input & input ) = 0;

	virtual void Render( IDirect3DDevice9 * dxDevice, int displayIndex );

    // Finally, allows us to deallocate resources and cleanup - mostly stuff allocated in Startup(...).
	virtual void Shutdown() = 0;

public:
	void RequestQuit();

    size_t DisplayCount() const;
    const std::pair< DisplayDetails, DisplayActual > & GetDisplay( size_t index ) const ;
	std::pair< DisplayDetails, DisplayActual > & GetDisplay( size_t index );
	bool GetDisplayByHWND( HWND hwnd, size_t & indexOut );
	
	HWND GetHWND() const;
    HINSTANCE GetHINSTANCE() const;
	bool IsQuitting() const;

	Input & GetInput();
	//IDirect3DDevice9 * GetDxDevice();

protected:
	HWND m_hwnd; // Primary.

    std::vector< DisplayDetails > m_pendingDisplays;
    std::vector< std::pair< DisplayDetails, DisplayActual > > m_actualDisplays;

    std::list< HWND > m_childHandles; // TODO: Remove for m_actualDisplays.
    HINSTANCE m_hInstance;
    int m_cmdShow;
	bool m_isQuitting;
	Input m_input;
	bool m_hardwareAcceleration;
	IDirect3DDevice9 * m_dxDevice;


private:
	static TesterBase * s_tester;

public:
	static TesterBase * GetInstance();
};

