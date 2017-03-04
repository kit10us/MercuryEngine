// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/ScriptEditor.h>
#include <meedr/ui/Window.h>

#include <Richedit.h>
#include <ShObjIdl.h>
#include <vector>


using namespace meedr;
using namespace ui;

ScriptEditor::ScriptEditor( HWND parent, int nCmdShow, int x, int y, me::IGame * game )
	: Window( parent, L"ScriptEditorWndClass" )
	, m_game{ game }
{
	AddContainer( new container::StackPanel( container::Stack::Vertical, 640, 480 ) );
	AddContainer( new container::StackPanel( container::Stack::Horizontal, SizeToContentWidth(), SizeToContentHeight() ) );
	AddControl( new Button( L"&Save", SizeToContentWidth(), DefaultHeight() ), "Save" );
	AddControl( new Button( L"&Load", SizeToContentWidth(), DefaultHeight() ), "Load" );
	StepDown();
	AddControl( new Richtext( FillWidth(), FillHeight() ), "Text" );
	Window::Create( L"Input Browser", x, y, nCmdShow );
}

IResult* ScriptEditor::OnCreate( Params params )
{
	return new Result( 0 );
}

IResult* ScriptEditor::OnDestroy( Params params )
{
	SendMessageA( GetParentHandle(), WM_USER + INPUTBROWSER_CLOSED, 0, 0 ); 
	return new Result(0);
}

IResult* ScriptEditor::OnControlCommand( ControlMessage message )
{
	if ( message.IsFor( "Save" ) )
	{
		return new Result( 0 );
	}
	else if ( message.IsFor( "Load" ) )
	{
		HWND textHandle = GetDlgItem( GetHandle(), GetControl( "Text" ) );

		std::vector< std::string > lines;

		char filename[ 256 ] = { 0 };
		OPENFILENAMEA ofn{};
		ofn.lStructSize = sizeof( OPENFILENAMEA );
		ofn.hwndOwner = GetHandle();
		//ofn.hInstance = GetInstance();
		ofn.lpstrFilter = "LUA Scripts (*.lua)\0*.lua\0All Files(*.*)\0*.*\0\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = 256;
		ofn.lpstrFileTitle = "Open Script...";
		ofn.Flags = OFN_FILEMUSTEXIST;
		if ( GetOpenFileNameA( &ofn ) )
		{
			FILE* fp = 0;
			fopen_s( &fp, filename, "r" );
			char str[ 256 ];
			std::string source = "";
			while ( fgets( str, 256, fp ) )
			{
				source += str;
				lines.push_back( str );
			}
			SendMessageA( textHandle, WM_SETTEXT, 0, (LPARAM)(char*)source.c_str() );
		}
		else
		{
		}

		// Formating...			
		std::map< std::string, CHARFORMAT > formats;
		formats[ "keyword" ] = {};
		formats[ "keyword" ].cbSize = sizeof( CHARFORMAT );
		formats[ "keyword" ].crTextColor = 0xFF0000;
		formats[ "keyword" ].dwMask = CFM_COLOR;

		std::vector< std::string > spaceStack;

		size_t first = 0;
		size_t last = lines[ 0 ].length();
		for ( size_t i = 0; i < lines.size(); i++ )
		{
			// Setup for current line...
			std::string line = lines[ i ];
			last = first + line.length();

			// Format...
			CHARRANGE charRange{ (long)first, (long)last };
			SendMessageA( textHandle, EM_EXSETSEL, 0, (LPARAM)&(charRange) );

			CHARFORMATA format{};
			format.cbSize = sizeof( CHARFORMAT );
			switch ( i % 3 )
			{
			case 0:
				format.crTextColor = 0xFF0000;
				break;
			case 1:
				format.crTextColor = 0x00FF00;
				break;
			case 2:
				format.crTextColor = 0x0000FF;
				break;
			default:
				format.crTextColor = 0xFFFFFF;
			}

			format.dwMask = CFM_COLOR;
			SendMessageA( textHandle, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&format );	  
				
			// Setup for next line...
			first += line.length();
		}	 

		/*		   
		CHARRANGE charRange{ 0, 10 };
		SendMessageA( textHandle, EM_EXSETSEL, 0, (LPARAM)&(charRange) );

		CHARFORMATA format{};
		format.cbSize = sizeof( CHARFORMAT );
		format.crTextColor = 0xFF0000;
		format.dwMask = CFM_COLOR;
		SendMessageA( textHandle, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&format );
		*/
		return new Result( 0 );
	}
	return new Unhandled();
}
