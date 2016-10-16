// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/Texture.h>
#include <dxi/PixelShader.h>
#include <dxi/VertexShader.h>
#include <dxi/VertexDeclaration.h>
#include <dxi/VertexBuffer.h>
#include <unify/Matrix.h>
#include <unify/Frame.h>

namespace dxi
{
    class RenderInfo;

	enum CullingMode
	{
		CullNone,
		CullCW,
		CullCCW,
		CullIgnore // Don't modify the culling state.
	};

	// Used to provide linking between a frame lookup, and the influence the frame has on the transform.
	typedef std::pair< size_t, float > FrameIndexAndInfluence;

	/// <summary>
	/// A gathering of rendering effects to be used before rendering primitives.
	/// </summary>
	class Effect
	{
	public:
		typedef std::shared_ptr< Effect > ptr;

		Effect();
		~Effect();

		Effect & operator = ( const Effect & effect );
		bool operator ==( const Effect & effect ) const;
		bool operator !=( const Effect & effect ) const;

		void Use( const RenderInfo & renderInfo );
		void SetTexture( unsigned char stage, Texture::ptr texture );
		void ClearTextures();
		void SetCulling( CullingMode mode );
		void SetPixelShader( PixelShader::ptr shader );
		void SetVertexShader( VertexShader::ptr shader );
		void AddFrame( size_t frameIndex, float influence );
		
		PixelShader::ptr GetPixelShader();
		VertexShader::ptr GetVertexShader();
		Texture::ptr GetTexture( unsigned char stage );
		bool UsesTransparency();

		/// <summary>
		/// If set, the scratch vertex buffer is available for use by transient render operations - those that don't want to commit to
		void SetScratchVertexBuffer( std::shared_ptr< VertexBuffer > vertexBuffer );
		std::shared_ptr< VertexBuffer > GetScratchVertexBuffer() const;

	protected:
		CullingMode m_culling;
		PixelShader::ptr m_pixelShader;
		VertexShader::ptr m_vertexShader;
		std::vector< Texture::ptr > m_textures;
		std::vector< FrameIndexAndInfluence > m_frameIndexAndInfluence;
		std::shared_ptr< VertexBuffer > m_scratchVertexBuffer;
	};
}