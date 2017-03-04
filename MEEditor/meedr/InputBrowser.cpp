// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/InputBrowser.h>

using namespace meedr;
using namespace ui;

#define USERMESSAGE_UPDATEINPUTDATE	 0

InputBrowser::InputBrowser( HWND parentHandle, int nCmdShow, int x, int y, me::IGame * game )
	: Window( parentHandle, L"InputBrowserWndClass" )
	, m_game{ game }
	, m_closing{ false }
{
	AddContainer( new container::StackPanel( container::Stack::Horizontal, 540, 440, 0 ) );
	AddContainer( new container::StackPanel( container::Stack::Vertical, FillWidth(), FillHeight() ) );
	AddControl( new Static( L"Type:", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( new Combobox( FillWidth(), DefaultHeight() ), "InputSource" );
	AddContainer( new container::StackPanel( container::Stack::Horizontal, FillWidth(), FillHeight() ) );
	AddControl( (new Listbox( 260, FillHeight() ) )->SetSorted( false ), "InputNames" );
	AddContainer( new container::StackPanel( container::Stack::Vertical, FillWidth(), FillHeight() ) );
	AddControl( new Static( L"Data:", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( new Static( L"PLACE FOR DATA", FillWidth(), FillHeight() ), "InputData" );
	Create( L"Input Browser", x, y, nCmdShow );	
}

InputBrowser::~InputBrowser()
{
	m_closing = true;
	using namespace std::chrono_literals;
	std::this_thread::sleep_for( 1s );
	m_updateData.detach();
	//m_updateData.join();
}

void InputBrowser::UpdateInputData()
{
	HWND hWndComboInputSource = GetDlgItem( GetHandle(), GetControl( "InputSource" ) );
	HWND hWndListInputNames = GetDlgItem( GetHandle(), GetControl( "InputNames" ) );
	HWND hWndStaticInputData = GetDlgItem( GetHandle(), GetControl( "InputData" ) );
 
	size_t sourceIndex = SendMessageA( hWndComboInputSource, CB_GETCURSEL, 0, 0 );
	size_t inputIndex = SendMessageA( hWndListInputNames, LB_GETCURSEL, 0, 0 );
	auto source = m_game->GetInputManager()->GetSource( sourceIndex );
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
		dataString += std::string( "\npressed:     " )+ (data->pressed ? "true" : "false");
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

void InputBrowser::UpdateInputManagerList( HWND hWnd )
{
	// Clear contents...
	SendMessageA( hWnd, CB_RESETCONTENT, 0, 0 );

	// Fill in resource types...
	for ( size_t i = 0; i < m_game->GetInputManager()->GetSourceCount(); i++ )
	{			  
		auto source = m_game->GetInputManager()->GetSource( i );
		std::string name = source->Name();
		SendMessageA( hWnd, CB_ADDSTRING, 0, (LPARAM)name.c_str() );
	}

	// Select first type:
	SendMessageA( hWnd, CB_SETCURSEL, 0, 0 );
}

void InputBrowser::UpdateInputSourceInputList( HWND hWndListInputNames )
{
	HWND hWndComboInputSource = GetDlgItem( GetHandle(), GetControl( "InputSource" ) );

	// Clear contents...
	SendMessageA( hWndListInputNames, (UINT)LB_RESETCONTENT, (WPARAM)0, (LPARAM)0 );

	size_t sourceIndex = SendMessageA( hWndComboInputSource, CB_GETCURSEL, 0, 0 );
	auto source = m_game->GetInputManager()->GetSource( sourceIndex );
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

Result* InputBrowser::OnCreate( Params params )
{
	return new Result( 0 );
}

void InputBrowser::Timer_UpdateInputData()
{
	while ( ! m_closing )
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for( 100ms );			
		SendMessageA( GetHandle(), WM_USER + USERMESSAGE_UPDATEINPUTDATE, 0, 0 );
	}
}

Result* InputBrowser::OnAfterCreate( Params params )
{
	UpdateInputManagerList( GetDlgItem( GetHandle(), GetControl( "InputSource" ) ) );
	UpdateInputSourceInputList( GetDlgItem( GetHandle(), GetControl( "InputNames" ) ) );
	m_updateData = std::thread( &InputBrowser::Timer_UpdateInputData, this );
	return new Result( 0 );
}

Result * InputBrowser::OnDestroy( Params params )
{
	SendMessageA( GetParentHandle(), WM_USER + INPUTBROWSER_CLOSED, 0, 0 );
	return new Result( 0 );
}

Result* InputBrowser::OnControlCommand( ControlMessage message )
{
	if ( message.IsFor( "InputSource" ) )
	{
		switch ( message.message )
		{
		case CBN_SELCHANGE:
		{
			HWND hWndListInputNames = GetDlgItem( GetHandle(), GetControl( "InputNames" ) );
			UpdateInputSourceInputList( hWndListInputNames );
			break;
		}
		}
	}

	return new Result( 0 );
}

Result* InputBrowser::OnUserMessage( UserMessageData message )
{
	switch ( message.message )
	{
	case USERMESSAGE_UPDATEINPUTDATE:
	{
		UpdateInputData();
		break;
	}		
	}
	return new Result( 0 );
}

