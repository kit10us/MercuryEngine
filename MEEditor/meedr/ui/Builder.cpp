// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Builder.h>
#include <cassert>

using namespace meedr;
using namespace ui;

Builder::Builder()
	: m_handle{ 0 }
	, m_rootContainer{ nullptr }
	, m_currentParent{ nullptr }
{
}

Builder::~Builder()
{
	m_currentParent = 0; // managed by m_rootContainer
	delete m_rootContainer;
}

void Builder::AddContainer( container::Container * container )
{
	if ( ! m_rootContainer )
	{
		m_rootContainer = container;
	}
	else
	{
		m_currentParent->AddChild( container );
	}
	m_currentParent = container;
}

void Builder::AddControl( IControl * control )
{	
	assert( m_currentParent );
	m_currentParent->AddChild( control );
}

void Builder::StepDown()
{
	m_currentParent = m_currentParent->GetParent();
	assert( m_currentParent );
}

HWND Builder::Create( HWND parent, HINSTANCE hInstance, std::wstring className, std::wstring title, int x, int y, void * lparam )
{
	assert( m_rootContainer );
	assert( parent );
	assert( m_handle == 0 );

	int width = m_rootContainer->GetWidth();
	int height = m_rootContainer->GetHeight();

	m_handle = CreateWindowW(
		className.c_str(),
		title.c_str(),
		WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		x,
		y,
		width,
		height,
		parent, 
		0, 
		hInstance, 
		lparam 
	);			

	m_rootContainer->Create( 0, 0, width - 16, height - 36, m_handle );

	return m_handle;
}

HWND Builder::GetHandle() const
{
	return m_handle;
}