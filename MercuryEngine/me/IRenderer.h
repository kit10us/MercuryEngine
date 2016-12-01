// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/Viewport.h>
#include <me/Display.h>
#include <me/IVertexBuffer.h>
#include <me/IIndexBuffer.h>
#include <me/IVertexShader.h>
#include <me/IPixelShader.h>
#include <me/ITexture.h>
#include <me/RenderInstance.h>
#include <memory>

namespace me
{
	class VertexDeclaration;

	struct CullMode
	{
		enum TYPE
		{
			None,
			Clockwise,
			CounteClockwise
		};
	};

	/// <summary>
	/// Supports access to the renderer.
	/// </summary>
	class IRenderer
	{
	public:
		typedef std::shared_ptr< IRenderer > ptr;

		virtual ~IRenderer() {}

		virtual const me::Display & GetDisplay() const = 0;

		virtual Viewport GetViewport() const = 0;

		/// <summary>
		/// Called before render to allow renderer to perapre.
		/// </summary>
		virtual void BeforeRender() = 0;

		/// <summary>
		/// Called after render to allow renderer to cleanup.
		/// </summary>
		virtual void AfterRender() = 0;

		// New support for render states
		virtual void SetCullMode( CullMode::TYPE mode ) = 0;

		virtual bool IsFullscreen() const = 0;

		virtual size_t GetIndex() const = 0;

		virtual void* GetHandle() const = 0;

		virtual void Render( const class RenderMethod & method, const RenderInfo & renderInfo, const unify::Matrix & instance ) = 0;

		virtual void RenderInstanced( const RenderMethod & method, const RenderInfo & renderInfo, const std::vector< unify::Matrix > & instances ) = 0;

		virtual IVertexBuffer::ptr ProduceVB( VertexBufferParameters parameters ) const = 0;
		virtual IIndexBuffer::ptr ProduceIB( IndexBufferParameters parameters ) const = 0;
		virtual IVertexShader::ptr ProduceVS( VertexShaderParameters parameters ) const = 0;
		virtual IPixelShader::ptr ProducePS( PixelShaderParameters parameters ) const = 0;
		virtual IVertexConstruct::ptr ProduceVC( const VertexDeclaration & vd, const IVertexShader & vs ) const = 0;
		virtual ITexture::ptr ProduceT( TextureParameters parameters ) const = 0;
	};
}

