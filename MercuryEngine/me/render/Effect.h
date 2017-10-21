// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/render/RenderInfo.h>
#include <me/render/ITexture.h>
#include <me/render/IPixelShader.h>
#include <me/render/IVertexShader.h>
#include <me/render/VertexDeclaration.h>
#include <me/render/IVertexBuffer.h>
#include <unify/Matrix.h>
#include <unify/Frame.h>

namespace me
{
	namespace render
	{
		// Used to provide linking between a frame lookup, and the influence the frame has on the transform.
		typedef std::pair< size_t, float > FrameIndexAndInfluence;

		/// <summary>
		/// A gathering of rendering effects to be used before rendering primitives.
		/// </summary>
		class Effect : public rm::IResource
		{
		public:
			typedef std::shared_ptr< Effect > ptr;

			Effect();
			Effect( unify::Path source );
			Effect( IVertexShader::ptr vs, IPixelShader::ptr ps, ITexture::ptr texture );
			Effect( IVertexShader::ptr vs, IPixelShader::ptr ps, std::initializer_list< ITexture::ptr > textures = {} );
			Effect( IVertexShader::ptr vs, IPixelShader::ptr ps, ITexture::ptr* begin, ITexture::ptr* end );
			~Effect();

			Effect & operator = ( const Effect & effect );
			bool operator ==( const Effect & effect ) const;
			bool operator !=( const Effect & effect ) const;

			/// <summary>
			/// This updates data to the vertex shader; don't use this if you intend to do this manually (this is likely to be deprecated.
			/// </summary>
			void UpdateData( const RenderInfo & renderInfo, const unify::Matrix * world, size_t world_size );

			void Use( IRenderer* renderer, const RenderInfo & renderInfo );

			void SetTexture( unsigned char stage, ITexture::ptr texture );
			void ClearTextures();
			void SetPixelShader( IPixelShader::ptr shader );
			void SetVertexShader( IVertexShader::ptr shader );

			IPixelShader::ptr GetPixelShader();
			IVertexShader::ptr GetVertexShader();
			ITexture::ptr GetTexture( unsigned char stage );
			const IPixelShader::ptr GetPixelShader() const;
			const IVertexShader::ptr GetVertexShader() const;
			const ITexture::ptr GetTexture( unsigned char stage ) const;
			bool IsTrans() const;

			/// <summary>
			/// Returns the largest width, and largest height of the textures.
			/// </summary>
			unify::Size< unsigned int > LargestTextureSizes() const;

			/// <summary>
			/// Returns the smallest width, and smallest height of the textures.
			/// </summary>
			unify::Size< unsigned int > SmallestTextureSizes() const;

			unify::Path GetSource() const;

		public: // IResource...
			size_t Owners() const;
			bool Reload() override;

		protected:
			unify::Path m_source;
			IPixelShader::ptr m_pixelShader;
			IVertexShader::ptr m_vertexShader;
			std::vector< ITexture::ptr > m_textures;
			BlendDesc m_blendDesc;
		};
	}
}