// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Control.h>

using namespace meedr;
using namespace ui;

Control::Control( int id, int width, int height )
	: m_parent{ nullptr }
	, m_id{ id }
	, m_fillWidthWeight{ 0 }
	, m_fillHeightWeight{ 0 }
	, m_wantedWidth{ width }
	, m_wantedHeight{ height }
	, m_actualX{ 0 }
	, m_actualY{ 0 }
	, m_actualWidth{ 0 }
	, m_actualHeight{ 0 }
{
}

Control::~Control()
{
}

int Control::GetDefaultWidth() const
{
	return -1;
}

int Control::GetDefaultHeight() const
{
	return -1;
}

void Control::SetFillWidthWeight( int weight )
{
	m_fillWidthWeight = weight;
}

int Control::GetFillWidthWeight() const
{
	return m_fillWidthWeight;
}

void Control::SetFillHeightWeight( int weight )
{
	m_fillHeightWeight = weight;
}

int Control::GetFillHeightWeight() const
{
	return m_fillHeightWeight;
}

int Control::GetWantedWidth() const
{
	return m_wantedWidth;
}

int Control::GetWantedHeight() const
{
	return m_wantedHeight;
}

int Control::GetID() const
{
	return m_id;
}

std::string Control::GetName() const
{
	return m_name;
}

int Control::GetActualX() const
{
	return m_actualX;
}

int Control::GetActualY() const
{
	return m_actualY;
}

int Control::GetActualWidth() const
{
	return m_actualWidth;
}

int Control::GetActualHeight() const
{
	return m_actualHeight;
}

void Control::SetParent( container::Container * parent )
{
	m_parent = parent;
}

container::Container * Control::GetParent()	 const
{
	return m_parent;
}

void Control::ComputePass1()
{
	if ( DefaultWidth() == GetWantedWidth() )
	{	
		m_actualWidth = GetDefaultWidth();
	}
	else if ( GetWantedWidth() > 0 )
	{
		m_actualWidth = GetWantedWidth();
	}

	if ( DefaultHeight() == GetWantedHeight() )
	{	
		m_actualHeight = GetDefaultHeight();
	}
	else if ( GetWantedHeight() > 0 )
	{
		m_actualHeight = GetWantedHeight();
	}
}

void Control::ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight )
{
	if ( FillWidth() == GetWantedWidth() )
	{
		m_actualWidth = fillWidthTotal / fillWidthTotalWeight * GetFillWidthWeight();
	}

	if ( FillHeight() == GetWantedHeight() )
	{
		m_actualHeight = fillHeightTotal / fillHeightTotalWeight * GetFillHeightWeight();
	}  
}

void Control::ComputePass3( int x, int y )
{
	m_actualX = x;
	m_actualY = y;
}
