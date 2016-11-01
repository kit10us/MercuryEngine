// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/Character.h>

using namespace dxi;
using namespace font;

Character::Character()
: m_source( Source::Invalid )
{
}

Character::Character( const Character & character )
: m_source( character.m_source )
, m_effect( character.m_effect )
, m_animationInstance( character.m_animationInstance )
, m_size( character.m_size )
, m_geometry( character.m_geometry )
, m_geometryPostScale2D( character.m_geometryPostScale2D )
, m_geometryPostOffset2D( character.m_geometryPostOffset2D )
, m_geometryPostScale3D( character.m_geometryPostScale3D )
, m_geometryPostOffset3D( character.m_geometryPostOffset3D )
{
}

Character::Character( const animation::Instance & animationInstance, Effect::ptr effect )
: m_source( Source::Sprite )
, m_effect( effect )
, m_animationInstance( animationInstance )
, m_size( animationInstance.GetFrame().GetSizeInPixels() )
{
}

Character::Character( Geometry::ptr geometry, const unify::Size< float > & sizeInPixels, float geometryPostScale3D, const unify::V3< float > & geometryPostOffset2D, float geometryPostScale2D, const unify::V3< float > & geometryPostOffset3D )
: m_source( Source::Geometry )
, m_geometry( geometry )
, m_size( sizeInPixels )
, m_geometryPostScale2D( geometryPostScale2D )
, m_geometryPostOffset2D( geometryPostOffset2D )
, m_geometryPostScale3D( geometryPostScale3D )
, m_geometryPostOffset3D( geometryPostOffset3D )
{
}

Character::~Character() throw ()
{
}

Character::Source::TYPE Character::GetSource() const
{
	return m_source;
}

std::shared_ptr< Effect > Character::GetEffect()
{
	return m_effect;
}

const std::shared_ptr< Effect > Character::GetEffect() const
{
	return m_effect;
}

const animation::Instance & Character::GetAnimationInstance() const
{
	return m_animationInstance;
}

std::shared_ptr< Geometry > Character::GetGeometry()
{
	return m_geometry;					  
}

const std::shared_ptr< Geometry > Character::GetGeometry() const
{
	return m_geometry;
}

unify::Size< float > Character::GetSize() const
{
	return m_size;
}

bool Character::empty() const
{
	return m_source == Source::Invalid;
}

void Character::Update( unify::Seconds elapsed )
{
	switch( m_source )
	{
	case Source::Sprite:
		m_animationInstance.Update( elapsed );
		break;
	case Source::Geometry:
		break;
	default:
		throw unify::Exception( "Attempted to update an invalid character!" );
	}
}

void Character::Render( const RenderInfo & renderInfo, const unify::Matrix & origin, const unify::V2< float > & offset, float scale, bool is3D )
{
	unify::Matrix world( origin );

	switch( m_source )
	{
	case Source::Sprite:
		{
			VertexBuffer & vb = *m_effect->GetScratchVertexBuffer();
			VertexDeclaration::ptr vd = vb.GetVertexDeclaration();

			VertexElement positionE = CommonVertexElement::Position();
			VertexElement texE = CommonVertexElement::TexCoords();

			unify::DataLock lock;
			vb.Lock( lock );
			const unify::TexArea & area = m_animationInstance.GetFrame().GetArea();
			unify::Size< float > halfSize( m_size.width * scale * 0.5f, m_size.height * scale * 0.5f );
			float depth = 1.0f;
			vd->WriteVertex( lock, 0, positionE, unify::V3< float >( 0 - halfSize.width, 0 - halfSize.height, depth ) );
			vd->WriteVertex( lock, 1, positionE, unify::V3< float >( 0 + halfSize.width, 0 - halfSize.height, depth ) );
			vd->WriteVertex( lock, 2, positionE, unify::V3< float >( 0 - halfSize.width, 0 + halfSize.height, depth ) );
			vd->WriteVertex( lock, 3, positionE, unify::V3< float >( 0 + halfSize.width, 0 + halfSize.height, depth ) );
			vd->WriteVertex( lock, 0, texE, unify::TexCoords( area.UL() ) );
			vd->WriteVertex( lock, 1, texE, unify::TexCoords( area.UR() ) );
			vd->WriteVertex( lock, 2, texE, unify::TexCoords( area.DL() ) );
			vd->WriteVertex( lock, 3, texE, unify::TexCoords( area.DR() ) );
			vb.Unlock();
			vb.Use();

			world.Translate( unify::V3< float >( offset.x, offset.y, 0 ) );

			RenderInfo myRenderInfo( renderInfo );
			myRenderInfo.SetWorldMatrix( world * renderInfo.GetWorldMatrix() );

			m_effect->Use( myRenderInfo );
			m_animationInstance.GetFrame().GetTexture()->Use( 0 );

			/*
			//TODO:
			win::DX::GetDxDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			*/
		}
		break;
	case Source::Geometry: // In screen coordinates
		{
			unify::Matrix temp = unify::MatrixRotationAboutAxis( unify::V3< float >( 1, 0, 0 ), unify::AnglePI() );
			
			if( is3D )
			{
				temp.Scale( unify::V3< float >( m_geometryPostScale3D * scale, m_geometryPostScale3D * scale, m_geometryPostScale3D * scale ) );
				temp *= world;
				temp.Translate( unify::V3< float >( offset.x, offset.y, -1 ) );
				temp.Translate( m_geometryPostOffset3D * scale );
			}
			else
			{
				temp.Scale( unify::V3< float >( m_geometryPostScale2D * scale, m_geometryPostScale2D * scale, m_geometryPostScale2D * scale ) );
				temp *= world;
				temp.Translate( unify::V3< float >( offset.x, offset.y, -1 ) );
				temp.Translate( m_geometryPostOffset2D * scale );
			}

			assert( 0 ); // This was changed form the "standalone", ensure this works...
			RenderInfo renderInfo( renderInfo );
			renderInfo.SetWorldMatrix( temp );
			m_geometry->Render( renderInfo, 0 );
		}
		break;
	}
}
