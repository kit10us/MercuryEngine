// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/Effect.h>
#include <me/render/IRenderer.h>

using namespace me;
using namespace render;

Effect::Effect()
{
}

Effect::Effect( unify::Path source )
	: m_source{ source }
{
}

Effect::Effect( IVertexShader::ptr vs, IPixelShader::ptr ps, ITexture::ptr texture )
	: m_vertexShader{ vs }
	, m_pixelShader{ ps }
	, m_textures{ texture }
{
}

Effect::Effect( IVertexShader::ptr vs, IPixelShader::ptr ps, std::initializer_list< ITexture::ptr > textures )
	: m_vertexShader{ vs }
	, m_pixelShader{ ps }
	, m_textures{ textures }
{
}

Effect::Effect( IVertexShader::ptr vs, IPixelShader::ptr ps, ITexture::ptr* begin, ITexture::ptr* end )
	: m_vertexShader{ vs }
	, m_pixelShader{ ps }
	, m_textures( begin, end )
{
}

Effect::~Effect()
{
}

Effect & Effect::operator = ( const Effect & effect )
{
	m_vertexShader = effect.m_vertexShader;
	m_pixelShader = effect.m_pixelShader;

	return *this;
}

bool Effect::operator == ( const Effect & effect ) const
{
	if ( m_vertexShader != effect.m_vertexShader ) return false;
	if ( m_pixelShader != effect.m_pixelShader ) return false;

	return true;
}

bool Effect::operator != ( const Effect & effect ) const
{
	return !( *this == effect );
}

Effect::ptr Effect::Duplicate()
{
	Effect::ptr newEffect{ new Effect( *this ) };
	return newEffect;
}

void Effect::UpdateData( const RenderInfo & renderInfo, const unify::Matrix * world, size_t world_size )
{
	unify::DataLock lock;

	auto constantTable = m_vertexShader->GetConstantBuffer()->GetTable();

	auto worldRef = constantTable->GetWorld();
	auto viewRef = constantTable->GetView();
	auto projRef = constantTable->GetProjection();

	size_t bufferIndex = 0;
	for( size_t bufferIndex = 0, buffer_count = constantTable->BufferCount(); bufferIndex < buffer_count; bufferIndex++ )
	{
		unify::DataLock lock;
		m_vertexShader->GetConstantBuffer()->LockConstants( bufferIndex, lock );

		// Set automatic variables...
		
		if ( bufferIndex == viewRef.buffer )
		{
			unsigned char * data = (lock.GetData<unsigned char>()) + viewRef.offsetInBytes;
			unify::Matrix* matrix = (unify::Matrix*)data;
			*matrix = renderInfo.GetViewMatrix();
		}
		
		if ( bufferIndex == projRef.buffer )
		{
			unsigned char * data = (lock.GetData<unsigned char>()) + projRef.offsetInBytes;
			unify::Matrix* matrix = (unify::Matrix*)data;
			*matrix = renderInfo.GetProjectionMatrix();
		}

		if ( world_size != 0 && bufferIndex == worldRef.buffer )
		{
			unsigned char * data = (lock.GetData<unsigned char>()) + worldRef.offsetInBytes;
			unify::Matrix* matrix = (unify::Matrix*)data;
			*matrix = world[0];
		}

		m_vertexShader->GetConstantBuffer()->UnlockConstants( bufferIndex, lock );
		bufferIndex++;
	}
}

void Effect::Use( IRenderer* renderer, const RenderInfo & renderInfo )
{		   
	m_pixelShader->Use();
	m_vertexShader->Use();

	renderer->UseTextures( m_textures );
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

void Effect::SetVertexShader( IVertexShader::ptr shader )
{
	m_vertexShader = shader;
}

IPixelShader::ptr Effect::GetPixelShader()
{
	return m_pixelShader;
}

IVertexShader::ptr Effect::GetVertexShader()
{
	return m_vertexShader;
}

ITexture::ptr Effect::GetTexture( unsigned char stage )
{
	return m_textures[ stage ];
}

const IPixelShader::ptr Effect::GetPixelShader() const
{
	return m_pixelShader;
}

const IVertexShader::ptr Effect::GetVertexShader() const
{
	return m_vertexShader;
}

const ITexture::ptr Effect::GetTexture( unsigned char stage ) const
{
	return m_textures[stage];
}

bool Effect::IsTrans() const
{
	return ( GetPixelShader() ? GetPixelShader()->IsTrans() : false ) || ( GetVertexShader() ? GetVertexShader()->IsTrans() : false );
}

unify::Size< unsigned int > Effect::LargestTextureSizes() const
{
	unify::Size< unsigned int > size( 0, 0 );
	for( auto texture : m_textures )
	{
		if( size.width == -1 || size.width < texture->ImageSize().width )
		{
			size.width = texture->ImageSize().width;
		}
		
		if( size.height == -1 || size.height < texture->ImageSize().height )
		{
			size.height = texture->ImageSize().height;
		}
	}
	return size;
}

unify::Size< unsigned int > Effect::SmallestTextureSizes() const
{
	unify::Size< unsigned int > size( 0, 0 );
	for( auto texture : m_textures )
	{
		if( size.width == -1 || size.width > texture->ImageSize().width )
		{
			size.width = texture->ImageSize().width;
		}

		if( size.height == -1 || size.height > texture->ImageSize().height )
		{
			size.height = texture->ImageSize().height;
		}
	}
	return size;
}

std::string Effect::GetSource() const
{
	return m_source.ToString();
}

size_t Effect::Owners() const
{
	return IResource::Owners();
}

bool Effect::Reload()
{
	return true; // We don't reload Effects... they have no real data.
}