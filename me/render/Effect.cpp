// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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