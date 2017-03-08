// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Window.h>
#include <cassert>

using namespace ui;

namespace
{
	LRESULT CALLBACK Builder_WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		static std::map< HWND, IWindow* > s_windows;
		IWindow * window = nullptr;
		{
			auto itr = s_windows.find( hWnd );
			window = itr != s_windows.end() ? itr->second : nullptr;
		}								   

		IResult::ptr result;
		switch ( message )
		{
		case WM_CREATE:
			window = (IWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
			s_windows[ hWnd ] = window;
			window->OnCreate( { wParam, lParam } );
			result.reset( new Unhandled() );
			break;
		case WM_DESTROY:
			result.reset( window->OnDestroy( { wParam, lParam } ) );
			s_windows.erase( hWnd );			
			break;
		case WM_INITDIALOG:
			result.reset( window->OnInitDialog( { wParam, lParam } ) );
			break;
		case WM_PAINT:
			result.reset( window->OnPaint( { wParam, lParam } ) );
			break;
		case WM_COMMAND:
		{
			int controlId = (int)LOWORD( wParam );
			int controlMessage = (int)HIWORD( wParam );
			if ( controlId != 0 )
			{
				IControl* control = window->GetControl( controlId );
				if ( control )
				{
					result.reset( window->OnControlCommand( { control, controlMessage } ) );
				}
			}
			break;					  
		struct UserMessage
		{	
			int message;
			Params params;
		};
		}
		default:
			if ( message >= WM_USER )
			{
				result.reset( window->OnUserMessage( UserMessageData{ (int)message - WM_USER, { wParam, lParam } } ) );
			}
			break;
		}

		if ( ! result || result->IsUnhandled() )
		{
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
		else
		{
			return result->GetResult();
		}
	}
}


Window::Window( HWND parent, std::wstring className )
	: m_hInstance{ (HINSTANCE)GetWindowLong( parent, GWL_HINSTANCE ) }
	, m_className{ className }
	, m_parent{ nullptr }
	, m_parentHandle{ parent }
	, m_handle{ 0 }
	, m_rootContainer{ nullptr }
	, m_currentParent{ nullptr }
{	
	WNDCLASS wc{};
	if ( !GetClassInfo( m_hInstance, className.c_str(), &wc ) )
	{
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)Builder_WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
		wc.hbrBackground = (HBRUSH)GetSysColorBrush( COLOR_3DFACE );
		wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );
		wc.lpszMenuName = 0;
		wc.lpszClassName = m_className.c_str();
		if ( ! RegisterClass( &wc ) )
		{
			throw std::exception( "Failed to register window class!" );
		}
	}
}

Window::Window( IWindow* parent, std::wstring className )
	: m_hInstance{ (HINSTANCE)GetWindowLong( parent->GetHandle(), GWL_HINSTANCE ) }
	, m_className{ className }
	, m_parent{ parent }
	, m_parentHandle{ parent->GetHandle() }
	, m_handle{ 0 }
	, m_rootContainer{ nullptr }
	, m_currentParent{ nullptr }
{	
	WNDCLASS wc{};
	if ( !GetClassInfo( m_hInstance, className.c_str(), &wc ) )
	{
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)Builder_WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
		wc.hbrBackground = (HBRUSH)GetSysColorBrush( COLOR_3DFACE );
		wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );
		wc.lpszMenuName = 0;
		wc.lpszClassName = m_className.c_str();
		if ( ! RegisterClass( &wc ) )
		{
			throw std::exception( "Failed to register window class!" );
		}
	}
}

Window::~Window()
{
	m_currentParent = 0; // managed by m_rootContainer
	delete m_rootContainer;
}

void Window::AddContainer( create::container::Container * container )
{
	if ( ! m_rootContainer )
	{
		m_rootContainer = container;
	}
	else
	{
		m_currentParent->AddChild( container );
	
	}
	container->SetParent( m_currentParent );
	m_currentParent = container;
	
}

void Window::AddControl( create::IControl * control, std::string name )
{	
	assert( m_currentParent );
	m_currentParent->AddChild( control );

	int id = m_controls.size() + 1;

	create::IControl::ptr controlPtr( control );
	m_controls[ id ] = controlPtr;
	m_controlsByName[ name ] = controlPtr;
	
	control->SetName( name );
	control->SetID( id );
}

void Window::StepDown( int steps )
{
	while ( steps-- )
	{
		m_currentParent = m_currentParent->GetParent();
		assert( m_currentParent );
	}
}

HWND Window::Create( std::wstring title, int x, int y, int nCmdShow )
{
	assert( m_rootContainer );
	assert( m_handle == 0 );

	if ( create::FillWidth() == m_rootContainer->GetWantedWidth() || create::FillHeight() == m_rootContainer->GetWantedHeight() )
	{
		throw std::exception( "Root container must have a constant size!" );
	}

	m_rootContainer->ComputePass1();
	m_rootContainer->ComputePass2( 0, 0, 0, 0 );

	m_rootContainer->ComputePass3( 0, 0 );

	int paddingWidth = 8 * 2;
	int paddingHeight = 40;// GetSystemMetrics( SM_CYEDGE ) + GetSystemMetrics( SM_CYSIZE ) + GetSystemMetrics( SM_CXPADDEDBORDER ) * 2;

	m_handle = CreateWindowW(
		m_className.c_str(),
		title.c_str(),
		WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		x,
		y,
		m_rootContainer->GetActualWidth() + paddingWidth,
		m_rootContainer->GetActualHeight() + paddingHeight,
		m_parentHandle, 
		0, 
		m_hInstance, 
		this
	);			

	m_rootContainer->Create( m_handle );

	ShowWindow( nCmdShow );

	IResult::ptr result( OnAfterCreate( { 0, 0 } ) );

	return m_handle;
}
					
IWindow* Window::GetParent() const
{
	return m_parent;
}

HWND Window::GetParentHandle() const
{
	return m_parentHandle;
}

HWND Window::GetHandle() const
{
	return m_handle;
}

HINSTANCE Window::GetInstance() const
{
	return (HINSTANCE)GetWindowLong( GetHandle(), GWL_HINSTANCE );
}

IControl* Window::GetControl( int controlId ) const
{
	auto itr = m_controls.find( controlId );
	return itr == m_controls.end() ? nullptr : itr->second.get()->GetControl();
}

IControl* Window::GetControl( std::string name ) const
{					
	auto itr = m_controlsByName.find( name );
	return itr == m_controlsByName.end() ? nullptr : itr->second.get()->GetControl();
}

void Window::GetWindowRect( RECT & rect ) const
{	 
	::GetWindowRect( GetHandle(), &rect );
}

void Window::MoveWindow( int x, int y, bool repaint )
{
	RECT inputBrowserRect{};
	GetWindowRect( inputBrowserRect );
	int width = inputBrowserRect.right - inputBrowserRect.left;
	int height = inputBrowserRect.bottom - inputBrowserRect.top;
	::MoveWindow( GetHandle(), x, y, width, height, repaint ? 1 : 0 );
}

void Window::ShowWindow( int nCmdShow )
{
	::ShowWindow( GetHandle(), nCmdShow );
}

void Window::SetForegroundWindow()
{
	::SetForegroundWindow( GetHandle() );
}

void Window::SetText( std::string text )
{
	SetWindowTextA( GetHandle(), text.c_str() );
}

std::string Window::GetText() const
{
	int length = GetWindowTextLengthA( GetHandle() );
	char * text = new char[ length ];
	GetWindowTextA( GetHandle(), text, length );
	std::string outText( text );
	delete [] text;
	return outText;
}

int Window::SendUserMessage( int message, Params params )
{
	return SendMessageA( GetHandle(), WM_USER + message, params.wParam, params.lParam ); 
}

IResult* Window::OnCreate( Params params )
{
	return new Unhandled();
}

IResult* Window::OnDestroy( Params params )
{
	return new Unhandled();
}

IResult* Window::OnInitDialog( Params params )
{
	return new Unhandled();
}

IResult* Window::OnAfterCreate( Params params )
{
	return new Unhandled();
}

IResult* Window::OnPaint( Params params )
{
	return new Unhandled();
}

IResult* Window::OnControlCommand( ControlMessage message )
{
	return new Unhandled();
}

IResult* Window::OnUserMessage( UserMessageData message )
{
	return new Unhandled();
}
