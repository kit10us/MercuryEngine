// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/container/StackPanel.h>
#include <vector>

using namespace meedr;
using namespace ui;
using namespace container;

StackPanel::StackPanel( Stack direction, int width, int height, int padding )
	: Container( 0 )
	, m_direction{ direction }
	, m_width{ width }
	, m_height{ height }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, FillWidth, FillHeight, int padding )
	: Container( 0 )
	, m_direction{ direction }
	, m_width{ -2 }
	, m_height{ -2 }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, FillWidth, int height, int padding )
	: Container( 0 )
	, m_direction{ direction }
	, m_width{ -2 }
	, m_height{ height }
	, m_padding{ padding }
{
}

StackPanel::StackPanel( Stack direction, int width, FillHeight, int padding )
	: Container( 0 )
	, m_direction{ direction }
	, m_width{ width }
	, m_height{ -2 }
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

Stack StackPanel::GetDirection()
{
	return m_direction;
}

int StackPanel::GetWidth()
{
	return m_width;
}

int StackPanel::GetHeight()
{
	return m_height;
}

void StackPanel::AddChild( IControl * child )
{
	m_children.push_back( child );
	child->SetParent( this );
}

void StackPanel::Create( int x, int y, int parentWidth, int parentHeight, HWND parent )
{
	int padding = m_padding;
	x += padding;
	y += padding;		

	// First pass, determine absolute sizes, and thus the left over parent size:
	std::vector< POINT > sizes( m_children.size() );
	
	int fillWidthTotalWeight = 0;
	int fillWidthCount = 0;
	int fillWidth = parentWidth;

	int fillHeightTotalWeight = 0;
	int fillHeightCount = 0;
	int fillHeight = parentHeight;

	auto itr = m_children.begin();
	for( int index = 0; index != m_children.size(); index++, itr++ )
	{
		auto && child = (*itr);

		// Width...
		if ( ui::DefaultWidth() == child->GetWidth() )
		{
			sizes[ index ].x = child->GetDefaultWidth();
			fillWidth -= sizes[ index ].x;
		}
		else if ( ui::FillWidth() == child->GetWidth() )
		{
			// size.x deterime elsewhere.
			fillWidthCount++;
			fillWidthTotalWeight += child->GetFillWidthWeight();
		}
		else
		{
			sizes[ index ].x = child->GetWidth();
			fillWidth -= sizes[ index ].x + padding;
		}

		// Height...
		if ( ui::DefaultHeight() == child->GetHeight() )
		{
			sizes[ index ].y = child->GetDefaultHeight();
			fillHeight -= sizes[ index ].y;
		}
		else if ( ui::FillHeight() == child->GetHeight() )
		{
			// size.y deterime elsewhere.
			fillHeightCount++;
			fillHeightTotalWeight += child->GetFillHeightWeight();
		}
		else
		{
			sizes[ index ].y = child->GetHeight();
			fillHeight -= sizes[ index ].y + padding;
		}

		switch ( m_direction )
		{
		case Stack::Vertical:
			fillHeight -= padding;
			break;
		case Stack::Horizontal:
			fillWidth -= padding;
			break;
		}
	}

	// Create controls and solve fill sizes (since we have the amounts now)...
	itr = m_children.begin();
	for( int index = 0; index != m_children.size(); index++, itr++ )
	{
		auto && child = (*itr);
		int width = 0;
		int height = 0;

		// Width...
		if ( ui::DefaultWidth() == child->GetWidth() )
		{		  
			width = sizes[ index ].x;
		}
		else if ( ui::FillWidth() == child->GetWidth() )
		{
			switch ( m_direction )
			{
			case Stack::Vertical:
				width = parentWidth - padding * 2;
				break;
			case Stack::Horizontal:
				width = (fillWidth / fillWidthTotalWeight) * child->GetFillWidthWeight();
				break;
			}
		}
		else
		{
			width = sizes[ index ].x;
		}

		// Height...
		if ( ui::DefaultHeight() == child->GetHeight() )
		{
			height = sizes[ index ].y;
		}
		else if ( ui::FillHeight() == child->GetHeight() )
		{
			switch ( m_direction )
			{
			case Stack::Vertical:
				height = (fillHeight / fillHeightTotalWeight) * child->GetFillHeightWeight();
				break;
			case Stack::Horizontal:
				height = parentHeight - padding * 2;
				break;
			}		
		}
		else
		{
			height = sizes[ index ].y;
		}

		child->Create( x, y, width, height, parent );
		switch ( m_direction )
		{
		case Stack::Vertical:
			x += 0;
			y += height + padding;
			break;
		case Stack::Horizontal:
			x += width + padding;
			y += 0;
			break;
		}

	}
}

int StackPanel::GetPadding()
{
	return m_padding;
}

void StackPanel::SetPadding( int padding )
{
	m_padding = padding;
}
