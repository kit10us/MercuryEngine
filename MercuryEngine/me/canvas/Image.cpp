// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/canvas/Image.h>
#include <me/render/RenderMethod.h>
#include <me/render/VertexUtil.h>
#include <algorithm>

using namespace me;
using namespace canvas;
using namespace render;

Image::Image( me::IGame * game, Effect::ptr effect, Anchor anchor, unify::V2< float > scale, unify::V2< float > offset )
	: Element( game, offset, {0, 0}, anchor )
	, m_effect( effect )
	, m_changed( true )
	, m_scale{ scale }
	, m_shrinkToFit{ true }
{
}
		
unify::Size< float > Image::GetSize( unify::Size< float > area ) const
{
	return m_effect->LargestTextureSizes();
}

void Image::Build( unify::Size< float > area )
{
	auto vd = m_effect->GetVertexShader()->GetVertexDeclaration();
	size_t vertexCount = 6;

	unify::Size< unsigned int > imageSize( m_effect->LargestTextureSizes() );

	size_t vbSizeInBytes = vd->GetSizeInBytes( 0 ) * vertexCount;
	std::shared_ptr< unsigned char > vertices( new unsigned char[vbSizeInBytes] );
	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

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
		scale.x = scale.x * ( GetActualSize().width / textSize.width );
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
		scale.x = scale.x * ( GetActualSize().width / textSize.width );
		break;

	case Anchor::BottomLeft:
		posUL = unify::V2< float >{ 0, GetActualPosition().y - textSize.height };
		break;
	case Anchor::Bottom:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width * 0.5f, GetActualPosition().y - textSize.height };
		break;
	case Anchor::BottomRight:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width, GetActualPosition().y - textSize.height };
		break;
	case Anchor::StretchBottom:
		posUL = unify::V2< float >{ 0, GetActualPosition().y - textSize.height };
		scale.x = scale.x * ( GetActualSize().width / textSize.width );
		break;

	case Anchor::StretchLeft:
		posUL = unify::V2< float >{ 0, 0 };
		scale.y = scale.y * ( GetActualSize().height / textSize.height );
		break;
	case Anchor::StretchTopBottom:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width * 0.5f, 0 };
		scale.y = scale.y * ( GetActualSize().height / textSize.height );
		break;
	case Anchor::StretchRight:
		posUL = unify::V2< float >{ GetActualPosition().x - textSize.width, 0 };
		scale.y = scale.y * ( GetActualSize().height / textSize.height );
		break;

	case Anchor::StretchFull:
		posUL = unify::V2< float >{ 0, 0 };
		scale.x = scale.x * ( GetActualSize().width / textSize.width );
		scale.y = scale.y * ( GetActualSize().height / textSize.height );
		break;
	}

	posUL += GetOffset();

	size_t vertex = 0;
	unify::TexArea texArea = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };

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

	VertexBufferParameters parameters{ vd, { { vertexCount, vertices.get() } } };
	m_vertexBuffer = GetGame()->GetOS()->GetRenderer(0)->ProduceVB( parameters );
	m_changed = false;
}

void Image::SetScale( unify::V2< float > scale )
{
	m_scale = scale;
}

unify::V2< float > Image::GetScale() const
{
	return m_scale;
}

void Image::UpdateLayout( UpdateParams params, unify::Size< float > area )
{
	if ( ! IsEnabled() ) return;

	unify::V2< float > backPosition = GetActualPosition();
	unify::Size< float > backSize = GetActualSize();
	
	Element::UpdateLayout( params, area );

	if ( GetActualPosition() != backPosition || GetActualSize() != backSize )
	{
		m_changed = true;
	}

	if ( m_changed )
	{
		Build( area );
	}
}

void Image::Update( UpdateParams params )
{
}
		
void Image::Render( render::Params params )
{
	if ( ! IsEnabled() ) return;

	m_vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleList( 0, 2, m_effect ) );
	
	unify::Matrix instance{ unify::MatrixIdentity() };
	render::MatrixFeed matrixFeed( render::MatrixFood_Matrices{ &instance, 1 }  );
	params.renderer->Render( method, params.renderInfo, matrixFeed );
}
		
void Image::OnSuspend()
{
}
		
void Image::OnResume()
{
}
