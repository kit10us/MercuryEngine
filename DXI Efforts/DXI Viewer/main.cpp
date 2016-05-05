#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment( lib, "Ws2_32.lib" )

#include "resource.h"
#include <commctrl.h>
#include <string>
#include <boost/lexical_cast.hpp>

#define WM_SOCKET_NOTIFY (WM_USER + 0)
#define IDT_CONNECTING (1)

BOOL CALLBACK SceneTabProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK ObjectTabProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK ConnectTabProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK ViewerProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

struct ConnectionStatus {
	enum TYPE {
		NotConnected, Connecting, Connected
	};
};

// The Viewer class simplifies development by representing the Viewer application. It allows
// shared data in a common location with OO design features.
class Viewer {
	ConnectionStatus::TYPE m_connectionStatus;
	HWND m_viewerHandle;
	HWND m_sceneTabHandle;
	HWND m_objectTabHandle;
	HWND m_connectTabHandle;
	SOCKET m_socket;

public:
	Viewer()
		: m_connectionStatus( ConnectionStatus::NotConnected )
		, m_viewerHandle( 0 )
		, m_sceneTabHandle( 0 )
		, m_objectTabHandle( 0 )
		, m_connectTabHandle( 0 )
		, m_socket( INVALID_SOCKET )
	{
		Disconnect();
	}

	bool Create( HINSTANCE instance, HINSTANCE previousInstance, char * commandLine, int commandShow )
	{
		LPWSTR x = MAKEINTRESOURCE( IDD_VIEWER );
		m_viewerHandle = CreateDialog( instance, MAKEINTRESOURCE( IDD_VIEWER ), 0, (DLGPROC)ViewerProc );
		if ( ! m_viewerHandle )
		{
			MessageBox( 0, TEXT("Failed to create viewer dialog!"), TEXT("ERROR"), MB_OK | MB_ICONERROR );
			return false;
		}

		ShowWindow( m_viewerHandle, commandShow );
		UpdateWindow( m_viewerHandle );
		return true;
	}

	void Go()
	{
		MSG msg;
		bool quit = false;
		while( ! quit )
		{ 
			while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{
				if ( ! IsDialogMessage( m_viewerHandle, &msg ) )
				{
					if( msg.message == WM_QUIT )
					{
						quit = true;
					}
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
		}
	}

	void SetSceneTabHandle( HWND handle )
	{
		m_sceneTabHandle = handle;
	}

	HWND GetSceneTabHandle() const
	{
		return m_sceneTabHandle;
	}

	void SetObjectTabHandle( HWND handle )
	{
		m_objectTabHandle = handle;
	}

	HWND GetObjectTabHandle() const
	{
		return m_objectTabHandle;
	}

	void SetConnectTabHandle( HWND handle )
	{
		m_connectTabHandle = handle;
	}

	HWND GetConnectTabHandle() const
	{
		return m_connectTabHandle;
	}

	void AddToConnectivityLog( const std::string & text, const std::string append = "\r\n" )
	{
		HWND connectivityLogHandle = GetDlgItem( GetConnectTabHandle(), IDC_CONNECTIVITYLOG );
		const size_t bufferSize = 1024;
		char buffer[ bufferSize ];
		int length = GetWindowTextA( connectivityLogHandle, buffer, bufferSize );

		// If we are going to exceed buffer size, truncate from the beginning of the buffer...
		if ( length + text.length() + append.length() + 1 > bufferSize )
		{
			// Find the next line after our additional length...
			int n = text.length() + append.length() + 1;
			while( buffer[ ++n ] != '\n' );
			strcpy_s( buffer, bufferSize, buffer + n );
		}

		strcat_s( buffer, bufferSize, text.c_str() );
		if ( append.empty() == false )
		{
			strcat_s( buffer, bufferSize, append.c_str() );
		}
		SetWindowTextA( connectivityLogHandle, buffer );
		int numberOfLines = SendMessage( connectivityLogHandle, EM_GETLINECOUNT, 0, 0 );
		SendMessage( connectivityLogHandle, EM_LINESCROLL, 0, numberOfLines );
	}

	void Connect()
	{
		WSADATA data;
		int error = WSAStartup( MAKEWORD(2,0), &data );
		if ( error )
		{
			AddToConnectivityLog( "Failed to startup WSA!" );
			return;
		}
		AddToConnectivityLog( std::string( "Starting " ) + data.szDescription + "..." );

		m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if ( m_socket == INVALID_SOCKET )
		{
			AddToConnectivityLog( "Failed to connect!" );
			WSACleanup();
			return;
		}
		AddToConnectivityLog( "Socket created." );

		if ( WSAAsyncSelect( m_socket, m_viewerHandle, WM_SOCKET_NOTIFY, FD_CONNECT | FD_READ ) == SOCKET_ERROR )
		{
			AddToConnectivityLog( "WSAAsyncSelect error!" );
			closesocket( m_socket );
			WSACleanup();
			return;
		}

		sockaddr_in socketAddress;
		HWND addressHandle = GetDlgItem( m_connectTabHandle, IDC_IPADDRESS ); 
		HWND portHandle = GetDlgItem( m_connectTabHandle, IDC_PORT );
		char address[ 256 ];
		GetWindowTextA( addressHandle, address, sizeof( address ) - 1 );
		char port[ 256 ];
		GetWindowTextA( portHandle, port, sizeof( port ) - 1 );
		socketAddress.sin_addr.S_un.S_addr = inet_addr( address );
		socketAddress.sin_family = AF_INET;
		socketAddress.sin_port = htons( atoi( port ) );

		connect( m_socket, (SOCKADDR*)&socketAddress, sizeof( socketAddress ) );
		if ( WSAGetLastError() != WSAEWOULDBLOCK )
		{
			AddToConnectivityLog( "Connection error!" );
			closesocket( m_socket );
			WSACleanup();
			return;
		}

		m_connectionStatus = ConnectionStatus::Connecting;
		AddToConnectivityLog( std::string( "Connecting to " ) + address + ":" + port, std::string() );
		EnableWindow( GetDlgItem( GetConnectTabHandle(), IDC_CONNECT ), FALSE );
		EnableWindow( GetDlgItem( GetConnectTabHandle(), IDC_DISCONNECT ), TRUE );
		SetTimer( GetConnectTabHandle(), IDT_CONNECTING, 1000, 0 );
	}

	void Disconnect()
	{
		if ( m_socket != INVALID_SOCKET )
		{
			closesocket( m_socket );
			m_socket = INVALID_SOCKET;
			WSACleanup();
			KillTimer( GetConnectTabHandle(), IDT_CONNECTING );
			AddToConnectivityLog( std::string() );
			AddToConnectivityLog( "Connection closed." );
			EnableWindow( GetDlgItem( g_Viewer.GetConnectTabHandle(), IDC_CONNECT ), TRUE );
			EnableWindow( GetDlgItem( g_Viewer.GetConnectTabHandle(), IDC_DISCONNECT ), FALSE );
		}
	}

} g_Viewer;


BOOL CALLBACK SceneTabProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

BOOL CALLBACK ObjectTabProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

BOOL CALLBACK ConnectTabProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		SetWindowText( GetDlgItem( hWnd, IDC_PORT ), TEXT("6060") );
		return TRUE;
	case WM_TIMER:
		switch( wParam )
		{
		case IDT_CONNECTING:
			g_Viewer.AddToConnectivityLog( ".", std::string() );
			break;
		}
		return TRUE;
	case WM_SOCKET_NOTIFY:
		{
			WORD event;
			WORD error;
			event = WSAGETSELECTEVENT( lParam );
			error = WSAGETSELECTERROR( lParam );

			switch( event )
			{
			case FD_CONNECT:
				if ( error )
				{
					g_Viewer.AddToConnectivityLog( std::string( "Connect error: " ) + boost::lexical_cast< std::string >( error )  );
				}
				return TRUE;
			case FD_READ:
				return TRUE;
			}
			return FALSE;
		}
		return TRUE;
	case WM_COMMAND:
		{
			WORD notifyCode = HIWORD( wParam );
			WORD id = LOWORD( wParam );
			HWND controlHandle = reinterpret_cast< HWND >( lParam );
			switch( id )
			{
			case IDC_CONNECT:
				g_Viewer.Connect();
				return TRUE;
			case IDC_DISCONNECT:
				g_Viewer.Disconnect();
				return TRUE;
			}
		}
		break;

	}
	return FALSE;
}

BOOL CALLBACK ViewerProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			// Create tabs...
			HINSTANCE instance = reinterpret_cast< HINSTANCE >( GetWindowLong( hWnd, GWL_HINSTANCE ) );
			HWND tabHandle = GetDlgItem( hWnd, IDC_TAB );

			g_Viewer.SetSceneTabHandle( CreateDialog( instance, MAKEINTRESOURCE( IDD_TABSCENE ), tabHandle, (DLGPROC)SceneTabProc ) );
			RECT tabRect;
			GetWindowRect( tabHandle, &tabRect );
			int left = 6;
			int top = 24;
			MoveWindow( g_Viewer.GetSceneTabHandle(), left, top, tabRect.right - tabRect.left - left - 2, tabRect.bottom - tabRect.top - top - 2, false );

			g_Viewer.SetObjectTabHandle( CreateDialog( instance, MAKEINTRESOURCE( IDD_TABOBJECT ), tabHandle, (DLGPROC)ObjectTabProc ) );
			MoveWindow( g_Viewer.GetObjectTabHandle(), left, top, tabRect.right - tabRect.left - left - 2, tabRect.bottom - tabRect.top - top - 2, false );

			g_Viewer.SetConnectTabHandle( CreateDialog( instance, MAKEINTRESOURCE( IDD_TABCONNECT ), tabHandle, (DLGPROC)ConnectTabProc ) );
			MoveWindow( g_Viewer.GetConnectTabHandle(), left, top, tabRect.right - tabRect.left - left - 2, tabRect.bottom - tabRect.top - top - 2, false );

			
			TCITEM item;
			item.mask = TCIF_TEXT | TCIF_PARAM;
			item.pszText = TEXT("Scenes");
			item.lParam = reinterpret_cast< LPARAM >( g_Viewer.GetSceneTabHandle() );
			if ( TabCtrl_InsertItem( tabHandle, 0, &item ) == -1 )
			{
				MessageBox( hWnd, TEXT("Failed to initialize Viewer!"), TEXT("ERROR"), MB_OK | MB_ICONERROR );
				return TRUE;
			}

			item.pszText = TEXT("Object");
			item.lParam = reinterpret_cast< LPARAM >( g_Viewer.GetObjectTabHandle() );
			if ( TabCtrl_InsertItem( tabHandle, 1, &item ) == -1 )
			{
				MessageBox( hWnd, TEXT("Failed to initialize Viewer!"), TEXT("ERROR"), MB_OK | MB_ICONERROR );
				return TRUE;
			}

			item.pszText = TEXT("Connect");
			item.lParam = reinterpret_cast< LPARAM >( g_Viewer.GetConnectTabHandle() );
			if ( TabCtrl_InsertItem( tabHandle, 2, &item ) == -1 )
			{
				MessageBox( hWnd, TEXT("Failed to initialize Viewer!"), TEXT("ERROR"), MB_OK | MB_ICONERROR );
				return TRUE;
			}
			ShowWindow( g_Viewer.GetConnectTabHandle(), SW_SHOWNORMAL );
			UpdateWindow( g_Viewer.GetConnectTabHandle() );

			TabCtrl_SetCurSel( tabHandle, 2 );

			return TRUE;
		}

		break;
	case WM_COMMAND:
		{
			WORD notifyCode = HIWORD( wParam );
			WORD id = LOWORD( wParam );
			HWND controlHandle = reinterpret_cast< HWND >( lParam );
			switch( id )
			{
			case IDC_CLOSE:
				PostQuitMessage( 0 );
				return TRUE;
			}
		}
		break;
	case WM_NOTIFY:
		{
			NMHDR * header = reinterpret_cast< NMHDR * >( lParam );
			switch( header->code )
			{
			case TCN_SELCHANGING:
				{
					int itemIndex = TabCtrl_GetCurSel( header->hwndFrom );
					TCITEM item;
					TabCtrl_GetItem( header->hwndFrom, itemIndex, &item );
					ShowWindow( reinterpret_cast< HWND >( item.lParam ), SW_HIDE );
				}
				return TRUE;
			case TCN_SELCHANGE:
				{
					int itemIndex = TabCtrl_GetCurSel( header->hwndFrom );
					TCITEM item;
					TabCtrl_GetItem( header->hwndFrom, itemIndex, &item );
					ShowWindow( reinterpret_cast< HWND >( item.lParam ), SW_SHOWNORMAL );
				}
				return TRUE;
			}
		}
		return TRUE;

	case WM_CLOSE:
		PostQuitMessage( 0 );
		return TRUE;
	}
	return FALSE;
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE previousInstance, char * commandLine, int commandShow )
{
	previousInstance; // NOT USED

	if ( g_Viewer.Create( instance, previousInstance, commandLine, commandShow ) )
	{
		g_Viewer.Go();
	}
	else
	{
		return 1;
	}
	return 0;
}
