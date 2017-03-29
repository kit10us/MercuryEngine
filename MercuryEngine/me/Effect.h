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
		Effect( IPixelShader::ptr ps, IVertexShader::ptr vs, std::initializer_list< ITexture::ptr > textures = {} );
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

		unify::Path GetSource() const;

	public: // IResource...
		size_t Owners() const;
		bool Reload() override;

	protected:
		unify::Path m_source;
		IPixelShader::ptr m_pixelShader;
		IVertexShader::ptr m_vertexShader;
		std::vector< ITexture::ptr > m_textures;
	};
}