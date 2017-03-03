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

	if ( FillWidth() == m_rootContainer->GetWantedWidth() || FillHeight() == m_rootContainer->GetWantedHeight() )
	{
		throw std::exception( "Root container must have a constant size!" );
	}

	m_rootContainer->ComputePass1();
	m_rootContainer->ComputePass2( 0, 0, 0, 0 );

	m_rootContainer->ComputePass3( 0, 0 );

	int paddingWidth = 8 * 2;
	int paddingHeight = 40;// GetSystemMetrics( SM_CYEDGE ) + GetSystemMetrics( SM_CYSIZE ) + GetSystemMetrics( SM_CXPADDEDBORDER ) * 2;

	m_handle = CreateWindowW(
		className.c_str(),
		title.c_str(),
		WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		x,
		y,
		m_rootContainer->GetActualWidth() + paddingWidth,
		m_rootContainer->GetActualHeight() + paddingHeight,
		parent, 
		0, 
		hInstance, 
		lparam 
	);			

	m_rootContainer->Create( m_handle );

	return m_handle;
}

HWND Builder::GetHandle() const
{
	return m_handle;
}