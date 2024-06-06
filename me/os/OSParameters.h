// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/os/IOSParameters.h>

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
			typedef void* LParam;
			typedef void* WndProc;

			struct OSFood
			{
				OSFood( HWnd _handle, unsigned int _message, WParam _wParam, LParam _lParam )
					: handle{ _handle }
					, message{ _message }
					, wParam{ _wParam }
					, lParam{ _lParam }
				{
				}

				HWnd handle;
				unsigned int message;
				WParam wParam;
				LParam lParam;
			};
		}

		class OSParameters : public IOSParameters
		{
			unify::Path m_programPath;
			unify::Path m_runPath;
			std::vector< std::string > m_arguments;

		public:
			OSParameters( unify::Path programPath, unify::Path runPath, std::vector< std::string > arguments );

			os::win::HWnd handle;
			os::win::HInstance hInstance;
			os::win::HInstance hPrevInstance;
			std::string cmdLine;
			int nCmdShow;
			os::win::WndProc wndProc;
			os::win::HWnd parentHandle;

		public: // me::os::IOSParameters
			unify::Path GetProgramPath() const override;
			unify::Path GetRunPath() const override;
			size_t GetArgumentCount() const override;
			std::string GetArgument( size_t i ) const override;
			const std::vector<std::string> & Arguments() const;
		};
	}
}