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
#include <me/RenderInfo.h>
#include <me/RenderMethod.h>
#include <me/ISketcher.h>
#include <unify/FrameLite.h>
#include <memory>

#include <me/MatrixFeed.h>

namespace me
{
	class VertexDeclaration;
	class IGame;

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

		virtual ~IRenderer() {};

		virtual IGame* GetGame() = 0;

		virtual const me::Display & GetDisplay() const = 0;

		virtual Viewport GetViewport() const = 0;

		/// <summary>
		/// Called before rendering.
		/// </summary>
		virtual void BeforeRender() = 0;

		/// <summary>
		/// Called before rendering solids.
		/// </summary>
		virtual void BeforeRenderSolids() = 0;

		/// <summary>
		/// Called before rendering transparencies.
		/// </summary>
		virtual void BeforeRenderTrans() = 0;

		/// <summary>
		/// Called after rendering.
		/// </summary>
		virtual void AfterRender() = 0;

		// New support for render states
		virtual void SetCullMode( CullMode::TYPE mode ) = 0;

		virtual bool IsFullscreen() const = 0;

		virtual size_t GetIndex() const = 0;

		virtual void* GetHandle() const = 0;

		virtual void Render( const RenderMethod & method, const RenderInfo & renderInfo, MatrixFeed & matrixFeed ) = 0;

		virtual ISketcher * GetSketcher() = 0;

		virtual IVertexBuffer::ptr ProduceVB( VertexBufferParameters parameters ) = 0;
		virtual IIndexBuffer::ptr ProduceIB( IndexBufferParameters parameters ) = 0;
		virtual IVertexShader::ptr ProduceVS( VertexShaderParameters parameters ) = 0;
		virtual IPixelShader::ptr ProducePS( PixelShaderParameters parameters ) = 0;
		virtual IVertexConstruct::ptr ProduceVC( const VertexDeclaration & vd, const IVertexShader & vs ) = 0;
		virtual ITexture::ptr ProduceT( TextureParameters parameters ) = 0;

		virtual void UseTextures( std::vector< me::ITexture::ptr > textures ) = 0;
	};
}

