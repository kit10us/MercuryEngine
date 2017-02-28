// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/container/Canvas.h>
#include <cassert>

using namespace meedr;
using namespace ui;
using namespace container;

Canvas::Canvas( int width, int height )
	: Container( 0 )
	, m_width{ width }
	, m_height{ height }
{											  
}

Canvas::~Canvas() 
{
	for ( auto child : m_children )
	{
		delete child;
	}
	m_children.clear();
}

int Canvas::GetWidth()
{
	return m_width;
}

int Canvas::GetHeight()
{
	return m_height;
}

void Canvas::AddChild( IControl * child )
{
	m_children.push_back( child );
	child->SetParent( this );
}

void Canvas::Create( int x, int y, int parentWidth, int parentHeight, HWND parent )
{		  
	assert( 0 ); // TODO:
}
