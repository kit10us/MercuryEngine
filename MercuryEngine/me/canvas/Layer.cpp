// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/canvas/Layer.h>

using namespace me;
using namespace canvas;

Layer::Layer( IGame * game, unify::V2< float > position, unify::Size< float > size, Anchor anchor )
	: Element( game, position, size, anchor )
{
}

void Layer::AddElement( IElement::ptr element )
{
	m_elements.push_back( element );
}

void Layer::AddElement( IElement::ptr element, std::string name )
{
	AddElement( element );
	m_elementMap[ name ] = element;
}

IElement::ptr Layer::FindElement( std::string name )
{
	auto itr = m_elementMap.find( name );
	if ( itr == m_elementMap.end() ) return IElement::ptr();
	return itr->second;
}

void Layer::UpdateLayout( UpdateParams params, unify::Size< float > area )
{
	if ( ! IsEnabled() ) return;

	Element::UpdateLayout( params, area );

	for( auto && element : m_elements )
	{
		element->UpdateLayout( params, GetActualSize() );
	}
}

void Layer::Update( UpdateParams params )
{
	if ( ! IsEnabled() ) return;

	for( auto && element : m_elements )
	{
		element->Update( params );
	}
}

void Layer::Render( render::Params params )
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