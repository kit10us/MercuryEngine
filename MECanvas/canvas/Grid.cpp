// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include "canvas/Grid.h"
#include "canvas/TextElement.h"

using namespace me;
using namespace canvas;

Grid::Grid( game::IGame * gameInstance, unify::RowColumn< size_t > rc, unify::Size< float > size, Anchor anchor, unify::V2< float > offset )
	: Element( gameInstance, offset, { 0, 0 }, anchor )
	, m_rc{ rc }
	, m_size{ size }
	, m_changed( true )
{
}

unify::Size< float > Grid::GetContentSize() const
{
	if( ! IsEnabled() ) return{ 0, 0 };
	
	return{ 100, 160 }; // TODO:
}

void Grid::UpdateLayout( UpdateParams params, unify::Rect< float > parentArea )
{
	if( !IsEnabled() ) return;

	unify::V2< float > backPosition = GetActualPosition();
	unify::Size< float > backSize = GetActualSize();

	Element::UpdateLayout( params, parentArea );

	/*
	if( GetActualPosition() != backPosition || GetActualSize() != backSize || m_changed )
	{
		unify::Size< float > s{ parentArea.Size() };
		unify::V2< float > offset{ 0, 0 };

		for( size_t r = 0; r < m_rc.row; r++ )
		{
			offset.y = 0;
			for( size_t c = 0; c < m_rc.column; c++ )
			{
				auto && item = m_items[r][c];
				item->SetOffset( offset );
				item->UpdateLayout( params, { parentArea.UL() + GetOffset(), { m_sizeToWidth.GetSize( parentArea.Width(), totalHorizontalWeight ), m_sizeToHeight.GetSize( parentArea.Height(), totalVerticalWeight ) } } );
				offset.x += width;
				offset.y += height;
			}
		}
		m_changed = false;
	}
	*/
}

void Grid::Update( const UpdateParams & params )
{
	if( !IsEnabled() ) return;
	/*
	for( auto && item : m_items )
	{
		item->Update( params );
	}
	*/
}

void Grid::Render( const render::Params & params )
{
	if( !IsEnabled() ) return;
	if( m_items.empty() ) return;

	/*
	for( auto && item : m_items )
	{
		item->Render( params );
	}
	*/
}

void Grid::OnSuspend()
{
}

void Grid::OnResume()
{
}

void Grid::AddItem( IElement::ptr item )
{
	/*
	m_items.push_back( item );
	m_changed = true;
	*/
}
