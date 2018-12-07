// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/Builder_WndProc.h>
#include <wab/ui/Menu.h>
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

		if ( message == WM_DESTROY )
		{
			IResult::ptr result( window->OnDestroy( { wParam, lParam } ) );
			s_windows.erase( hWnd );
			return DefWindowProc( hWnd, message, wParam, lParam );
		}

		IResult::ptr result;
		switch ( message )
		{
		case WM_CLOSE:
			result.reset( window->OnClose( { wParam, lParam } ) );
			break;
		case WM_INITDIALOG:
			result.reset( window->OnInitDialog( { wParam, lParam } ) );
			break;
		case WM_PAINT:
			result.reset( window->OnPaint( { wParam, lParam } ) );
			break;
		case WM_COMMAND:
		{
			// Check if the message is for menu items...
			if ( ! lParam )
			{
				int menuItemId = LOWORD( wParam );
				MenuItem* item = window->GetMenuItem( menuItemId );
				result.reset( window->OnMenuCommand( { item } ) );
			}
			else
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
			}
			break;		
		}
		case WM_NOTIFY:
		{				   
			NMHDR * hdr = (NMHDR*)lParam;
			IControl * control = window->GetControl( hdr->idFrom );
			if ( control )
			{
				result.reset( window->OnNotify( { control, hdr->code, lParam } ) );
			}
			break;
		}
		case WM_MENUSELECT:
		{
			WORD idOrIndex = LOWORD( wParam );
			WORD flags = HIWORD( wParam );
			HMENU menuHandle = (HMENU)lParam;
			Menu* menu = window->GetMenu( menuHandle );
			if ( menu )
			{
				MenuItem* item = window->GetMenuItem( idOrIndex );
				result.reset( window->OnMenuSelect( message::MenuSelect{ item, menu, flags } ) );
			}
			break;
		}
		case WM_TIMER:
		{
			result.reset( window->OnTimer( { wParam } ) );
			break;
		}
		default:
			if ( message >= WM_USER )
			{
				result.reset( window->OnUser( { (int)message - WM_USER, { wParam, lParam } } ) );
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
