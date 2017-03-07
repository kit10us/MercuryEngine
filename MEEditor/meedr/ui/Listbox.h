// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Listbox : public Control
		{
		public:		   			
			Listbox( HWND parent, HWND handle, create::IControl * createControl );

			virtual ~Listbox();

		public: // WinApi functions...	
			int AddFile( std::string filename );
			int AddString( std::string text );
			int DeleteString( int index );
			int GetCurSel();

			int ResetContent();
			int SetCurSel( int sel );
		};
	}
}