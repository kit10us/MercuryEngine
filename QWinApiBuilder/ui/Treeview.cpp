// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <ui/Treeview.h>
#include <Commctrl.h>

#pragma comment( lib, "Comctl32" )

using namespace ui;

Treeview::Treeview( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

Treeview::~Treeview()
{
}