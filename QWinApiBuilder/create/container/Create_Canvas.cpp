// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <create/container/Create_Canvas.h>
#include <cassert>

using namespace create;
using namespace container;

Canvas::Canvas( int width, int height )
	: Container( width, height )
{											  
}

Canvas::~Canvas() 
{
	m_children.clear();
}

void Canvas::AddChild( IControl * child )
{
	m_children.push_back( child );
}

void Canvas::ComputePass1()
{
	assert( 0 ); // TODO
}

void Canvas::ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight )
{
	assert( 0 ); // TODO
}

void Canvas::ComputePass3( int x, int y )
{
	assert( 0 ); // TODO
}

void Canvas::Create( HWND parent )
{
	assert( 0 ); // TODO
}