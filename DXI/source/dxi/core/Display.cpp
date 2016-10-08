// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Display.h>

using namespace dxi;
using namespace core;

Display Display::CreateDialog( WORD dialogResourceID, HWND parent, unify::V2< float > position, DLGPROC dialogProc )
{
	Display display;
	display.m_isDialog = true;
	display.m_handle = {};
	display.m_parentHandle = parent;
	display.m_position = position;
	display.m_size = unify::Size< float >();
	display.m_resourceID = dialogResourceID;
	display.m_resourceName.clear(); // What ever - ignored later.
	display.m_isFullscreen = false; // What ever - ignored later.
	display.m_dialogProc = dialogProc;
	return display;
}

Display Display::CreateWindowedDirectXDisplay( unify::Size< float > size, unify::V2< float > position, HWND handle, HWND parent )
{
	Display display;
	display.m_isDialog = false;
	display.m_handle = handle;
	display.m_parentHandle = parent;
	display.m_position = position;
	display.m_size = size;
	display.m_resourceID = 0; // What ever - ignored later.
	display.m_resourceName.clear(); // What ever - ignored later.
	display.m_isFullscreen = false;
	display.m_dialogProc = 0;
	return display;
}

Display Display::CreateFullscreenDirectXDisplay( unify::Size< float > size, HWND handle )
{
	Display display;
	display.m_isDialog = false;
	display.m_handle = handle;
	display.m_parentHandle = 0;
	display.m_position = unify::V2< float >(); // What ever - ignored later.
	display.m_size = size;
	display.m_resourceID = 0; // What ever - ignored later.
	display.m_resourceName.clear(); // What ever - ignored later.
	display.m_isFullscreen = true;
	display.m_dialogProc = 0;
	return display;
}

Display::Display()
: m_isDialog { false }
, m_handle {}
, m_parentHandle {}
, m_position {}
, m_size {}
, m_resourceID {}
, m_isFullscreen {}
, m_dialogProc {}	
, m_depth{ 0.0f, 1000.0f }
{
}

bool Display::IsDialog() const
{
	return m_isDialog;
}

bool Display::IsFullscreen() const
{
	return m_isFullscreen;
}

unify::Size< float > Display::GetSize() const
{
	return m_size;
}

unify::V2< float > Display::GetPosition() const
{
	return m_position;
}

unify::MinMax< float > Display::GetDepth() const
{
	return m_depth;
}

HWND Display::GetHandle() const
{
	return m_handle;
}

void Display::SetHandle( HWND handle )
{
	m_handle = handle;
}

HWND Display::GetParentHandle() const
{
	return m_parentHandle;
}

const char * Display::GetDialogTemplateName() const
{
	return !m_resourceName.empty() ? m_resourceName.c_str() : MAKEINTRESOURCEA( m_resourceID );
}

DLGPROC Display::GetDialogProc() const
{
	return m_dialogProc;
}