// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Control.h>
#include <meedr/create/Create_IControl.h>

using namespace meedr;
using namespace ui;

Control::Control( HWND parent, HWND handle, create::IControl * createControl )
	: m_parentHandle{ parent }
	, m_handle{ handle }
	, m_createControl{ createControl }
{
}

Control::~Control()
{
}

HWND Control::GetParentHandle() const
{
	return m_parentHandle;
}

HWND Control::GetHandle() const
{
	return m_handle;
}

std::string Control::GetName() const
{
	return m_createControl->GetName();
}

int Control::GetID() const
{
	return m_createControl->GetID();
}

void Control::SetText( std::string text )
{
	SendMessageA( GetHandle(), WM_SETTEXT, 0, (LPARAM)(char*)text.c_str() );
}

std::string Control::GetText() const
{
	int length = GetWindowTextLengthA( GetHandle() );
	if ( length == 0 ) return "";

	char * text = new char[ length + 1 ];
	GetWindowTextA( GetHandle(), text, length + 1 );
	std::string outText( text );
	delete [] text;
	return outText;
}

bool Control::GetEnabled() const
{
	return	IsWindowEnabled( GetHandle() ) ? true : false;
}

void Control::SetEnable( bool enable )
{
	EnableWindow( GetHandle(), enable ? 1 : 0 );
}