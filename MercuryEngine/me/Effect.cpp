// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Effect.h>

using namespace me;

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

void Effect::UpdateData( const RenderInfo & renderInfo, const unify::Matrix * world, size_t world_size )
{
	unify::DataLock lock;

	const me::shader::ConstantBuffer * constants = m_vertexShader->GetConstants();

	auto worldRef = constants->GetWorld();
	auto viewRef = constants->GetView();
	auto projRef = constants->GetProjection();

	size_t bufferIndex = 0;
	for( size_t bufferIndex = 0, buffer_count = constants->BufferCount(); bufferIndex < buffer_count; bufferIndex++ )
	{
		unify::DataLock lock;
		m_vertexShader->LockConstants( bufferIndex, lock );

		// Set automatic variables...
		
		if ( bufferIndex == viewRef.buffer )
		{
			unsigned char * data = ((unsigned char *)lock.GetData()) + viewRef.offsetInBytes;
			unify::Matrix* matrix = (unify::Matrix*)data;
			*matrix = renderInfo.GetViewMatrix();
		}
		
		if ( bufferIndex == projRef.buffer )
		{
			unsigned char * data = ((unsigned char *)lock.GetData()) + projRef.offsetInBytes;
			unify::Matrix* matrix = (unify::Matrix*)data;
			*matrix = renderInfo.GetProjectionMatrix();
		}

		if ( world_size != 0 && bufferIndex == worldRef.buffer )
		{
			unsigned char * data = ((unsigned char *)lock.GetData()) + worldRef.offsetInBytes;
			unify::Matrix* matrix = (unify::Matrix*)data;
			*matrix = world[0];
		}

		m_vertexShader->UnlockConstants( bufferIndex, lock );
		bufferIndex++;
	}
}

void Effect::Use()
{		   
	m_pixelShader->Use();

	m_vertexShader->Use();

	for( size_t i = 0; i < m_textures.size(); ++i )
	{
		if ( m_textures[ i ] )
		{
			m_textures[ i ]->Use( i );
		}
	}
}

void Effect::SetCulling( CullingMode mode )
{
	m_culling = mode;
}

void Effect::SetTexture( unsigned char byteStage, ITexture::ptr texture )
{
	// Ensure we have enough stages...
	if ( m_textures.size() <= byteStage )
	{
		m_textures.resize( byteStage + 1, ITexture::ptr() );
	}
	m_textures[ byteStage ] = texture;
}

void Effect::ClearTextures()
{
	m_textures.clear();
}

void Effect::SetPixelShader( IPixelShader::ptr shader )
{
	m_pixelShader = shader;
}

IPixelShader::ptr Effect::GetPixelShader()
{
	return m_pixelShader;
}

void Effect::SetVertexShader( IVertexShader::ptr shader )
{
	m_vertexShader = shader;
}

IVertexShader::ptr Effect::GetVertexShader()
{
	return m_vertexShader;
}

ITexture::ptr Effect::GetTexture( unsigned char stage )
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
