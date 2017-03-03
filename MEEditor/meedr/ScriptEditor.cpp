// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/ScriptEditor.h>
#include <meedr/ui/Builder.h>

static HWND s_parentWindow;
static HWND s_windowHwnd;

#define IDC_COMBOBOX_INPUTSOURCE	0x01
#define IDC_LISTBOX_INPUTNAMES		0x02
#define IDC_STATIC_INPUTDATA		0x03

#define ID_TIMER_UPDATEINPUTDATA	1

static me::IGame * s_game;

namespace
{
	void UpdateInputData()
	{
		HWND hWndComboInputSource = GetDlgItem( s_windowHwnd, IDC_COMBOBOX_INPUTSOURCE );
		HWND hWndListInputNames = GetDlgItem( s_windowHwnd, IDC_LISTBOX_INPUTNAMES );
		HWND hWndStaticInputData = GetDlgItem( s_windowHwnd, IDC_STATIC_INPUTDATA );

		size_t sourceIndex = SendMessageA( hWndComboInputSource, CB_GETCURSEL, 0, 0 );
		size_t inputIndex = SendMessageA( hWndListInputNames, LB_GETCURSEL, 0, 0 );
		auto source = s_game->GetInputManager()->GetSource( sourceIndex );
		size_t subSource = 0;
		me::input::IData::ptr dataRaw = source->GetInputData( subSource, inputIndex );
		me::input::InputType type = dataRaw ? dataRaw->type : me::input::InputType::Invalid;

		std::string dataString( "" );
		switch ( type )
		{
		default:
			dataString += "<unknown>";
			break;
		case me::input::InputType::Button:
		{
			me::input::ButtonData * data( reinterpret_cast<me::input::ButtonData*>(dataRaw.get()) );
			dataString += std::string( "down:        " ) + (data->down ? "true" : "false");
			dataString += std::string( "\npressed:     " ) + (data->pressed ? "true" : "false");
			break;
		}
		case me::input::InputType::Toggle:
		{
			me::input::ToggleData * data( reinterpret_cast<me::input::ToggleData*>(dataRaw.get()) );
			dataString += std::string( "on:          " ) + (data->on ? "true" : "false");
			break;
		}
		case me::input::InputType::Tracker:
		{
			me::input::TrackerData * data( reinterpret_cast<me::input::TrackerData*>(dataRaw.get()) );
			dataString += std::string( "position: " ) + data->position.ToString();
			dataString += std::string( "\nchange:   " ) + data->change.ToString();
			break;
		}
		case me::input::InputType::Trigger:
		{
			me::input::TriggerData * data( reinterpret_cast<me::input::TriggerData*>(dataRaw.get()) );
			dataString += std::string( "value:    " ) + unify::Cast< std::string >( data->value );
			break;
		}
		case me::input::InputType::Stick:
		{
			me::input::StickData * data( reinterpret_cast<me::input::StickData*>(dataRaw.get()) );
			dataString += std::string( "axis:     " ) + data->axis.ToString();
			break;
		}
		}

		SendMessageA( hWndStaticInputData, WM_SETTEXT, 0, (LPARAM)dataString.c_str() );
	}

	void UpdateInputManagerList( me::IGame * game, HWND hWnd )
	{
		// Clear contents...
		SendMessageA( hWnd, CB_RESETCONTENT, 0, 0 );

		// Fill in resource types...
		for ( size_t i = 0; i < game->GetInputManager()->GetSourceCount(); i++ )
		{
			auto source = game->GetInputManager()->GetSource( i );
			std::string name = source->Name();
			SendMessageA( hWnd, CB_ADDSTRING, 0, (LPARAM)name.c_str() );
		}

		// Select first type:
		SendMessageA( hWnd, CB_SETCURSEL, 0, 0 );
	}

	void UpdateInputSourceInputList( me::IGame * game, HWND hWndListInputNames )
	{
		HWND hWndComboInputSource = GetDlgItem( s_windowHwnd, IDC_COMBOBOX_INPUTSOURCE );

		// Clear contents...
		SendMessageA( hWndListInputNames, (UINT)LB_RESETCONTENT, (WPARAM)0, (LPARAM)0 );

		size_t sourceIndex = SendMessageA( hWndComboInputSource, CB_GETCURSEL, 0, 0 );
		auto source = game->GetInputManager()->GetSource( sourceIndex );
		size_t subSource = 0;

		for ( size_t i = 0; i < source->InputCount( subSource ); i++ )
		{
			std::string name = source->InputName( subSource, i ) + " - " + me::input::InputTypeToString( source->GetInputType( subSource, i ) );
			SendMessageA( hWndListInputNames, LB_ADDSTRING, 0, (LPARAM)name.c_str() );
		}

		// Select first type:
		SendMessageA( hWndListInputNames, LB_SETCURSEL, 0, 0 );

		UpdateInputData();
	}

	void CALLBACK TimerProc_UpdateInputData( HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
	{
		UpdateInputData();
		SetTimer( hWnd, ID_TIMER_UPDATEINPUTDATA, 100, TimerProc_UpdateInputData );
	}

	LRESULT CALLBACK ScriptEditor_WndProcChild( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		static me::IGame * game;
		switch ( message )
		{
		case WM_CREATE:
		{
			CREATESTRUCT * createStruct = (CREATESTRUCT*)lParam;
			game = (me::IGame*)createStruct->lpCreateParams;
			SetTimer( hWnd, ID_TIMER_UPDATEINPUTDATA, 100, TimerProc_UpdateInputData );
			break;
		}
		case WM_DESTROY:
		{
			SendMessageA( s_parentWindow, WM_INPUTBROWSER_CLOSED, 0, 0 );
			break;
		}
		case WM_COMMAND:
		{
			auto control = LOWORD( wParam );
			auto controlMessage = HIWORD( wParam );
			switch ( control )
			{
			case IDC_COMBOBOX_INPUTSOURCE:
			{
				switch ( controlMessage )
				{
				case CBN_SELCHANGE:
				{
					HWND hWndListInputNames = GetDlgItem( hWnd, IDC_LISTBOX_INPUTNAMES );
					UpdateInputSourceInputList( game, hWndListInputNames );
					break;
				}
				}
				break;
			}
			}
			break;
		}
		}

		return DefWindowProc( hWnd, message, wParam, lParam );
	}
}

HWND meedr::CreateScriptEditor( me::IGame * game, HINSTANCE hInstance, HWND parentHandle, int nCmdShow, int x, int y )
{
	s_parentWindow = parentHandle;
	s_game = game;

	// Regardless of windowed or not, we need a window...
	WNDCLASS wc{};

	if ( !GetClassInfo( hInstance, L"ScriptEditorWndClass", &wc ) )
	{
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)ScriptEditor_WndProcChild;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
		wc.hbrBackground = (HBRUSH)GetSysColorBrush( COLOR_3DFACE );
		wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"ScriptEditorWndClass";
		if ( !RegisterClass( &wc ) )
		{
			throw std::exception( "Failed to register window class!" );
		}
	}

	using namespace ui;

	Builder builder;
	builder.AddContainer( new container::StackPanel( container::Stack::Horizontal, 540, 440 ) );
	builder.AddContainer( new container::StackPanel( container::Stack::Vertical, FillWidth(), FillHeight() ) );
	builder.AddControl( new Static( L"Type:", SizeToContentWidth(), DefaultHeight() ) );
	builder.AddControl( new Combobox( FillWidth(), DefaultHeight(), IDC_COMBOBOX_INPUTSOURCE ) );
	builder.AddContainer( new container::StackPanel( container::Stack::Horizontal, FillWidth(), FillHeight() ) );
	builder.AddControl( (new Listbox( 260, FillHeight(), IDC_LISTBOX_INPUTNAMES ))->SetSorted( false ) );
	builder.AddContainer( new container::StackPanel( container::Stack::Vertical, FillWidth(), FillHeight() ) );
	builder.AddControl( new Static( L"Data:", SizeToContentWidth(), DefaultHeight() ) );
	builder.AddControl( new Static( L"PLACE FOR DATA", FillWidth(), FillHeight(), IDC_STATIC_INPUTDATA ) );
	s_windowHwnd = builder.Create( s_parentWindow, hInstance, L"ScriptEditorWndClass", L"Input Browser", x, y, game );
	
	ShowWindow( s_windowHwnd, nCmdShow );

	UpdateInputManagerList( game, GetDlgItem( s_windowHwnd, IDC_COMBOBOX_INPUTSOURCE ) );
	UpdateInputSourceInputList( game, GetDlgItem( s_windowHwnd, IDC_LISTBOX_INPUTNAMES ) );

	return s_windowHwnd;
}

