// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/TextElement.h>
#include <me/RenderMethod.h>
#include <me/VertexUtil.h>

using namespace me;
using namespace scene2d;

TextElement::TextElement( me::IGame * game, Effect::ptr effect, std::string text, Anchor anchor, unify::V2< float > scale, unify::V2< float > offset )
	: Element( game, offset, {0, 0}, anchor )
	, m_text( text )
	, m_effect( effect )
	, m_changed( true )
	, m_scale{ scale }
	, m_shrinkToFit{ true }
{
}
		
unify::Size< float > TextElement::GetSize( unify::Size< float > area ) const
{
	if ( m_text.empty() || ! IsEnabled() ) return { 0, 0 };
	
	unify::Size< unsigned int > imageSize( m_effect->GetTexture(0)->ImageSize() );

	unify::V2< float > posUL(0, 0);

	// Determine sizes...
	unify::Size< float > textSize {};
	for( auto c : m_text )
	{	
		unify::TexArea texArea = m_effect->GetTexture(0)->GetSpriteDictionary().GetAscii( c );

		unify::Size< float > size( imageSize.width * m_scale.x * texArea.Width(), imageSize.height * m_scale.y * texArea.Height() );

		unify::V2< float > posDR( posUL.x + size.width, posUL.y + size.height );
		posUL.x = posDR.x;
		textSize.width = std::max( textSize.width, posDR.x );
		textSize.height = std::max( textSize.height, posDR.y );
	}
	return textSize;
}

void TextElement::BuildText( unify::Size< float > area )
{
	if ( m_text.empty() ) return;

	auto vd = m_effect->GetVertexShader()->GetVertexDeclaration();
	size_t vertexCount = m_text.length() * 6;


	unify::Size< unsigned int > imageSize( m_effect->GetTexture(0)->ImageSize() );

	size_t vbSizeInBytes = vd->GetSizeInBytes(0) * vertexCount;
	std::shared_ptr< unsigned char > vertices( new unsigned char[ vbSizeInBytes ] );
	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes(0), vertexCount, false, 0 );

	VertexElement positionE = CommonVertexElement::Position( 0 );
	VertexElement texcoordsE = CommonVertexElement::TexCoords( 0 );

	unify::V2< float > scale( m_scale );

	unify::Size< float > textSize = GetSize( area );

	unify::V2< float > posUL( { 0, 0 } );
	switch( GetAnchor() )
	{
	case Anchor::Free:
		// Do nothing (handled by adding offset later)
		break;
	case Anchor::TopLeft:
		posUL = unify::V2< float >{ 0, 0 };
		break;
	case Anchor::Top:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width * 0.5f, 0 };
		break;
	case Anchor::TopRight:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width, 0 };
		break;
	case Anchor::StretchTop:
		posUL = unify::V2< float >{ 0, 0 };
		scale.x = scale.x * (GetActualSize().width / textSize.width);
		break;

	case Anchor::Left:
		posUL = unify::V2< float >{ 0, GetActualPosition().y - textSize.height * 0.5f };
		break;
	case Anchor::Center:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width * 0.5f, GetActualPosition().y - textSize.height * 0.5f };
		break;
	case Anchor::Right:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width, GetActualPosition().y - textSize.height * 0.5f };
		break;
	case Anchor::StretchLeftRight:
		posUL = unify::V2< float >{ 0, GetActualPosition().y - textSize.height * 0.5f };
		scale.x = scale.x * (GetActualSize().width / textSize.width);
		break;

	case Anchor::BottomLeft:
		posUL = unify::V2< float >{ 0, GetActualPosition().y - textSize.height };
		break;
	case Anchor::Bottom:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width * 0.5f, GetActualPosition().y - textSize.height};
		break;
	case Anchor::BottomRight:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width, GetActualPosition().y - textSize.height };
		break;
	case Anchor::StretchBottom:
		posUL = unify::V2< float >{ 0, GetActualPosition().y - textSize.height };
		scale.x = scale.x * (GetActualSize().width / textSize.width);
		break;

	case Anchor::StretchLeft:
		posUL = unify::V2< float >{ 0, 0 };
		scale.y = scale.y * (GetActualSize().height / textSize.height);
		break;
	case Anchor::StretchTopBottom:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width * 0.5f, 0 };
		scale.y = scale.y * (GetActualSize().height / textSize.height);
		break;
	case Anchor::StretchRight:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width, 0 };
		scale.y = scale.y * (GetActualSize().height / textSize.height);
		break;

	case Anchor::StretchFull:
		posUL = unify::V2< float >{ 0, 0 };
		scale.x = scale.x * (GetActualSize().width / textSize.width);
		scale.y = scale.y * (GetActualSize().height / textSize.height);
		break;
	}

	posUL += GetOffset();

	size_t vertex = 0;
	for( auto c : m_text )
	{	
		unify::TexArea texArea = m_effect->GetTexture(0)->GetSpriteDictionary().GetAscii( c );

		unify::Size< float > size( imageSize.width * scale.x * texArea.Width(), imageSize.height * scale.y * texArea.Height() );

		unify::V2< float > posDR( posUL.x + size.width, posUL.y + size.height );

		WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posUL.x, posUL.y ) );
		WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( texArea.ul.u, texArea.ul.v ) );
		vertex++;

		WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posDR.x, posUL.y ) );
		WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( texArea.dr.u, texArea.ul.v ) );
		vertex++;

		WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posUL.x, posDR.y ) );
		WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( texArea.ul.u, texArea.dr.v ) );
		vertex++;

		WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posDR.x, posUL.y ) );
		WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( texArea.dr.u, texArea.ul.v ) );
		vertex++;

		WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posDR.x, posDR.y ) );
		WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( texArea.dr.u, texArea.dr.v ) );
		vertex++;

		WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posUL.x, posDR.y ) );
		WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( texArea.ul.u, texArea.dr.v ) );
		vertex++;

		posUL.x = posDR.x;
	}

	VertexBufferParameters parameters{ vd, { { vertexCount, vertices.get() } } };
	m_vertexBuffer = GetGame()->GetOS()->GetRenderer(0)->ProduceVB( parameters );
	m_changed = false;
}

void TextElement::SetText( std::string text )
{
	m_text = text;
	m_changed = true;
}

void TextElement::SetScale( unify::V2< float > scale )
{
	m_scale = scale;
}

unify::V2< float > TextElement::GetScale() const
{
	return m_scale;
}

void TextElement::UpdateLayout( IRenderer * renderer, const RenderInfo & renderInfo, unify::Size< float > area )
{
	if ( ! IsEnabled() ) return;

	unify::V2< float > backPosition = GetActualPosition();
	unify::Size< float > backSize = GetActualSize();
	
	Element::UpdateLayout( renderer, renderInfo, area );

	if ( GetActualPosition() != backPosition || GetActualSize() != backSize )
	{
		m_changed = true;
	}

	if ( m_changed )
	{
		BuildText( area );
	}
}

void TextElement::Update( IRenderer * renderer, const RenderInfo & renderInfo )
{
}
		
void TextElement::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( m_text.empty() ) return;
	if ( ! IsEnabled() ) return;

	m_vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleList( 0, m_text.length() * 2, m_effect ) );
	
	unify::Matrix instance{ unify::MatrixIdentity() };
	MatrixFeed matrixFeed( &instance, 1 );
	//renderer->Render( method, renderInfo, &instance, 1 );
	renderer->Render( method, renderInfo, matrixFeed );
}
		
void TextElement::OnSuspend()
{
}
		
void TextElement::OnResume()
{
}
