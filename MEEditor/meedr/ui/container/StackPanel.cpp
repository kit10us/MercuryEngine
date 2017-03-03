// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/container/StackPanel.h>
#include <algorithm>
#include <vector>

using namespace meedr;
using namespace ui;
using namespace container;

StackPanel::StackPanel( Stack direction, FillWidth widthWeight, FillHeight heightWeight, int padding )
	: Container( 0, FillWidth::Value(), FillHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}

StackPanel::StackPanel( Stack direction, FillWidth widthWeight, SizeToContentHeight, int padding )
	: Container( 0, FillWidth::Value(), SizeToContentHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillWidthWeight( widthWeight.weight );
}

StackPanel::StackPanel( Stack direction, FillWidth widthWeight, int height, int padding )
	: Container( 0, FillWidth::Value(), height )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillWidthWeight( widthWeight.weight );
}


StackPanel::StackPanel( Stack direction, SizeToContentWidth, FillHeight heightWeight, int padding )
	: Container( 0, SizeToContentWidth::Value(), FillHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillHeightWeight( heightWeight.weight );
}

StackPanel::StackPanel( Stack direction, SizeToContentWidth, SizeToContentHeight, int padding )
	: Container( 0, SizeToContentWidth::Value(), SizeToContentHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, SizeToContentWidth, int height, int padding )
	: Container( 0, SizeToContentWidth::Value(), height )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, int width, FillHeight heightWeight, int padding )
	: Container( 0, width, FillHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillHeightWeight( heightWeight.weight );
}

StackPanel::StackPanel( Stack direction, int width, SizeToContentHeight, int padding )
	: Container( 0, width, SizeToContentHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, int width, int height, int padding )
	: Container( 0, width, height )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::~StackPanel() 
{
	for ( auto child : m_children )
	{
		delete child;
	}
	m_children.clear();
}

Stack StackPanel::GetDirection() const
{
	return m_direction;
}

void StackPanel::AddChild( IControl * child )
{
	m_children.push_back( child );
	child->SetParent( this );
}

int StackPanel::GetPadding() const
{
	return m_padding;
}

void StackPanel::SetPadding( int padding )
{
	m_padding = padding;
}

void StackPanel::ComputePass1()
{
	// Gather my values...
	Control::ComputePass1();

	// Compute children values...
	m_totalChildrenWidth = GetPadding();
	m_totalChildrenHeight = GetPadding();
	m_totalChildrenFillWidthWeight = 0;
	m_totalChildrenFillHeightWeight = 0;
	for ( auto && child : m_children )
	{
		child->ComputePass1();
		int childWidth = child->GetActualWidth();
		int childHeight = child->GetActualHeight();

		switch ( GetDirection() )
		{
		case Stack::Vertical:
			m_totalChildrenWidth = max( childWidth + GetPadding() * 2, m_totalChildrenWidth );
			m_totalChildrenHeight += childHeight + GetPadding();
			break;
		case Stack::Horizontal:
			m_totalChildrenWidth += childWidth + GetPadding();
			m_totalChildrenHeight = max( childHeight + GetPadding() * 2, m_totalChildrenHeight );
			break;
		}

		m_totalChildrenFillWidthWeight += child->GetFillWidthWeight();
		m_totalChildrenFillHeightWeight += child->GetFillHeightWeight();
	}

	if ( SizeToContentWidth() == GetWantedWidth() )
	{
		if ( m_totalChildrenFillWidthWeight )
		{
			throw std::exception( "Vertical StackPanel set to size to content width, however a child is set to fill width!" );
		}
		m_actualWidth = m_totalChildrenWidth;
	}

	if ( SizeToContentHeight() == GetWantedHeight() )
	{
		if ( m_totalChildrenFillHeightWeight )
		{
			throw std::exception( "Horizontal StackPanel set to size to content height, however a child is set to fill height!" );
		}
		m_actualHeight = m_totalChildrenHeight;
	}
}

void StackPanel::ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight )
{			  
	// After ComputePass2, I'm guaranteed to have actual sizes.
	Control::ComputePass2( fillWidthTotal, fillHeightTotal, fillWidthTotalWeight, fillHeightTotalWeight );

	int childrenFillWidth = GetActualWidth() - m_totalChildrenWidth;
	int childrenFillHeight = GetActualHeight() - m_totalChildrenHeight;
	for ( auto && child : m_children )
	{
		switch ( GetDirection() )
		{
		case Stack::Vertical:
			child->ComputePass2( childrenFillWidth, childrenFillHeight, child->GetFillWidthWeight(), m_totalChildrenFillHeightWeight );
			break;
		case Stack::Horizontal:
			child->ComputePass2( childrenFillWidth, childrenFillHeight, m_totalChildrenFillWidthWeight, child->GetFillHeightWeight() );
			break;
		}
	}
}

void StackPanel::ComputePass3( int x, int y )
{
	Control::ComputePass3( x, y );

	// Compute children values...
	x += GetPadding();
	y += GetPadding();
	for ( auto && child : m_children )
	{
		child->ComputePass3( x, y );
		switch ( GetDirection() )
		{
		case Stack::Vertical:
			y += child->GetActualHeight() + GetPadding();
			break;

		case Stack::Horizontal:
			x += child->GetActualWidth() + GetPadding();
			break;
		}
	}	
}

void StackPanel::Create( HWND parent )
{
	for( auto && child : m_children )
	{
		child->Create( parent );
	}
}
