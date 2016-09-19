// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/Texture.h>
#include <dxi/Blend.h>
#include <dxi/PixelShader.h>
#include <dxi/VertexShader.h>
#include <dxi/VertexDeclaration.h>
#include <dxi/VertexBuffer.h>
#include <dxi/Transform.h>
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
		typedef std::shared_ptr< Effect > shared_ptr;
		typedef std::weak_ptr< Effect > weak_ptr;

		Effect();
		~Effect();

		Effect & operator = ( const Effect & effect );
		bool operator ==( const Effect & effect ) const;
		bool operator !=( const Effect & effect ) const;

		void Use( const RenderInfo & renderInfo );
		bool HasBlend() const;
		void SetTransformConstant( Transform::Index::TYPE trans, int iConstant );
		void SetTexture( unsigned char stage, Texture::shared_ptr texture );
		void ClearTextures();
		void SetBlend( const Blend & blend );
		void SetCulling( CullingMode mode );
		void SetPixelShader( PixelShader::shared_ptr shader );
		void SetVertexShader( VertexShader::shared_ptr shader );
		void AddFrame( size_t frameIndex, float influence );
		
		Blend & GetBlend();
		PixelShader::shared_ptr GetPixelShader();
		VertexShader::shared_ptr GetVertexShader();
		Texture::shared_ptr GetTexture( unsigned char stage );
		bool UsesTransparency();

		/// <summary>
		/// If set, the scratch vertex buffer is available for use by transient render operations - those that don't want to commit to
		void SetScratchVertexBuffer( std::shared_ptr< VertexBuffer > vertexBuffer );
		std::shared_ptr< VertexBuffer > GetScratchVertexBuffer() const;

	protected:
		CullingMode m_culling;
		Blend m_blend;
		PixelShader::shared_ptr m_pixelShader;
		VertexShader::shared_ptr m_vertexShader;
		std::vector< Texture::shared_ptr > m_textures;
		std::vector< FrameIndexAndInfluence > m_frameIndexAndInfluence;
		std::shared_ptr< VertexBuffer > m_scratchVertexBuffer;
	};
}