// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/canvas/Layer.h>
#include <me/exception/UnsupportedValue.h>

using namespace me;
using namespace canvas;

Layer::Layer( game::IGame * gameInstance, unify::V2< float > position, unify::Size< float > size, Anchor anchor )
	: Element( gameInstance, position, size, anchor )
{
}

void Layer::AddElement( IElement::ptr element )
{
	m_elements.push_back( element );
}

void Layer::AddElement( IElement::ptr element, std::string name )
{
	if( name.empty() )
	{
		throw unify::Exception( "Attempted to add an element to a Layer with an invalid name!" );
	}

	AddElement( element );
	element->SetName( name );
	m_elementMap[ name ] = element;
}

void Layer::RemoveElement( IElement::ptr element )
{
	for ( auto itr = m_elementMap.begin(); itr != m_elementMap.end(); itr++ )
	{
		if ( itr->second == element )
		{
			m_elementMap.erase( itr );
		}
	}
	m_elements.remove( element );
}

void Layer::RemoveElement( std::string name )
{
	auto itr = m_elementMap.find( name );
	if ( itr != m_elementMap.end() )
	{
		m_elements.remove( itr->second );
		m_elementMap.erase( name );
	}
}

IElement::ptr Layer::FindElement( std::string name )
{
	auto itr = m_elementMap.find( name );
	if ( itr == m_elementMap.end() )
	{
		return IElement::ptr();
	}
	return itr->second;
}

std::string Layer::ElementName( IElement::const_ptr element ) const
{
	for (auto itr = m_elementMap.begin(); itr != m_elementMap.end(); itr++)
	{
		if (itr->second == element)
		{
			return itr->first;
		}
	}
	
	return std::string();
}

void Layer::BringToFront( IElement::ptr element )
{
	if( *m_elements.begin() != element )
	{
		m_elements.remove( element );
		m_elements.push_front( element);
	}
}

/// <summary>
/// Send an element to the back.
/// </summary>
void Layer::SendToBack( IElement::ptr element )
{
	if (*m_elements.rbegin() != element)
	{
		m_elements.remove( element );
		m_elements.push_back( element );
	}
}

void Layer::UpdateLayout( UpdateParams params, unify::Rect< float > parentArea )
{
	if ( ! IsEnabled() ) return;

	Element::UpdateLayout( params, parentArea );

	for( auto && element : m_elements )
	{
		element->UpdateLayout( params, { parentArea.UL(), GetActualSize() } );
	}
}

void Layer::Update( const UpdateParams & params )
{
	if ( ! IsEnabled() ) return;

	for( auto && element : m_elements )
	{
		element->Update( params );
	}
}

void Layer::Render( const render::Params & params )
{
	if ( ! IsEnabled() ) return;

	for( auto && element : m_elements )
	{
		element->Render( params );
	}
}

void Layer::OnSuspend()
{
	for( auto && element : m_elements )
	{
		element->OnSuspend();
	}
}

void Layer::OnResume()
{
	for( auto && element : m_elements )
	{
		element->OnResume();
	}
}