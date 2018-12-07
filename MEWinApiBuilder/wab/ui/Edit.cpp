// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/ui/Edit.h>
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
