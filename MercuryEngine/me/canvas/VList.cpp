// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include "me/canvas/VList.h"
#include "me/canvas/TextElement.h"

using namespace me;
using namespace canvas;

VList::VList( game::IGame * gameInstance, size_t count, unify::Size< float > size, Anchor anchor, unify::V2< float > offset )
	: Element( gameInstance, offset, { 0, 0 }, anchor )
	, m_count{ count }
	, m_size{ size }
	, m_changed( true )
{
}

unify::Size< float > VList::GetContentSize() const
{
	if( ! IsEnabled() ) return{ 0, 0 };
	
	return{ 100, 160 }; // TODO:
}

void VList::UpdateLayout( UpdateParams params, unify::Rect< float > parentArea )
{
	if( !IsEnabled() ) return;

	unify::V2< float > backPosition = GetActualPosition();
	unify::Size< float > backSize = GetActualSize();

	Element::UpdateLayout( params, parentArea );

	if( GetActualPosition() != backPosition || GetActualSize() != backSize || m_changed )
	{
		float itemWidth = parentArea.Width() - GetOffset().x;
		float itemHeight = parentArea.Height() / m_count;
		unify::V2< float > offset{ GetOffset() };
		for( auto && item : m_items )
		{
			item.GetItem()->UpdateLayout( params, { parentArea.UL() + offset, unify::Size< float >{ itemWidth, itemHeight } } );
			offset.y += itemHeight;
		}
		m_changed = false;
	}
}

void VList::Update( const UpdateParams & params )
{
	if( !IsEnabled() ) return;

	for( auto && item : m_items )
	{
		item.GetItem()->Update( params );
	}
}

void VList::Render( const render::Params & params )
{
	if( !IsEnabled() ) return;
	if( m_items.empty() ) return;

	for( auto && item : m_items )
	{
		item.GetItem()->Render( params );
	}
}

void VList::OnSuspend()
{
}

void VList::OnResume()
{
}

void VList::AddItem( IElement::ptr item, std::string name, std::string tag )
{
	auto listItem = ListItem{ item, tag };
	m_items.push_back( listItem );
	item->SetName( name );
	m_changed = true;
}

IElement::ptr VList::FindItem( std::string name )
{
	for (auto && item : m_items)
	{
		if (unify::StringIs( name, item.GetItem()->GetName() ))
		{
			return item.GetItem();
		}
	}
	throw unify::Exception( "Item not found within VList!" );
}
