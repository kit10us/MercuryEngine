// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IRenderer.h>
#include <me/Display.h>
#include <unify/unify.h>
#include <unify/Size.h>
#include <vector>
#include <string>
#include <memory>

// TODO: Move this to the places that offend, instead of here, so we can be more portabile (or less ugly, really)
// Because Windows likes to define...
#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace me
{
	namespace os
	{
		namespace win
		{
			typedef void* HWnd;
			typedef void* HInstance;
			typedef long * LResult;
			typedef unsigned int * WParam;
			typedef long LParam;
			typedef void* WndProc;

			struct OSFood
			{
				OSFood( HWnd _handle, unsigned int _message, WParam _wParam, LParam _lParam )
					: handle { _handle }
					, message { _message }
					, wParam { _wParam }
					, lParam { _lParam }
				{
				}

				HWnd handle;
				unsigned int message;
				WParam wParam;
				LParam lParam;
			};
		}
	}

	struct OSParameters
	{
		//typedef LRESULT ( __stdcall* WndProc )(HWnd, unsigned int, WParam, LParam);

		OSParameters()
			: hWnd{}
			, hInstance{}
			, hPrevInstance{}
			, lpszCmdLine{}
			, wndProc{}
		{
		}

		os::win::HWnd hWnd;
		os::win::HInstance hInstance;
		os::win::HInstance hPrevInstance;
		char* lpszCmdLine;
		int nCmdShow;
		os::win::WndProc wndProc;
	};


	/// <summary>
	/// Interface for Operating System specific data. This class is for abstraction from OS.
	/// </summary>
	class IOS
	{
	public:
		typedef std::shared_ptr< IOS > ptr;

		virtual ~IOS() { }

		virtual void * Feed( std::string target, void * data ) = 0;

		/// <summary>
		/// Get name of our program.
		/// </summary>
		virtual std::string GetName() const = 0;

		/// <summary>
		/// Get a vector of command line arguments, where 0 is the first argument.
		/// </summary>
		virtual const std::vector< std::string > & GetCommandLine() const = 0;

		/// <summary>
		/// Add a display.
		/// </summary>	  
		virtual void AddDisplay( Display display ) = 0;	

		virtual int RendererCount() const = 0;

		/// <summary>
		/// Get the system Renderer interface.
		/// </summary>
		virtual IRenderer * GetRenderer( int index ) const = 0;

		/// <summary>
		/// Set if we have focus. This is dependant on the OS, focus means the user's input
		/// is directed to us.
		/// </summary>
		virtual void SetHasFocus( bool hasFocus ) = 0;

		/// <summary>
		/// Get if we have focus. This is dependant on the OS, focus means the user's input
		/// is directed to us.
		/// </summary>
		virtual bool GetHasFocus() const = 0;
						  
		/// <summary>
		/// Build renderers.
		/// </summary>
		virtual void BuildRenderers() = 0;

		/// <summary>
		/// Startup and initalize Operating System specific dependencies.
		/// </summary>
		virtual void Startup() = 0;

		/// <summary>
		/// Cleanup up and unitialize Operating System specific dependencies.
		/// </summary>
		virtual void Shutdown() = 0;

		/// <summary>
		/// Write debug text, no new line.
		/// </summary>
		virtual void DebugWrite( const std::string & text ) = 0;
			
		/// <summary>
		/// Write a debug message.
		/// </summary>
		virtual void DebugWriteLine( const std::string & line ) = 0;
	};
}