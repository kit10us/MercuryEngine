// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <ui/Edit.h>
#include <Windowsx.h>

using namespace ui;

Edit::Edit( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

Edit::~Edit()
{
}

bool Edit::SetReadonly( bool readonly )
{
	return Edit_SetReadOnly( GetHandle(), readonly ? 1 : 0 ) ? true : false;
}
