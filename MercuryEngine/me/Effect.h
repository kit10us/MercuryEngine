// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/RenderInfo.h>
#include <me/ITexture.h>
#include <me/IPixelShader.h>
#include <me/IVertexShader.h>
#include <me/VertexDeclaration.h>
#include <me/IVertexBuffer.h>
#include <unify/Matrix.h>
#include <unify/Frame.h>

namespace me
{
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

		void Use( const RenderInfo & renderInfo, const unify::Matrix * world, size_t world_size );
		void SetTexture( unsigned char stage, ITexture::ptr texture );
		void ClearTextures();
		void SetCulling( CullingMode mode );
		void SetPixelShader( IPixelShader::ptr shader );
		void SetVertexShader( IVertexShader::ptr shader );
		void AddFrame( size_t frameIndex, float influence );
		
		IPixelShader::ptr GetPixelShader();
		IVertexShader::ptr GetVertexShader();
		ITexture::ptr GetTexture( unsigned char stage );
		bool UsesTransparency();

	protected:
		CullingMode m_culling;
		IPixelShader::ptr m_pixelShader;
		IVertexShader::ptr m_vertexShader;
		std::vector< ITexture::ptr > m_textures;
		std::vector< FrameIndexAndInfluence > m_frameIndexAndInfluence;
		IVertexBuffer::ptr m_scratchVertexBuffer;
	};
}