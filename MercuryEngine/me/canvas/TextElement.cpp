// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/canvas/TextElement.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>
#include <algorithm>

using namespace me;
using namespace canvas;
using namespace render;

TextElement::TextElement( me::IGame * game, Effect::ptr effect, std::string text, Anchor anchor, unify::V2< float > scale, unify::V2< float > offset )
	: Element( game, offset, {0, 0}, anchor )
	, m_text( text )
	, m_effect( effect )
	, m_changed( true )
	, m_scale{ scale }
	, m_shrinkToFit{ true }
{
}
		
unify::Size< float > TextElement::GetContentSize() const
{
	if ( m_text.empty() || ! IsEnabled() ) return { 0, 0 };
	
	unify::Size< unsigned int > imageSize( m_effect->GetTexture(0)->ImageSize() );

	unify::V2< float > posUL(0, 0);

	// Determine sizes...
	unify::Size< float > textSize {};
	for( auto c : m_text )
	{	
		unify::TexArea texArea = m_effect->GetTexture(0)->GetSpriteDictionary().GetAscii( c );

		// Character size...
		unify::Size< float > size( imageSize.width * texArea.Width(), imageSize.height * texArea.Height() );

		// Scale...
		size *= { m_scale.x, m_scale.y };

		unify::V2< float > posDR( posUL.x + size.width, posUL.y + size.height );
		posUL.x = posDR.x;
		textSize.width = std::max( textSize.width, posDR.x );
		textSize.height = std::max( textSize.height, posDR.y );
	}
	return textSize;
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

void TextElement::UpdateLayout( UpdateParams params, unify::Rect< float > parentArea )
{
	if ( ! IsEnabled() ) return;

	unify::V2< float > backPosition = GetActualPosition();
	unify::Size< float > backSize = GetActualSize();
	
	Element::UpdateLayout( params, parentArea );

	auto actualPosition = GetActualPosition();
	auto actualSize = GetActualSize();

	if ( backPosition != actualPosition || backSize != actualSize || m_changed )
	{
		if( m_text.empty() ) return;

		unify::V2< float > scale( m_scale );
		unify::Size< unsigned int > imageSize( m_effect->GetTexture( 0 )->ImageSize() );
		unify::Size< float > textSize = GetContentSize();
		unify::Size< float > actualSize = GetActualSize();

		unify::V2< float > posUL( { 0, 0 } );
		posUL = parentArea.UL();
		switch( GetAnchor() )
		{
		case Anchor::Free:
			// Do nothing (handled by adding offset later)
			break;
		case Anchor::TopLeft:
			// Do nothing (handled by adding offset later)
			break;
		case Anchor::Top:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width * 0.5f, 0 };
			break;
		case Anchor::TopRight:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width, 0 };
			break;
		case Anchor::StretchTop:
			posUL += unify::V2< float >{ 0, 0 };
			scale.x = m_scale.x;
			break;

		case Anchor::Left:
			posUL += unify::V2< float >{ 0, actualPosition.y - textSize.height * 0.5f };
			break;
		case Anchor::Center:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width * 0.5f, actualPosition.y - textSize.height * 0.5f };
			break;
		case Anchor::Right:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width, actualPosition.y - textSize.height * 0.5f };
			break;
		case Anchor::StretchLeftRight:
			posUL += unify::V2< float >{ 0, actualPosition.y - textSize.height * 0.5f };
			scale.x = scale.x;
			break;

		case Anchor::BottomLeft:
			posUL += unify::V2< float >{ 0, actualPosition.y - textSize.height };
			break;
		case Anchor::Bottom:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width * 0.5f, actualPosition.y - textSize.height };
			break;
		case Anchor::BottomRight:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width, actualPosition.y - textSize.height };
			break;
		case Anchor::StretchBottom:
			posUL += unify::V2< float >{ 0, actualPosition.y - textSize.height };
			scale.x = scale.x;
			break;

		case Anchor::StretchLeft:
			posUL += unify::V2< float >{ 0, 0 };
			scale.y = scale.y * ( imageSize.height / textSize.height );
			break;
		case Anchor::StretchTopBottom:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width * 0.5f, 0 };
			scale.y = scale.y * ( imageSize.height / textSize.height );
			break;
		case Anchor::StretchRight:
			posUL += unify::V2< float >{ actualPosition.x - textSize.width, 0 };
			scale.y = scale.y * ( imageSize.height / textSize.height );
			break;

		case Anchor::StretchFull:
			posUL += unify::V2< float >{ 0, 0 };
			scale.x = scale.x * ( imageSize.width / textSize.width );
			scale.y = scale.y * ( imageSize.height / textSize.height );
			break;
		}

		posUL += GetOffset();

		// Write to vertices...

		auto vd = m_effect->GetVertexShader()->GetVertexDeclaration();
		size_t vertexCount = m_text.length() * 6;

		size_t vbSizeInBytes = vd->GetSizeInBytes( 0 ) * vertexCount;
		std::shared_ptr< unsigned char > vertices( new unsigned char[vbSizeInBytes] );
		unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

		VertexElement positionE = CommonVertexElement::Position( 0 );
		VertexElement texcoordsE = CommonVertexElement::TexCoords( 0 );

		size_t vertex = 0;
		for( auto c : m_text )
		{
			unify::TexArea charTexArea = m_effect->GetTexture( 0 )->GetSpriteDictionary().GetAscii( c );

			unify::Size< float > size( imageSize.width * scale.x * charTexArea.Width(), imageSize.height * scale.y * charTexArea.Height() );

			unify::V2< float > posDR{ posUL + unify::V2< float >( size ) };

			WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posUL.x, posUL.y ) );
			WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( charTexArea.ul.u, charTexArea.ul.v ) );
			vertex++;

			WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posDR.x, posUL.y ) );
			WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( charTexArea.dr.u, charTexArea.ul.v ) );
			vertex++;

			WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posUL.x, posDR.y ) );
			WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( charTexArea.ul.u, charTexArea.dr.v ) );
			vertex++;

			WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posDR.x, posUL.y ) );
			WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( charTexArea.dr.u, charTexArea.ul.v ) );
			vertex++;

			WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posDR.x, posDR.y ) );
			WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( charTexArea.dr.u, charTexArea.dr.v ) );
			vertex++;

			WriteVertex( *vd, lock, vertex, positionE, unify::V2< float >( posUL.x, posDR.y ) );
			WriteVertex( *vd, lock, vertex, texcoordsE, unify::TexCoords( charTexArea.ul.u, charTexArea.dr.v ) );
			vertex++;

			posUL.x = posDR.x;
		}

		VertexBufferParameters parameters{ vd, { { vertexCount, vertices.get() } } };
		m_vertexBuffer = GetGame()->GetOS()->GetRenderer( 0 )->ProduceVB( parameters );
		m_changed = false;
	}
}

void TextElement::Update( const UpdateParams & params )
{
}
		
void TextElement::Render( render::Params params )
{
	if ( m_text.empty() ) return;
	if ( ! IsEnabled() ) return;

	m_vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleList( 0, m_text.length() * 2, m_effect ) );
	
	unify::Matrix instance{ unify::MatrixIdentity() };
	render::MatrixFeed matrixFeed( render::MatrixFood_Matrices{ &instance, 1 }  );
	params.renderer->Render( method, params.renderInfo, matrixFeed );
}
		
void TextElement::OnSuspend()
{
}
		
void TextElement::OnResume()
{
}
