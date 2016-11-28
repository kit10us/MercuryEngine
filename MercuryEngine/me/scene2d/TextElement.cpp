// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/TextElement.h>
#include <me/RenderMethod.h>
#include <me/VertexUtil.h>

using namespace me;
using namespace scene2d;

TextElement::TextElement( me::IGame * game, Effect::ptr effect, std::string text )
	: m_game( game )
	, m_text( text )
	, m_effect( effect )
	, m_changed( true )
{
}
		
void TextElement::BuildText()
{
	if ( m_text.empty() ) return;

	VertexBufferParameters parameters;
	parameters.vertexDeclaration = m_effect->GetVertexShader()->GetVertexDeclaration();
	parameters.numVertices = m_text.length() * 6;

	auto vd = parameters.vertexDeclaration;

	unify::Size< unsigned int > imageSize( m_effect->GetTexture(0)->ImageSize() );

	m_effect->GetTexture(0)->GetSpriteDictionary();

	size_t vbSizeInBytes = vd->GetSize(0) * parameters.numVertices;
	std::shared_ptr< unsigned char > vertices( new unsigned char[ vbSizeInBytes ] );
	parameters.source = vertices.get();
	unify::DataLock lock( vertices.get(), vd->GetSize(0), parameters.numVertices, false, 0 );

	VertexElement positionE = CommonVertexElement::Position( 0 );
	VertexElement texcoordsE = CommonVertexElement::TexCoords( 0 );

	unify::V2< float > scale( 1, 1 );

	size_t vertex = 0;
	unify::V2< float > posUL(0, 0);
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

	m_vertexBuffer = m_game->GetOS()->GetRenderer(0)->ProduceVB( parameters );
	m_changed = false;
}

void TextElement::SetText( std::string text )
{
	m_text = text;
	m_changed = true;
}

void TextElement::Update( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( m_changed )
	{
		BuildText();
	}
}
		
void TextElement::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( m_text.empty() ) return;

	m_vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleList( 0, m_text.length() * 2, m_effect ) );
	renderer->Render( method, renderInfo, { unify::MatrixIdentity() } );
}
		
void TextElement::OnSuspend()
{
}
		
void TextElement::OnResume()
{
}
