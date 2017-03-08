// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class Combobox : public Control
	{
	public:		   			
		Combobox( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Combobox();

	public: // WinApi functions...
		void ResetContent();
		void AddString( std::string text );
		void SetCurSel( int sel );
		int GetCurSel() const;
	};
}