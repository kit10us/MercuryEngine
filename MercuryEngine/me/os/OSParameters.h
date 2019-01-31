// Copyright (c) 2003 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

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

		struct OSParameters
		{
			OSParameters()
				: hWnd{}
				, hInstance{}
				, hPrevInstance{}
				, cmdLine{}
				, wndProc{}
			{
			}

			os::win::HWnd hWnd;
			os::win::HInstance hInstance;
			os::win::HInstance hPrevInstance;
			std::string cmdLine;
			int nCmdShow;
			os::win::WndProc wndProc;
		};
	}
}