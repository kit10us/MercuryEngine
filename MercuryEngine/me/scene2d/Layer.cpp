// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/Layer.h>

using namespace me;
using namespace scene2d;
using namespace scene;

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

void Layer::UpdateLayout( IRenderer * renderer, const RenderInfo & renderInfo, unify::Size< float > area )
{
	if ( ! IsEnabled() ) return;

	Element::UpdateLayout( renderer, renderInfo, area );

	for( auto && element : m_elements )
	{
		element->UpdateLayout( renderer, renderInfo, GetActualSize() );
	}
}

void Layer::Update( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( ! IsEnabled() ) return;

	for( auto && element : m_elements )
	{
		element->Update( renderer, renderInfo );
	}
}

void Layer::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( ! IsEnabled() ) return;

	for( auto && element : m_elements )
	{
		element->Render( renderer, renderInfo );
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