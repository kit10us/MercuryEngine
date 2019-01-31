// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/Display.h>
#include <me/render/BufferSet.h>
#include <me/render/ITexture.h>
#include <me/render/RenderInfo.h>
#include <me/render/Effect.h>
#include <me/render/RenderMethod.h>
#include <me/render/MatrixFeed.h>
#include <memory>

namespace me
{
	namespace render
	{
		/// <summary>
		/// Supports access to the renderer.
		/// </summary>
		class IRenderer
		{
		public:
			typedef std::shared_ptr< IRenderer > ptr;

			virtual ~IRenderer() {};

			virtual const render::Display & GetDisplay() const = 0;

			virtual render::Viewport GetViewport() const = 0;

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

			virtual bool IsFullscreen() const = 0;

			virtual size_t GetIndex() const = 0;

			virtual void* GetHandle() const = 0;

			/// <summary>
			/// Render geometry.
			/// </summary>
			virtual void Render( const RenderInfo & renderInfo, const RenderMethod & method, Effect::ptr effect, IConstantBuffer * vertexCB, IConstantBuffer * pixelCB, MatrixFeed & matrixFeed ) = 0;

			/// <summary>
			/// Render geometry.
			/// </summary>
			virtual void Render( const RenderInfo & renderInfo, const RenderMethod & method, BufferSet * bufferSet, MatrixFeed & matrixFeed ) = 0;

			/// <summary>
			/// Create a vertex buffer.
			/// </summary>
			virtual IVertexBuffer::ptr ProduceVB( VertexBufferParameters parameters ) = 0;
			
			/// <summary>
			/// Create an index buffer.
			/// </summary>
			virtual IIndexBuffer::ptr ProduceIB( IndexBufferParameters parameters ) = 0;

			/// <summary>
			/// Create a vertex shader.
			/// </summary>
			virtual IVertexShader::ptr ProduceVS( VertexShaderParameters parameters ) = 0;

			/// <summary>
			/// Create a pixel shader.
			/// </summary>
			virtual IPixelShader::ptr ProducePS( PixelShaderParameters parameters ) = 0;

			/// <summary>
			/// Create a vertex construct.
			/// </summary>
			virtual IVertexConstruct::ptr ProduceVC( const me::render::VertexDeclaration & vd, const IVertexShader & vs ) = 0;

			/// <summary>
			/// Create a texture.
			/// </summary>
			virtual ITexture::ptr ProduceT( TextureParameters parameters ) = 0;

			virtual void UseTextures( std::vector< ITexture::ptr > textures ) = 0;
		};
	}
}

