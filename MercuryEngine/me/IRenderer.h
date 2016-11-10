// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/Viewport.h>
#include <me/Display.h>
#include <me/IVertexBuffer.h>
#include <me/IIndexBuffer.h>
#include <me/IVertexShader.h>
#include <me/IPixelShader.h>
#include <me/IVertexDeclaration.h>
#include <me/ITexture.h>
#include <memory>

namespace me
{
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

		virtual void Render( class RenderMethod ) const = 0;

		virtual me::IVertexBuffer::ptr ProduceVB( me::VertexBufferParameters parameters ) = 0;
		virtual me::IIndexBuffer::ptr ProduceIB( me::IndexBufferParameters parameters ) = 0;
		virtual me::IVertexShader::ptr ProduceVS( VertexShaderParameters parameters ) = 0;
		virtual me::IPixelShader::ptr ProducePS( PixelShaderParameters parameters ) = 0;
		virtual me::IVertexDeclaration::ptr ProduceVD( VertexDeclarationParameters parameters ) = 0;
		virtual me::ITexture::ptr ProduceT( TextureParameters parameters ) = 0;
	};
}

