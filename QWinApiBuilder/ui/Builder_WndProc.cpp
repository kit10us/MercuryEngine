// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Builder_WndProc.h>
#include <map>
#include <cassert>

namespace ui
{
	LRESULT CALLBACK Builder_WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		static std::map< HWND, IWindow* > s_windows;
		IWindow * window = nullptr;
		{
			auto itr = s_windows.find( hWnd );
			window = itr != s_windows.end() ? itr->second : nullptr;
		}			

		if ( message == WM_CREATE )
		{
			window = (IWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
			s_windows[ hWnd ] = window;
			window->OnCreate( { wParam, lParam } );
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
		else if ( ! window )
		{
			return DefWindowProc( hWnd, message, wParam, lParam );
		}

		IResult::ptr result;
		switch ( message )
		{
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
		}
		case WM_NOTIFY:
		{				   
			NMHDR * hdr = (NMHDR*)lParam;
			IControl * control = window->GetControl( hdr->idFrom );
			if ( control )
			{
				result.reset( window->OnNotify( NotifyMessage{ control, hdr->code, lParam } ) );
			}
			break;
		}
		case WM_TIMER:
		{
			result.reset( window->OnTimer( TimerMessage{ wParam } ) );
			break;
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
