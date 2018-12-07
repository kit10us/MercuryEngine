// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/container/Create_StackPanel.h>
#include <algorithm>
#include <vector>

using namespace create;
using namespace container;

StackPanel::StackPanel( Stack direction, FillWidth widthWeight, FillHeight heightWeight, int padding )
	: Container( FillWidth::Value(), FillHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}

StackPanel::StackPanel( Stack direction, FillWidth widthWeight, SizeToContentHeight, int padding )
	: Container( FillWidth::Value(), SizeToContentHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillWidthWeight( widthWeight.weight );
}

StackPanel::StackPanel( Stack direction, FillWidth widthWeight, int height, int padding )
	: Container( FillWidth::Value(), height )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillWidthWeight( widthWeight.weight );
}


StackPanel::StackPanel( Stack direction, SizeToContentWidth, FillHeight heightWeight, int padding )
	: Container( SizeToContentWidth::Value(), FillHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillHeightWeight( heightWeight.weight );
}

StackPanel::StackPanel( Stack direction, SizeToContentWidth, SizeToContentHeight, int padding )
	: Container( SizeToContentWidth::Value(), SizeToContentHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, SizeToContentWidth, int height, int padding )
	: Container( SizeToContentWidth::Value(), height )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, int width, FillHeight heightWeight, int padding )
	: Container( width, FillHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
	SetFillHeightWeight( heightWeight.weight );
}

StackPanel::StackPanel( Stack direction, int width, SizeToContentHeight, int padding )
	: Container( width, SizeToContentHeight::Value() )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, int width, int height, int padding )
	: Container( width, height )
	, m_direction{ direction }
	, m_padding{ padding }
{
}

StackPanel::~StackPanel() 
{
	m_children.clear();
}

Stack StackPanel::GetDirection() const
{
	return m_direction;
}

void StackPanel::AddChild( IControl * child )
{
	m_children.push_back( child );
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
						 
	// Calculate children width and height available for fill controls/containers...
	int childrenFillWidth = GetActualWidth();
	int childrenFillHeight = GetActualHeight();
	switch ( GetDirection() )
	{
	case Stack::Vertical:
		childrenFillHeight -= m_totalChildrenHeight;
		break;
	case Stack::Horizontal:
		childrenFillWidth -= m_totalChildrenWidth;
		break;
	}												 

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
