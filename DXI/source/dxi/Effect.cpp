// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Effect.h>
#include <dxi/core/Game.h>

using namespace dxi;

Effect::Effect()
	: m_culling( CullNone )
{
}

Effect::~Effect()
{
}

Effect & Effect::operator = ( const Effect & effect )
{
	m_culling = effect.m_culling;

	m_frameIndexAndInfluence = effect.m_frameIndexAndInfluence;

	m_vertexShader = effect.m_vertexShader;
	m_pixelShader = effect.m_pixelShader;

	return *this;
}

bool Effect::operator == ( const Effect & effect ) const
{
	if( m_culling != effect.m_culling ) return false;
	if ( m_vertexShader != effect.m_vertexShader ) return false;
	if ( m_pixelShader != effect.m_pixelShader ) return false;

	return true;
}

bool Effect::operator != ( const Effect & effect ) const
{
	return !( *this == effect );
}

void Effect::Use( const RenderInfo & renderInfoIn )
{
	RenderInfo renderInfo( renderInfoIn );

	/*
	TODO:
	switch( m_culling )
	{
	case CullNone:
		win::DX::GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		break;
	case CullCW:
		win::DX::GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		break;
	case CullCCW:
		win::DX::GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
		break;
	case CullIgnore:
		break;
	}
	*/


	// MATRIX...
	const unify::FrameSetInstance * frameSetInstance = renderInfo.GetFrameSetInstance();
	if( ! renderInfo.IsOptionTrue( RenderOption::NoFrame ) && frameSetInstance && m_frameIndexAndInfluence.size() > 0 )
	{
		unify::Matrix world = renderInfo.GetWorldMatrix();

		// Get our animated frames lookup table. If it doesn't exist, then we don't want to use it.
		for( size_t i = 0, end = m_frameIndexAndInfluence.size(); i != end; ++i )
		{
			FrameIndexAndInfluence & frameIndexAndInfluence = m_frameIndexAndInfluence[ i ];
			const size_t frameIndex = frameIndexAndInfluence.first;
			const float influence = frameIndexAndInfluence.second;
			
			const unify::Matrix & a = frameSetInstance->OriginalLocal( frameIndex );
			const unify::Matrix & b = frameSetInstance->Local( frameIndex );
			
			unify::Matrix inverseA( a );
			inverseA.Invert();
			
			unify::Matrix immediate( inverseA * (b * influence) );

			world *= immediate;
		}

		assert( 0 ); // TODO:
		renderInfo.SetWorldMatrix( world );
	}

	if( m_pixelShader )
	{
		m_pixelShader->Use( renderInfo );
	}

	if( m_vertexShader )
	{
		m_vertexShader->Use( renderInfo );
	}

	if ( m_textures.empty() )
	{
		// TODO: DX11
		//Texture::UnsetTexture( 0 );
	}

	for( size_t i = 0; i < m_textures.size(); ++i )
	{
		if ( m_textures[ i ] )
		{
			m_textures[ i ]->Use( i );
		}
		else
		{
			// TODO: DX11
			//Texture::UnsetTexture( i );
		}
	}
}

void Effect::SetCulling( CullingMode mode )
{
	m_culling = mode;
}

void Effect::SetTexture( unsigned char byteStage, Texture::ptr texture )
{
	// Ensure we have enough stages...
	if ( m_textures.size() <= byteStage )
	{
		m_textures.resize( byteStage + 1, Texture::ptr() );
	}
	m_textures[ byteStage ] = texture;
}

void Effect::ClearTextures()
{
	m_textures.clear();
}

void Effect::SetPixelShader( PixelShader::ptr shader )
{
	m_pixelShader = shader;
}

PixelShader::ptr Effect::GetPixelShader()
{
	return m_pixelShader;
}

void Effect::SetVertexShader( VertexShader::ptr shader )
{
	m_vertexShader = shader;
}

VertexShader::ptr Effect::GetVertexShader()
{
	return m_vertexShader;
}

Texture::ptr Effect::GetTexture( unsigned char stage )
{
	return m_textures[ stage ];
}

void Effect::AddFrame( size_t frameIndex, float influence )
{
	FrameIndexAndInfluence frameIndexAndInfluence( frameIndex, influence );
	m_frameIndexAndInfluence.push_back( frameIndexAndInfluence );
}

bool Effect::UsesTransparency()
{
	return GetPixelShader() ? GetPixelShader()->IsTrans() : false;
}

void Effect::SetScratchVertexBuffer( std::shared_ptr< VertexBuffer > vertexBuffer )
{
	m_scratchVertexBuffer = vertexBuffer;
}

std::shared_ptr< VertexBuffer > Effect::GetScratchVertexBuffer() const
{
	return m_scratchVertexBuffer;
}
